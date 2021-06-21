//*------------------------------------*
//* Program: Enigma in the Wine Cellar *
//* Version: 5.0 for Linux OS          *
//* File:    FinePoint.h               *
//* Date:    October 14, 2016          *
//* Author:  Chris Sterne              *
//*                                    *
//* FinePoint class header.            *
//*------------------------------------*
 
#ifndef __FINEPOINT_H__
#define __FINEPOINT_H__

#include <gtkmm.h>

class CFinePoint
{
  public:
    // Public methods.
  
    CFinePoint();
    void Clear();
    void operator+=( const CFinePoint& aPoint );
    void operator-=( const CFinePoint& aPoint );
    
    // Public data.
    
    gfloat iEast;
    gfloat iAbove;
    gfloat iNorth;
    gfloat iRotateEast;
    gfloat iRotateAbove;
    gfloat iRotateNorth;
};

#endif // __FINEPOINT_H__