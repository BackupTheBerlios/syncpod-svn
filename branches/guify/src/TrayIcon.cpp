// Copyright (C) 2009 by Jean-Baptiste LAB

#include "TrayIcon.h"
#include "I_TrayObserver.h"

using namespace GnomeSyncpod;

TrayIcon::TrayIcon() : m_pObserver(0)
{
  m_refStatusIcon = Gtk::StatusIcon::create(Gtk::Stock::HOME);

  // Setting up the UIManager:
  Glib::RefPtr<Gtk::ActionGroup> refActionGroup = Gtk::ActionGroup::create();
  refActionGroup->add( 
    Gtk::Action::create("Preferences", Gtk::Stock::PREFERENCES),
    sigc::bind(sigc::mem_fun(*this, &TrayIcon::on_menuitem_selected),
               "Preferences") );

//   refActionGroup->add(
//     Gtk::Action::create("Info", Gtk::Stock::INFO),
//     sigc::bind(sigc::ptr_fun(&ExampleWindow::on_menuitem_selected),
//                "Information") );

//   refActionGroup->add(
//     Gtk::Action::create("Help", Gtk::Stock::HELP),
//     sigc::bind(sigc::ptr_fun(&ExampleWindow::on_menuitem_selected),
//                "Help") );

//   refActionGroup->add( 
//     Gtk::Action::create("About", Gtk::Stock::QUIT),
//     sigc::mem_fun(*this, &ExampleWindow::hide) );

//   refActionGroup->add( 
//     Gtk::Action::create("Quit", Gtk::Stock::QUIT),
//     sigc::mem_fun(*this, &ExampleWindow::hide) );

  m_refUIManager = Gtk::UIManager::create();
  m_refUIManager->insert_action_group(refActionGroup);

  Glib::ustring ui_info =
    "<ui>"
    "  <popup name='Popup'>"
    "    <menuitem action='Preferences' />"
    "    <separator/>"
    "    <separator/>"
    "  </popup>"
    "</ui>";

  m_refUIManager->add_ui_from_string(ui_info);

  m_refStatusIcon->signal_popup_menu().connect(
    sigc::mem_fun(*this, &TrayIcon::on_statusicon_popup) );

}

TrayIcon::~TrayIcon()
{
}

void TrayIcon::on_menuitem_selected (const Glib::ustring& item_name)
{
  if(m_pObserver)
    m_pObserver->on_menuitem_selected(item_name);
}

void TrayIcon::on_statusicon_popup(guint button, guint32 time)
{
  Gtk::Menu* pMenu = static_cast<Gtk::Menu*>( 
    m_refUIManager->get_widget("/Popup") );

  m_refStatusIcon->popup_menu_at_position(*pMenu, button, time);
}

void TrayIcon::set_tooltip (const Glib::ustring& tooltip_text)
{
  m_refStatusIcon->set_tooltip(tooltip_text);
}

void TrayIcon::set_observer(const I_TrayObserver *observer)
{
  m_pObserver = observer;
}
