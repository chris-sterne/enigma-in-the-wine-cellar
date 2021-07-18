// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2016, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the ErrorDialog class implementation.  The ErrorDialog class
// displays a dialog with an error message.
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
#include "ErrorDialog.h"

//*---------------------*
//* Local declarations. *
//*---------------------*

static const char* KNoOpenGLMessage = _("No OpenGL Context");
static const char* KUnknown         = "???";

//*----------------------*
//* Default constructor. *
//*----------------------*

CErrorDialog::CErrorDialog()
{
	// Initialize the dialog.

	set_title( _("Error") );
	set_resizable( FALSE );
	add_button(_("Ok"), Gtk::RESPONSE_OK );
	set_modal( TRUE );

	// Keep a pointer to the label so it can be easily accessed later
	// to change its contents.

	iMessage = std::unique_ptr<Gtk::Label>( new Gtk::Label() );

	// Add extra dialog widgets to the content area.
	
	Gtk::Box* Content = get_content_area();
  Content->add( *iMessage );
  Content->set_border_width( 5 );
	return;
}

//*----------------------------------------*
//* This method displays the Error dialog. *
//*----------------------------------------*

Gtk::ResponseType CErrorDialog::Run( CErrorDialog::ID aID )
{
	switch( aID )
	{
		case CErrorDialog::ID::ENoOpenGL:
			iMessage->set_label( KNoOpenGLMessage );
			break;

		default:
			iMessage->set_label( KUnknown );
			break;
	}

	// show_all() must be called to display all added widgets.
	
	show_all();

	// Pass the method up to the base class.  The dialog will block input
	// until the "Close" button is pressed.  The dialog is then hidden
	// until needed again.
  
	Gtk::ResponseType Response = (Gtk::ResponseType)Gtk::Dialog::run();
	hide();

  return Response;
}
