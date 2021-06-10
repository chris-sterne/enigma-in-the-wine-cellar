// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the ObjectList class implementation.  The ObjectList class
// manages the simple structural objects within a game world.
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

#include "ObjectList.h"

//--------------------------------
// This method is the constructor.
//--------------------------------

Enigma::ObjectList::ObjectList()
{
	// Initialize the cached iterator.
	
	m_iterator = end();
}

//--------------------------------------------
// This method clears the list of all objects.
//--------------------------------------------

void Enigma::ObjectList::clear()
{
	// Clear the container in the base class, then update the cached iterator.

	std::vector<std::shared_ptr<Enigma::Object>>::clear();
	m_iterator = end();
}

//-------------------------------------------
// This method adds a new object to the list.
//-------------------------------------------

void Enigma::ObjectList::emplace_back(std::shared_ptr<Enigma::Object> object)
{
	// Add the object to the base class, then update the cached iterator.
	
	std::vector<std::shared_ptr<Enigma::Object>>::emplace_back(object);	
	m_iterator = end();
}

//----------------------------------------------------------------------------
// This method returns pointers to all objects in a world position.  These
// objects are pre-sorted in ascending order based on their position (Above,
// North, East), the intention being more efficient searches relative to the
// last position found.
//----------------------------------------------------------------------------
// location: World location with position to be examined.
// buffer:   Buffer to receive object pointers.
//----------------------------------------------------------------------------

void Enigma::ObjectList::find(const Location& location,
             std::vector<std::shared_ptr<Enigma::Object>>& buffer)
{
	// Move the iterator backwards while searching for the end a group
	// of objects with a matching position.  If unsuccessful, the iterator
	// will fall on an object with a position too low, or on the first
	// object in the container.

	while ((m_iterator != begin())
	  &&  ((m_iterator == end())
	  ||
	    ((*m_iterator)->m_location.m_above > location.m_above)
	  ||
	    (((*m_iterator)->m_location.m_above == location.m_above)
	  && ((*m_iterator)->m_location.m_north > location.m_north))
	  || 
	    (((*m_iterator)->m_location.m_above == location.m_above)
	  && ((*m_iterator)->m_location.m_north == location.m_north)
	  && ((*m_iterator)->m_location.m_east > location.m_east))))
	{
		m_iterator --;
	}

	// Move the iterator to the first object past the last in a sequence
	// with a matching position, or to an object with a position too high
	// if there are no objects with a matching position.

	while ((m_iterator != end())
	  && (((*m_iterator)->m_location.m_above < location.m_above)
	  ||
	    (((*m_iterator)->m_location.m_above == location.m_above)
	  && ((*m_iterator)->m_location.m_north < location.m_north))
	  || 
	    (((*m_iterator)->m_location.m_above == location.m_above)
	  && ((*m_iterator)->m_location.m_north == location.m_north)
	  && ((*m_iterator)->m_location.m_east <= location.m_east))))
	{
		m_iterator ++;
	}

	// Copy objects with a matching location to the buffer.

	while (m_iterator != begin())
	{
		// Since the iterator following a seek will be on the object
		// just past a desired object (or past the end of the container),
		// move the iterator back one step.

		m_iterator --;

    if  (((*m_iterator)->m_location.m_east == location.m_east)
      && ((*m_iterator)->m_location.m_above == location.m_above)
      && ((*m_iterator)->m_location.m_north == location.m_north))
		{
			// The object has a matching location.  Copy the iterator
      // to this object into the provided buffer.

			buffer.emplace_back(*m_iterator);
		}
		else
		{
			// The object does not have a matching location.
			// Break out of the loop.
			
			break;
		}
	}
}
