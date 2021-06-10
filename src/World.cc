// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the World class implementation.  The World class manages
// all objects within a game world.
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

#include <iostream>
#include "glibmm/fileutils.h"
#include "World.h"

//--------------------
// Local declarations.
//--------------------

// Buffer used while loading world objects.

struct Buffer                
{
	Enigma::Object::ID m_id;
	Enigma::Location m_location;
	Enigma::Location m_location_saved;
	Enigma::Location m_location_restart;
	Enigma::Item::Category m_category;
	guint8 m_number;
	guint8 m_owner;
	guint8 m_owner_saved;
	guint8 m_owner_restart;
	bool m_selected;
	bool m_selected_saved;
	bool m_selected_restart;
	bool m_used;
	bool m_used_saved;
	bool m_used_restart;
	bool m_active;
	bool m_active_saved;
	bool m_active_restart;
	bool m_outdoor;
	bool m_outdoor_saved;
	bool m_outdoor_restart;
	bool m_savable;
};

// Private function declarations.

guint find_line(std::string& source, guint offset);

void read_description(const std::string& filedata,
                      guint& index,
                      Glib::ustring& description);

void read_object(const std::string& filedata,
                 guint& index,
                 Buffer& buffer);

void read_item(const std::string& filedata,
               guint& index,
               Buffer& buffer);
                    
void read_player(const std::string& filedata,
                 guint& index,
                 Buffer& buffer);                  

void write_description(std::string& filedata,
                       Glib::ustring& description);

void write_object(std::string& filedata,
                  std::shared_ptr<Enigma::Object> object,
                  guint16& east, guint16& above, guint16& north);

void write_item(std::string& filedata,
                std::shared_ptr<Enigma::Item> item,
                bool savable);

void write_player(std::string& filedata,
                  std::shared_ptr<Enigma::Player> player,
                  bool savable);

void write_keyvalue_16bit(std::string& filedata,
                          Enigma::World::Key key,
                          guint16 value);

void write_keyvalue_8bit(std::string& filedata,
                         Enigma::World::Key key,
                         guint8 value);

void write_keyvalue_boolean(std::string& filedata,
                            Enigma::World::Key key,
                            gboolean value);

void write_keyvalue_string(std::string& filedata,
                           Enigma::World::Key key,
                           std::string& string);

//--------------------------------
// This method is the constructor.
//--------------------------------

Enigma::World::World()
{
	// Create storage for world information.
	
	m_boundary = std::make_shared<Enigma::Boundary>();
	m_objects  = std::make_shared<Enigma::ObjectList>();
	m_items    = std::make_shared<Enigma::ItemList>();
	m_players  = std::make_shared<Enigma::PlayerList>();
}

//----------------------------------------------------
// This method clears a game world of all information.
//----------------------------------------------------

void Enigma::World::clear()
{	
	// Clear all world information.

	m_filename.clear();
	m_description.clear();
	m_boundary->clear();
	m_objects->clear();
	m_items->clear();
	m_players->clear();
	
	// Set the world to be not internally savable.
	
	m_savable = false;
}

//-------------------------------------------
// This method returns the world description.
//-------------------------------------------

const Glib::ustring& Enigma::World::description()
{
	return m_description;
}

//-----------------------------------------
// This method returns the world item list.
//-----------------------------------------

std::shared_ptr<Enigma::ItemList>& Enigma::World::items()
{
	return m_items;
}

//-------------------------------------------
// This method returns the world player list.
//-------------------------------------------

std::shared_ptr<Enigma::PlayerList>& Enigma::World::players()
{
	return m_players;
}

//-------------------------------------------------------
// This method restores the world to its "restart" state.
//-------------------------------------------------------

void Enigma::World::restart()
{
	m_items->restart();
	m_players->restart();
}

//---------------------------------------------
// This method loads a game world from a file.
//---------------------------------------------
// name: Game world filename.
// RETURN: TRUE if successful, or FALSE if not.
//---------------------------------------------

