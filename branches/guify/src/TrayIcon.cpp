// Copyright (C) 2009 by Jean-Baptiste LAB

#include "TrayIcon.h"
#include "I_TrayObserver.h"

using namespace GnomeSyncpod;

TrayIcon::TrayIcon() : m_pMenu(0),m_pObserver(0)
{
  m_refStatusIcon = Gtk::StatusIcon::create(Gtk::Stock::HOME);
  m_refStatusIcon->signal_popup_menu().connect(
    sigc::mem_fun(*this, &TrayIcon::on_statusicon_popup) );
  m_refStatusIcon->signal_activate().connect(
    sigc::mem_fun(*this, &TrayIcon::on_clicked) );
}

void TrayIcon::set_menu(Gtk::Widget* pMenu)
{
  m_pMenu = pMenu;
}

TrayIcon::~TrayIcon()
{
}

void TrayIcon::on_clicked()
{
  if(m_pObserver)
    m_pObserver->on_trayicon_clicked();
}

void TrayIcon::on_statusicon_popup(guint button, guint32 time)
{
  if(m_pMenu)
    m_refStatusIcon->popup_menu_at_position(*((Gtk::Menu*)m_pMenu), button, time);
}

void TrayIcon::set_tooltip (const Glib::ustring& tooltip_text)
{
  m_refStatusIcon->set_tooltip(tooltip_text);
}

void TrayIcon::set_observer(I_TrayObserver *observer)
{
  m_pObserver = observer;
}
