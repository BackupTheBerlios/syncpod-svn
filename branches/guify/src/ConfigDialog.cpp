// Copyright (C) 2009 by Jean-Baptiste LAB

#include "ConfigDialog.h"

using namespace GnomeSyncpod;

ConfigDialog::ConfigDialog() : m_ok("OK"),
                               m_cancel("Cancel")
{
  set_title("Gnome Syncpod Config");
  set_default_size(100,100);

  add(m_box);
  
// Connect the clicked signal of the button to
//   m_ok.signal_clicked().connect(sigc::mem_fun(*this,
//                                               &ConfigDialok::on_ok_button_clicked) );
//   m_box.add(m_ok);
//   m_box.add(m_cancel);
  m_box.pack_start(m_ok);
  m_box.pack_start(m_cancel);
  
// Make the button the default widget
  m_ok.set_flags(Gtk::CAN_DEFAULT);
  m_ok.grab_default();

  show_all_children();
}

ConfigDialog::~ConfigDialog()
{
}
