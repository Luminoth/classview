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
#include <gtk--/window.h>
#include <gtk--/scrolledwindow.h>
#include <gtk--/menubar.h>
#include <gtk--/menu.h>
#include <gtk--/box.h>
#include <gtk--/tree.h>
#include <gtk--/tooltips.h>
#include <gtk--/statusbar.h>
using SigC::slot;

#include <unistd.h>

#include "MainWindow.h"
#include "AboutDialog.h"
#include "PreferencesDialog.h"
#include "MessageBox.h"
#include "FileSelection.h"
#include "TreeItem.h"
#include "FileSystem.h"
#include "Config.h"
#include "main.h"
#include "shared.h"


/*
 *  impl classes
 *
 */


class MainWindowImpl : public Gtk::Window
{
public:
    MainWindowImpl(const int width, const int height, const std::string& title="");

public:
    inline Gtk::Tree* getTree() { return m_tree; }

    void enable_close(const bool enable);

private:
    // menu creation
    void createMenubar();
    void createHelpMenu();
    void createDebugMenu();
    void createOptionsMenu();
    void createFileMenu();

    // callbacks
    void runCtagsMenuCB();
    void openCurrentDirTagCB();
    void openMenuCB();
    void preferencesMenuCB();
    void configMenuCB();
    void aboutMenuCB();

    // event handlers
    inline virtual gint delete_event_impl(GdkEventAny*) { Gtk::Main::quit(); return 0; }

private:
    Gtk::Tree* m_tree;
    Gtk::MenuBar* m_menubar;
};


/*
 *  extern globals
 *
 */

extern Config::ConfigMap g_configMap;
extern std::string g_home;
extern std::string g_curWorkingDir;
extern std::string g_defWorkingDir;


/*
 *  MainWindow member functions
 *
 */


MainWindow::MainWindow(const int width, const int height, const std::string& title) : m_impl(new MainWindowImpl(width, height, title))
{
}


MainWindow::~MainWindow()
{
    if(m_impl != NULL) {
        delete m_impl;
        m_impl = NULL;
    }
}


Gtk::Tree* MainWindow::addTreeSubtree(TreeItem* const item)
{
    Gtk::Tree* subtree = manage(new Gtk::Tree());
    subtree->set_selection_mode(GTK_SELECTION_SINGLE);
    subtree->set_view_mode(GTK_TREE_VIEW_ITEM);
    item->set_subtree(*subtree);
    return subtree;
}


TreeItem* MainWindow::addTreeItem(const std::string& label, const std::string& name, Gtk::Tree* const parent)
{
    TreeItem* item = SigC::manage(new TreeItem(label, name));

    static Gtk::Tooltips tips;
    tips.set_tip(*item, label);
/* TODO: do we have tooltips or not? */
tips.disable();

    if(NULL == parent) {
        if(NULL != m_impl)
            m_impl->getTree()->tree().push_back(*item);
    } else
        parent->tree().push_back(*item);
    item->show();
    return item;
}


Gtk::Tree* MainWindow::getTree()
{
    return NULL != m_impl ? m_impl->getTree() : NULL;
}


void MainWindow::show()
{
    if(NULL != m_impl)
        m_impl->show();
}


void MainWindow::enable_close(const bool enable)
{
    if(NULL != m_impl)
        m_impl->enable_close(enable);
}


/*
 *  MainWindowImpl member functions
 *
 */


