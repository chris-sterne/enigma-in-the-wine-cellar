// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the ObjectList class header.  The ObjectList class manages
// the simple objects within a game map.
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
 
#ifndef __ENIGMA_OBJECTLIST_H__
#define __ENIGMA_OBJECTLIST_H__

#include <vector>
#include "Object.h"

namespace Enigma
{
	class ObjectList : public std::vector<std::shared_ptr<Enigma::Object>>
	{
		public:			
			// Public methods.

			ObjectList();
			void clear();
			void emplace_back(std::shared_ptr<Enigma::Object> object);
			void find(const Enigma::Location& position,
			          std::vector<std::shared_ptr<Enigma::Object>>& buffer);

		private:
			// Cached container iterator.
			
			std::vector<std::shared_ptr<Enigma::Object>>::iterator m_iterator; 
	};
}

#endif // __ENIGMA_OBJECTLIST_H__
