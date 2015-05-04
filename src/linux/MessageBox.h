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


#if !defined MESSAGEBOX_H
#define MESSAGEBOX_H


#include <string>

#include <gtk/gtkenums.h>


class MessageBoxImpl;


/*
 *  A message box
 *
 */

class MessageBox
{
public:
    MessageBox(const std::string& title, const std::string& message, const GtkJustification justify=GTK_JUSTIFY_CENTER);
    virtual ~MessageBox();

private:
    MessageBoxImpl* m_impl;
};


#endif
