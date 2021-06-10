// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the Application class header.  The Application class serves
// as a foundation on which the game is constructed.
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
 
#ifndef __ENIGMA_APPLICATION_H__
#define __ENIGMA_APPLICATION_H__

#include <gtkmm/builder.h>
#include <gtkmm/application.h>
#include <gtkmm/dialog.h>
#include <gtkmm/stack.h>
#include "PlayerView.h"
#include "ItemsView.h"
#include "WorldsView.h"
#include "HelpView.h"
#include "AboutView.h"

namespace Enigma
{
	class Application : public Gtk::Application
	{	
		public:
			// Public methods.
		
			Application();

		protected:
			// Overridden base class methods.
		
			void on_activate() override;
		
		private:		
			// Private methods.
			
			void do_shutdown();
			void show_items();
			void show_worlds();
			void show_help();
			void show_about();
			void show_player();
			void load_world(const std::string& filename);
			void on_activate_finalize();
			void on_restart_response(int response_id);

			// Private data.

			Glib::RefPtr<Gtk::Builder> m_builder;      // User interface.
			Gtk::Stack* m_view_pager;                  // View pager widget.
			Gtk::Dialog* m_restart_dialog;             // Game restart dialog.
			
			std::shared_ptr<Enigma::World> m_world;    // Current game world.
			
			// View widgets contained in the view pager.
			
			Enigma::PlayerView m_playerview;
			Enigma::ItemsView m_itemsview;
			Enigma::WorldsView m_worldsview;
			Enigma::HelpView m_helpview;
			Enigma::AboutView m_aboutview;
	};
}

#endif // __ENIGMA_APPLICATION_H__
