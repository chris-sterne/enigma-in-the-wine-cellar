// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the WorldsView class implementation. The WorldsView class
// manages the available game worlds.
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
#include <gtkmm/stack.h>
#include <gtkmm/scrolledwindow.h>
#include <glibmm/fileutils.h>
#include <glibmm/miscutils.h>
#include "World.h"
#include "WorldsView.h"

//--------------------
// Local declarations.
//--------------------

static const std::string ui_name          = "WorldsView.ui";
static const std::string worlds_directory = "worlds";

//--------------------------------
// This method is the constructor.
//--------------------------------

Enigma::WorldsView::WorldsView()
{	
	// Build the user interface from its Glade file.

	std::string path = G_RESOURCE_PREFIX;
	path += ui_name;
	
	m_builder = Gtk::Builder::create_from_resource(path);

	// Add the top-level parent of the user interface in the builder
	// to the WorldsView box.
	
	Gtk::Stack* stack = nullptr;
	m_builder->get_widget("top_parent", stack);	
	this->pack_start(*stack, true, true, 0);

	// Connect to "clicked" signal from the "OK" button in the "no worlds"
	// interface page.
		
	Gtk::Button* button = nullptr;
	m_builder->get_widget("ok_button_no_worlds", button);
	
	button->signal_clicked()
		.connect(sigc::mem_fun(*this, &WorldsView::on_cancel));

	// Connect to "clicked" signal from the "Cancel" button in the "worlds"
	// interface page.

	button = nullptr;
	m_builder->get_widget("cancel_button_worlds", button);
	
	button->signal_clicked()
		.connect(sigc::mem_fun(*this, &WorldsView::on_cancel));

	// Connect to "clicked" signal from the "Play" button.
		
	button = nullptr;
	m_builder->get_widget("play_button", button);
	
	button->signal_clicked()
		.connect(sigc::mem_fun(*this, &WorldsView::on_play));

	// Connect to "clicked" signal from the world browsing buttons.
		
	m_left_button = nullptr;
	m_builder->get_widget("left_button", m_left_button);
	
	m_left_button->signal_clicked()
		.connect(sigc::mem_fun(*this, &WorldsView::on_left));

	m_right_button = nullptr;
	m_builder->get_widget("right_button", m_right_button);
	
	m_right_button->signal_clicked()
		.connect(sigc::mem_fun(*this, &WorldsView::on_right));

	// Save pointers to world information display widgets.
		
	m_description_text = nullptr;
	m_builder->get_widget("description_text", m_description_text);

	m_required_remains = nullptr;
	m_builder->get_widget("required_remains", m_required_remains);

	m_bonus_remains = nullptr;
	m_builder->get_widget("bonus_remains", m_bonus_remains);

	m_secret_remains = nullptr;
	m_builder->get_widget("secret_remains", m_secret_remains);

	m_skull_remains = nullptr;
	m_builder->get_widget("skull_remains", m_skull_remains);

	// Assemble a list of world records for all valid game worlds
	// in the worlds directory.

	Enigma::World world;
	Enigma::WorldsView::Record record;
	Glib::ustring filename;
	Glib::Dir directory(worlds_directory);
	std::list<std::string> filenames(directory.begin(), directory.end());

	for (std::list<std::string>::iterator iterator = filenames.begin();
	     iterator != filenames.end();
	     iterator ++)
	{
		filename = Glib::build_filename(worlds_directory, *iterator);

		if (world.load(filename))
		{
			// The world was successfully loaded.  Add information
			// about the world to its record.

			record.m_filename     = filename;
			record.m_description  = world.description();
			record.m_item_remains = world.items()->get_remains();

			// Add world record to the record list.
			
			m_records.emplace_back(record);
		}
	}
	
	// Initialize the record list iterators.

	m_browse_iterator = m_records.begin();
	m_active_iterator = m_records.end();
	
	if (m_records.begin() == m_records.end())
	{
		// There are no game worlds.  Show a minimal "no worlds"
		// user interface.
		
		stack->set_visible_child("no_worlds");
	}
	else
	{
		// There is at least one game world.  Show the full "worlds"
		// user interface, and update information for the world
		// referenced by the record browse iterator. 
		
		stack->set_visible_child("worlds");
		update();
	}
}

//---------------------------------------------------------------
// This method sets the filename of a game world to be played.
// If the filename is present in the world records, a description
// and remaining item totals will be displayed, and the filename
// will be emitted in a signal.
//---------------------------------------------------------------
// world: Game world filename.
//---------------------------------------------------------------

