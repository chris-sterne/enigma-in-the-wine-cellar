// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the Boundary class implementation.  The Boundary class
// describes the position limits of a game world.
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
#include "Boundary.h"

//--------------------------------
// This method is the constructor.
//--------------------------------

Enigma::Boundary::Boundary()
{
	// Initialize instance values.
	
	clear();
}

//---------------------------------
// This method clears the boundary.
//---------------------------------

void Enigma::Boundary::clear()
{
	// Initialize boundary values.  The lower corner values are set higher
	// than the upper corner values to indicate an empty boundary.	
	
	m_lower_east  = 1;
	m_lower_above = 1;
	m_lower_north = 1;
	m_upper_east  = 0;
	m_upper_above = 0;
	m_upper_north = 0;
}

//-----------------------------------------------------
// This method returns TRUE if the boundary is cleared.
//-----------------------------------------------------

bool Enigma::Boundary::is_clear()
{
	// A lower east value that is greater than the upper east value
	// is a sufficient indication of a cleared boundary.
	
	return (m_lower_east > m_upper_east);
}

//-----------------------------------------
// This method interacts with the boundary.
//-----------------------------------------
// interaction: Interaction data.
//-----------------------------------------

void Enigma::Boundary::interact(Enigma::Interaction& interaction)
{
	// The boundary will block if the interaction occurs in a direction
	// that encounters a boundary limit.
	
	switch (interaction.m_direction)
	{
		case Enigma::Location::Direction::NORTH:
			interaction.m_blocks = (interaction.m_location.m_north == m_upper_north);
			break;
			
		case Enigma::Location::Direction::SOUTH:
			interaction.m_blocks = (interaction.m_location.m_north == m_lower_north);
			break;
			
		case Enigma::Location::Direction::EAST:
			interaction.m_blocks = (interaction.m_location.m_east == m_upper_east);
			break;
			
		case Enigma::Location::Direction::WEST:
			interaction.m_blocks = (interaction.m_location.m_east == m_lower_east);
			break;
			
		case Enigma::Location::Direction::ABOVE:
			interaction.m_blocks = (interaction.m_location.m_above == m_upper_above);
			break;
			
		case Enigma::Location::Direction::BELOW:
			interaction.m_blocks = (interaction.m_location.m_above == m_lower_above);
			break;
			
		default:
			interaction.m_blocks = false;
			break;
	}

	// The boundary will provide support if the interaction occurs beyond
	// the limits.  This is not a normal situation, but at least the application
	// will not remain in long loop trying to become stable if a software bug
	// allowed the boundary to be crossed.  Support is also provided on the
	// lowest below surface, mainly to "fill in the gaps" in test worlds.
	
	interaction.m_supports =
			((interaction.m_location.m_north > m_upper_north)
		|| (interaction.m_location.m_north < m_lower_north)
		|| (interaction.m_location.m_east > m_upper_east)
		|| (interaction.m_location.m_east < m_lower_east)
		|| (interaction.m_location.m_above > m_upper_above)
		|| (interaction.m_location.m_above <= m_lower_above));
}

//--------------------------------------------------------------------
// This method adjusts the boundary as needed to contain the position.
// of a world location.
//--------------------------------------------------------------------
// location: World location to be bound.
//--------------------------------------------------------------------

void Enigma::Boundary::adjust(const Enigma::Location& location)
{
	adjust(location.m_east, location.m_above, location.m_north);
}

//----------------------------------------------------------------------
// This method adjusts the boundary as needed to contain a new position.
//----------------------------------------------------------------------
// east:  East position value.
// above: Above position value.
// north: North position value.
//----------------------------------------------------------------------

void Enigma::Boundary::adjust(guint16 east, guint16 above, guint16 north)
{
	if (is_clear())
	{
		// The boundary is currently cleared.  Set both boundary corners
		// to the position value.
		
		m_lower_east  = east;
		m_lower_above = above;
		m_lower_north = north;
		m_upper_east  = east;
		m_upper_above = above;
		m_upper_north = north;
	}
	else
	{
		// Expand the West-East boundaries as needed.
	
		if (east < m_lower_east)
			m_lower_east = east;
		else if (east > m_upper_east)
			m_upper_east = east;
			
		// Expand the Below-Above boundaries as needed.
	
		if (above < m_lower_above)
			m_lower_above = above;
		else if (above > m_upper_above)
			m_upper_above = above;
			
		// Expand the South-North boundaries as needed.
	
		if (north < m_lower_north)
			m_lower_north = north;
		else if (north > m_upper_north)
			m_upper_north = north;
	}
}
