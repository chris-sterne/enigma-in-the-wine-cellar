// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the Sound class header.  The Sound class plays a sound effect.
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
 
#ifndef __ENIGMA_SOUND_H__
#define __ENIGMA_SOUND_H__

#include <string>

namespace Enigma
{
	class Sound
	{
		public:
			// Public methods.

			Sound();
			void load(std::string name);
			void play();
			static void initialize();
			static void destroy();
			
		private:
			// Private data.
			
			std::string m_name;   // Sound name.			
	};
}

#endif // __ENIGMA_SOUND_H__
