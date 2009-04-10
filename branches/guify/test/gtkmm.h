// -*- c++ -*-

#ifndef GTKMM_EXAMPLEWINDOW_H
#define GTKMM_EXAMPLEWINDOW_H

#include <gtkmm.h>
#include <libnotifymm.h>

class ExampleWindow : public Gtk::Window 
{
public:
  ExampleWindow ();
  virtual ~ExampleWindow ();

protected:
  virtual void toggle_hide ();

  //Signal handlers:
  virtual  void  on_blink_clicked     ();
  virtual  bool  on_delete_event      (GdkEventAny*);
  virtual  void  on_statusicon_popup  (guint button, guint32 time);
  static   void  on_menuitem_selected (const Glib::ustring& item_name);

  // Child widgets:
  Gtk::VButtonBox  m_ButtonBox;
  Gtk::Button      m_Button_blink;
  Gtk::Button      m_Button_hide; 
  Gtk::Button      m_Button_quit;

  Glib::RefPtr<Gtk::StatusIcon>  m_refStatusIcon;
  Glib::RefPtr<Gtk::UIManager>   m_refUIManager;

  std::pair<int, int>  window_position;
  Notify::Notification *n;
};

#endif // GTKMM_EXAMPLEWINDOW_H

