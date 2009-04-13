#include "gtkmm.h"
#include <libnotifymm.h>
#include <iostream>

ExampleWindow::ExampleWindow()
  : m_ButtonBox(Gtk::BUTTONBOX_SPREAD, 2),
    m_Button_blink("blink"),
    m_Button_hide("hide"),
    m_Button_quit("quit")
{
  set_title("Gtk::StatusIcon example");
  set_border_width(5);
  set_default_size(120, 100);
  
  // Setting up the UIManager:
  Glib::RefPtr<Gtk::ActionGroup> refActionGroup = Gtk::ActionGroup::create();
  refActionGroup->add( 
    Gtk::ToggleAction::create("Toggle0", "enable something", "", true) );

  refActionGroup->add( 
    Gtk::ToggleAction::create("Toggle1", "enable something else") );

  refActionGroup->add( 
    Gtk::Action::create("Preferences", Gtk::Stock::PREFERENCES),
    sigc::bind(sigc::ptr_fun(&ExampleWindow::on_menuitem_selected),
               "Preferences") );

  refActionGroup->add(
    Gtk::Action::create("Info", Gtk::Stock::INFO),
    sigc::bind(sigc::ptr_fun(&ExampleWindow::on_menuitem_selected),
               "Information") );

  refActionGroup->add(
    Gtk::Action::create("Help", Gtk::Stock::HELP),
    sigc::bind(sigc::ptr_fun(&ExampleWindow::on_menuitem_selected),
               "Help") );

  refActionGroup->add( 
    Gtk::Action::create("Quit", Gtk::Stock::QUIT),
    sigc::mem_fun(*this, &ExampleWindow::hide) );

  m_refUIManager = Gtk::UIManager::create();
  m_refUIManager->insert_action_group(refActionGroup);

  Glib::ustring ui_info =
    "<ui>"
    "  <popup name='Popup'>"
    "    <menuitem action='Toggle0' />"
    "    <menuitem action='Toggle1' />"
    "    <menuitem action='Preferences' />"
    "    <separator/>"
    "    <menuitem action='Info' />"
    "    <menuitem action='Help' />"
    "    <separator/>"
    "    <menuitem action='Quit' />"
    "  </popup>"
    "</ui>";

  m_refUIManager->add_ui_from_string(ui_info);

  // Setting up the StatusIcon:
  // You should use your own icon in real life.
  m_refStatusIcon = Gtk::StatusIcon::create(Gtk::Stock::HOME);
  m_refStatusIcon->set_tooltip("gtkmm StatusIcon example tooltip");

  n = new Notify::Notification("JBL", "Something","appointment-new", m_refStatusIcon);
  n->set_timeout(3000); //3 seconds
  // StatusIcon's signals
  m_refStatusIcon->signal_activate().connect(
    sigc::mem_fun(*this, &ExampleWindow::toggle_hide) );

  m_refStatusIcon->signal_popup_menu().connect(
    sigc::mem_fun(*this, &ExampleWindow::on_statusicon_popup) );

  // Buttons' signals:
  m_Button_blink.signal_clicked().connect( 
    sigc::mem_fun(*this, &ExampleWindow::on_blink_clicked) );

  m_Button_hide.signal_clicked().connect(
    sigc::mem_fun(*this, &ExampleWindow::toggle_hide) );

  m_Button_quit.signal_clicked().connect( 
    sigc::mem_fun(*this, &ExampleWindow::hide) );

  // Packing:
  m_ButtonBox.pack_start(m_Button_blink);
  m_ButtonBox.pack_start(m_Button_hide);
  m_ButtonBox.pack_start(m_Button_quit);

  add(m_ButtonBox);

  Notify::init("Example JB");
  show_all_children();
}

ExampleWindow::~ExampleWindow()
{
}

// Hide the window by Gdk::Window::hide() or re-shows it by Gtk::Window::show()
void ExampleWindow::toggle_hide()
{
  if ( get_window()->is_visible() ) {
    get_position(window_position.first, window_position.second);
    get_window()->hide();
  }
  else {
    get_window()->show();
    move(window_position.first, window_position.second); // move it at 
    // the previous
    // position
  }
}  

// Activate blinking on StatusIcon
void ExampleWindow::on_blink_clicked()
{
  try
  {
    n->show();
  }
  catch (Glib::Error& err)
  {
  }

  if ( m_refStatusIcon->get_blinking() )
    return;

  // Stop the blinking mode:
  struct TimeOut {
    static bool disactive_blinking(Glib::RefPtr<Gtk::StatusIcon>& refSI)
      {
        refSI->set_blinking(false);
        return false; // disconnect the timeout signal
      }
  };

  Glib::signal_timeout().connect(
    sigc::bind(sigc::ptr_fun(&TimeOut::disactive_blinking),
               m_refStatusIcon), 3000);
  
  m_refStatusIcon->set_blinking();
}

// To close ExampleWindow you must click on Quit,
// delete_event signal just hides it.
bool ExampleWindow::on_delete_event(GdkEventAny*)
{
  toggle_hide();
  return true;
}

// This shows an example popup menu:
void ExampleWindow::on_statusicon_popup(guint button, guint32 time)
{
  Gtk::Menu* pMenu = static_cast<Gtk::Menu*>( 
    m_refUIManager->get_widget("/Popup") );

  m_refStatusIcon->popup_menu_at_position(*pMenu, button, time);
}

void ExampleWindow::on_menuitem_selected(const Glib::ustring& item) //static
{
  g_print("You have selected `%s'\n", item.data());
}

int main(int argc, char** argv)
{
  Gtk::Main kit(argc, argv);

  ExampleWindow window;
  Gtk::Main::run(window);
}
