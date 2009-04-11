// Copyright (C) 2009 by Jean-Baptiste LAB

#include "Syncpod.h"

using namespace GnomeSyncpod;

Syncpod::Syncpod() : 
  m_showAtStartup(false),
  m_minimizeToTray(true)
{
  set_title("Gtk::StatusIcon example");
  set_border_width(5);
  set_default_size(100,100);

  m_statusIcon.set_tooltip("gnome-syncpod is idle");
  m_statusIcon.set_observer(this);
  
  if (m_minimizeToTray)
  {
    signal_hide().connect(sigc::mem_fun(this, &Syncpod::minimize));
  }
  else
  {
    // As we do not use Gtk::Main::run(window)
    signal_hide().connect(sigc::ptr_fun(&Gtk::Main::quit));
  }
}

Syncpod::~Syncpod()
{
}

void Syncpod::toggleVisible()
{
  if(get_window()->is_visible()) 
  {
    get_position(window_position.first, window_position.second);
    get_window()->hide();
  }
  else
  {
    get_window()->show();
    move(window_position.first, window_position.second);
  }
}

void Syncpod::minimize()
{
  get_position(window_position.first, window_position.second);
  get_window()->hide();  
}

bool Syncpod::showAtStartup()
{
  return m_showAtStartup;
}

bool Syncpod::minimizeToTray()
{
  return m_minimizeToTray;
}

void Syncpod::on_menuitem_selected(const Glib::ustring& item_name)
{
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
