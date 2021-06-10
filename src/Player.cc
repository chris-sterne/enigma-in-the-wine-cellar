// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the Player class implementation.  An instance of the Player
// class represents a player in a game map.
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

#include "Player.h"
#include "ObjectList.h"
#include "Sound.h"

//-------------------
// Local declarations
//-------------------

static const std::string bump_name = "Bump.ogg";
static Enigma::Sound bump_sound;

//-------------------------------------------------------------------
// This constructor has an Object ID argument which is used to create
// the Object being turned into a player.
//-------------------------------------------------------------------
// id: Object ID.
//-------------------------------------------------------------------

Enigma::Player::Player(Enigma::Object::ID id) : Enigma::Viewer(id)
{
	// Initialize instance data.
	
	m_active  = false;
	m_outdoor = false;
	m_falling = false;
	m_number  = 0;

	// Set initial Saved player state.

	m_location_saved = m_location;
	m_active_saved   = m_active;
	m_outdoor_saved  = m_outdoor;

	// Set initial Restart player state.

	m_location_restart = m_location;
	m_active_restart   = m_active;
	m_outdoor_restart  = m_outdoor;
}

//--------------------------------------------------------
// This method restores the player to its "restart" state.
//--------------------------------------------------------

void Enigma::Player::restart()
{
	// Set current state from "restart" state.

	m_location      = m_location_restart;
	m_next_location = m_location_restart;
	m_active        = m_active_restart;
	m_outdoor       = m_outdoor_restart;

	// Prepare player for the next game.
	
	prepare();
}

//------------------------------------------------------
// This method restores the player to its "saved" state.
//------------------------------------------------------

void Enigma::Player::saved()
{
	// Set current state from "saved" state.

	m_location      = m_location_saved;
	m_next_location = m_location_saved;
	m_active        = m_active_saved;
	m_outdoor       = m_outdoor_saved;

	// Prepare player for the next game.
	
	prepare();
}

//------------------------------------------------------------------
// This method is a common entry point following the restoration of
// a player's state from a game file or internal state.  The player
// will be prepared to start playing.
//------------------------------------------------------------------

void Enigma::Player::prepare()
{
	// Clear pending motions in the parent viewer, and disconnect from
	// the viewer "update done" slot.  This ensures no motion is carried
	// forward to the next game.
	
	clear_motion();
	slot_update_done().disconnect();
	
	// Clear pending actions from the queue so none are not carried
	// forward to the next game.
	
	while (!m_action_queue.empty())
		m_action_queue.pop();

	// Refill list of owned items from the world items.  This assumes
	// the world items have been restored to their "restart" state.
	
	m_owned_items.clear();
	m_world_items->find(m_number, m_owned_items);
}

//-----------------------------------------------------------------------
// This method adds an action to the queue.
//-----------------------------------------------------------------------
// action: Action to be added.
//-----------------------------------------------------------------------

void Enigma::Player::queue_action(Enigma::Player::Action action)
{
	// Add the new action to the queue.  Begin handling this new action
	// if the queue was originally empty (player was not busy handling
	// another action).

	m_action_queue.push(action);
	
	if (m_action_queue.size() == 1)
		handle_action();
}

//-------------------------------------------------
// This method handles player actions in the queue.
//-------------------------------------------------

void Enigma::Player::handle_action()
{	
	if (!m_action_queue.empty())
	{
		// Get the front action from the queue, but leave it in queue until
		// it has been handled.  This indicates the player is busy handling 
		// the action.
		
		Enigma::Player::Action action = m_action_queue.front();
		
		switch (action)
		{
			case Enigma::Player::Action::HOLD_STILL:
				hold_still();
				break;
				
			case Enigma::Player::Action::TURN_LEFT:
				turn_left();
				break;
				
			case Enigma::Player::Action::TURN_RIGHT:
				turn_right();
				break;
				
			case Enigma::Player::Action::MOVE_FRONT:
				move(m_location.m_rotation);
				break;
				
			case Enigma::Player::Action::MOVE_BACK:
				move(Enigma::Location::get_opposite(m_location.m_rotation));
				break;
				
			default:
				action_handled();
				break;
		}
	}
}

//--------------------------------------------------------------------
// This method attempt to have the player remain at rest in the room.
// This will occur if there is a supporting object (platform) present
// and no objects that will move the player.
//--------------------------------------------------------------------

