// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the FineLocation class header.  The FineLocation class
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
 
#ifndef __ENIGMA_FINELOCATION_H__
#define __ENIGMA_FINELOCATION_H__

#include "Location.h"

namespace Enigma
{
	class FineLocation : public Enigma::Location
	{
		public:
			// Public methods.

			FineLocation();
			FineLocation(Enigma::Location& location);
			void set(Enigma::Location& location);
			void clear();

			// Public data.
			
			float m_fine_east;
			float m_fine_above;
			float m_fine_north;
			float m_fine_rotate_east;
			float m_fine_rotate_above;
			float m_fine_rotate_north;
			
		private:
			// Private methods.
			
			void clear_fine();
	};
}

#endif // __ENIGMA_FINELOCATION_H__