void Enigma::WorldsView::set_world(const std::string filename)
{
	std::vector<WorldsView::Record>::iterator iterator;
	
	for (iterator = m_records.begin();
	     iterator != m_records.end();
	     iterator ++)
	{
		if ((*iterator).m_filename == filename)
		{
			// The game world filename was found in the world records.
			// Move the iterators to the record, and update the display.
			
			m_browse_iterator = iterator;
			m_active_iterator = iterator;
			update();
			
			// Emit the game world filename.

			m_signal_play.emit(filename);
			break;
		}
	}
}

//---------------------------------------------------------------------
// This method updates the remaining items in the record for the active
// world, and also updates the display of remaining items.
//---------------------------------------------------------------------
// totals: Totals of remaining items.
//---------------------------------------------------------------------

void Enigma::WorldsView::set_item_remains(
	const Enigma::ItemList::Totals& totals)
{	
	// If the active iterator is referencing a record, update the item
	// remains in this record and in the display of remaining items.
	
	if (m_active_iterator != m_records.end())
	{		
		(*m_active_iterator).m_item_remains = totals;
		update_remains(totals);
	}
}

//------------------------------------------------------
// This method is called the "Cancel" button is pressed.
//------------------------------------------------------

void Enigma::WorldsView::on_cancel()
{  
	// If the active iterator is referencing a record, move the browse
	// iterator back to the active iterator and update the display
	// with information for the active world.  Otherwise, leave the browse
	// iterator and display unchanged.
  
  if (m_active_iterator != m_records.end())
	{
		m_browse_iterator = m_active_iterator;
		update();
	}
	
	// Emit the "cancel" signal.

  m_signal_cancel.emit();
}

//----------------------------------------------------
// This method is called the "Play" button is pressed.
//----------------------------------------------------

void Enigma::WorldsView::on_play()
{		
	if (m_browse_iterator != m_active_iterator)
	{
		// A different world is selected.  Set the active iterator
		// to this world, and emit the "play" signal with the world
		// filename.
		
		m_active_iterator = m_browse_iterator;
  	m_signal_play.emit((*m_active_iterator).m_filename);
  }
  else
  {
  	// The same world is selected.  Emit the "cancel" signal.
  	
  	m_signal_cancel.emit();
  }
}

//--------------------------------------------------------
// This method is called the "<" (left) button is pressed.
//--------------------------------------------------------

void Enigma::WorldsView::on_left()
{
	// If possible, move the browse iterator to the previous record.

	if (m_browse_iterator != m_records.begin())
		m_browse_iterator --;
	
	update();
}

//---------------------------------------------------------
// This method is called the ">" (right) button is pressed.
//---------------------------------------------------------

void Enigma::WorldsView::on_right()
{
	// If possible, move the browse iterator to the next record.
	
	if ((m_browse_iterator + 1) != m_records.end())		
		m_browse_iterator ++;

	update();
}

//-----------------------------------------------------------------
// This method displays world information for the record referenced
// by the browse iterator. 
//-----------------------------------------------------------------

void Enigma::WorldsView::update()
{
	// Set the sensitivity of the record browse buttons based on the location
	// of the browse iterator in the list.
	
	m_left_button->set_sensitive(m_browse_iterator != m_records.begin());
	m_right_button->set_sensitive((m_browse_iterator + 1) != m_records.end());
		
	// Display description of the world.
	
	Glib::RefPtr<Gtk::TextBuffer> buffer = m_description_text->get_buffer();
	buffer->set_text((*m_browse_iterator).m_description);
	
	// Updates the display of remaining items.
	
	update_remains((*m_browse_iterator).m_item_remains);
}

//----------------------------------------------------
// This method updates the display of remaining items.
//----------------------------------------------------
// remains: Totals of remaining items.
//----------------------------------------------------

void Enigma::WorldsView::
	update_remains(const Enigma::ItemList::Totals& remains)
{	
	m_required_remains->set_text(std::to_string(remains.m_required));
	m_bonus_remains->set_text(std::to_string(remains.m_bonus));
	m_secret_remains->set_text(std::to_string(remains.m_secret));
	m_skull_remains->set_text(std::to_string(remains.m_skull));
}

//------------------------------------------------
// This method returns the "cancel" signal server.
//------------------------------------------------

Enigma::WorldsView::type_signal_cancel Enigma::WorldsView::signal_cancel()
{
  return m_signal_cancel;
}

//----------------------------------------------
// This method returns the "play" signal server.
//----------------------------------------------

Enigma::WorldsView::type_signal_play Enigma::WorldsView::signal_play()
{
  return m_signal_play;
}
