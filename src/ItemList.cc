// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the ItemList class implementation.  The ItemList class manages
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

#include <cstdio>
#include <glibmm/i18n.h>
#include <glibmm/main.h>
#include <gtkmm/label.h>
#include <gtkmm/image.h>
#include "ItemList.h"
#include "Sound.h"

//--------------------
// Local declarations.
//--------------------

static const std::string ui_name = "ItemList.ui";

static const std::string item_found_name = "ItemFound.ogg";
static Enigma::Sound item_found_sound;

static const std::string skull_found_name = "SkullFound.ogg";
static Enigma::Sound skull_found_sound;

// Item dialog messages.

static const std::string required_text =
	_("This is a required item.");

static const std::string bonus_text =
	_("This is a bonus item.");

static const std::string secret_text =
	_("This is a secret item!");
	
static const std::string skull_text =
	_("Unfortunately, today is your expiry date.");

static const std::string all_required_text =
	_("All required items have now been found!");

static const std::string all_bonus_text =
	_("All bonus items have now been found!");

static const std::string all_secret_text =
	_("All secret items have now been found!");

static const std::string all_text =
	_("All items have now been found!");

//--------------------------------
// This method is the constructor.
//--------------------------------

Enigma::ItemList::ItemList()
{
	// Build the item dialog from its Glade file.

	std::string path = G_RESOURCE_PREFIX;
	path += ui_name;
	
	m_builder = Gtk::Builder::create_from_resource(path);
	
	// Get pointer to dialog shown when item is found.

	m_dialog = nullptr;
	m_builder->get_widget("dialog", m_dialog);
	
	// Connect to the "Found Item" dialog response signal, which will
	// be emitted when the dialog "Close" button is clicked.   
	
	m_dialog->signal_response()
		.connect(sigc::mem_fun(*this, &Enigma::ItemList::on_dialog_response));
}

//----------------------------------
// This method clears the item list. 
//----------------------------------

void Enigma::ItemList::clear()
{
	// Pass method to the parent to erase list contents.

	std::vector<std::shared_ptr<Enigma::Item>>::clear();
	
	// Hide the item dialog in case it is showing.
	
	m_dialog->hide();
}

//---------------------------------------------------------
// This method restores all items to their "restart" state. 
//---------------------------------------------------------

void Enigma::ItemList::restart()
{
	// Hide the item dialog in case it is showing.
	
	m_dialog->hide();
	
	// Restore items to their "restart" state.
	
	std::vector<std::shared_ptr<Enigma::Item>>::iterator iterator;
	
	for (iterator = begin(); iterator != end(); iterator ++)		
		(*iterator)->restart();
	
	// Emit the remaining item totals in a signal.
	
	do_remains();
}

//---------------------------------------------------------------
// This method returns pointers to all items in a world position. 
//---------------------------------------------------------------
// location: World location with position to be searched.
// buffer:   Buffer to receive item pointers.
//---------------------------------------------------------------

void Enigma::ItemList::find(const Location& position,
             std::vector<std::shared_ptr<Enigma::Item>>& buffer)
{
	std::shared_ptr<Enigma::Object> object;
	std::vector<std::shared_ptr<Enigma::Item>>::iterator iterator;
	
	for (iterator = begin(); iterator != end(); iterator ++)
	{
		if ((*iterator)->m_active)
		{
			object = (*iterator)->m_object;
			
			if  ((object->m_location.m_above == position.m_above)
				&& (object->m_location.m_north == position.m_north)
				&& (object->m_location.m_east == position.m_east))
			{			
				buffer.emplace_back(*iterator);
			}
		}
	}
}

//-------------------------------------------------------------
// This method returns pointers to all items owned by a player.
//-------------------------------------------------------------
// owner:  Player number.
// buffer: Buffer to receive item pointers.
//-------------------------------------------------------------

void Enigma::ItemList::find(guint8 owner,
             std::vector<std::shared_ptr<Enigma::Item>>& buffer)
{
	std::vector<std::shared_ptr<Enigma::Item>>::iterator iterator;
	
	for (iterator = begin(); iterator != end(); iterator ++)
	{
		// Add item to buffer if it has been found by the player.
	
		if (!(*iterator)->m_active && ((*iterator)->m_owner == owner))
			buffer.emplace_back(*iterator);
	}
}

//---------------------------------------------------------
// This method is called when the player has found an item.
//---------------------------------------------------------
// owner: Player number.
// item:  Item than has been found.
//---------------------------------------------------------

