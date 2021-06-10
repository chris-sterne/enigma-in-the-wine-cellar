// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the PlayerList class implementation.  The PlayerList class
// manages players within a game world.
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

#include "PlayerList.h"

//--------------------------------
// This method is the constructor.
//--------------------------------

Enigma::PlayerList::PlayerList()
{
}

//-----------------------------------------------------------
// This method restores all players to their "restart" state. 
//-----------------------------------------------------------

void Enigma::PlayerList::restart()
{
	// Restore players to their "restart" state.
	
	std::vector<std::shared_ptr<Enigma::Player>>::iterator iterator;
	
	for (iterator = begin(); iterator != end(); iterator ++)		
		(*iterator)->restart();

	// Emit active player in a signal.
	
	do_player();
}

//-----------------------------------------------------------
// This method restores all players to their "saved" state. 
//-----------------------------------------------------------

void Enigma::PlayerList::saved()
{
	// Restore players to their "saved" state.
	
	std::vector<std::shared_ptr<Enigma::Player>>::iterator iterator;
	
	for (iterator = begin(); iterator != end(); iterator ++)		
		(*iterator)->saved();

	// Emit active player in a signal.
	
	do_player();
}

//---------------------------------------------------------------------
// This method prepares all players for playing.  Players restored from
// internal states will prepare themselves automatically, but players
// restored from a game file will need to be manually prepared after
// the game file has been loaded.
//---------------------------------------------------------------------

void Enigma::PlayerList::prepare()
{	
	std::vector<std::shared_ptr<Enigma::Player>>::iterator iterator;
	
	for (iterator = begin(); iterator != end(); iterator ++)
		(*iterator)->prepare();

	// Emit active player in a signal.
	
	do_player();
}

//-----------------------------------------------------------------
// This method returns pointers to all players in a world position. 
//-----------------------------------------------------------------
// location: World location with position to be searched.
// buffer:   Buffer to receive player pointers.
//-----------------------------------------------------------------

void Enigma::PlayerList::find(const Location& position,
             std::vector<std::shared_ptr<Enigma::Player>>& buffer)
{
	std::shared_ptr<Enigma::Object> object;
	std::vector<std::shared_ptr<Enigma::Player>>::iterator iterator;
	
	for (iterator = begin(); iterator != end(); iterator ++)
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

//----------------------------------------------------------
// This method emits the active player in a "player" signal.
//----------------------------------------------------------

void Enigma::PlayerList::do_player()
{
	std::vector<std::shared_ptr<Enigma::Player>>::iterator iterator;
	
	for (iterator = begin(); iterator != end(); iterator ++)
	{
		if ((*iterator)->m_active)
		{
			// Emit active player in a signal.
			
			m_signal_player.emit(*iterator);
			break;
		}
	}
}

//------------------------------------------------
// This method returns the "player" signal server.
//------------------------------------------------

Enigma::PlayerList::type_signal_player Enigma::PlayerList::signal_player()
{
	return m_signal_player;
}
