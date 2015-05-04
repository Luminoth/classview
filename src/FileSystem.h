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


#if !defined FILESYSTEM_H
#define FILESYSTEM_H


#include <string>


namespace FileSystem
{
    /*
     *  prototypes
     *
     */

    // creates a directory
    void createPath(const std::string& path);

    // runs ctags in the current directory
    void runCtags();

    // opens the editor at the specified tag
    void openEditor(const std::string& tag);

    // executes a command
    const int exec(const std::string& command);


    /*
     *  inline functions
     *
     */

    // strips the path from a filename
    inline std::string stripPath(const std::string& filename)
    {
        unsigned int pos = filename.rfind("/");
        return (pos != std::string::npos ? filename.substr(pos+1) : filename);
    }

    // gets the path from a filename
    inline std::string getPath(const std::string& filename)
    {
        unsigned int pos = filename.rfind("/");
        return (pos != std::string::npos ? filename.substr(0, pos+1) : filename);
    }
};


#endif
