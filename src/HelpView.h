/*--------------------------------------------*
 * Program: Enigma in the Wine Cellar         *
 * Version: 2.0 for Linux OS                  *
 * File:    HelpView.cpp                      *
 * Date:    February 5, 2016                  *
 * Author:  Chris Sterne                      *
 *                                            *
 * HelpView class header.                     *
 *--------------------------------------------*/

#ifndef __HELPVIEW_H__
#define __HELPVIEW_H__

#include <gtkmm.h>

class CHelpView : public Gtk::Grid
{
  public:
    // Public methods.

    CHelpView();
		void On_Done();

		// View done signal accessor.
		
	  typedef sigc::signal<void> type_signal_done;
		type_signal_done signal_done();
		
		private:
			// Private data.

			std::unique_ptr<Gtk::TextView> iTextView;   // Help text viewer.
			type_signal_done m_signal_done;             // View done signal server.
};

#endif // __HELPVIEW_H__