// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the AirVent class header.  The AirVent class is a wall vent
// that brings fresh air into rooms.
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
 
#ifndef __ENIGMA_AIRVENT_H__
#define __ENIGMA_AIRVENT_H__

#include "Object.h"
#include "FineLocation.h"
#include "Interaction.h"

namespace Enigma
{
	class AirVent : public Enigma::Object
	{
		public:
			// Public methods.

			AirVent();
			void render(const Enigma::FineLocation& viewer) override;
			static void initialize();
	};
}

#endif // __ENIGMA_AIRVENT_H__
