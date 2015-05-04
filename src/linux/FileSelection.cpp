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


#include <gtk--/main.h>
#include <gtk--/fileselection.h>

#include <unistd.h>

#include "FileSelection.h"
#include "Config.h"
#include "FileSystem.h"
#include "main.h"


/*
 *  impl classes
 *
 */


class FileSelectionImpl : public Gtk::FileSelection
{
public:
    explicit FileSelectionImpl(const std::string& title);

public:
    // callbacks
    void okButtonCB();
    void cancelButtonCB();

    // event handlers
    inline virtual gint delete_event_impl(GdkEventAny*) { Gtk::Main::quit(); return 0; }
};


/*
 *  external globals
 *
 */


extern std::string g_home;
extern std::string g_curWorkingDir;
extern Config::ConfigMap g_configMap;


/*
 *  FileSelection member functions
 *
 */


FileSelection::FileSelection(const std::string& title) : m_impl(new FileSelectionImpl(title))
{
}


FileSelection::~FileSelection()
{
    if(m_impl != NULL) {
        delete m_impl;
        m_impl = NULL;
    }
}


/*
 *  FileSelectionImpl member functions
 *
 */


FileSelectionImpl::FileSelectionImpl(const std::string& title) : Gtk::FileSelection(title)
{
    set_filename(g_curWorkingDir);
    get_ok_button()->clicked.connect(SigC::slot(this, &FileSelectionImpl::okButtonCB));
    get_cancel_button()->clicked.connect(SigC::slot(this, &FileSelectionImpl::cancelButtonCB));
    show();

    Gtk::Main::run();
}


void FileSelectionImpl::okButtonCB()
{
    g_curWorkingDir = FileSystem::getPath(get_filename());
    chdir(g_curWorkingDir.c_str());

    g_configMap[Config::TAGS_FILE[0]] = get_filename();
    Main::parseTagsFile();
    Gtk::Main::quit();
}


void FileSelectionImpl::cancelButtonCB()
{
    Gtk::Main::quit();
}
