// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the ItemList class header.  The ItemList class manages
// items within a game world.
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
 
#ifndef __ENIGMA_ITEMLIST_H__
#define __ENIGMA_ITEMLIST_H__

#include <vector>
#include <gtkmm/dialog.h>
#include <gtkmm/builder.h>
#include "Item.h"

namespace Enigma
{
	class ItemList : public std::vector<std::shared_ptr<Enigma::Item>>
	{
		public:			
			// Public declarations.
			
			struct Totals
			{
				short m_required;
				short m_bonus;
				short m_secret;
				short m_skull;
			};
			
			// Public methods.

			ItemList();
			void clear();
			void restart();
			Enigma::ItemList::Totals get_remains();
			void do_remains();
			void found(guint8 owner, std::shared_ptr<Enigma::Item>);
			
			void find(const Enigma::Location& position,
			          std::vector<std::shared_ptr<Enigma::Item>>& buffer);
			          
			void find(guint8 owner,
			          std::vector<std::shared_ptr<Enigma::Item>>& buffer);
			          
			static void initialize();
			
			// "remains" signal type and server accessor.

			typedef sigc::signal<void, Enigma::ItemList::Totals&>
				type_signal_remains;
			
			type_signal_remains signal_remains();

		private:
			// Private methods.

			void on_dialog_response(int response_id);
		
			// Private data.
			
			Glib::RefPtr<Gtk::Builder> m_builder;   // User interface.
			Gtk::Dialog* m_dialog;                  // Item found dialog.
			type_signal_remains m_signal_remains;   // "remains" signal server.
	};
}

#endif // __ENIGMA_ITEMLIST_H__
