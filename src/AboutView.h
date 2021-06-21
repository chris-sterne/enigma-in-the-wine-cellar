/*--------------------------------------------*
 * Program: Enigma in the Wine Cellar         *
 * Version: 2.0 for Linux OS                  *
 * File:    AboutView.cpp                     *
 * Date:    January 26, 2016                  *
 * Author:  Chris Sterne                      *
 *                                            *
 * AboutView class header.                    *
 *--------------------------------------------*/

#ifndef __ABOUTVIEW_H__
#define __ABOUTVIEW_H__

#include <gtkmm.h>

class CAboutView : public Gtk::Grid
{
  public:
    // Public methods.

    CAboutView();
    void On_Done();

    // View done signal accessor.
		
    typedef sigc::signal<void> type_signal_done;
    type_signal_done signal_done();

    private:
      // Private data.

      type_signal_done m_signal_done;     // View done signal server.
};

#endif // __ABOUTVIEW_H__