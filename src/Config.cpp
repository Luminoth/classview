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


#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>

#include "Config.h"
#include "FileSystem.h"
#include "shared.h"


/*
 *  globals
 *
 */

Config::ConfigMap g_configMap;


/*
 *  external globals
 *
 */

extern std::string g_home;


/*
 *  Config namespace
 *
 */

namespace Config
{
    /*
     *  constants
     *
     */

    const std::string CONFIG_FILE = "config";


    /*
     *  functions
     *
     */


    // strips ALL whitespace from a config option
    std::string stripOption(const std::string& option)
    {
        std::string t;
        for(unsigned int i=0; i<option.length(); i++) {
            if(!isspace(option[i]))
                t += option[i];
        }
        return t;
    }


    // strips whitespace from the front and rear of a config value (leaving internal whitespace)
    std::string stripValue(const std::string& value)
    {
        unsigned int i=0;
        std::string t;

        while(isspace(value[i]))
            i++;
        t = value.substr(i);

        i = t.length()-1;
        while(isspace(t[i]))
            t.erase(i--);

        return t;
    }


    void defaultConfig()
    {
        g_configMap[TAGS_FILE[0]] = TAGS_FILE[1];
        g_configMap[EDITOR[0]]    = EDITOR[1];
        g_configMap[CTAGS[0]]     = CTAGS[1];
        g_configMap[WIDTH[0]]     = WIDTH[1];
        g_configMap[HEIGHT[0]]    = HEIGHT[1];
    }


    void read()
    {
        const std::string dir = g_home + "." + Shared::PROGNAME + "/" + CONFIG_FILE;
        std::ifstream infile(dir.c_str());
        if(!infile) {
            write();
            return;
        }

        std::string line;
        while(!infile.eof()) {
            std::getline(infile, line);
            if(line.empty())
                continue;

            unsigned int pos = line.find('=');
            if(pos != std::string::npos) {
                std::string name  = stripOption(line.substr(0, pos));
                std::string value = stripValue(line.substr(pos+1));
                g_configMap[name] = value;
            } else
                std::cerr << "Invalid line found in config file: " << line << std::endl;
        }

        infile.clear();
        infile.close();
    }


    void write()
    {
        std::string dir = g_home + "." + Shared::PROGNAME;
        FileSystem::createPath(dir);
        dir += "/" + CONFIG_FILE;

        std::ofstream outfile(dir.c_str());
        if(!outfile) {
            perror("Couldn't write config file");
            return;
        }

        ConfigMap::const_iterator it;
        for(it = g_configMap.begin(); it != g_configMap.end(); ++it)
            outfile << stripOption(it->first) << " = " << stripValue(it->second) << std::endl;

        outfile.clear();
        outfile.close();
    }
};
