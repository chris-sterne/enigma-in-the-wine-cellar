// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the Player class header.  An instance of the Player class
// represents a player in a game map.
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

#ifndef __ENIGMA_PLAYER_H__
#define __ENIGMA_PLAYER_H__

#include <queue>
#include <glibmm/main.h>
#include <gdkmm/glcontext.h>
#include "Viewer.h"

namespace Enigma
{	
	class Player : public Enigma::Viewer
	{
		public:
			// Pubic definitions.
			
			enum class Action
			{
				NONE = 0,
				HOLD_STILL,
				MOVE_FRONT,
				MOVE_BACK,
				MOVE_RIGHT,
				MOVE_LEFT,
				TURN_RIGHT,
				TURN_LEFT,
				TOTAL
			};
			
		  // Public methods.
		  
		  Player(Enigma::Object::ID id);
		  void queue_action(Enigma::Player::Action action);
		  void do_items();
		  void restart();
		  void saved();
		  void prepare();
		  static void initialize();

			// "items" signal type and server accessor.

			typedef sigc::signal<void, std::vector<std::shared_ptr<Enigma::Item>>&>
				type_signal_items;

			type_signal_items signal_items();

			// Current player state (Viewer superclass has location).
			
			bool m_active;
			bool m_outdoor;
			bool m_falling;
			
			// Saved player state.
			
			Enigma::Location m_location_saved;
			bool m_active_saved;
			bool m_outdoor_saved;
			
			// Restart player state.
			
			Enigma::Location m_location_restart;
			bool m_active_restart;
			bool m_outdoor_restart;

			guint8 m_number;   // Player number used for item owner.

		private:
			// Private methods.
			
			void handle_action();
			void action_handled();
			void hold_still();
			void turn_left();
			void turn_right();
			void move(Enigma::Location::Direction direction);
	
			// Private data.

			std::queue<Enigma::Player::Action> m_action_queue;
			type_signal_location m_signal_location;
			type_signal_items m_signal_items;
			std::vector<std::shared_ptr<Enigma::Item>> m_owned_items;
			
			// Room objects.
			
			std::vector<std::shared_ptr<Enigma::Object>> m_room_objects;
			std::vector<std::shared_ptr<Enigma::Item>> m_room_items;
	};
}

#endif // __ENIGMA_PLAYER_H__


