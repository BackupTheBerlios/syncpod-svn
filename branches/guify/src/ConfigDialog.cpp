// Copyright (C) 2009 by Jean-Baptiste LAB

#include "ConfigDialog.h"

using namespace GnomeSyncpod;

ConfigDialog::ConfigDialog()
{
  set_title("Gnome Syncpod Config");
  set_default_size(100,100);

  get_vbox()->pack_start(m_box);
  add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  add_button(Gtk::Stock::OK,     Gtk::RESPONSE_ACCEPT);
  set_default_response(Gtk::RESPONSE_ACCEPT);

  get_vbox()->show_all();
}

ConfigDialog::~ConfigDialog()
{
}
