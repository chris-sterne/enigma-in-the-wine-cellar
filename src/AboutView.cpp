/*----------------------------------------------*
 * Program: Enigma in the Wine Cellar           *
 * Version: 5.0.1 for Linux OS                  *
 * File:    AboutView.cpp                       *
 * Date:    November 21, 2016                   *
 * Author:  Chris Sterne                        *
 *                                              *
 * AboutView class.                             *
 *----------------------------------------------*
 * This class displays application information. *
 *----------------------------------------------*/

#include <glibmm/i18n.h>
#include "AboutView.h"

//*---------------------*
//* Local declarations. *
//*---------------------*

static const Glib::ustring KCopyrightText =
_("By Chris Sterne\n\
Copyright 2005, 2016");

static const Glib::ustring KDescriptionText =
_("This FREEWARE exploration puzzle game was ported to the Linux OS from the \
Palm OS and Nokia Symbian OS versions.  OpenGL (Mesa 3-D graphics library) is \
used for image rendering within a custom GTK+ widget.  The 3D image meshes \
were created with Blender 2.76b, and exported as Stanford PLY v1.0 files.  \
The sound effects were created using Audacity 2.1.2, and are played using the \
Libcanberra sound library.  The game was written in C++ using Anjuta 3.18.2 \
IDE, with GTK+ 3.18 for the user interface.");

//*--------------------------*
//* C++ default constructor. *
//*--------------------------*

CAboutView::CAboutView()
{
  Gtk::Image* Image1 = Gtk::manage( new Gtk::Image );
  Image1->set_from_resource( "/org/game/EnigmaWC/Bitmaps/About.png" );
  Image1->set_hexpand(TRUE);
  Image1->set_halign( Gtk::ALIGN_END );
  Image1->set_padding( 0, 0 );
	
  Gtk::Image* Image2 = Gtk::manage( new Gtk::Image );
  Image2->set_from_resource( "/org/game/EnigmaWC/Bitmaps/About.png" );
  Image2->set_hexpand(TRUE);
  Image2->set_halign( Gtk::ALIGN_START );
  Image2->set_padding( 0, 0 );
	
  Gtk::Box* Box = Gtk::manage( new Gtk::Box( Gtk::ORIENTATION_VERTICAL ));
	
  Gtk::Label* Title = Gtk::manage( new Gtk::Label );
  Title->set_markup( _("<b>Enigma in the Wine Cellar</b>") );
  Title->set_hexpand(TRUE);
  Title->set_halign( Gtk::ALIGN_CENTER );
  Title->set_padding( 0, 3 );

  Gtk::Label* Version = Gtk::manage( new Gtk::Label );
  Version->set_markup( _("<b>Version 5.0.1</b>") );
  Version->set_hexpand(TRUE);
  Version->set_halign( Gtk::ALIGN_CENTER );
	
  Box->pack_start( *Title, FALSE, 0 );
  Box->pack_start( *Version, FALSE, 0 );
	
  attach( *Image1, 0, 0, 1, 1 );
  attach( *Box,    1, 0, 1, 1 );
  attach( *Image2, 2, 0, 1, 1 );

  // Prepare the copyright text label.
	
  Gtk::Label* Copyright = Gtk::manage( new Gtk::Label );
  Copyright->set_padding( 0, 3 );
  Copyright->set_markup( KCopyrightText );
  Copyright->set_halign( Gtk::ALIGN_CENTER );
  Copyright->set_hexpand(TRUE);

  attach( *Copyright, 1, 1, 1, 1 );

  // Prepare the Website URL button.
	
  Gtk::LinkButton* LinkButton = Gtk::manage( new Gtk::LinkButton );
  LinkButton->set_uri( "http://enigmawc.shawwebspace.ca/" );
  LinkButton->set_label( _("Website") );
  LinkButton->set_halign( Gtk::ALIGN_CENTER );
  LinkButton->set_hexpand(FALSE);

  attach( *LinkButton, 1, 2, 1, 1 );
	
  // Prepare the description TextView widget.
	
  Gtk::TextView* TextView = Gtk::manage( new Gtk::TextView() );
  TextView->set_wrap_mode( Gtk::WRAP_WORD );
  TextView->set_editable(FALSE);
  TextView->set_cursor_visible(FALSE);
  TextView->set_hexpand(TRUE);
  TextView->set_vexpand(TRUE);
	
  // Add description text to the TextView built-in buffer.
	
  Glib::RefPtr<Gtk::TextBuffer> Buffer = TextView->get_buffer();
  Buffer->set_text( KDescriptionText );

  // Add the TextView widget to a ScrolledWindow.
  // Scrollbar linking and control is added automatically.
	
  Gtk::ScrolledWindow* ScrolledWindow = Gtk::manage( new Gtk::ScrolledWindow() );
  ScrolledWindow->add( *TextView );
    
  Gtk::Button* Button = Gtk::manage( new Gtk::Button( _("  Done  "), false ));
  Button->set_halign(Gtk::ALIGN_END);    
  Button->set_relief(Gtk::RELIEF_NORMAL);
  Button->set_border_width (10);
        
  attach( *ScrolledWindow, 0, 3, 3, 1);
  attach( *Button,         0, 4, 3, 1);

  // Connect to the button clicked signal, which will cause the AboutView widget
  // to emit a "Done" signal. 
	
  Button->signal_clicked().connect( sigc::mem_fun( *this, &CAboutView::On_Done ));
  return;
}

//*--------------------------------------------------*
//* This method returns the view done signal server. *
//*--------------------------------------------------*

CAboutView::type_signal_done CAboutView::signal_done()
{
  return m_signal_done;
}

//*-----------------------------------------------------*
//* This method is called the "Done" button is pressed. *
//*-----------------------------------------------------*

void CAboutView::On_Done()
{	
  m_signal_done.emit();
  return;
}
