// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the UserAction class header.  The UserAction class converts
// user interface input into player motion actions.
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
 
#ifndef __ENIGMA_USERACTION_H__
#define __ENIGMA_USERACTION_H__

#include <sigc++/sigc++.h>
#include <gdkmm/event.h>
#include "Player.h"

namespace Enigma
{
	class UserAction
	{
		public:			
			// Public methods.

			UserAction();
			bool button_press(GdkEventButton* button_event);
			void resize(int width, int height);
			
			// Player action signal accessor.

			typedef sigc::signal<void, Enigma::Player::Action> type_signal_action;
			type_signal_action signal_action();
			
		private:
			// Private data.
			
			type_signal_action m_signal_action;   // Player action signal server.
			
			// Input area geometry.
			
			int m_width;       // Width of input area.
			int m_height;      // Height of input area.
			gdouble m_slope;   // Slope of diagonal across input area.
	};
}

#endif // __ENIGMA_CONTROLLER_H__