bool Enigma::World::load(const std::string& filename)
{	
	// Clear the old world information.
	
	clear();
	
	// Read the entire world file into a filedata buffer.  Return immediately
	// if an error occurred.

	std::string filedata;

	try
	{
		filedata = Glib::file_get_contents(filename);
	}
	catch(Glib::Error error)
	{
		return false;
	}

	// Return immediately if the first line of the world file does not have
	// the proper indentification code.

	if (!filedata.compare("ewc\n"))
		return false;

	// Confirm that the file header has the correct ending.  The header
	// contents between the identification code and ending are ignored. 

	guint index = 0;
	guint size  = 0;
	guint found_header;

	do
	{
		size         = find_line(filedata, index);
		found_header = filedata.compare(index, size, "end_header\n");
		index += size;
	}
	while ((size != 0) && (found_header != 0));

	// Return immediately if the end of the header was not found.  Otherwise,
	// the index now points to the beginning of a keyvalue array (key and
	// value byte pairs) describing the world.

	if (found_header != 0)
		return false;

	// Initialize buffer to accumulate information from the filedata.
	// This buffer allows reusing information from previously loaded
	// objects (this is mainly used for simple objects only).

	Buffer buffer;
	
	buffer.m_id = Enigma::Object::ID::NONE;
	buffer.m_location.clear();
	buffer.m_location_saved.clear();
	buffer.m_location_restart.clear();
	buffer.m_number           = 0;
	buffer.m_owner            = 0;
	buffer.m_owner_saved      = 0;
	buffer.m_owner_restart    = 0;
	buffer.m_selected         = false;
	buffer.m_selected_saved   = false;
	buffer.m_selected_restart = false;
	buffer.m_used             = false;
	buffer.m_used_saved       = false;
	buffer.m_used_restart     = false;
	buffer.m_active           = false;
	buffer.m_active_saved     = false;
	buffer.m_active_restart   = false;
	buffer.m_outdoor          = false;
	buffer.m_outdoor_saved    = false;
	buffer.m_outdoor_restart  = false;
	buffer.m_savable          = false;
	
	// Begin reading keyvalue information from the filedata.

	bool valid_data    = true;
	bool active_player = false;
	bool done          = false;
	Enigma::World::Key key;
	guint8 value;

	while (((filedata.size() - index ) >= 2) && !done)
	{
		// A complete keyvalue pair (2 bytes) is available.  Read the keyvalue,
		// but keep the keyvalue array index on the element header. 

		key   = (Enigma::World::Key)filedata.at(index);
		value = (guint8)filedata.at(index + 1);

		switch (key)
		{
			case Enigma::World::Key::DESCRIPTION:
				// A Description element header has been encountered.
				// Read filedata information to the description string.
				
				read_description(filedata, index, m_description);
				break;
		      
			case Enigma::World::Key::OBJECT:
				// An Object element header has been encountered.
				// Read filedata information to the buffer.

				read_object(filedata, index, buffer);

				// Validate the extracted Object information.
				
				if  ((buffer.m_id
				      < Enigma::Object::ID::TOTAL)
					&& (buffer.m_location.m_surface
					    < Enigma::Location::Direction::TOTAL)
					&& (buffer.m_location.m_rotation
					    < Enigma::Location::Direction::TOTAL))
				{            
					// The buffer contains valid data for an Object.
					// Create a new Object from the buffer information.

					std::shared_ptr<Enigma::Object> object =
						Enigma::Object::create(buffer.m_id);
					
					if (object)
					{
						// Set the location of the Object.
						
						object->m_location = buffer.m_location;
						m_boundary->adjust(buffer.m_location);
						
						// Add the new Object to the world.

						m_objects->emplace_back(object);
					}
				}
				else
				{
					// The buffer contains invalid Object information.
					// Force an exit with an invalid data error.

					valid_data = false;
					done       = true;
				}

				break;

			case Enigma::World::Key::ITEM:
				// An Item element header has been encountered.
				// Read filedata information to the buffer.
				
				read_item(filedata, index, buffer);
				
				// Validate the extracted Item information.
				
				if  ((buffer.m_id
				      < Enigma::Object::ID::TOTAL)
					&& (buffer.m_location.m_surface
					    < Enigma::Location::Direction::TOTAL)
					&& (buffer.m_location.m_rotation
					    < Enigma::Location::Direction::TOTAL)
					&& (buffer.m_category
					    < Enigma::Item::Category::TOTAL))
				{
					// The buffer contains valid data for an Item.
					// Create an Item from the buffer information.

					std::shared_ptr<Enigma::Item> item =
						std::make_shared<Enigma::Item>(buffer.m_id);
					
					if (item)
					{
						item->m_category = buffer.m_category;
						
						// Set the "current" state of the Item. 
						
						item->m_object->m_location = buffer.m_location;
						m_boundary->adjust(buffer.m_location);
						
						item->m_active   = buffer.m_active;
						item->m_owner    = buffer.m_owner;
						item->m_selected = buffer.m_selected;
						item->m_used     = buffer.m_used;
						
						// Set the "saved" state of the Item if such information
						// was provided by the world file.
						
						if (buffer.m_savable)
						{
							item->m_active_saved   = buffer.m_active_saved;
							item->m_owner_saved    = buffer.m_owner_saved;
							item->m_selected_saved = buffer.m_selected_saved;
							item->m_used_saved     = buffer.m_used_saved;

							m_savable = true;
						}
						
						// Set the "restart" state of the Item.
						
						item->m_active_restart   = buffer.m_active_restart;
						item->m_owner_restart    = buffer.m_owner_restart;
						item->m_selected_restart = buffer.m_selected_restart;
						item->m_used_restart     = buffer.m_used_restart;
					}
					
					// Add the new Item to the world.

					m_items->emplace_back(item);
				}

				break;

			case Enigma::World::Key::PLAYER:
				// A Player element header has been encountered.
				// Read filedata information to the buffer.

				read_player(filedata, index, buffer);
				
				// Validate the extracted Player information.
				
				if  ((buffer.m_id
				      < Enigma::Object::ID::TOTAL)
					&& (buffer.m_location.m_surface
					    < Enigma::Location::Direction::TOTAL)
					&& (buffer.m_location.m_rotation
					    < Enigma::Location::Direction::TOTAL)
					&& (buffer.m_location_saved.m_surface
					    < Enigma::Location::Direction::TOTAL)
					&& (buffer.m_location_saved.m_rotation
					    < Enigma::Location::Direction::TOTAL)
					&& (buffer.m_location_restart.m_surface
					    < Enigma::Location::Direction::TOTAL)
					&& (buffer.m_location_restart.m_rotation
					    < Enigma::Location::Direction::TOTAL))
				{            
					// The buffer contains valid data for a Player.						
					// Create a Player from the buffer information.

					std::shared_ptr<Enigma::Player> player =
						std::make_shared<Enigma::Player>(buffer.m_id);
					
					if (player)
					{
						// Provide the player with world information.
	
						player->set_objects(m_objects);
						player->set_items(m_items);
						player->set_boundary(m_boundary);
						
						// Set the player number and increment number for next player.
						
						player->m_number = buffer.m_number;
						buffer.m_number ++;
						
						// Set the "current" state of the player.
						
						player->m_location = buffer.m_location;
						player->m_next_location = buffer.m_location;
						m_boundary->adjust(buffer.m_location);

						player->m_active  = buffer.m_active;
						player->m_outdoor = buffer.m_outdoor;
						
						// Update cumulative active player flag.  This flag
						// will be TRUE if at least one player is active.
						
						active_player = active_player || player->m_active;
						
						// Set the "saved" state of the Player if such information
						// was provided.
						
						if (buffer.m_savable)
						{
							m_savable = true;

							player->m_location_saved = buffer.m_location_saved;
							m_boundary->adjust(buffer.m_location_saved);
							
							player->m_active_saved  = buffer.m_active_saved;
							player->m_outdoor_saved = buffer.m_outdoor_saved;
						}
						
						// Set the "restart" state of the Player.
						
						player->m_location_restart = buffer.m_location_restart;
						m_boundary->adjust(buffer.m_location_restart);
						
						player->m_active_restart  = buffer.m_active_restart;
						player->m_outdoor_restart = buffer.m_outdoor_restart;
						
						// Add the new Player to the world.

						m_players->emplace_back(player);
					}
				}
				else
				{
					// The buffer contains invalid Player information.
					// Force an exit with an invalid data error.

					valid_data = false;
					done       = true;
				}

				break;
				
			case Enigma::World::Key::END:
				// The end of the keyvalue array has been found.  Ensure it has
				// the correct value, and is last in the keyvalue stream.

				if ((value != 0) || ((filedata.size() - index) != 2))
					valid_data = false;

				done = true;
				break;

			default:
				// An unrecognized element header has been encountered.
				// Force an exit with an invalid data error.

				valid_data = false;
				done       = true;
				break;
		}
	}

	// If the filedata stream contained invalid data, no players,
	// or no active players, clear the game world and return FALSE.

	if (!valid_data || !active_player || (buffer.m_number == 0))
	{
		clear();
		return false;
	}

	// The filedata stream contained valid data and an active player.
	// Record the filename for when the game world is saved.
	
	m_filename = filename;
	
	// Have the item list emit totals of the remaining items.
	
	m_items->do_remains();
	
	// Have the player list prepare all players and emit the active one.
	
	m_players->prepare();
	
	// Return TRUE to indicate a successful world loading.
	
	return true;
}

