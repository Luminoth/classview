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


#if !defined CONFIG_H
#define CONFIG_H


#if defined WIN32
    #pragma warning(disable : 4786)
#endif


#include <string>
#include <map>


namespace Config
{
    /*
     *  default config values
     *
     */

    const std::string TAGS_FILE[2] = { "tags_file", "tags" };
    const std::string EDITOR[2]    = { "editor", "xterm -bg 'black' -fg 'white' -e vim -t" };
    const std::string CTAGS[2]     = { "ctags", "ctags *" };
    const std::string WIDTH[2]     = { "width", "175" };
    const std::string HEIGHT[2]    = { "height", "325" };


    /*
     *  typedefs
     *
     */

    typedef std::map<std::string, std::string> ConfigMap;


    /*
     *  prototypes
     *
     */

    // fills the config map with default config values
    void defaultConfig();

    // read/write the config file
    void read();
    void write();
};


#endif
