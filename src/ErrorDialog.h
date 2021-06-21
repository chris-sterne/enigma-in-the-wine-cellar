/*------------------------------------*
 * Program: Enigma in the Wine Cellar *
 * Version: 3.0 for Linux OS          *
 * File:    ErrorDialog.h             *
 * Date:    June 25, 2016             *
 * Author:  Chris Sterne              *
 *                                    *
 * ErrorDialog class header.          *
 *------------------------------------*/
 
#ifndef __ERRORDIALOG_H__
#define __ERRORDIALOG_H__

#include <gtkmm.h>

class CErrorDialog : public Gtk::Dialog
{
	public:
		enum class ID
		{
			ENoOpenGL = 0
		};
		
		// Public methods.

		CErrorDialog();
		Gtk::ResponseType Run( CErrorDialog::ID aID );

	private:
		// Private data.

		std::unique_ptr<Gtk::Label> iMessage;      // Error message widget.
};

#endif // __ERRORDIALOG_H__