// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2016, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the Window class implementation.  The Window class
// is the main application window.
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
// more details.
//
// You should have received a copy of the GNU General Public License along
// with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <glibmm/i18n.h>
#include "Window.h"

//*----------------------*
//* Default constructor. *
//*----------------------*

CWindow::CWindow() : Gtk::ApplicationWindow()
{ 
	set_title( _("Enigma in the Wine Cellar") );
  set_default_size( 160*3 + 60*3, 160*3 );
  //set_default_size( 160*3, 160*3 );
  return;
}
