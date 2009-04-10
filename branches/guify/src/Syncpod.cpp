// Copyright (C) 2009 by Jean-Baptiste LAB

#include "Syncpod.h"

using namespace GnomeSyncpod;

Syncpod::Syncpod() : 
  m_showAtStartup(true),
  m_minimizeToTray(true)
{
  set_title("Gtk::StatusIcon example");
  set_border_width(5);
  set_default_size(100,100);

  m_refStatusIcon = Gtk::StatusIcon::create(Gtk::Stock::HOME);
  m_refStatusIcon->set_tooltip("gtkmm StatusIcon example tooltip");

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

bool Syncpod::showAtStartup()
{
  return m_showAtStartup;
}

bool Syncpod::minimizeToTray()
{
  return m_minimizeToTray;
}

void Syncpod::minimize()
{
  get_position(window_position.first, window_position.second);
  get_window()->hide();  
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

int main(int argc, char** argv)
{
  Gtk::Main kit(argc, argv);

  Syncpod window;

  if(window.showAtStartup())
  {
    window.show();
    kit.run();
  }
  else
  {
    kit.run(window);
  }
}