MainWindowImpl::MainWindowImpl(const int width, const int height, const std::string& title) : Gtk::Window()
{
    m_tree = NULL;
    m_menubar = NULL;

    // window properties
    set_border_width(5);
    set_title(title);
    set_policy(true, true, true);

    // create menus
    createMenubar();
    m_menubar->show();

    // create the tree
    m_tree = manage(new Gtk::Tree());
    m_tree->set_selection_mode(GTK_SELECTION_SINGLE);
    m_tree->show();

    // create the scrolled window
    Gtk::ScrolledWindow* scrolledWindow = manage(new Gtk::ScrolledWindow());
    scrolledWindow->set_policy(GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    scrolledWindow->set_usize(width, height);
    scrolledWindow->add_with_viewport(*m_tree);
    scrolledWindow->show();

    // create the statusbar
/* TODO: make this useful */
    Gtk::Statusbar* statusbar = manage(new Gtk::Statusbar());
    unsigned int statusbarContext = statusbar->get_context_id(title);
    statusbar->push(statusbarContext, title);
    statusbar->show();

    // add everything together
    Gtk::VBox* box = manage(new Gtk::VBox());
    box->pack_start(*m_menubar, false, false);
    box->pack_start(*scrolledWindow);
    box->pack_end(*statusbar, false, false);
    box->show();
    add(*box);
}


void MainWindowImpl::enable_close(const bool enable)
{
/*    Gtk::Menu* file = m_menubar->items().front()->get_submenu();
    Gtk::Menu_Helpers::MenuList& list = file->items();
    Gtk::Menu_Helpers::MenuList::iterator it = list.begin();

    it++; it++; it++; it++;
    (*it)->set_sensitive(enable);*/
}


void MainWindowImpl::createMenubar()
{
/* NOTE: according to the gtkmm docs, menubar doesn't have O(1) tail lookup, so add backwards */
    m_menubar = manage(new Gtk::MenuBar());
    createHelpMenu();
    createDebugMenu();
    createOptionsMenu();
    createFileMenu();
}


void MainWindowImpl::createHelpMenu()
{
    static Gtk::Tooltips tips;

    Gtk::Menu* help = manage(new Gtk::Menu());
    help->items().push_back(Gtk::Menu_Helpers::MenuElem("About...", slot(this, &MainWindowImpl::aboutMenuCB)));
    tips.set_tip(*help->items().back(), "About " + Shared::PROGNAME);

    m_menubar->items().push_front(Gtk::Menu_Helpers::MenuElem("_Help", "<control>H", *help));
    m_menubar->items().front()->right_justify();
}


void MainWindowImpl::createDebugMenu()
{
#if defined DEBUG
    static Gtk::Tooltips tips;

    Gtk::Menu* debug = manage(new Gtk::Menu());
    debug->items().push_back(Gtk::Menu_Helpers::MenuElem("Config Variables...", slot(this, &MainWindowImpl::configMenuCB)));
    tips.set_tip(*debug->items().back(), "Display config variables");

    m_menubar->items().push_front(Gtk::Menu_Helpers::MenuElem("_Debug", *debug));
#endif
}


void MainWindowImpl::createOptionsMenu()
{
    static Gtk::Tooltips tips;

    Gtk::Menu* options = manage(new Gtk::Menu());
    options->items().push_back(Gtk::Menu_Helpers::MenuElem("_Preferences...", "<control>P", slot(this, &MainWindowImpl::preferencesMenuCB)));
    tips.set_tip(*options->items().back(), "Configure " + Shared::PROGNAME);

    m_menubar->items().push_front(Gtk::Menu_Helpers::MenuElem("_Options", "<control>O", *options));
}


void MainWindowImpl::createFileMenu()
{
    static Gtk::Tooltips tips;

    Gtk::Menu* file = manage(new Gtk::Menu());

    file->items().push_back(Gtk::Menu_Helpers::MenuElem("_Run Ctags", "<control>R", slot(this, &MainWindowImpl::runCtagsMenuCB)));
    tips.set_tip(*file->items().back(), "Runs ctags in the current directory");

    file->items().push_back(Gtk::Menu_Helpers::MenuElem("Open Current Tag", "<control>T", slot(this, &MainWindowImpl::openCurrentDirTagCB)));
    tips.set_tip(*file->items().back(), "Opens the tags file in the current directory");

    file->items().push_back(Gtk::Menu_Helpers::MenuElem("_Open...", slot(this, &MainWindowImpl::openMenuCB)));
    tips.set_tip(*file->items().back(), "Opens a tags file");

    file->items().push_back(Gtk::Menu_Helpers::MenuElem("_Close", "<control>C", slot(Main::closeTagsFile)));
//    file->items().back()->set_sensitive(false);
    tips.set_tip(*file->items().back(), "Closes the currently open tags file");

    file->items().push_back(Gtk::Menu_Helpers::SeparatorElem());

    file->items().push_back(Gtk::Menu_Helpers::MenuElem("E_xit", "<control>Q", Gtk::Main::quit.slot()));
    tips.set_tip(*file->items().back(), "Exits " + Shared::PROGNAME);

    m_menubar->items().push_front(Gtk::Menu_Helpers::MenuElem("_File", "<control>F", *file));
}


void MainWindowImpl::runCtagsMenuCB()
{
    FileSystem::runCtags();
/* TODO: do we chdir() to the default working dir, or stay at current working dir? */
    g_configMap[Config::TAGS_FILE[0]] = Config::TAGS_FILE[1];
    Main::parseTagsFile();
}


void MainWindowImpl::openCurrentDirTagCB()
{
    g_curWorkingDir = g_defWorkingDir;
    chdir(g_curWorkingDir.c_str());

    g_configMap[Config::TAGS_FILE[0]] = Config::TAGS_FILE[1];
    Main::parseTagsFile();
}


void MainWindowImpl::openMenuCB()
{
    FileSelection("Open tags file...");
}


void MainWindowImpl::preferencesMenuCB()
{
    PreferencesDialog();
}


void MainWindowImpl::configMenuCB()
{
    std::string t;
    for(Config::ConfigMap::const_iterator it = g_configMap.begin(); it != g_configMap.end(); ++it)
        t += it->first + " = " + it->second + "\n";
    t += "\n";
    t += "Home Directory = " + g_home + "\n";
    t += "Current Working Directory = " + g_curWorkingDir + "\n";
    t += "Default Working Directory = " + g_defWorkingDir + "\n";
    MessageBox("Config Variables", t, GTK_JUSTIFY_LEFT);
}


void MainWindowImpl::aboutMenuCB()
{
    AboutDialog();
}
