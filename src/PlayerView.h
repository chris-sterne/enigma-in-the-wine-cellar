// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the PlayerView class header.  The PlayerView class displays
// the player's location and visible surroundings within a game map.
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

#ifndef __ENIGMA_PLAYERVIEW_H__
#define __ENIGMA_PLAYERVIEW_H__

#include <gtkmm/box.h>
#include <gtkmm/builder.h>
#include <gtkmm/label.h>
#include <gtkmm/glarea.h>
#include <gdkmm/event.h>
#include "World.h"
#include "UserAction.h"

namespace Enigma
{
	class PlayerView : public Gtk::Box
	{
		public:
		  // Public methods.

			PlayerView();
			void set_player(std::shared_ptr<Enigma::Player>& player);
			void set_location(const Enigma::Location& location);
			
			// "startup done" signal accessor.

			typedef sigc::signal<void> type_signal_startup_done;
			type_signal_startup_done signal_startup_done();
			
		private:
			// Private methods.

			void on_realize_area();
			bool render_startup(const Glib::RefPtr<Gdk::GLContext>& context);
			void on_startup_done();

			// Private data.

			Glib::RefPtr<Gtk::Builder> m_builder;   // User interface.
			Gtk::Label* m_location;                 // Pointer to location display.
			Gtk::GLArea* m_area;                    // Pointer to view area.
			Enigma::UserAction m_user_action;       // User interface actions.
			
			// Signal connections between active player.
			
			sigc::connection m_connection_render;
			sigc::connection m_connection_action;
			sigc::connection m_connection_queue_render;
			sigc::connection m_connection_location;
			
			// Startup done signal server.
			
			type_signal_startup_done m_signal_startup_done;        
	};
}

#endif // __ENIGMA_PLAYERVIEW_H__
