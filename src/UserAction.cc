// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the UserAction class implementation.  The UserAction class
// converts user interface input into player motion actions.
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

#include "UserAction.h"

//--------------------------------
// This method is the constructor.
//--------------------------------

Enigma::UserAction::UserAction()
{
	// Initialize input area geometry.

	m_width  = 0;
	m_height = 0;
	m_slope  = 0.0f;
}

//------------------------------------
// This method resizes the input area.
//------------------------------------
// width:  Width area.
// height: Hight of area.
//------------------------------------

void Enigma::UserAction::resize(int width, int height)
{	
	// Save input area size.

	m_width  = width;
	m_height = height;
	
	// Calculate slope of diagonal line across input area.
	
	if (width > 0)
		m_slope = (gdouble)height / width;
	else
		m_slope = (gdouble)height;
}

//------------------------------------------------------------------
// This method handles mouse button press events from an input area.
//------------------------------------------------------------------
// key_event: Pointer to GdkEventButton.       
// RETURN:    TRUE if button press was handled.
//------------------------------------------------------------------

bool Enigma::UserAction::button_press(GdkEventButton* button_event)
{
	bool handled = true;
	
	if (button_event->type == GDK_BUTTON_PRESS)
	{
		// Get input area coordinates (x, y) of button press.
		
		gdouble x = button_event->x;
		gdouble y = button_event->y;
		
		// Calculate y coordinate associated with the x coordinate on diagonal
		// lines that cross the input area.  These lines divide the input area
		// into four triangular regions, each of which is assigned a particular
		// player action (NOTE: (0,0) is in upper-left corner):
		//
		//  y1\  move   /
		//     \ front /
		//      \     /
		//       \   /
		//  turn  \ /  turn
		//  left   x   right
		//        / \
		//       /   \
		//      /     \
		//     / move  \
		//  y2/  back   \
		
		gdouble y1 = m_slope * x;
		gdouble y2 = (gdouble)m_height - y1;
		
		// Emit the player action associated with the button press region. 
		
		if (y >= y1)
		{
			if (y >= y2)
				m_signal_action.emit(Enigma::Player::Action::MOVE_BACK);
			else
				m_signal_action.emit(Enigma::Player::Action::TURN_LEFT);
		}
		else
		{	
			if (y >= y2)
				m_signal_action.emit(Enigma::Player::Action::TURN_RIGHT);
			else
				m_signal_action.emit(Enigma::Player::Action::MOVE_FRONT);
		}
	}
	else
		handled = false;
	
	return handled;
}

//-----------------------------------------------------
// This method returns the player action signal server.
//-----------------------------------------------------

Enigma::UserAction::type_signal_action Enigma::UserAction::signal_action()
{
	return m_signal_action;
}
