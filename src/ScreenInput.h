// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2016, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the ScreenInput class header.  The ScreenInput class maps
// a screen area to various game actions, allowing mouse clicks to move
// the player. 
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
 
#ifndef __SCREENINPUT_H__
#define __SCREENINPUT_H__

#include <gtkmm.h>

class CScreenInput
{
  public:
		// Public declarations.

		enum Action
		{
		  ENone = 0,
			EFront,
			EBack,
			ELeft,
			ERight,
			ETurnLeft,
			ETurnRight,
			TOTAL
		};
		
    // Public methods.
    
    CScreenInput();
		void SetBounds( const Gdk::Rectangle& aBounds );
		CScreenInput::Action GetAction( gdouble aX, gdouble aY );

	private:
		// Private class.

		class CActionArea
		{
			public:
				// Public methods.

				CActionArea()
				{
					iAction = CScreenInput::ENone;
					return;
				};

				// Public data.

				Gdk::Rectangle iUnitArea;               // Unit area of screen.
				Gdk::Rectangle iArea;                   // Scaled up unit area of screen.
				CScreenInput::Action iAction;           // Associated action code.
		};
		
		// Private data

		std::list<CActionArea> iActionAreas;      // List of screen action areas.
};

#endif // __SCREENINPUT_H__
