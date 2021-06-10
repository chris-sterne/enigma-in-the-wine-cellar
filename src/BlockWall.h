// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the BlockWall class header.  A BlockWall is an object
// (made from stone blocks) that can block or support a player.
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
 
#ifndef __ENIGMA_BLOCKWALL_H__
#define __ENIGMA_BLOCKWALL_H__

#include "Object.h"
#include "FineLocation.h"
#include "Interaction.h"

namespace Enigma
{
	class BlockWall : public Enigma::Object
	{
		public:
			// Public methods.

			BlockWall();
			void render(const Enigma::FineLocation& viewer) override;
			void interact(Enigma::Interaction& interaction) override;
			static void initialize();
	};
}

#endif // __ENIGMA_BLOCKWALL_H__