//-------------------------------------------------------------------------
// This method saves a game world to a file.
//-------------------------------------------------------------------------
// filename: Game world filename, or empty string to use internal filename.
// RETURN:   TRUE if successful, or FALSE if not.
//-------------------------------------------------------------------------

bool Enigma::World::save(const std::string& filename)
{
	std::string name = filename;
	
	// If an empty filename string is provided, use the internal filename
	// that was saved when loading the game world.
	
	if (name.length() == 0)
		name = m_filename;

	// If the internal filename is also empty, the world will not be saved.

	if (name.length() == 0)
		return false;

	// All world information is streamed into a filedata buffer before
	// being saved to a file.  Begin by adding an identification header
	// followed by a summary of the world.  A text file viewer will display
	// this information when reading a game world file, although the rest
	// of the file will be unreadable.

	std::string filedata =
"ewc\n\
format binary_byte 1.0\n\
comment Enigma in the Wine Cellar 1.0 game world\n\
comment Created by Enigma in the Wine Cellar 1.0\n";

	filedata.append("element object ");
	filedata.append(std::to_string(m_objects->size()));
	filedata.push_back('\n');

	filedata.append("element teleporter 0");
	filedata.push_back('\n');

	filedata.append("element player ");
	filedata.append(std::to_string(m_players->size()));
	filedata.push_back('\n');

	filedata.append("element item ");
	filedata.append(std::to_string(m_items->size()));
	filedata.push_back('\n');

	filedata.append("element description 1");
	filedata.push_back('\n');

	filedata.append("element controller 0");
	filedata.push_back('\n');

	filedata.append("end_header\n");

	// Write KeyValues for the world description.

	write_description(filedata, m_description);
	
	// Write KeyValues for all simple Objects.  Position keyvalues are written
	// only if a change occurs in their value, which reduces the file size.
	
	guint16 east  = 0;
	guint16 north = 0;  	
	guint16 above = 0; 

	std::vector<std::shared_ptr<Enigma::Object>>::const_iterator
		object_iterator;

	for (object_iterator = m_objects->begin();
	     object_iterator != m_objects->end();
	     ++ object_iterator)
	{
		write_object(filedata, *object_iterator, east, above, north);
	}

	// Write KeyValues for all Items.
	
	std::vector<std::shared_ptr<Enigma::Item>>::const_iterator
		item_iterator;

	for (item_iterator = m_items->begin();
	     item_iterator != m_items->end();
	     ++ item_iterator)
	{
		write_item(filedata, *item_iterator, m_savable);
	}
	
	// Write KeyValues for all Players.
	
	std::vector<std::shared_ptr<Enigma::Player>>::const_iterator
		player_iterator;
	
	for (player_iterator = m_players->begin();
	     player_iterator != m_players->end();
	     ++ player_iterator)
	{
		write_player(filedata, *player_iterator, m_savable);
	}

	// Write KeyValue array terminator.

	write_keyvalue_8bit(filedata,
	                    Enigma::World::Key::END,
	                    0);
	                    
	// Write the completed filedata to the file.

	bool result;

	try
	{
		Glib::file_set_contents(name, filedata.data(), filedata.size());
		result = true;
	}
	catch (Glib::Error error)
	{
		result = false;
	}
    
	return result;
}

