// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the World class header.  The World class manages all objects
// within a game world.
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
 
#ifndef __ENIGMA_WORLD_H__
#define __ENIGMA_WORLD_H__

#include <list>
#include "Boundary.h"
#include "ObjectList.h"
#include "ItemList.h"
#include "PlayerList.h"

namespace Enigma
{
	class World
	{
		public:
			// Public declarations.
			
			enum class Key       // World file key definitions.
			{
				NONE = 0,
				END,               // Introduces the end of keyvalue array.		
				OBJECT,            // Introduces a simple object.
				PLAYER,            // Introduces a player object.
				ITEM,              // Introduces an item object.
				DESCRIPTION,       // Introduces a description of the map.
				SURFACE,           // Surface on which object is located.
				ROTATION,          // Rotation of object on surface.
				ABOVE,             // Above location of object.
				NORTH,             // North location of object.
				EAST,              // East location of object.
				BANK,              // Adds high byte onto previous key's value.
				CATEGORY,          // Category of item.
				OWNER,             // Owner of item.
				ACTIVE,            // Active player or item.
				LENGTH,            // Length of a data section.
				DATA,              // Indicates the start of data section.
				SAVED,             // Saved value.
				RESTART,           // Restart value.
				CURRENT,           // Current value.    
				CONTROLLER,        // Introduces a map controller.
				SIGNAL,            // Map controller signal name block.
				CODE,              // Map controller bytecode block.
				SENSE,             // Sensor state signal.
				STATE,             // Functional state signal.
				VISIBILITY,        // Visibility state signal.
				PRESENCE,          // Presence state signal.
				OUTDOOR,           // Player's environment.
				SELECTED,          // Selected state of item.
				USED,              // Used state of item.
				TELEPORTER,        // Introduces a teleporter object.
				ARRIVAL,           // Teleporter arrival value.
				TOTAL
			};
			
			enum class Language  // Description key language values.
			{
				NONE = 0,
				ENGLISH,
				TOTAL
			};
		
			// Public methods.

			World();
			void clear();
			bool load(const std::string& filename);
			bool save(const std::string& filename);
			void restart();
			const Glib::ustring& description();
			std::shared_ptr<Enigma::ItemList>& items();
			std::shared_ptr<Enigma::PlayerList>& players();

		private:
			// Private data.
			
			std::string m_filename;
			Glib::ustring m_description;
			std::shared_ptr<Enigma::Boundary> m_boundary;
			std::shared_ptr<Enigma::ObjectList> m_objects;
			std::shared_ptr<Enigma::ItemList> m_items;
			std::shared_ptr<Enigma::PlayerList> m_players;
			bool m_savable;
	};
}

#endif // __ENIGMA_WORLD_H__
