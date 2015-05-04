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
#include <gtk--/box.h>
#include <gtk--/label.h>
#include <gtk--/entry.h>
#include <gtk--/checkbutton.h>
#include <gdk/gdkkeysyms.h>

#include "PreferencesDialog.h"
#include "Config.h"


/*
 *  impl classes
 *
 */

class PreferencesDialogImpl : public Gtk::Dialog
{
public:
    PreferencesDialogImpl();

private:
    // saves the preferences options
    void savePreferences();

    // callbacks
    void defaultButtonCB();
    void applyButtonCB();
    void okButtonCB();
    void cancelButtonCB();

    // event handlers
    inline virtual gint delete_event_impl(GdkEventAny*) { Gtk::Main::quit(); return 0; }

private:
    Gtk::Entry* m_editorEntry;
    Gtk::Entry* m_ctagsEntry;
};


/*
 *  extern globals
 *
 */

extern Config::ConfigMap g_configMap;


/*
 *  PreferencesDialog member functions
 *
 */


PreferencesDialog::PreferencesDialog() : m_impl(new PreferencesDialogImpl())
{
}


PreferencesDialog::~PreferencesDialog()
{
    if(NULL != m_impl) {
        delete m_impl;
        m_impl = NULL;
    }
}


/*
 *  PreferencesDialogImpl member functions
 *
 */


PreferencesDialogImpl::PreferencesDialogImpl() : Gtk::Dialog()
{
    m_editorEntry = NULL;
    m_ctagsEntry = NULL;

    // dialog properties
    set_title("Preferences");
    set_position(GTK_WIN_POS_CENTER);
    set_modal(true);
    set_border_width(10);
    set_policy(false, false, true);

    // create the Default button
    Gtk::Button* defaultButton = manage(new Gtk::Button("Default"));
    defaultButton->clicked.connect(SigC::slot(this, &PreferencesDialogImpl::defaultButtonCB));
    defaultButton->set_usize(80, -1);
    defaultButton->set_flags(GTK_CAN_DEFAULT);
    defaultButton->show();

    // create the Apply button
    Gtk::Button* applyButton = manage(new Gtk::Button("Apply"));
    applyButton->clicked.connect(SigC::slot(this, &PreferencesDialogImpl::applyButtonCB));
    applyButton->set_usize(80, -1);
    applyButton->set_flags(GTK_CAN_DEFAULT);
    applyButton->show();

    // create the OK button and make it default
    Gtk::Button* okButton = manage(new Gtk::Button("OK"));
    okButton->clicked.connect(SigC::slot(this, &PreferencesDialogImpl::okButtonCB));
    okButton->set_usize(80, -1);
    okButton->set_flags(GTK_CAN_DEFAULT);
    okButton->grab_default();
    okButton->show();

    // create the Cancel button
    Gtk::Button* cancelButton = manage(new Gtk::Button("Cancel"));
    cancelButton->clicked.connect(SigC::slot(this, &PreferencesDialogImpl::cancelButtonCB));
    cancelButton->set_usize(80, -1);
    cancelButton->set_flags(GTK_CAN_DEFAULT);
    cancelButton->show();

    // add the buttons to the hbox
    Gtk::HBox* hbox = get_action_area();
    hbox->set_spacing(10);
    hbox->pack_start(*defaultButton, false, false);
    hbox->pack_start(*applyButton, false, false);
    hbox->pack_start(*okButton, false, false);
    hbox->pack_start(*cancelButton, false, false);

    // editor stuff
    Gtk::HBox* editorBox = manage(new Gtk::HBox());
    Gtk::Label* editorLabel = manage(new Gtk::Label("Editor Command: "));
    editorLabel->show();
    editorBox->pack_start(*editorLabel, false, false);
    m_editorEntry = manage(new Gtk::Entry());
    m_editorEntry->set_text(g_configMap[Config::EDITOR[0]]);
    m_editorEntry->show();
    editorBox->pack_end(*m_editorEntry);
    editorBox->show();

    // ctags stuff
    Gtk::HBox* ctagsBox = manage(new Gtk::HBox());
    Gtk::Label* ctagsLabel = manage(new Gtk::Label("Ctags Command: "));
    ctagsLabel->show();
    ctagsBox->pack_start(*ctagsLabel, false, false);
    m_ctagsEntry = manage(new Gtk::Entry());
    m_ctagsEntry->set_text(g_configMap[Config::CTAGS[0]]);
    m_ctagsEntry->show();
    ctagsBox->pack_end(*m_ctagsEntry);
    ctagsBox->show();

    // add everything to the vbox
    Gtk::VBox* vbox = get_vbox();
    vbox->set_spacing(5);
    vbox->pack_start(*editorBox, false, false);
    vbox->pack_start(*ctagsBox, false, false);

    show();
    Gtk::Main::run();
}


void PreferencesDialogImpl::savePreferences()
{
    g_configMap[Config::EDITOR[0]] = m_editorEntry->get_text();
    g_configMap[Config::CTAGS[0]] = m_ctagsEntry->get_text();

    Config::write();
}


void PreferencesDialogImpl::defaultButtonCB()
{
    Config::defaultConfig();

    m_editorEntry->set_text(g_configMap[Config::EDITOR[0]]);
    m_ctagsEntry->set_text(g_configMap[Config::CTAGS[0]]);
}


void PreferencesDialogImpl::applyButtonCB()
{
    savePreferences();
}


void PreferencesDialogImpl::okButtonCB()
{
    savePreferences();
    Gtk::Main::quit();
}


void PreferencesDialogImpl::cancelButtonCB()
{
    Gtk::Main::quit();
}