//-------------------------------------------------------------------
// This private function returns the number of bytes in a string line
// of a line ending with the newline '\n' or \x00 characters.          
//-------------------------------------------------------------------
// source: Source string to be searched.
// offset: Initial byte offset into the string. 
// RETURN: Line length, or zero if no line is available.             
//-------------------------------------------------------------------

guint find_line(std::string& source, guint offset)
{
	// Count the number of characters before the line terminator.

	guint index = offset;

	while ((index < source.size())
	    && (source.at(index) != '\n')
	    && (source.at(index) != '\x00'))
	{
		++ index;
	}

	// Include the line terminator in the final count, unless the end
	// of the source string has been reached.

	if (index >= source.size())
		return (index - offset);
	else
		return (1 + index - offset);
}

//------------------------------------------------------------------
// This private function reads a Description from a filedata stream.
//------------------------------------------------------------------
// filedata: Keyvalue stream.
// index:    Keyvalue index.
// buffer:   Reference to description string buffer.
// RETURN:   String buffer containing description text.
//------------------------------------------------------------------

void read_description(const std::string& filedata,
                      guint& index,
                      Glib::ustring& description)
{
  // Skip over the header keyvalue.  At present, the language value
  // is ignored.
  
  index += 2;
  
  // Accumulate Description information from the keyvalue stream until
  // an element header or keyvalue array terminator is encountered.
 
  Enigma::World::Key bank_state = Enigma::World::Key::NONE;
  Enigma::World::Key key;
  guint8 value;
  guint16 length = 0;
  bool done = false;
  
  while (((filedata.size() - index ) >= 2 ) && !done)
  {
    key = (Enigma::World::Key)filedata.at(index);
    index ++;

    value = filedata.at(index);
    index ++;

    switch(key)
    {
      case Enigma::World::Key::LENGTH:      
        length = (guint16)value;
        bank_state = key;
        break;
    
      case Enigma::World::Key::DATA:
        // The keyvalue is followed by a block of description text.
        
        description = filedata.substr(index, length);
        index += length;
        break;
        
      case Enigma::World::Key::BANK:
        // The value of a previous keyvalue is to have a high byte added.
        // Adding only one extra bank is supported, extending the value
        // to a maximum of 16 bits.

        if (bank_state == Enigma::World::Key::LENGTH)
          length |= (guint16)(value << 8);
                    
        break;
        
      case Enigma::World::Key::OBJECT:
      case Enigma::World::Key::TELEPORTER:
      case Enigma::World::Key::PLAYER:
      case Enigma::World::Key::ITEM:
      case Enigma::World::Key::DESCRIPTION:
      case Enigma::World::Key::CONTROLLER:
      case Enigma::World::Key::END:
        // An element keyvalue or array end keyvalue has been encountered.
        // Move the array index back to this keyvalue before exiting.
			
        index -= 2;
        done = true;
        break;
        
      default:
        break;
    }
  }
  
  return;
}

//--------------------------------------------------------------
// This private function reads an Object from a filedata stream.
//--------------------------------------------------------------
// filedata: Keyvalue stream.
// index:    Keyvalue index.
// buffer:   Reference to data buffer.
// RETURN:   Buffer filled with keyvalue Object data.
//---------------------------------------------------------

void read_object(const std::string& filedata,
                 guint& index,
                 Buffer& buffer)
{
	// Record the Object ID from the header keyvalue.

	++ index;
	buffer.m_id = (Enigma::Object::ID)filedata.at(index);
	++ index;

	// Accumulate Object information from the keyvalue stream until an element
	// header or keyvalue sequence terminator is encountered.

	Enigma::World::Key bank_state = Enigma::World::Key::NONE;
	Enigma::World::Key key;
	guint8 value;
	guint16 high_value;
	bool done = false;

	while (((filedata.size() - index) >= 2 ) && !done)
	{
		key = (Enigma::World::Key)filedata.at(index);
		index ++;

		value = filedata.at(index);
		index ++;

		switch (key)
		{
			case Enigma::World::Key::SURFACE:      
				buffer.m_location.m_surface = (Enigma::Location::Direction)value;
				break;

			case Enigma::World::Key::ROTATION: 
				buffer.m_location.m_rotation = (Enigma::Location::Direction)value;
				break;

			case Enigma::World::Key::ABOVE:        
				buffer.m_location.m_above = (guint16)value;
				bank_state = key;
				break;

			case Enigma::World::Key::NORTH:        
				buffer.m_location.m_north = (guint16)value;
				bank_state = key;
				break;

			case Enigma::World::Key::EAST:        
				buffer.m_location.m_east = (guint16)value;
				bank_state = key;
				break;

			case Enigma::World::Key::BANK:
				// The value of a previous keyvalue is to have a high byte added.
				// Adding only one extra bank is supported, extending the value
				// to a maximum of 16 bits.

				high_value = (guint16)(value << 8);

				if (bank_state == Enigma::World::Key::ABOVE)
					buffer.m_location.m_above |= high_value;
				else if (bank_state == Enigma::World::Key::NORTH)
					buffer.m_location.m_north |= high_value;
				else if (bank_state == Enigma::World::Key::EAST)
					buffer.m_location.m_east |= high_value;

				break;

			case Enigma::World::Key::OBJECT:
			case Enigma::World::Key::TELEPORTER:
			case Enigma::World::Key::PLAYER:
			case Enigma::World::Key::ITEM:
			case Enigma::World::Key::DESCRIPTION:
			case Enigma::World::Key::CONTROLLER:
			case Enigma::World::Key::END:
				// An element keyvalue or array end keyvalue has been encountered.
				// Move the array index back to this keyvalue before exiting.

				index -= 2;
				done = true;
				break;

			default:
				break;
		}
	}
}

