/*------------------------------------*
 * Program: Enigma in the Wine Cellar *
 * Version: 5.0 for Linux OS          *
 * File:    GameDialog.h              *
 * Date:    October 12, 2015          *
 * Author:  Chris Sterne              *
 *                                    *
 * GameDialog class header.           *
 *------------------------------------*/
 
#ifndef __GAMEDIALOG_H__
#define __GAMEDIALOG_H__

#include <gtkmm.h>

class CGameDialog : public Gtk::Dialog
{
	public:
		enum class ID
		{
			ELoad = 0,
			ESave,
			ERestart
		};
		
		// Public methods.

		CGameDialog( Gtk::Window& aParent );
		Gtk::ResponseType Run( CGameDialog::ID aID );

	private:
		// Private data.

		std::unique_ptr<Gtk::Label> iMessage;      // Item message widget.
};

#endif // __GAMEDIALOG_H__