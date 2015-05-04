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
#include <gtk--/dialog.h>
#include <gtk--/button.h>
#include <gtk--/label.h>
#include <gtk--/box.h>

#include "AboutDialog.h"
#include "shared.h"


/*
 *  impl classes
 *
 */


class AboutDialogImpl : public Gtk::Dialog
{
public:
    AboutDialogImpl();

private:
    // callbacks
    void okButtonCB();

    // event handlers
    inline virtual gint delete_event_impl(GdkEventAny*) { Gtk::Main::quit(); return 0; }
};


/*
 *  AboutDialog member functions
 *
 */


AboutDialog::AboutDialog() : m_impl(new AboutDialogImpl())
{
}


AboutDialog::~AboutDialog()
{
    if(m_impl != NULL) {
        delete m_impl;
        m_impl = NULL;
    }
}


/*
 *  AboutDialogImpl member functions
 *
 */


AboutDialogImpl::AboutDialogImpl() : Gtk::Dialog()
{
    // dialog properties
    set_usize(300, -1);
    set_title("About " + Shared::PROGNAME);
    set_position(GTK_WIN_POS_CENTER);
    set_modal(true);
    set_policy(false, false, true);

    // the OK button
    Gtk::Button* button = manage(new Gtk::Button("OK"));
    button->clicked.connect(SigC::slot(this, &AboutDialogImpl::okButtonCB));
    button->set_flags(GTK_CAN_DEFAULT);
    button->grab_default();
    button->set_usize(80, -1);
    button->show();

    // the button box
    Gtk::HBox* hbox = get_action_area();
    hbox->pack_start(*button, false, false);

    // the about label
    std::string l = "\n\nClass Viewer\nVersion: " + Shared::VERSION + "\n\n(c) 2002 Energon Software\n\nAuthors:\n\n" + Shared::AUTHORS + "\n" + Shared::WEB + "\n\n";
    Gtk::Label* label = manage(new Gtk::Label(l));
    label->show();

    // the label box
    Gtk::VBox* vbox = get_vbox();
    vbox->pack_start(*label);

    show();
    Gtk::Main::run();
}


void AboutDialogImpl::okButtonCB()
{
    Gtk::Main::quit();
}
