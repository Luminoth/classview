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
#include <gtk--/button.h>
#include <gtk--/label.h>
#include <gtk--/box.h>

#include "MessageBox.h"


/*
 *  impl classes
 *
 */


class MessageBoxImpl : public Gtk::Window
{
public:
    MessageBoxImpl(const std::string& title, const std::string& message, const GtkJustification justify=GTK_JUSTIFY_CENTER);

private:
    // callbacks
    void okButtonCB();

    // event handlers
    inline virtual gint delete_event_impl(GdkEventAny*) { Gtk::Main::quit(); return 0; }
};


/*
 *  MessageBox member functions
 *
 */


MessageBox::MessageBox(const std::string& title, const std::string& message, const GtkJustification justify) : m_impl(new MessageBoxImpl(title, message, justify))
{
}


MessageBox::~MessageBox()
{
    if(m_impl != NULL) {
        delete m_impl;
        m_impl = NULL;
    }
}


/*
 *  MessageBoxImpl member functions
 *
 */


MessageBoxImpl::MessageBoxImpl(const std::string& title, const std::string& message, const GtkJustification justify) : Gtk::Window(GTK_WINDOW_DIALOG)
{
    // window properties
    set_title(title);
    set_position(GTK_WIN_POS_CENTER);
    set_border_width(10);
    set_modal(true);
    set_policy(false, false, true);

    // the OK button
    Gtk::Button* button = manage(new Gtk::Button("OK"));
    button->clicked.connect(SigC::slot(this, &MessageBoxImpl::okButtonCB));
    button->set_flags(GTK_CAN_DEFAULT);
    button->grab_default();
    button->set_usize(80, -1);
    button->show();

    // the button box
    Gtk::HBox* hbox = manage(new Gtk::HBox());
    hbox->pack_start(*button, true, false);
    hbox->show();

    // the message label
    Gtk::Label* label = manage(new Gtk::Label(message));
//    label->set_line_wrap(true);
    label->set_justify(justify);
    label->show();

    // the label box
    Gtk::VBox* vbox = manage(new Gtk::VBox());
    vbox->set_border_width(5);
    vbox->pack_start(*label);
    vbox->show();

    // the main vbox
    Gtk::VBox* mainbox = manage(new Gtk::VBox());
    mainbox->set_spacing(5);
    mainbox->pack_start(*vbox);
    mainbox->pack_end(*hbox, false);
    mainbox->show();

    add(*mainbox);
    show();
    Gtk::Main::run();
}


void MessageBoxImpl::okButtonCB()
{
    Gtk::Main::quit();
}