//------------------------------------------------------------
// This private function reads an Item from a filedata stream.
//------------------------------------------------------------
// filedata: Filedata stream.
// index:    Filedata index.
// buffer:   Reference to buffer.
// RETURN:   Buffer filled with keyvalue Item data.
//-------------------------------------------------------

void read_item(const std::string& filedata,
               guint& index,
               Buffer& buffer)
{  
  // Record information from the header keyvalue.
  
  ++ index;
  buffer.m_id = (Enigma::Object::ID)filedata.at(index);
  ++ index;
  
  // Record Item information from the keyvalue stream until an element
  // header or keyvalue sequence terminator is encountered.
 
  Enigma::World::Key bank_state  = Enigma::World::Key::NONE;
  Enigma::World::Key group_state = Enigma::World::Key::NONE;
  Enigma::World::Key key;
  guint8 value;
  guint16 high_value;
  bool done = false;
  
  while (((filedata.size() - index ) >= 2) && !done)
  {
    key = (Enigma::World::Key)filedata.at(index);
    index ++;

    value = filedata.at(index);
    index ++;
    
    switch (key)
    {
      case Enigma::World::Key::SURFACE:      
        buffer.m_location.m_surface = (Enigma::Location::Direction)value;
        break;
      
      case Enigma::World::Key::ROTATION: 
        buffer.m_location.m_rotation = (Enigma::Location::Direction)value;
        break;

      case Enigma::World::Key::CATEGORY:
        buffer.m_category = (Enigma::Item::Category)value;
        break;

      case Enigma::World::Key::EAST:
        buffer.m_location.m_east = (guint16)value;
        bank_state = key;
        break;

      case Enigma::World::Key::NORTH:        
        buffer.m_location.m_north = (guint16)value;
        bank_state = key;
        break;
      
      case Enigma::World::Key::ABOVE:        
        buffer.m_location.m_above = (guint16)value;
        bank_state = key;
        break;
      
      case Enigma::World::Key::BANK:
        // The value of a previous keyvalue is to have a high byte added.
        // Adding only one extra bank is supported, extending the value
        // to a maximum of 16 bits.
                
        high_value = (guint16)(value << 8);
        
        if (bank_state == Enigma::World::Key::ABOVE)
          buffer.m_location.m_above |= high_value;
        else if (bank_state == Enigma::World::Key::NORTH)
          buffer.m_location.m_north |= high_value;
        else if (bank_state == Enigma::World::Key::EAST)
          buffer.m_location.m_east |= high_value;
                    
        break;

      case Enigma::World::Key::ACTIVE:
        // This key indicates the arrival of a group of item Active
        // values, with the current value as the first member
        // in the group.
        
        buffer.m_active = (gboolean)value;
        group_state     = key;
        break;
        
      case Enigma::World::Key::SELECTED:
        // This key indicates the arrival of a group of item Selected
        // values, with the current value as the first member
        // in the group.
        
        buffer.m_selected = (gboolean)value;
        group_state       = key;
        break;

      case Enigma::World::Key::USED:
        // This key indicates the arrival of a group of item Used
        // values, with the current value as the first member
        // in the group.
        
        buffer.m_used = (gboolean)value;
        group_state   = key;
        break;

      case Enigma::World::Key::OWNER:
        // This key indicates the arrival of a group of item Owner
        // values, with the current value as the first member
        // in the group.
        
        buffer.m_owner = (guint8)value;
        group_state    = key;
        break;
      
      case Enigma::World::Key::SAVED:
        // This is the Saved value member in a group of values.
        // Select the appropriate destination.
        
        if (group_state == Enigma::World::Key::ACTIVE)
          buffer.m_active_saved = (gboolean)value;
        else if (group_state == Enigma::World::Key::SELECTED)
          buffer.m_selected_saved = (gboolean)value;
        else if (group_state == Enigma::World::Key::USED)
          buffer.m_used_saved = (gboolean)value;
        else if (group_state == Enigma::World::Key::OWNER)
          buffer.m_owner_saved = (guint8)value;
        
				// Set control flag allowing world to be saved.

				buffer.m_savable = true;
				break;
      
      case Enigma::World::Key::RESTART:
        // This is the Restart value member in a group of values.
        // Select the appropriate destination.

        if (group_state == Enigma::World::Key::ACTIVE)
          buffer.m_active_restart = (gboolean)value;
        else if (group_state == Enigma::World::Key::SELECTED)
          buffer.m_selected_restart = (gboolean)value;
        else if (group_state == Enigma::World::Key::USED)
          buffer.m_used_restart = (gboolean)value;        
        else if (group_state == Enigma::World::Key::OWNER)
          buffer.m_owner_restart = (guint8)value;
                  
        break;
 
      case Enigma::World::Key::OBJECT:
      case Enigma::World::Key::TELEPORTER:
      case Enigma::World::Key::PLAYER:
      case Enigma::World::Key::ITEM:
      case Enigma::World::Key::DESCRIPTION:
      case Enigma::World::Key::CONTROLLER:
      case Enigma::World::Key::END:
        // An element keyvalue or array end keyvalue has been encountered.
        // Move the array index back to this keyvalue before exiting.
			
        index -= 2;
        done = true;
        break;
       
      default:
        break;
    }
  }
  
  return;
}

