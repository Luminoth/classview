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
----
You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software Foundation,
Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
==========
*/


#if !defined SHARED_H
#define SHARED_H


#include <string>


namespace Shared
{
    /*
     *  constants
     *
     */

    const std::string PROGNAME = "classview";
    const std::string VERSION  = "1.1.2";
    const std::string WEB      = "http://energon.home.mindspring.com/";
    const std::string AUTHORS  = "Shane Lillie <aszwad@softhome.net>\n";   // seperate each author by a newline


    /*
     *  typedefs
     *
     */

    typedef unsigned char byte;
};


#endif
