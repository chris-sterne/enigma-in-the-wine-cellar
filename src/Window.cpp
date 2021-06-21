//*--------------------------------------------*
//* Program: Enigma in the Wine Cellar         *
//* Version: 5.0 for Linux OS                  *
//* File:    Window.cpp                        *
//* Date:    October 12, 2016                  *
//* Author:  Chris Sterne                      *
//*--------------------------------------------*
//* This class is the main application window. *
//*--------------------------------------------*

#include <glibmm/i18n.h>
#include "Window.h"

//*----------------------*
//* Default constructor. *
//*----------------------*

CWindow::CWindow() : Gtk::ApplicationWindow()
{ 
	set_title( _("Enigma in the Wine Cellar") );
  set_default_size( 160*3, 160*3 );
  return;
}