//-------------------------------------------------------------
// This private function reads a Player from a filedata stream.
//-------------------------------------------------------------
// filedata: Filedata stream.
// index:    Filedata index.
// buffer:   Reference to buffer.
// RETURN:   Buffer filled with keyvalue Player data.
//-------------------------------------------------------------

void read_player(const std::string& filedata,
                 guint& index,
                 Buffer& buffer)
{
	// Record the Player ID from the header keyvalue.

	++ index;
	buffer.m_id = (Enigma::Object::ID)filedata.at(index);
	++ index;

	// Accumulate Player information from the keyvalue stream until an element
	// header or keyvalue sequence terminator is encountered.

	Enigma::World::Key group_state  = Enigma::World::Key::NONE;
	Enigma::World::Key member_state = Enigma::World::Key::NONE;
	Enigma::World::Key key;
	guint8 value;
	guint16 high_value;
	bool done = false;

	while (((filedata.size() - index ) >= 2) && !done)
	{
		key = (Enigma::World::Key)filedata.at(index);
		index ++;

		value = filedata.at(index);
		index ++;

		switch (key)
		{
			case Enigma::World::Key::SURFACE:
				// This key indicates the arrival of a group of object surface
				// values, with the current value as the first member in
				// the group.    

				buffer.m_location.m_surface = (Enigma::Location::Direction)value;
				group_state  = key;
				member_state = key;
				break;

			case Enigma::World::Key::ROTATION:
				// This key indicates the arrival of a group of object rotation
				// values, with the current value as the first member in
				// the group.

				buffer.m_location.m_rotation = (Enigma::Location::Direction)value;
				group_state  = key;
				member_state = key;
				break;

			case Enigma::World::Key::EAST:        
				// This key indicates the arrival of a group of object East
				// locations, with the current value as the first member in
				// the group.

				buffer.m_location.m_east = (guint16)value;
				group_state  = key;
				member_state = key;
				break;

			case Enigma::World::Key::NORTH:        
				// This key indicates the arrival of a group of object North
				// locations, with the current value as the first member in
				// the group.

				buffer.m_location.m_north = (guint16)value;
				group_state  = key;
				member_state = key;
				break;

			case Enigma::World::Key::ABOVE:
				// This key indicates the arrival of a group of object Above
				// locations, with the current value as the first member in
				// the group.

				buffer.m_location.m_above = (guint16)value;
				group_state  = key;
				member_state = key;
				break;

			case Enigma::World::Key::BANK:
				// The value of a previous keyvalue is to have a high byte added.
				// Adding only one extra bank is supported, extending the value
				// to a maximum of 16 bits.

				high_value = (guint16)(value << 8);

				if (group_state == Enigma::World::Key::EAST)
				{
					if (member_state == Enigma::World::Key::EAST)
						buffer.m_location.m_east |= high_value;
					else if (member_state == Enigma::World::Key::SAVED)
						buffer.m_location_saved.m_east |= high_value;
					else if (member_state == Enigma::World::Key::RESTART)
						buffer.m_location_restart.m_east |= high_value;
				}
				else if ( group_state == Enigma::World::Key::NORTH)
				{
					if (member_state == Enigma::World::Key::NORTH)
						buffer.m_location.m_north |= high_value;
					else if (member_state == Enigma::World::Key::SAVED)
						buffer.m_location_saved.m_north |= high_value;
					else if (member_state == Enigma::World::Key::RESTART)
						buffer.m_location_restart.m_north |= high_value;
				}
				else if (group_state == Enigma::World::Key::ABOVE)
				{
					if (member_state == Enigma::World::Key::ABOVE)
						buffer.m_location.m_above |= high_value;
					else if (member_state == Enigma::World::Key::SAVED)
						buffer.m_location_saved.m_above |= high_value;
					else if (member_state == Enigma::World::Key::RESTART)
						buffer.m_location_restart.m_above |= high_value;
				}

				break;

			case Enigma::World::Key::ACTIVE:
				// This key indicates the arrival of a group of Active
				// values, with the current value as the first member
				// in the group.

				buffer.m_active = (gboolean)value;
				group_state  = key;
				member_state = key;
				break;

			case Enigma::World::Key::OUTDOOR:
				// This key indicates the arrival of a group of Outdoor
				// values, with the current value as the first member
				// in the group.

				buffer.m_outdoor = (gboolean)value;
				group_state  = key;
				member_state = key;
				break;

			case Enigma::World::Key::SAVED:
				// This is the Saved value member in a group of values.
				// Select the appropriate destination.

				if (group_state == Enigma::World::Key::SURFACE)
					buffer.m_location_saved.m_surface =
						(Enigma::Location::Direction)value;
				else if (group_state == Enigma::World::Key::ROTATION)
					buffer.m_location_saved.m_rotation =
						(Enigma::Location::Direction)value;
				else if (group_state == Enigma::World::Key::EAST)
					buffer.m_location_saved.m_east = (guint16)value;
				else if (group_state == Enigma::World::Key::NORTH)
					buffer.m_location_saved.m_north = (guint16)value;
				else if (group_state == Enigma::World::Key::ABOVE)
					buffer.m_location_saved.m_above = (guint16)value;
				else if (group_state == Enigma::World::Key::ACTIVE)
					buffer.m_active_saved = (gboolean)value;
				else if (group_state == Enigma::World::Key::OUTDOOR)
					buffer.m_outdoor_saved = (gboolean)value;

				member_state = key;

				// Set control flag allowing world to be saved.

				buffer.m_savable = true;
				break;

			case Enigma::World::Key::RESTART:
				// This is the Restart value member in a group of values.
				// Select the appropriate destination.

				if (group_state == Enigma::World::Key::SURFACE)
					buffer.m_location_restart.m_surface =
						(Enigma::Location::Direction)value;
				else if (group_state == Enigma::World::Key::ROTATION)
					buffer.m_location_restart.m_rotation =
						(Enigma::Location::Direction)value;
				else if (group_state == Enigma::World::Key::EAST)
					buffer.m_location_restart.m_east = (guint16)value;
				else if (group_state == Enigma::World::Key::NORTH)
					buffer.m_location_restart.m_north = (guint16)value;
				else if (group_state == Enigma::World::Key::ABOVE)
					buffer.m_location_restart.m_above = (guint16)value;
				else if (group_state == Enigma::World::Key::ACTIVE)
					buffer.m_active_restart = (gboolean)value;
				else if (group_state == Enigma::World::Key::OUTDOOR)
					buffer.m_outdoor_restart = (gboolean)value;

				member_state = key;
				break;

			case Enigma::World::Key::OBJECT:
			case Enigma::World::Key::TELEPORTER:
			case Enigma::World::Key::PLAYER:
			case Enigma::World::Key::ITEM:
			case Enigma::World::Key::DESCRIPTION:
			case Enigma::World::Key::CONTROLLER:
			case Enigma::World::Key::END:
				// An element keyvalue or array end keyvalue has been encountered.
				// Move the array index back to this keyvalue before exiting.

				index -= 2;
				done = true;
				break;

			default:
				break;
		}
	}
}

