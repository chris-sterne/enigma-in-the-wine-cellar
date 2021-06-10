// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the Object class header.  The Object class is the base
// class of all objects within a game world.
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
 
#ifndef __ENIGMA_OBJECT_H__
#define __ENIGMA_OBJECT_H__

#include <memory>
#include <gdkmm/pixbuf.h>
#include "FineLocation.h"
#include "Interaction.h"

namespace Enigma
{	
	class Object
	{	
		public:
			// Public definitions.
			
			enum class ID         // Object ID values.
			{
				NONE = 0,
				WINECELLAR,
				BLOCKWALL,
				STONEWALL,
				LADDER,
				LADDEREND,
				PERSON,
				BLOCKER,
				MOVER,
				TURNER,
				FACER,
				SURFACER,
				MUSHROOMS,
				CORKSCREW,
				WINEBOTTLE,
				BREAD,
				CHEESE,
				KNIFE,
				APPLE,	
				WINEGLASS,
				PLACEMAT,
				GRAPES,
				SKULL,
				EDGEDRAIN,
				AIRVENT,
				TOTAL
			};
		
			// Public methods

			Object();
			static std::shared_ptr<Enigma::Object> create(Enigma::Object::ID id);
			static void initialize();
			
			// Virtual methods
			
			virtual void render(const Enigma::FineLocation& viewer);
			virtual void interact(Enigma::Interaction& interaction);
			virtual Glib::RefPtr<Gdk::Pixbuf> thumbnail();
			virtual const std::string& singular_name();
			virtual const std::string& plural_name();
			
			// Public data

			Enigma::Object::ID m_id;       // Object ID.
			Enigma::Location m_location;   // Location of object in world.
	};
}

#endif // __ENIGMA_OBJECT_H__
