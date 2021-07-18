// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2016, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the FinePoint class implementation.  The FinePoint class
// handles fine object positioning.
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

#include "FinePoint.h"

//*--------------------------*
//* C++ default constructor. *
//*--------------------------*

CFinePoint::CFinePoint()
{
  // Initialize instance data.

  Clear();
  return;
}

//*--------------------------------------*
//* This method sets all values to zero. *
//*--------------------------------------*

void CFinePoint::Clear()
{
  iEast        = 0;
  iAbove       = 0;
  iNorth       = 0;
  iRotateEast  = 0;
  iRotateAbove = 0;
  iRotateNorth = 0;
  
  return;
}

//*-------------------------------------------------*
//* This method overrides the += addition operator. *
//*-------------------------------------------------*

void CFinePoint::operator+=( const CFinePoint& aPoint )
{
  iEast        += aPoint.iEast;
  iAbove       += aPoint.iAbove;
  iNorth       += aPoint.iNorth;
  iRotateEast  += aPoint.iRotateEast;
  iRotateAbove += aPoint.iRotateAbove;
  iRotateNorth += aPoint.iRotateNorth;

  return;
}

//*-------------------------------------------------*
//* This method overrides the -= addition operator. *
//*-------------------------------------------------*

void CFinePoint::operator-=( const CFinePoint& aPoint )
{
  iEast        -= aPoint.iEast;
  iAbove       -= aPoint.iAbove;
  iNorth       -= aPoint.iNorth;
  iRotateEast  -= aPoint.iRotateEast;
  iRotateAbove -= aPoint.iRotateAbove;
  iRotateNorth -= aPoint.iRotateNorth;

  return;
}
