// Copyright (C) 2009 by Jean-Baptiste LAB

#ifndef CONFIGDIALGOG_H
#define CONFIGDIALGOG_H

#include <gtkmm.h>

namespace GnomeSyncpod
{
  class ConfigDialog : public Gtk::Window
  {
    Gtk::VBox m_box;
    Gtk::Button m_ok;
    Gtk::Button m_cancel;
  public:
    ConfigDialog();
    virtual ~ConfigDialog();
  };
}

#endif
