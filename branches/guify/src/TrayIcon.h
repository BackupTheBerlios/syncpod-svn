// Copyright (C) 2009 by Jean-Baptiste LAB


#ifndef TRAYICON_H
#define TRAYICON_H

#include <gtkmm.h>

namespace GnomeSyncpod
{
  class I_TrayObserver;

  class TrayIcon : public Gtk::StatusIcon
  {
  private:
    Glib::RefPtr<Gtk::StatusIcon>  m_refStatusIcon;
    Glib::RefPtr<Gtk::UIManager>   m_refUIManager;
    const I_TrayObserver *m_pObserver;
    
    void  on_menuitem_selected (const Glib::ustring& item_name);
  public:
    TrayIcon();
    virtual ~TrayIcon();
    void on_statusicon_popup(guint button, guint32 time);
    void set_tooltip(const Glib::ustring& tooltip_text);
    void set_observer(const I_TrayObserver *observer);
  };
}

#endif
