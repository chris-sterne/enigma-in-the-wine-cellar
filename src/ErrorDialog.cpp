/*-----------------------------------------------------*
 * Program: Enigma in the Wine Cellar                  *
 * Version: 3.0 for Linux OS                           *
 * File:    ErrorDialog.cpp                            *
 * Date:    June 25, 2015                              *
 * Author:  Chris Sterne                               *
 *                                                     *
 * ErrorDialog class.                                  *
 *-----------------------------------------------------*
 * This class displays a dialog with an error message. *
 *-----------------------------------------------------*/

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