void Enigma::Player::hold_still()
{	
	// Disconnect from the viewer "update done" slot.
	
	slot_update_done().disconnect();
	
	// Update the room object lists.

	m_room_objects.clear();
	m_room_items.clear();
	
	m_world_objects->find(m_location, m_room_objects);		
	m_world_items->find(m_location, m_room_items);
	
	// Handle all items found in the room.
	
	bool found = false;
	std::vector<std::shared_ptr<Enigma::Item>>::iterator item_iterator;
	
	for (item_iterator = m_room_items.begin();
	     item_iterator != m_room_items.end();
	     item_iterator ++)
	{		
		// Inform the world item list that the item has been found.
		// A dialog to announce the find will be displayed.
		
		m_world_items->found(m_number, *item_iterator);
		
		// Add item to the list of owned items, and set a flag to indicate
		// a change has occurred in this list.
		
		m_owned_items.emplace_back(*item_iterator);
		found = true;
	}
	
	// Emit updated list of owned items in an "items" signal. 
	
	if (found)
		do_items();
	
	// Handle all objects found in the room.
	
	Enigma::Interaction interaction;
	interaction.m_location      = m_location;
	interaction.m_next_rotation = m_location.m_rotation;
	bool supported = false;

	std::vector<std::shared_ptr<Enigma::Object>>::iterator object_iterator;
	std::vector<std::shared_ptr<Enigma::Object>>::iterator blocker_iterator;

	for (object_iterator = m_room_objects.begin();
	     object_iterator != m_room_objects.end();
	     object_iterator ++)
	{
		// Set the interaction direction to CENTER to test for objects
		// that will automatically move the player.
		
		interaction.m_direction = Enigma::Location::Direction::CENTER;
		(*object_iterator)->interact(interaction);

		// Set a flag if at least one room object supports the player.
		
		if (interaction.m_supports)
			supported = true;
		
		// Set the next viewer rotation if an object will turn the player.
		// This will be an immediate rather than transition change.
		
		if (interaction.m_next_rotation != interaction.m_location.m_rotation)
			set_rotation(interaction.m_next_rotation);
		
		// If there is an object that will move the player, ensure the
		// direction of motion is unblocked before accepting the move.
		
		if (interaction.m_next_direction != interaction.m_direction)
		{			
			interaction.m_direction = interaction.m_next_direction;

			for (blocker_iterator = m_room_objects.begin();
			     blocker_iterator != m_room_objects.end();
			     blocker_iterator ++)
			{
				(*blocker_iterator)->interact(interaction);
				
				if (interaction.m_blocks)
					break;
			}
			
			// If no room objects block the motion, test the world boundary.
			
			if (!interaction.m_blocks)
				m_world_boundary->interact(interaction);

			// If nothing will block the player from moving in the new direction,
			// post a motion request in the viewer, and set the hold_still()
			// callback method to call after the player has moved into the new
			// position.
			
			if (!interaction.m_blocks)
			{
				set_move_one(interaction.m_direction);
				slot_update_done() = sigc::mem_fun(*this,&Enigma::Player::hold_still);
			}
		}
	}

	if (!supported)
	{
		// No room objects support the player.  Test the world boundary
		// for support.
	
		interaction.m_direction = Enigma::Location::Direction::CENTER;
		m_world_boundary->interact(interaction);
		supported = interaction.m_supports;
	}
	
	if (supported)
	{
		// A platform provides support for the player.  Play a "bump" sound
		// if the player was falling and has now stopped.
		
		if (m_falling)
		{
			m_falling = false;		
			bump_sound.play();
		}
	}
	else
	{
		// Begin falling since no platform provides support for the player.
		// Set the hold_still() callback method to call after the viewer
		// has moved into the room below.
		
		slot_update_done() = sigc::mem_fun(*this, &Enigma::Player::hold_still);
		set_move_one(interaction.m_location.m_surface);
		m_falling = true;
	}

	// Request an update from the viewer.  This may be a single view
	// frame rendering of the room, or the beginning of a transition
	// to a new room.
	
	update();
	
	// If no callback method has been set, this completes the handling
	// of the original action that led to hold_still() being called.
	
	if (slot_update_done().empty())
		action_handled();
}

//----------------------------------------------------------
// This method moves the player one position in a direction.
//----------------------------------------------------------
// direction: Direction of move.
//----------------------------------------------------------

