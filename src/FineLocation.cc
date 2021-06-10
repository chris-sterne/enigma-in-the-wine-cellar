// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the FineLocation class implementation.  The FineLocation class
// allows for finer positioning of an object within a game world.
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

#include "FineLocation.h"

//--------------------------------
// This method is the constructor.
//--------------------------------

Enigma::FineLocation::FineLocation()
{
	// Clear fine location values.  The parent class instance will
	// clear its coarse location values during construction.
	
	clear_fine();
}

//-------------------------------------------------------
// This constructor copies values from a coarse location.
//-------------------------------------------------------

Enigma::FineLocation::FineLocation(Enigma::Location& location)
{
	// Copy coarse location values and clear fine location values.

	set(location);
}

//----------------------------------------
// This method clears all location values.
// ---------------------------------------

void Enigma::FineLocation::clear()
{
	// Clear coarse and fine location values.
		
	Enigma::Location::clear();
	clear_fine();
}

//---------------------------------------------------------
// This method sets location values from a coarse Location.
// --------------------------------------------------------

void Enigma::FineLocation::set(Enigma::Location& location)
{
	// Copy coarse location values.

	m_east     = location.m_east;
	m_above    = location.m_above;
	m_north    = location.m_north;
	m_surface  = location.m_surface;
	m_rotation = location.m_rotation;
	
	// Clear fine location values.
	
	clear_fine();
}

//-----------------------------------------------------
// This private method clears all fine location values.
// ----------------------------------------------------

void Enigma::FineLocation::clear_fine()
{	
	// Clear fine location values.

	m_fine_east         = 0.0f; 
	m_fine_above        = 0.0f;
	m_fine_north        = 0.0f;
	m_fine_rotate_east  = 0.0f;
	m_fine_rotate_above = 0.0f;
	m_fine_rotate_north = 0.0f;
}
