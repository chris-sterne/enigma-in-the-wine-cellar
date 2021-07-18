// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2016, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the AboutView class header.  The AboutView class displays
// information about the application (name, version, author, etc.).
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
