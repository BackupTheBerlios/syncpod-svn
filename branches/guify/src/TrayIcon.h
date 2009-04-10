// Copyright (C) 2009 by Jean-Baptiste LAB


#ifndef TRAYICON_H
#define TRAYICON_H

#include <gtkmm.h>

namespace GnomeSyncpod
{
  class TrayIcon : public Gtk::StatusIcon
  {
  public:
    TrayIcon();
    virtual ~TrayIcon();
  };
}

#endif
