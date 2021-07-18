// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2016, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the MapLocation class implementation.  The MapLocation class
// stores a location in a game map.
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

//*------------------------------------------------------*
//* Program: Enigma in the Wine Cellar                   *
//* Version: 5.0 for Linux OS                            *
//* File:    MapLocation.cpp                             *
//* Date:    October 9, 2016                             *
//* Author:  Chris Sterne                                *
//*------------------------------------------------------*
//* This class stores a location in a game map.          *
//*------------------------------------------------------*

#include <glibmm/i18n.h>
#include "MapLocation.h"

//*----------------------*
//* Default constructor. *
//*----------------------*

CMapLocation::CMapLocation()
{
  return;
}

//*---------------------------------------------------------*
//* This constructor allows initializing the instance data. *
//*---------------------------------------------------------*

CMapLocation::CMapLocation( guint16 aEast, guint16 aNorth, guint16 aAbove )
{
  iEast  = aEast;
  iNorth = aNorth;
  iAbove = aAbove;
  return;
}

//*-----------------------------------------------*
//* This method sets all location values to zero. *
//*-----------------------------------------------*

void CMapLocation::Clear()
{
  iEast  = 0;
  iNorth = 0;
  iAbove = 0;
  return;
}

//*---------------------------------------------------*
//* This method overrides the == comparison operator. *
//*---------------------------------------------------*

gboolean CMapLocation::operator==( const CMapLocation& aLocation ) const
{
  return (( iEast == aLocation.iEast )
		&& ( iNorth == aLocation.iNorth )
		&& ( iAbove == aLocation.iAbove ));
}

//*---------------------------------------------------*
//* This method overrides the != comparison operator. *
//*---------------------------------------------------*

gboolean CMapLocation::operator!=( const CMapLocation& aLocation ) const
{
  return (!( *this == aLocation ));
}