//-----------------------------------------------------------------
// This private function writes a Description to a filedata stream.
//-----------------------------------------------------------------
// filedata:    Filedata stream.
// description: Description text.
//-----------------------------------------------------------------

void write_description(std::string& filedata, Glib::ustring& description)
{
	// Add header for a description.

	write_keyvalue_8bit(filedata,
	                    Enigma::World::Key::DESCRIPTION,
	                    (guint8)Enigma::World::Language::ENGLISH);

	// Add description length keyvalue.

	write_keyvalue_16bit(filedata,
	                     Enigma::World::Key::LENGTH,
	                     (guint16)description.bytes());

	// Write data keyvalue, followed by description text in UTF-8 format.

	write_keyvalue_8bit(filedata,
	                    Enigma::World::Key::DATA,
	                    0);

	filedata.append(description);
}

//-------------------------------------------------------------
// This private function writes an Object to a filedata stream.
//-------------------------------------------------------------
// filedata: Filedata KeyValue stream.
// object:   Object object.
// east:     East position of previous object.
// above:    Above position of previous object.
// north:    North position of previous object.
//-------------------------------------------------------------

void write_object(std::string& filedata,
                  std::shared_ptr<Enigma::Object> object,
                  guint16& east, guint16& above, guint16& north)
{
	// Add the header for a simple object.

	write_keyvalue_8bit(filedata,
	                    Enigma::World::Key::OBJECT,
	                    (guint8)object->m_id);

	// Write an East keyvalue if the East position of the object
	// differs from that of the previous object.
	
	if (object->m_location.m_east != east)
	{
		east = object->m_location.m_east;
		
		write_keyvalue_16bit(filedata,
			                   Enigma::World::Key::EAST,
			                   east);
	}

	// Write an Above keyvalue if the Above position of the object
	// differs from that of the previous object.
	
	if (object->m_location.m_above != above)
	{
		above = object->m_location.m_above;
		
		write_keyvalue_16bit(filedata,
			                   Enigma::World::Key::ABOVE,
			                   above);
	}

	// Write a North keyvalue if the North position of the object
	// differs from that of the previous object.
	
	if (object->m_location.m_north != north)
	{
		north = object->m_location.m_north;
		
		write_keyvalue_16bit(filedata,
			                   Enigma::World::Key::NORTH,
			                   north);
	}

	// Add a surface keyvalue.

	write_keyvalue_8bit(filedata,
	                    Enigma::World::Key::SURFACE,
	                    (guint8)object->m_location.m_surface);

	// Add a rotation keyvalue.

	write_keyvalue_8bit(filedata,
	                    Enigma::World::Key::ROTATION,
	                    (guint8)object->m_location.m_rotation);
}

//-----------------------------------------------------------
// This private function writes an Item to a filedata stream.
//-----------------------------------------------------------
// filedata: Filedata stream.
// item:     Item object.
// savable:  TRUE if game world is savable.
//-----------------------------------------------------------

