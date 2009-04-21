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
    Gtk::Widget* m_pMenu;
    I_TrayObserver *m_pObserver;
    
    void on_clicked();
    void on_statusicon_popup(guint, guint32);
 public:
    TrayIcon();
    virtual ~TrayIcon();
    void set_tooltip(const Glib::ustring& tooltip_text);
    void set_observer(I_TrayObserver *observer);
    void set_menu(Gtk::Widget*);
  };
}

#endif
