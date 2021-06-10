// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the Mover class implementation.  A Mover object is an
// invisible object that automatically moves the player one position in a
// direction.
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
 
#include "Mover.h"
#include "Mesh.h"

//--------------------------------
// This method is the constructor.
//--------------------------------

Enigma::Mover::Mover()
{
	// Customize base class data.
	
	m_id = Enigma::Object::ID::MOVER;
}

//---------------------------------------
// This method interacts with the object.
//---------------------------------------
// interaction: Interaction data.
//---------------------------------------

void Enigma::Mover::interact(Enigma::Interaction& interaction)
{
	// Get Mover orientation.
	
	Enigma::Location::Direction surface  = m_location.m_surface;
	Enigma::Location::Direction rotation = m_location.m_rotation;
	
	// Determine the effects of a Mover interaction with player.
		
	if  ((surface == Enigma::Location::Direction::CENTER)
	  || (surface == interaction.m_location.m_surface))
	{
		// A central Mover or one the same surface as the player will support
		// and move the player in a direction.  The initial interaction direction
		// should generally be CENTER or NONE to detect this change.
		
		interaction.m_supports       = true;
		interaction.m_next_direction = rotation;
	}
	else
	{
		// A Mover on other surfaces has no effect.
	
		interaction.m_supports       = false;
		interaction.m_next_direction = interaction.m_direction;
	}
	
	// Movers do not block so they cannot block each other if one or more
	// central ones are present in a room.
	
	interaction.m_blocks = false;
}

//-------------------------------------------------------------
// This static method is called within an active OpenGL context
// to initialize class resources.
//-------------------------------------------------------------

void Enigma::Mover::initialize()
{
	// There are no class resources to initialize.
}
