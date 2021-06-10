// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the Grapes class header.  The Grapes class describes a bunch
// of grapes object.
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
 
#ifndef __ENIGMA_GRAPES_H__
#define __ENIGMA_GRAPES_H__

#include "Object.h"

namespace Enigma
{
	class Grapes : public Enigma::Object
	{
		public:
			// Public methods.

			Grapes();
			void render(const Enigma::FineLocation& viewer) override;
			Glib::RefPtr<Gdk::Pixbuf> thumbnail() override;
			const std::string& singular_name() override;
			const std::string& plural_name() override;
			static void initialize();
	};
}

#endif // __ENIGMA_GRAPES_H__
