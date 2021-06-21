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