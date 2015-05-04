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


#if !defined WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#include "MainWindow.h"
#include "../FileSystem.h"
#include "../Config.h"
#include "../main.h"
#include "../shared.h"


/*
 *  impl classes
 *
 */


class MainWindowImpl
{
public:
    MainWindowImpl(const int width, const int height, const std::string& title="");

private:
    HWND m_hwnd;
};


/*
 *  extern globals
 *
 */

extern Config::ConfigMap g_configMap;
extern std::string g_home;


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


/*
 *  MainWindowImpl member functions
 *
 */


MainWindowImpl::MainWindowImpl(const int width, const int height, const std::string& title)
{
}
