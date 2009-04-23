// Copyright (C) 2009 by Jean-Baptiste LAB

#include "Syncpod.h"
#include "ConfigDialog.h"

using namespace GnomeSyncpod;

Syncpod::Syncpod() : 
  m_showAtStartup(true),
  m_minimizeToTray(true)
{
  set_title("Gnome Syncpod");
  set_default_size(100,100);

  m_statusIcon.set_tooltip("gnome-syncpod is idle");
  m_statusIcon.set_observer(this);

  m_configDialog.set_transient_for(*this);
  m_configDialog.set_modal(true);
  
  if (m_minimizeToTray)
  {
    signal_hide().connect(sigc::mem_fun(this, &Syncpod::minimize));
  }
  else
  {
    // As we do not use Gtk::Main::run(window)
    signal_hide().connect(sigc::ptr_fun(&Gtk::Main::quit));
  }

  Glib::RefPtr<Gtk::ActionGroup> refActionGroup = Gtk::ActionGroup::create();
  refActionGroup->add( Gtk::Action::create("MenuFile", "_File") );
  refActionGroup->add( Gtk::Action::create("Quit", Gtk::Stock::QUIT),
                         sigc::mem_fun(*this, &Syncpod::on_action_file_quit) );
  refActionGroup->add( Gtk::Action::create("Preferences", Gtk::Stock::PREFERENCES),
                       sigc::bind(sigc::mem_fun(*this, &Syncpod::on_trayicon_menuitem_selected),
                                  "Preferences") );

  m_refUIManager = Gtk::UIManager::create();
  m_refUIManager->insert_action_group(refActionGroup);
  add_accel_group(m_refUIManager->get_accel_group());

  Glib::ustring ui_info =
    "<ui>"
    " <menubar name='MenuBar'>"
    "   <menu action='MenuFile'>"
    "     <menuitem action='Preferences' />"
    "     <separator/>"
    "     <menuitem action='Quit'/>"
    "   </menu>"
    " </menubar>"
    " <toolbar name='ToolBar'>"
    "   <toolitem action='Preferences'/>"
    "   <separator/>"
    "   <toolitem action='Quit'/>"
    " </toolbar>"
    " <popup name='TrayPopup'>"
    "   <menuitem action='Preferences' />"
    "   <separator/>"
    "   <menuitem action='Quit'/>"
    " </popup>"
    "</ui>";
  m_refUIManager->add_ui_from_string(ui_info);

  add(m_box);

  Gtk::Widget* pTrayMenu = m_refUIManager->get_widget("/TrayPopup");
  m_statusIcon.set_menu(pTrayMenu);

  Gtk::Widget* pMenubar = m_refUIManager->get_widget("/MenuBar");
  m_box.pack_start(*pMenubar, Gtk::PACK_SHRINK);

  Gtk::Widget* pToolbar = m_refUIManager->get_widget("/ToolBar") ;
  m_box.pack_start(*pToolbar, Gtk::PACK_SHRINK);
  
  show_all_children();
}

Syncpod::~Syncpod()
{
}

void Syncpod::toggle_visible()
{
  if(is_visible()) 
  {
    get_position(window_position.first, window_position.second);
    hide();
  }
  else
  {
    show();
    move(window_position.first, window_position.second);
  }
}

void Syncpod::minimize()
{
  get_position(window_position.first, window_position.second);
  hide();  
}

void Syncpod::on_action_file_quit()
{
  Gtk::Main::quit();
}

bool Syncpod::showAtStartup()
{
  return m_showAtStartup;
}

bool Syncpod::minimizeToTray()
{
  return m_minimizeToTray;
}

void Syncpod::on_trayicon_menuitem_selected(const Glib::ustring&)
{
  m_configDialog.show();
}

void Syncpod::on_trayicon_clicked()
{
  toggle_visible();
}

int main(int argc, char** argv)
{
  Gtk::Main kit(argc, argv);

  Syncpod window;

  if(window.showAtStartup())
  {
    window.show();
  }
  kit.run();
}
