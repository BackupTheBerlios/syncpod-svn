// Copyright (C) 2009 by Jean-Baptiste LAB

#ifndef I_TRAYOBSERVER_H
#define I_TRAYOBSERVER_H

namespace GnomeSyncpod
{
  class I_TrayObserver
  {
  public:
    virtual void on_trayicon_clicked() = 0;
  };
}

#endif