void write_item(std::string& filedata,
                std::shared_ptr<Enigma::Item> item,
                bool savable)
{
	// Add header for an Item object.
	
	write_keyvalue_8bit(filedata,
	                    Enigma::World::Key::ITEM,
	                    (guint8)item->m_object->m_id);

	// Write location.

	write_keyvalue_16bit(filedata,
	                     Enigma::World::Key::EAST,
	                     item->m_object->m_location.m_east);

	write_keyvalue_16bit(filedata,
	                     Enigma::World::Key::ABOVE,
	                     item->m_object->m_location.m_above);

	write_keyvalue_16bit(filedata,
	                     Enigma::World::Key::NORTH,
	                     item->m_object->m_location.m_north);

	write_keyvalue_8bit(filedata,
	                    Enigma::World::Key::SURFACE,
	                    (guint8)item->m_object->m_location.m_surface);

	write_keyvalue_8bit(filedata,
	                    Enigma::World::Key::ROTATION,
	                    (guint8)item->m_object->m_location.m_rotation);

	// Write category.

	write_keyvalue_8bit(filedata,
	                    Enigma::World::Key::CATEGORY,
	                    (guint8)item->m_category);

	// Write Current and Restart Active states.

	write_keyvalue_boolean(filedata,
	                       Enigma::World::Key::ACTIVE,
	                       item->m_active);

	write_keyvalue_boolean(filedata,
	                       Enigma::World::Key::RESTART,
	                       item->m_active_restart);
}

//-----------------------------------------------------------
// This private function writes a Player to a filedata stream.
//-----------------------------------------------------------
// filedata: Filedata stream.
// player:   Player object.
// savable:  TRUE if game world is savable.
//-----------------------------------------------------------

void write_player(std::string& filedata,
                  std::shared_ptr<Enigma::Player> player,
                  bool savable)
{
	// Add header for a player object.

	write_keyvalue_8bit(filedata,
	                    Enigma::World::Key::PLAYER,
	                    (guint8)player->m_object->m_id);

	// Write Current and Restart East position keyvalues.

	write_keyvalue_16bit(filedata,
	                     Enigma::World::Key::EAST,
	                     player->m_location.m_east);

	write_keyvalue_16bit(filedata,
	                     Enigma::World::Key::RESTART,
	                     player->m_location_restart.m_east);

	// Write Current and Restart Above position keyvalues.

	write_keyvalue_16bit(filedata,
	                     Enigma::World::Key::ABOVE,
	                     player->m_location.m_above);

	write_keyvalue_16bit(filedata,
	                     Enigma::World::Key::RESTART,
	                     player->m_location_restart.m_above);

	// Write Current and Restart North position keyvalues.

	write_keyvalue_16bit(filedata,
	                     Enigma::World::Key::NORTH,
	                     player->m_location.m_north);

	write_keyvalue_16bit(filedata,
	                     Enigma::World::Key::RESTART,
	                     player->m_location_restart.m_north);

	// Write Current and Restart surface keyvalues.

	write_keyvalue_8bit(filedata,
	                    Enigma::World::Key::SURFACE,
	                    (guint8)player->m_location.m_surface);

	write_keyvalue_8bit(filedata,
	                    Enigma::World::Key::RESTART,
	                    (guint8)player->m_location_restart.m_surface);

	// Write Current and Restart rotation keyvalues.

	write_keyvalue_8bit(filedata,
	                    Enigma::World::Key::ROTATION,
	                    (guint8)player->m_location.m_rotation);

	write_keyvalue_8bit(filedata,
	                    Enigma::World::Key::RESTART,
	                    (guint8)player->m_location_restart.m_rotation);

	// Add Current and Restart Active state keyvalues.

	write_keyvalue_boolean(filedata,
		                     Enigma::World::Key::ACTIVE,
		                     player->m_active);

	write_keyvalue_boolean(filedata,
	                       Enigma::World::Key::RESTART,
	                       player->m_active_restart);
}

//------------------------------------------------------------
// This private function writes a keyvalue with a 16-bit value
// to a filedata stream.
//------------------------------------------------------------
// filedata: Filedata stream.
// key:      Keyvalue key.
// value:    16-bit value.
//------------------------------------------------------------

void write_keyvalue_16bit(std::string& buffer,
                          Enigma::World::Key key,
                          guint16 value)
{
	buffer.push_back((guchar)key);
	buffer.push_back((guchar)value);

	// Write a bank keyvalue if the level exceeds that for a byte.

	if (value > G_MAXUINT8)
	{
		buffer.push_back((guchar)Enigma::World::Key::BANK);
		buffer.push_back((guchar)(value >> 8));
	}
}

//------------------------------------------------------------
// This private function writes a keyvalue with an 8-bit value
// to a filedata stream.
//------------------------------------------------------------
// filedata: Filedata stream.
// key:      Keyvalue key.
// value:    8-bit value.
//------------------------------------------------------------

void write_keyvalue_8bit(std::string& filedata,
                         Enigma::World::Key key,
                         guint8 value)
{
	filedata.push_back((guchar)key);
	filedata.push_back((guchar)value);
}

//-------------------------------------------------------------
// This private function writes a keyvalue with a boolean value
// to a filedata stream.    
//-------------------------------------------------------------
// filedata: Filedata stream.
// key:      Keyvalue key.
// value:    boolean value.
//-------------------------------------------------------------

void write_keyvalue_boolean(std::string& filedata,
                             Enigma::World::Key key,
                             gboolean value)
{
	filedata.push_back((guchar)key);

	if (value)
		filedata.push_back((guchar)1);
	else
		filedata.push_back((guchar)0);
}

//------------------------------------------------------------
// This private function writes a keyvalue followed by a short
// string (255 character maximum) to a filedata stream.
//------------------------------------------------------------
// filedata: Filedata stream.
// key:      Keyvalue key.
// string:   Reference to string to be written.
//------------------------------------------------------------

void write_keyvalue_string(std::string& filedata,
                           Enigma::World::Key key,
                           std::string& string)
{
	// Write keyvalue if the string is not empty.

	if (string.size())
	{
		filedata.push_back((guchar)key);
		filedata.push_back((guint8)string.size());                          
		filedata.append(string);
	}
}
