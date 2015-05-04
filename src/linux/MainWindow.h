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


#if !defined MAINWINDOW_H
#define MAINWINDOW_H


#include <string>


namespace Gtk
{
    class Tree;
};


class MainWindowImpl;
class TreeItem;


class MainWindow
{
public:
    MainWindow(const int width, const int height, const std::string& title="");
    virtual ~MainWindow();

public:
    // adds a subtree under the passed item
    Gtk::Tree* addTreeSubtree(TreeItem* const item);

    // adds a tree item under a tree (or under the main tree if parent == NULL), name is passed to TreeItem
    TreeItem* addTreeItem(const std::string& label, const std::string& name="", Gtk::Tree* const parent=NULL);

    // gets the tree view from the window
    Gtk::Tree* getTree();

    // shows the window
    void show();

    // enables/disables the Close menu option
    void enable_close(const bool enable);

private:
    MainWindowImpl* m_impl;
};


#endif
