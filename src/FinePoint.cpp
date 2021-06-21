//*---------------------------------------------*
//* Program: Enigma in the Wine Cellar          *
//* Version: 5.0 for Linux OS                   *
//* File:    FinePoint.cpp                      *
//* Date:    October 14, 2016                   *
//* Author:  Chris Sterne                       *
//*---------------------------------------------*
//* This class handles fine object positioning. *
//*---------------------------------------------*

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