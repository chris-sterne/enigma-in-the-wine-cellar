// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the Object class header.  The Item class describes an object
// that can be found by a player.
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
 
#ifndef __ENIGMA_ITEM_H__
#define __ENIGMA_ITEM_H__

#include "Object.h"

namespace Enigma
{
	class Item
	{	
		public:
			// Public definitions.
			
			enum class Category   // Item categories.
			{
				NONE = 0,
				REQUIRED,           // Item is required.
				BONUS,              // Item is a bonus.
				SECRET,             // Item is a secret bonus.
				SKULL,              // Item indicates player is trapped.
				TOTAL
			};
						
			// Public methods.
			
			Item(Enigma::Object::ID id);
			void render(const Enigma::FineLocation& viewer);
			void interact(Enigma::Interaction& interaction);
			void restart();

			// Public data.
			
			std::shared_ptr<Enigma::Object> m_object;
			Enigma::Item::Category m_category;
			
			// Current item state.
			
			bool m_active;             // TRUE if item not yet found.
			guint8 m_owner;            // Player number of item owner.
			bool m_selected;           // TRUE if item selected for use.
			bool m_used;               // TRUE if item successfully used.
			
			// Saved item state.
			
			bool m_active_saved;
			guint8 m_owner_saved;
			bool m_selected_saved;
			bool m_used_saved;
			
			// Restart item state.
			
			bool m_active_restart;
			guint8 m_owner_restart;
			bool m_selected_restart;
			bool m_used_restart;
	};
}

#endif // __ENIGMA_ITEM_H__