void Enigma::Player::move(Enigma::Location::Direction direction)
{
	// Prepare interaction information for objects.
	
	Enigma::Interaction interaction;
	
	interaction.m_location  = m_location;
	interaction.m_direction = direction;
	
	// The player searches for an object in the room that provides support
	// (act as a platform), and has the highest interaction zone along the
	// interaction direction.

	short zone = -5;
	
	std::vector<std::shared_ptr<Enigma::Object>>::iterator iterator =
		m_room_objects.begin();
	
	std::vector<std::shared_ptr<Enigma::Object>>::iterator platform_iterator =
		m_room_objects.end();
	
	while (iterator != m_room_objects.end())
	{
		(*iterator)->interact(interaction);
		
		if (interaction.m_supports && (interaction.m_zone > zone))
		{
			// A more prominent platform has been found.
			
			platform_iterator = iterator;
			zone              = interaction.m_zone;
		}
		
		iterator ++;
	}

	if (platform_iterator != m_room_objects.end())
	{
		// The player has found a platform object. Determine its effect on
		// the original interaction direction.

		(*platform_iterator)->interact(interaction);
		interaction.m_direction = interaction.m_next_direction;
	}
	else
	{
		// The player could not find a platform object.  See if the world
		// boundary will provide support.
		
		m_world_boundary->interact(interaction);
		
		// If the world boundary does not provide support, the player starts
		// falling down.
		
		if (!interaction.m_supports)
		{
			m_falling = true;
			interaction.m_direction = m_location.m_surface;
		}
	}
	
	// Test for an object in the final interaction direction that will block
	// motion in that direction.
	
	iterator = m_room_objects.begin();
	
	std::vector<std::shared_ptr<Enigma::Object>>::iterator blocker_iterator =
		m_room_objects.end();
	
	while (iterator != m_room_objects.end())
	{
		(*iterator)->interact(interaction);
	
		// Exit from the loop if a blocker has been found.
		// Otherwise, continue searching.
		
		if (interaction.m_blocks)
		{
			blocker_iterator = iterator;
			iterator         = m_room_objects.end();
		}
		else
			iterator ++;
	}

	// Test the world boundary for blocking.
	
	m_world_boundary->interact(interaction);

	if ((blocker_iterator == m_room_objects.end())
	  && !interaction.m_blocks)
	{
		// The player is not blocked from moving in the new direction.  Set
		// hold_still() callback method to call after viewer has moved into
		// the new room.
		
		slot_update_done() = sigc::mem_fun(*this, &Enigma::Player::hold_still);

		// Initiate a move by the viewer.
		
		set_move_one(interaction.m_direction);
		update();
	}
	else
	{		
		// The player is blocked from moving.  This completes the handling
		// of the move action.
		
		bump_sound.play();
		action_handled();
	}
}

//------------------------------------------------
// This method handles the action of turning left.
//------------------------------------------------

void Enigma::Player::turn_left()
{	
	// Set callback method to call after viewer has turned left.
	
	slot_update_done() = sigc::mem_fun(*this, &Enigma::Player::action_handled);

	// Initiate a left turn by the viewer. 
	
	set_turn_left();
	update();
}

//-------------------------------------------------
// This method handles the action of turning right.
//-------------------------------------------------

void Enigma::Player::turn_right()
{	
	// Set callback method to call after viewer has turned right.
	
	slot_update_done() = sigc::mem_fun(*this, &Enigma::Player::action_handled);

	// Initiate a right turn by the viewer. 
	
	set_turn_right();
	update();
}

//--------------------------------------------------------
// This method is called after an action has been handled.
//--------------------------------------------------------

void Enigma::Player::action_handled()
{	
	// Disconnect from the viewer "update done" slot.
	
	slot_update_done().disconnect();
	
	// Remove the current action from the queue.

	if (!m_action_queue.empty())
		m_action_queue.pop();

	// Handle the next available action.

	handle_action();
}

//----------------------------------------------------------------
// This method emits the list of owned items in an "items" signal.
//----------------------------------------------------------------

void Enigma::Player::do_items()
{
	m_signal_items.emit(m_owned_items);
}

//---------------------------------------------------------------
// This method returns the player's "items" signal server.
//---------------------------------------------------------------

Enigma::Player::type_signal_items Enigma::Player::signal_items()
{
	return m_signal_items;
}

//------------------------------------------------
// This static method initializes class resources.
//------------------------------------------------

void Enigma::Player::initialize()
{
	// Prepare sounds.
	
	bump_sound.load(bump_name);
}
