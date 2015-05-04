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


#include "TreeItem.h"
#include "FileSystem.h"


TreeItem::TreeItem(const std::string& label, const std::string& tag) : Gtk::TreeItem(label)
{
    m_tag = tag;
    button_press_event.connect(SigC::slot(this, &TreeItem::mouseClickCB));
}


int TreeItem::mouseClickCB(GdkEventButton* event)
{
    switch(event->type)
    {
    case GDK_BUTTON_PRESS:
        break;
    case GDK_2BUTTON_PRESS:
        switch(event->button)
        {
        case 1:
            if(!m_tag.empty())
                FileSystem::openEditor(m_tag);
            break;
        default:
            return false;
        }
        return true;
    case GDK_3BUTTON_PRESS:
        break;
    default:
        return false;
    }
    return false;
}
