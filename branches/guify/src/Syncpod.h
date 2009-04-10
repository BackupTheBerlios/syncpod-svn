// Copyright (C) 2009 by Jean-Baptiste LAB

#ifndef GNOMESYNCPOD_H
#define GNOMESYNCPOD_H

#include <gtkmm.h>

namespace GnomeSyncpod
{
  class Syncpod : public Gtk::Window 
  {
    Glib::RefPtr<Gtk::StatusIcon>  m_refStatusIcon;
    Glib::RefPtr<Gtk::UIManager>   m_refUIManager;
    std::pair<int, int>  window_position;

    bool m_showAtStartup;
    bool m_minimizeToTray;
  
    void toggleVisible();
    void minimize();  
  public:
    Syncpod();
    virtual ~Syncpod();
    bool showAtStartup();
    bool minimizeToTray();
  };
}

#endif
