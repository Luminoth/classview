/*
==========
Copyright 2002 Energon Software

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software Foundation,
Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
==========
*/


#if defined WIN32
    #pragma warning(disable : 4786)
#endif


#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <iostream>
#include <string>
#include <map>
using std::string;


#if defined WIN32
    #if !defined WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #include <windows.h>
#else
    #include <unistd.h>
    #include <signal.h>

    #include <gtk--/main.h>
    #include <gtk--/tree.h>
#endif

#include "main.h"
#include "Parser.h"
#include "MainWindow.h"
#if !defined WIN32
    #include "linux/TreeItem.h"
    #include "linux/MessageBox.h"
#endif
#include "FileSystem.h"
#include "Config.h"


/*
 *  globals
 *
 */

string      g_home          = "/";    // user's home directory
string      g_curWorkingDir = "/";    // current working directory
string      g_defWorkingDir = "/";    // default working directory
bool        g_parse         = false;
MainWindow* g_window        = NULL;
Parser::FileInfo* g_fileInfo = NULL;


/*
 *  extern globals
 *
 */

extern Config::ConfigMap g_configMap;
extern char* optarg;


namespace Main
{
    /*
     *  constants
     *
     */

    const string TITLE  = "Class Viewer by Energon Software";


    /*
     *  functions
     *
     */


    // prints program usage
    inline void printUsage(const string& progName)
    {
        std::cout << std::endl << "Usage: " << FileSystem::stripPath(progName) << std::endl;
    }


    // makes sure the config width is valid
    const bool verifyWidth()
    {
        string w = g_configMap[Config::WIDTH[0]];
        for(unsigned int i=0; i<w.length(); i++) {
            if(!isdigit(w[i]))
                return false;
        }
        return w.length() < 5;
    }


    // makes sure the config height is valid
    const bool verifyHeight()
    {
        string h = g_configMap[Config::HEIGHT[0]];
        for(unsigned int i=0; i<h.length(); i++) {
            if(!isdigit(h[i]))
                return false;
        }
        return h.length() < 5;
    }


    // closes any open tags file
    void closeTagsFile()
    {
#if !defined WIN32
        g_window->getTree()->tree().clear();
        g_window->enable_close(false);
#endif
        if(NULL != g_fileInfo) {
            delete g_fileInfo;
            g_fileInfo = NULL;
        }
    }


    // cleans up at program exit
    void cleanup()
    {
//        closeTagsFile();
        if(NULL != g_window) {
            delete g_window;
            g_window = NULL;
        }
    }


    // processes command line arguments
    void processArguments(const int argc, char* const argv[])
    {
#if defined WIN32
#else
        int opt = getopt(argc, argv, "f:");
        while(opt != -1) {
            switch(opt)
            {
            case 'f':
                g_configMap[Config::TAGS_FILE[0]] = string(optarg);
                g_parse = true;
                break;
            case ':':
            case '?':
            default:
                printUsage(argv[0]);
                exit(1);
            }
            opt = getopt(argc, argv, "f:");
        }
#endif
    }


    // parses the tags file specified in the config map
    void parseTagsFile()
    {
        closeTagsFile();

        try {
            g_fileInfo = Parser::parse();
        } catch(Parser::ParserException& e) {
            MessageBox("Error", e.what());
            return;
        }
        std::sort(g_fileInfo->lines.begin(), g_fileInfo->lines.end());

#if !defined WIN32
        std::map<string, Gtk::Tree*> subtreeMap;
        for(unsigned int i=0; i<g_fileInfo->lines.size(); i++) {
            TreeItem* item = NULL;
            const Parser::LineInfo& info = g_fileInfo->lines[i];

            if(info.owner == "Globals") {
                if(info.isTopLevel()) {
                    if(subtreeMap[info.name] == NULL) {
                        item = g_window->addTreeItem(info.description, info.name);
                        subtreeMap[info.name] = g_window->addTreeSubtree(item);
                    }
                } else {
                    if(subtreeMap[info.owner] == NULL)
                        subtreeMap[info.owner] = g_window->addTreeSubtree(g_window->addTreeItem(info.owner));
                }
            } else {
                if(info.isTopLevel()) {
                    if(subtreeMap[info.owner + "::" + info.name] == NULL) {
                        item = g_window->addTreeItem(info.description, info.name, subtreeMap[info.owner]);
                        subtreeMap[info.owner + "::" + info.name] = g_window->addTreeSubtree(item);
                    }
                }
            }

            if(!info.isTopLevel()) {
                if(subtreeMap[info.owner] != NULL)
                    item = g_window->addTreeItem(info.description, info.name, subtreeMap[info.owner]);
                else
                    std::cerr << "Could not add info: " << std::endl << info << std::endl << std::endl;
            }
        }
        g_window->enable_close(true);
#endif
    }

#if !defined WIN32
    // signal handler
    void sighandler(int signum)
    {
        switch(signum)
        {
        case SIGINT:
            std::cout << "Caught SIGINT, exiting" << std::endl;
            exit(0);
        case SIGSEGV:
            std::cerr << "Memory overrun (Segmentation fault) detected, exiting" << std::endl;
            exit(1);
        default:
            std::cerr << "Caught signal " << signum << ", continuing anyway" << std::endl;
        };
    }
#endif
};


#if defined WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
/* FIXME: g_home = user's My Documents folder */
    g_home = "C:\\";

    //MSG message;
/* FIXME: create window */

/*    while(GetMessage(&message, NULL, 0, 0)) {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    return message.wParam;*/
return FALSE;
}
#else
int main(int argc, char* argv[])
{
    Gtk::Main gtk(argc, argv);
    g_home = string(getenv("HOME")) + "/";

    // get the current working directory, 512 should be enough, and the buffer isn't NULL so getcwd should behave sanely
    char cwd[512];
    if(!getcwd(cwd, 512)) {
        std::cerr << "Couldn't store current working directory, using home directory" << std::endl;
        g_defWorkingDir = g_home;
    } else
        g_defWorkingDir = string(cwd) + "/";
    g_curWorkingDir = g_defWorkingDir;

    // set signal handlers
    struct sigaction sigAction;
    sigAction.sa_handler = Main::sighandler;
    sigemptyset(&sigAction.sa_mask);
    sigAction.sa_flags = 0;
    if(-1 == sigaction(SIGINT, &sigAction, NULL))
        perror("Unable to set SIGINT handler, continuing anyway");
    if(-1 == sigaction(SIGSEGV, &sigAction, NULL))
        perror("Unable to set SIGSEGV handler, continuting anyway");

    atexit(Main::cleanup);

    // read the config options
    Config::defaultConfig();
    Config::read();
    Config::write();    // do this to make it easy to add new config options

    Main::processArguments(argc, argv);

    if(!Main::verifyWidth())
        g_configMap[Config::WIDTH[0]] = Config::WIDTH[1];
    if(!Main::verifyHeight())
        g_configMap[Config::HEIGHT[0]] = Config::HEIGHT[1];
    g_window = new MainWindow(atoi(g_configMap[Config::WIDTH[0]].c_str()), atoi(g_configMap[Config::HEIGHT[0]].c_str()), Main::TITLE);

    if(g_parse)
        Main::parseTagsFile();

    // show and run
    g_window->show();
    gtk.run();

    return 0;
}
#endif
