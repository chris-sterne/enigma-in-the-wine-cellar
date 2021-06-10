// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the HelpView class header.  The HelpView class displays
// the game's objective and instructions. 
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

#ifndef __HELPVIEW_H__
#define __HELPVIEW_H__

#include <gtkmm/builder.h>
#include <gtkmm/box.h>

namespace Enigma
{
	class HelpView : public Gtk::Box
	{
		public:
		  // Public methods.

		  HelpView();
			void on_done();

			// View done signal accessor.
			
			typedef sigc::signal<void> type_signal_done;
			type_signal_done signal_done();
			
		private:
			// Private data.
			
			Glib::RefPtr<Gtk::Builder> m_builder;  // User interface.
			type_signal_done m_signal_done;        // View done signal server.
	};
}

#endif // __HELPVIEW_H__
