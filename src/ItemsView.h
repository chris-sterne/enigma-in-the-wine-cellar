// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the ItemsView class header.  The ItemsView class displays
// a player's inventory of found items.
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

#ifndef __ENIGMA_ITEMSVIEW_H__
#define __ENIGMA_ITEMSVIEW_H__

#include <gtkmm/builder.h>
#include <gtkmm/box.h>
#include <gtkmm/treeview.h>
#include <gtkmm/liststore.h>
#include "World.h"

namespace Enigma
{
	class ItemsView : public Gtk::Box
	{
		public:
			// Public methods.

			ItemsView();
			void set_player(std::shared_ptr<Enigma::Player>& player);
			void set_items(std::vector<std::shared_ptr<Enigma::Item>>& items);
			void on_done();

			// View done signal accessor.

			typedef sigc::signal<void> type_signal_done;
			type_signal_done signal_done();

		private:
			// Private methods.

			void thumbnail_data_function(Gtk::CellRenderer* const& renderer,
			                             const Gtk::TreeIter& iterator);

			void name_data_function(Gtk::CellRenderer* const& renderer,
			                        const Gtk::TreeIter& iterator);
		
			// Private classes.
		
			class ItemColumns : public Gtk::TreeModel::ColumnRecord
			{
				public:
					Gtk::TreeModelColumn<Enigma::Object::ID> m_id;
					Gtk::TreeModelColumn<int> m_total;
					Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf>> m_thumbnail;
					Gtk::TreeModelColumn<Glib::ustring> m_text;
					
					ItemColumns()
					{ 
						add(m_id);
						add(m_total);
						add(m_text);
						add(m_thumbnail);
					}
			};
			
			// Private data.
			
			Glib::RefPtr<Gtk::Builder> m_builder;  // User interface.
			type_signal_done m_signal_done;        // View done signal server.
			
			Gtk::TreeView* m_treeview;
			Glib::RefPtr<Gtk::ListStore> m_liststore;
			ItemsView::ItemColumns m_columnrecord;
			
			sigc::connection m_connection_items;
	};
}

#endif // __ENIGMA_ITEMSVIEW_H__
