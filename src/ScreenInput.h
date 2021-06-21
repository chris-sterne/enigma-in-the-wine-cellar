/*------------------------------------*
 * Program: Enigma in the Wine Cellar *
 * Version: 2.0 for Linux OS          *
 * File:    ScreenInput.h             *
 * Date:    Movember 5, 2015          *
 * Author:  Chris Sterne              *
 *                                    *
 * ScreenInput class header.          *
 *------------------------------------*/
 
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