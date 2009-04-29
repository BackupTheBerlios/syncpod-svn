// Copyright (C) 2009 by Jean-Baptiste LAB

#ifndef GNOMESYNCPOD_H
#define GNOMESYNCPOD_H

#include <gtkmm.h>

#include "TrayIcon.h"
#include "I_TrayObserver.h"
#include "ConfigDialog.h"

namespace GnomeSyncpod
{
  class Syncpod : public Gtk::Window, public I_TrayObserver
  {
    TrayIcon m_statusIcon;
    ConfigDialog m_configDialog;
    Glib::RefPtr<Gtk::UIManager>   m_refUIManager;
    std::pair<int, int>  window_position;
    Gtk::VBox m_box;

    bool m_showAtStartup;
    bool m_minimizeToTray;
  
    void toggle_visible();
    void minimize();  
    void on_action_file_quit();
    void on_preferences_ok(int);
    virtual void on_trayicon_menuitem_selected(const Glib::ustring& item_name);
  public:
    Syncpod();
    virtual ~Syncpod();
    bool showAtStartup();
    bool minimizeToTray();
    virtual void on_trayicon_clicked();
  };
}

#endif
