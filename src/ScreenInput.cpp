// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2016, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the ScreenInput class implementation.  The ScreenInput class
// maps a screen area to various game actions, allowing mouse clicks to move
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

#include "ScreenInput.h"

// The screen is divided into areas for game actions. The center vertical region
// is divided into 2, the outer columns are divided into 4, and the horizontal
// width is divided into 3.  The area is considered as a grid that is then scaled
// to fit within provided screen bounds.
//
//  -----------------------
//  |      |       |      |
//  |      |   2   |      |
//  |  1   |       |   3  |
//  |      |-------|      |
//  |      |       |      |
//  -------|   5   |------|
//  |  4   |       |   6  |
//  -----------------------
//
// 1: Turn Left
// 2: Move Front
// 3: Turn Right
// 4: Move Left
// 5: Move Back
// 6: Move Right

//*----------------------*
//* Default constructor. *
//*----------------------*

CScreenInput::CScreenInput()
{
	// Fill a list of screen ActionAreas.  The most likely actions are placed
	// at the beginning of the list so they will be found first.

	CActionArea ActionArea;

	ActionArea.iUnitArea = Gdk::Rectangle( 1, 0, 1, 2 );
	ActionArea.iAction   = CScreenInput::EFront;
	iActionAreas.push_back( ActionArea );

	ActionArea.iUnitArea = Gdk::Rectangle( 0, 0, 1, 3 );
	ActionArea.iAction   = CScreenInput::ETurnLeft;
	iActionAreas.push_back( ActionArea );

	ActionArea.iUnitArea = Gdk::Rectangle( 2, 0, 1, 3 );
	ActionArea.iAction   = CScreenInput::ETurnRight;
	iActionAreas.push_back( ActionArea );

	ActionArea.iUnitArea = Gdk::Rectangle( 1, 2, 1, 2 );
	ActionArea.iAction   = CScreenInput::EBack;
	iActionAreas.push_back( ActionArea );

	ActionArea.iUnitArea = Gdk::Rectangle( 0, 3, 1, 1 );
	ActionArea.iAction   = CScreenInput::ELeft;
	iActionAreas.push_back( ActionArea );

	ActionArea.iUnitArea = Gdk::Rectangle( 2, 3, 1, 1 );
	ActionArea.iAction   = CScreenInput::ERight;
	iActionAreas.push_back( ActionArea );

	return;
}

//*-------------------------------------------------------------------------*
//* This method divides up the provided bounds into areas for game actions. *
//*-------------------------------------------------------------------------*

void CScreenInput::SetBounds( const Gdk::Rectangle& aBounds )
{
	// Calculate size of screen sub-areas.
	
	int ColumnWidth = aBounds.get_width() / 3;
	int RowHeight   = aBounds.get_height() / 4;
	int X;
	int Y;
	int Width;
	int Height;
	
	// Assign coordinates to the scaled up unit action areas.

	std::list<CActionArea>::iterator ActionArea;

	for ( ActionArea = iActionAreas.begin();
				ActionArea != iActionAreas.end(); 
				ActionArea ++ )
	{
		// Scale up the unit action area.

		X      = (*ActionArea).iUnitArea.get_x() * ColumnWidth;
		Y      = (*ActionArea).iUnitArea.get_y() * RowHeight;
		Width  = (*ActionArea).iUnitArea.get_width() * ColumnWidth;
		Height = (*ActionArea).iUnitArea.get_height() * RowHeight;

		// Assign scaled-up area to action area.
		
		(*ActionArea).iArea.set_x( X );
		(*ActionArea).iArea.set_y( Y );
		(*ActionArea).iArea.set_width( Width );
		(*ActionArea).iArea.set_height( Height );
	}

	return;
}

//*---------------------------------------------------------------------------------*
//* This method determines the game action based on mouse button click coordinates. *
//*---------------------------------------------------------------------------------*
//* aX, aY: Screen coordinates of mouse button click.                               *
//*---------------------------------------------------------------------------------*

CScreenInput::Action CScreenInput::GetAction( gdouble aX, gdouble aY )
{
	// Find which screen action area the mouse button click falls within.
	
	gdouble LeftX;
	gdouble TopY;
	gdouble RightX;
	gdouble BottomY;

	// Begin with a default action code of ENone.
	
	CScreenInput::Action Action = CScreenInput::ENone;
	std::list<CActionArea>::iterator ActionArea;

	for ( ActionArea = iActionAreas.begin();
				ActionArea != iActionAreas.end(); 
				ActionArea ++ )
	{
		LeftX   = (gdouble)(*ActionArea).iArea.get_x();
		TopY    = (gdouble)(*ActionArea).iArea.get_y();
		RightX  = LeftX + (*ActionArea).iArea.get_width();
		BottomY = TopY + (*ActionArea).iArea.get_height();

		// If the coordinates fall within an area, break out of the loop 
		// with its action code.  Otherwise, move to the next ActionArea.
		
		if (( aX >= LeftX ) && ( aX <= RightX ) && ( aY >= TopY ) && ( aY <= BottomY ))
		{
			Action = (*ActionArea).iAction;
			break;
		}
	}
		
	return Action; 
}
