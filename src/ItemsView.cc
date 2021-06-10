// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the ItemsView class implementation.  The ItemsView class
// displays a player's inventory of found items.
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

#include <glibmm/i18n.h>
#include <gtkmm/button.h>
#include "ItemsView.h"

//--------------------
// Local declarations.
//--------------------

static const std::string ui_name = "ItemsView.ui";

//--------------------------------
// This method is the constructor.
//--------------------------------

Enigma::ItemsView::ItemsView()
{  
	// Build the user interface from its Glade file.

	std::string path = G_RESOURCE_PREFIX;
	path += ui_name;
	
	m_builder = Gtk::Builder::create_from_resource(path);

	// Add the top-level box of the user interface in the builder
	// to the ItemsView box (a box within a box).
	
	Gtk::Box* box = nullptr;
	m_builder->get_widget("parent_box", box);	
	this->pack_start(*box, true, true, 0);
  
	// Connect to the Done button clicked signal, which will allow
	// the ItemsView widget to emit a "Done" signal. 
	
	Gtk::Button* button = nullptr;
	m_builder->get_widget("done_button", button);
	
	button->signal_clicked()
		.connect(sigc::mem_fun(*this, &ItemsView::on_done));
	
	m_treeview = nullptr;
	m_builder->get_widget("tree_viewer", m_treeview);
		
	Glib::RefPtr<Gtk::TreeSelection> selection = m_treeview->get_selection();
	selection->set_mode(Gtk::SELECTION_NONE);
	
	// Create a treeview liststore.  Each entry contains information about
	// items owned by a player.  This is provided to the column renderers
	// by the data functions when needed.
	
	m_liststore = Gtk::ListStore::create(m_columnrecord);
	
	// Create a treeview column for the item thumbnail, and add a renderer
	// data function.
		
	Gtk::TreeViewColumn* column = Gtk::manage(new Gtk::TreeViewColumn);
	m_treeview->append_column(*column);

	Gtk::CellRendererPixbuf* thumbnail_renderer =
		Gtk::manage(new Gtk::CellRendererPixbuf);

	column->pack_start(*thumbnail_renderer,true);
	
	column->set_cell_data_func(*thumbnail_renderer,
		sigc::mem_fun(*this, &ItemsView::thumbnail_data_function));

	// Create a treeview column for the item name, and add a renderer
	// data function.

	column = Gtk::manage(new Gtk::TreeViewColumn);
	m_treeview->append_column(*column);

	Gtk::CellRendererText* name_renderer =
		Gtk::manage(new Gtk::CellRendererText);

	column->pack_start(*name_renderer, true);
	
	column->set_cell_data_func(*name_renderer,
		sigc::mem_fun(*this, &ItemsView::name_data_function));
}

//--------------------------------------------------------------
// This method establishes signal connections with a new player,
// and disconnecting signals to a previous player.
//--------------------------------------------------------------
// player: Active player.
//--------------------------------------------------------------

void Enigma::ItemsView::set_player(std::shared_ptr<Enigma::Player>& player)
{
	// Connect player "items" signal to ItemsView, replacing an existing
	// connection.  This signal is emitted by a player when a change occurs
	// in its item list.  Trigger an emission of this signal to update
	// the item list.

	m_connection_items.disconnect();
	
	m_connection_items = player->signal_items()
		.connect(sigc::mem_fun(*this, &Enigma::ItemsView::set_items));
		
	player->do_items();
}

//---------------------------------------------------------
// This method rebuilds the liststore from a list of items.
//---------------------------------------------------------
// items: List of items.
//---------------------------------------------------------

void Enigma::ItemsView::set_items(
	std::vector<std::shared_ptr<Enigma::Item>>& items)
{		
	// Unset the treeview liststore before clearing it.  A new liststore
	// will be created from the item list.
	
	m_treeview->unset_model();
  m_liststore->clear(); 
	
	Gtk::TreeModel::Row row;
	Gtk::TreeNodeChildren::iterator row_iterator;
	std::vector<std::shared_ptr<Enigma::Item>>::iterator item_iterator;

	for (item_iterator = items.begin();
	     item_iterator != items.end();
	     item_iterator ++)
	{		
		row_iterator = m_liststore->children().begin();
		
		while (row_iterator != m_liststore->children().end())
		{
			row_iterator ++;
		}
		
		if (row_iterator == m_liststore->children().end())
		{
			// The liststore does not contain a similar item.  Append a new
			// liststore row for the item and initialize the column data.
			
			row_iterator = m_liststore->append();
			row = *row_iterator;
			
			row[m_columnrecord.m_id] =
				(*item_iterator)->m_object->m_id;
			
			row[m_columnrecord.m_total] =
				1;
			row[m_columnrecord.m_text] =
				(*item_iterator)->m_object->singular_name();
			
			row[m_columnrecord.m_thumbnail] =
				(*item_iterator)->m_object->thumbnail();
		}
		else
		{
			// The liststore contains a similar item.  Increase its total.
			
			row = *row_iterator;
			row[m_columnrecord.m_total] = row[m_columnrecord.m_total] + 1;
		}
	}
	
	// Add the new liststore to the treeview.
  
	m_treeview->set_model(m_liststore);
}

//--------------------------------------------------------------------------
// This method is called when the column thumbnail cells are to be rendered.
// An appropriate item thumbnail image is drawn.
//--------------------------------------------------------------------------

void Enigma::ItemsView::
	thumbnail_data_function(Gtk::CellRenderer* const& renderer,
	                        const Gtk::TreeIter& iterator)
{
	// Exit if there is no iterator.

	if (!iterator)
		return;	

	// Assign the thumbnail image to the renderer.

	Gtk::TreeModel::Row row = *iterator;
	
	((Gtk::CellRendererPixbuf*)renderer)->property_pixbuf() =
		row[m_columnrecord.m_thumbnail];
}

//----------------------------------------------------------------------
// This method is called when the column name cells are to be rendered.
// Data is read from the model and converted into an appropriate content
// for display.
//----------------------------------------------------------------------

void Enigma::ItemsView::
	name_data_function(Gtk::CellRenderer* const& renderer,
	                   const Gtk::TreeIter& iterator)
{	
	// Exit if there is no iterator.

	if (!iterator)
		return;

	// Assemble the item number with description text.

	Glib::ustring full_text;
	const char* text;
	Gtk::TreeModel::Row row = *iterator;

	((Gtk::CellRendererText*)renderer)->property_text() =
		row[m_columnrecord.m_text];
}

//---------------------------------------------------
// This method returns the view "done" signal server.
//---------------------------------------------------

Enigma::ItemsView::type_signal_done Enigma::ItemsView::signal_done()
{
  return m_signal_done;
}

//----------------------------------------------------
// This method is called the "Done" button is pressed.
//----------------------------------------------------

void Enigma::ItemsView::on_done()
{
	// Emit the view "done" signal.

	m_signal_done.emit();
}
