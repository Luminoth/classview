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


#include <cstdlib>
#include <iostream>
#include <string>

#if defined WIN32
    #if !defined WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #include <windows.h>
    #include <direct.h>
    #define mkdir(s, m) _mkdir(s)
#else
    #include <sys/stat.h>
    #include <sys/types.h>
    #include <sys/wait.h>
    #include <unistd.h>
    #include <pthread.h>
#endif
#include <errno.h>

#include "FileSystem.h"
#include "Config.h"
#include "linux/MessageBox.h"


/*
 *  external globals
 *
 */

extern Config::ConfigMap g_configMap;
extern int errno;


/*
 *  functions
 *
 */


void FileSystem::createPath(const std::string& path)
{
    unsigned int i=0;
    for(i=0; i<path.size(); i++) {
        if(path[i] == '/' || path[i] == '\\')
            mkdir(path.substr(0, i).c_str(), 0755);
    }
    if(path[path.length()-1] != '/' || path[i] == '\\')
        mkdir(path.c_str(), 0755);
}


void FileSystem::runCtags()
{
    const std::string command = g_configMap[Config::CTAGS[0]];
    if(exec(command))
        MessageBox("Error", "Could not execute '" + command + "'");
    else
        MessageBox("Error", "'" + command + "' run successfully");
}


void* openEditorThread(void* arg)
{
/* FIXME: bleh yet again... */
    if(NULL == arg) {
        MessageBox("Error", "openEditorThread() requires a string argument!");
//        pthread_exit(0);
    }

    const std::string* tag = reinterpret_cast<std::string*>(arg);
    const std::string editor = g_configMap[Config::EDITOR[0]];

    if(FileSystem::exec(editor + " " + *tag))
        MessageBox("Error", "Could not execute '" + editor + " " + *tag + "'");
//    pthread_exit(0);
    return NULL;
}


void FileSystem::openEditor(const std::string& tag)
{
/* FIXME: bleh... not enough time to think this through right */
    // start the editor in a new thread
    // we don't explicitly kill threads as the user has the right to keep the editor open...
//    pthread_t t;
    std::string s = tag;
/*    if(0 != pthread_create(&t, NULL, openEditorThread, &s)) {
        MessageBox("Error", "Could not create editor thread");
        return;
    }*/
    openEditorThread(&s);
}


const int FileSystem::exec(const std::string& command)
{
#if defined WIN32
/* FIXME: use execv in Win32 */
    return system(command.c_str());
#else
    if(command.empty())
        return 1;

    pid_t pid = fork();
    if(-1 == pid)
        return -1;
    else if(0 == pid) {
        char* argv[4];
        argv[0] = "sh";
        argv[1] = "-c";
        argv[2] = const_cast<char*>(command.c_str());
        argv[3] = 0;
        execv("/bin/sh", argv);
        exit(127);
    }

    int status = 0;
    do {
        if(waitpid(pid, &status, 0) == -1) {
            if(errno != EINTR)
                return -1;
        } else
            return status;
    } while(1);
#endif
}
