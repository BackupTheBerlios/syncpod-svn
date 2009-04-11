// Copyright (C) 2009 by Jean-Baptiste LAB

#ifndef GNOMESYNCPOD_H
#define GNOMESYNCPOD_H

#include <gtkmm.h>

#include "TrayIcon.h"
#include "I_TrayObserver.h"

namespace GnomeSyncpod
{
  class Syncpod : public Gtk::Window, public I_TrayObserver
  {
    TrayIcon m_statusIcon;
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
    virtual void on_menuitem_selected(const Glib::ustring& item_name);
  };
}

#endif
