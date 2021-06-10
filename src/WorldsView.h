// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the WorldsView class header.  The WorldsView class manages
// the available game worlds.
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

#ifndef __WORLDSVIEW_H__
#define __WORLDSVIEW_H__

#include <gtkmm/builder.h>
#include <gtkmm/box.h>
#include <gtkmm/grid.h>
#include <gdkmm/event.h>
#include <gtkmm/textview.h>
#include "ItemList.h"

namespace Enigma
{
	class WorldsView : public Gtk::Box
	{
		public:
		  // Public methods.

		  WorldsView();
		  void set_world(const std::string filename);
		  void set_item_remains(const Enigma::ItemList::Totals& totals);

			// Cancel signal accessor.
			
			typedef sigc::signal<void> type_signal_cancel;
			type_signal_cancel signal_cancel();
			
			// Play signal accessor.
			
			typedef sigc::signal<void, const std::string&> type_signal_play;
			type_signal_play signal_play();
			
		private:
			// Private methods.
			
			void on_cancel();
			void on_play();
			void on_left();
			void on_right();
			void update();
			void update_remains(const Enigma::ItemList::Totals& totals);
		
			// Private data.
			
			struct Record      // Record describing a world.
			{
				std::string m_filename;
				Glib::ustring m_description;
				Enigma::ItemList::Totals m_item_remains;
			};
			
			Glib::RefPtr<Gtk::Builder> m_builder;   // User interface.
			type_signal_cancel m_signal_cancel;     // Cancel signal server.
			type_signal_play m_signal_play;         // Play signal server.
			
			// Pointers to world information display widgets.
			
			Gtk::TextView* m_description_text;
			Gtk::Label* m_required_remains;
			Gtk::Label* m_bonus_remains;
			Gtk::Label* m_secret_remains;
			Gtk::Label* m_skull_remains;
			Gtk::Button* m_left_button;
			Gtk::Button* m_right_button;

			// List of world records and iterators.
			
			std::vector<WorldsView::Record> m_records;
			std::vector<WorldsView::Record>::iterator m_active_iterator;
			std::vector<WorldsView::Record>::iterator m_browse_iterator;
	};
}

#endif // __WORLDSVIEW_H__
