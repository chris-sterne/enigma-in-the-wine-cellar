// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the Mover class header.  A Mover object is an invisible
// object that automatically moves the player one position in a direction.
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
 
#ifndef __ENIGMA_MOVER_H__
#define __ENIGMA_MOVER_H__

#include "Object.h"
#include "FineLocation.h"
#include "Interaction.h"

namespace Enigma
{
	class Mover : public Enigma::Object
	{
		public:
			// Public methods.

			Mover();
			void interact(Enigma::Interaction& interaction) override;
			static void initialize();
	};
}

#endif // __ENIGMA_MOVER_H__
