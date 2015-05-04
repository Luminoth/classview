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


#if !defined TREEITEM_H
#define TREEITEM_H


/* FIXME: pimpl this... it's gonna be tough, but it can be done */


#include <string>

#include <gtk--/treeitem.h>


/*
 *  An item in the tree
 *
 */

class TreeItem : public Gtk::TreeItem
{
public:
    // name is what is opened by vim through ctags (no open if tag is empty)
    TreeItem(const std::string& label, const std::string& tag="");

private:
    // callbacks
    int mouseClickCB(GdkEventButton* event);

private:
    std::string m_tag;
};


#endif