void Enigma::ItemList::found(guint8 owner, std::shared_ptr<Enigma::Item> item)
{	
	// Set the item status to inactive (found) and set its owner.
		
	item->m_active = false;
	item->m_owner  = owner;
		
	Enigma::ItemList::Totals totals = get_remains();

	// Populate the item dialog with information.
	
	Gtk::Label* name = nullptr;
	m_builder->get_widget("item_name", name);
	
	name->set_label(item->m_object->singular_name());
	
	Gtk::Image* thumbnail = nullptr;
	m_builder->get_widget("item_thumbnail", thumbnail);
	
	thumbnail->set(item->m_object->thumbnail());
	
	// Select the appropriate item message.

	Gtk::Label* message = nullptr;
	m_builder->get_widget("item_message", message);
	
	if (item->m_category == Enigma::Item::Category::SKULL)
	{
		// A skull item has been found.
		
		skull_found_sound.play();
		message->set_label(skull_text);
	}
	else
	{
		// A required, bonus, or secret item has been found.
		
		item_found_sound.play();
		
		if  ((totals.m_required == 0)
		  && (totals.m_bonus == 0)
		  && (totals.m_secret == 0))
		{
			// All items (excluding skulls) have been found. 
		
			message->set_label(all_text);
		}
		else
		{
			switch (item->m_category)
			{
				case Enigma::Item::Category::REQUIRED:
					// A required item has been found.
					
					if (totals.m_required > 0)
						message->set_label(required_text);
					else
						message->set_label(all_required_text);

					break;
				
				case Enigma::Item::Category::BONUS:
					// A bonus item has been found.
					
					if (totals.m_bonus > 0)
						message->set_label(bonus_text);
					else
						message->set_label(all_bonus_text);
					
					break;
				
				case Enigma::Item::Category::SECRET:
					// A secret item has been found.
					
					if (totals.m_secret > 0)
						message->set_label(secret_text);
					else
						message->set_label(all_secret_text);
					
					break;
					
				default:
					break;
			}
		}
	}

	// Display an unblocking dialog with item information.
	
	m_dialog->show_all();
	
	// Emit the remaining item totals in a signal.
	
	m_signal_remains(totals);
}

//---------------------------------------------------
// This method returns totals of the remaining items.
//---------------------------------------------------
 
Enigma::ItemList::Totals Enigma::ItemList::get_remains()
{
	// Prepare data structure for item totals.
	
	Enigma::ItemList::Totals totals;
	
	totals.m_required = 0;
	totals.m_bonus    = 0;
	totals.m_secret   = 0;
	totals.m_skull    = 0;
	
	// Count the remaining items for each item category.
	
	std::vector<std::shared_ptr<Enigma::Item>>::iterator iterator;
	
	for (iterator = begin(); iterator != end(); iterator ++)
	{
		if ((*iterator)->m_active)
		{
			// Item is still active (not yet found).
			
			switch ((*iterator)->m_category)
			{
				case Enigma::Item::Category::REQUIRED: totals.m_required ++; break;
				case Enigma::Item::Category::BONUS:    totals.m_bonus ++;    break;
				case Enigma::Item::Category::SECRET:   totals.m_secret ++;   break;
				case Enigma::Item::Category::SKULL:    totals.m_skull ++;    break;
				default: break;
			}
		}
	}
	
	return totals;
}

//------------------------------------------------------------
// This method counts the remaining items and emits the totals
// in a "remains" signal.
//------------------------------------------------------------

void Enigma::ItemList::do_remains()
{
	// Emit "remains" signal with totals of remaining items.

	Enigma::ItemList::Totals totals = get_remains();
	m_signal_remains(totals);
}

//------------------------------------------------------------------
// This method hides the "Found Item" dialog when the "Close" button
// is clicked.
//------------------------------------------------------------------
// response_id: Response ID from dialog.
//------------------------------------------------------------------

void Enigma::ItemList::on_dialog_response(int response_id)
{  
  if (response_id == Gtk::RESPONSE_CLOSE)
    m_dialog->hide();
}

//-------------------------------------------------
// This method returns the "remains" signal server.
//-------------------------------------------------

Enigma::ItemList::type_signal_remains Enigma::ItemList::signal_remains()
{
	return m_signal_remains;
}

//------------------------------------------------
// This static method initializes class resources.
//------------------------------------------------

void Enigma::ItemList::initialize()
{
	// Prepare sounds.
	
	item_found_sound.load(item_found_name);
	skull_found_sound.load(skull_found_name);
}
