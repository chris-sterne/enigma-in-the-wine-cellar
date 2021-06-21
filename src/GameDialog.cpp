/*------------------------------------------------------*
 * Program: Enigma in the Wine Cellar                   *
 * Version: 2.0 for Linux OS                            *
 * File:    GameDialog.cpp                              *
 * Date:    February 9, 2015                            *
 * Author:  Chris Sterne                                *
 *                                                      *
 * GameDialog class.                                    *
 *------------------------------------------------------*
 * This class displays a dialog requesting confirmation *
 * for changes about to be made to the game map.        *
 *------------------------------------------------------*/

#include <glibmm/i18n.h>
#include "GameDialog.h"

//*---------------------*
//* Local declarations. *
//*---------------------*

static const char* KLoadMessage    = _("Load the saved game?");
static const char* KSaveMessage    = _("Save the game?");
static const char* KRestartMessage = _("Restart the game?");
static const char* KUnknown        = "???";

//*----------------------*
//* Default constructor. *
//*----------------------*

CGameDialog::CGameDialog( Gtk::Window& aParent )
{
	// Initialize the dialog.

	set_title( _("Game") );
	set_resizable( FALSE );
	add_button(_("Cancel"), Gtk::RESPONSE_CANCEL );
	add_button(_("Ok"), Gtk::RESPONSE_OK );
	set_modal( TRUE );
  set_transient_for( aParent );

	// Keep a pointer to the label so it can be easily accessed later
	// to change its contents.

	iMessage = std::unique_ptr<Gtk::Label>( new Gtk::Label() );

	// Add extra dialog widgets to the content area.
	
	Gtk::Box* Content = get_content_area();
  Content->add( *iMessage );
  Content->set_border_width( 5 );
	return;
}

//*---------------------------------------*
//* This method displays the Game dialog. *
//*---------------------------------------*

Gtk::ResponseType CGameDialog::Run( CGameDialog::ID aID )
{
	switch( aID )
	{
		case CGameDialog::ID::ELoad:
			iMessage->set_label( KLoadMessage );
			break;

		case CGameDialog::ID::ESave:
			iMessage->set_label( KSaveMessage );
			break;

		case CGameDialog::ID::ERestart:
			iMessage->set_label( KRestartMessage );
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