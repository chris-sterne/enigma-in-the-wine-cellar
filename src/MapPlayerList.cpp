//*-------------------------------------------------------------*
//* Program: Enigma in the Wine Cellar                          *
//* Version: 5.0 for Linux OS                                   *
//* File:    MapPlayerList.cpp                                  *
//* Date:    October 14, 2016                                   *
//* Author:  Chris Sterne                                       *
//*-------------------------------------------------------------*
//* This class manages a list of map players, which are objects *
//* that can move around and interact with other objects.       *
//*-------------------------------------------------------------*

#include "MapPlayerList.h"

//*----------------------*
//* Default constructor. *
//*----------------------*

CMapPlayerList::CMapPlayerList()
{	
	return;
}

//*------------------------------------------------------------*
//* This method return the player list to the empty condition. *
//*------------------------------------------------------------*

void CMapPlayerList::Clear()
{
	// Clear the list in the base class.
	
	clear();
	return;
}

//*-------------------------------------------------------*
//* This method changes all players to their saved state. *
//*-------------------------------------------------------*

void CMapPlayerList::Load()
{
  std::list<CMapPlayer>::iterator Player;
	
  for ( Player = begin(); Player != end(); ++ Player )
  {
    (*Player).iSurface  = (*Player).iSurfaceSaved;
    (*Player).iRotation = (*Player).iRotationSaved;
    (*Player).iLocation = (*Player).iLocationSaved;
    (*Player).iActive   = (*Player).iActiveSaved;
    (*Player).iOutdoor  = (*Player).iOutdoorSaved;
  }
	
	return;
}

//*---------------------------------------------*
//* This method saves the state of all players. *
//*---------------------------------------------*

void CMapPlayerList::Save()
{
  std::list<CMapPlayer>::iterator Player;
	
  for ( Player = begin(); Player != end(); ++ Player )
  {
    (*Player).iSurfaceSaved  = (*Player).iSurface;
    (*Player).iRotationSaved = (*Player).iRotation;
    (*Player).iLocationSaved = (*Player).iLocation;
    (*Player).iActiveSaved   = (*Player).iActive;
    (*Player).iOutdoorSaved  = (*Player).iOutdoor;
  }
  
	return;
}

//*---------------------------------------------------------*
//* This method changes all players to their restart state. *
//*---------------------------------------------------------*

void CMapPlayerList::Restart()
{
  std::list<CMapPlayer>::iterator Player;
	
  for ( Player = begin(); Player != end(); ++ Player )
  {
    (*Player).iSurface  = (*Player).iSurfaceRestart;
    (*Player).iRotation = (*Player).iRotationRestart;
    (*Player).iLocation = (*Player).iLocationRestart;
    (*Player).iActive   = (*Player).iActiveRestart;
    (*Player).iOutdoor  = (*Player).iOutdoorRestart;
  }
	
	return;
}

//*----------------------------------------------------------*
//* This method returns an iterator to the active player.    *
//*----------------------------------------------------------*
//* RETURN: Iterator to active player.  If no active player  *
//*         was found, this iterator will point past the end *
//*         of the players list.                             *
//*----------------------------------------------------------*

std::list<CMapPlayer>::iterator CMapPlayerList::GetActive()
{
  std::list<CMapPlayer>::iterator Player;
  
  for ( Player = begin(); Player != end(); ++ Player )
  {
    if ( (*Player).iActive )
    break;
  }
	
	// If the game map has players, but no active players were found,
	// set the first first one as the active player.

  if (( Player == end()) && !empty())
  {
    Player = begin();
    (*Player).iActive = TRUE;
  }

  return Player;
}

//*--------------------------------------------------------------*
//* This method switches active players, and returns an iterator *
//* to the new active player.                                    *
//*--------------------------------------------------------------*
//* RETURN: Iterator to new active player.                       *
//*--------------------------------------------------------------*

std::list<CMapPlayer>::iterator CMapPlayerList::SwitchActive()
{
  std::list<CMapPlayer>::iterator Player = GetActive();
  
  if ( Player != end() )
  {
    // Make the old player inactive.
    
    (*Player).iActive = FALSE;
    
    // Move to the next player in the list.
    
    ++ Player;
    
    if ( Player == end() )
      Player = begin();
    
    // Make the new player active.

    (*Player).iActive = TRUE;
  }

  return Player;
}

//*------------------------------------------------------------------*
//* This method TRUE if the player iterator is valid (does not point *
//* past the end of the player list.                                 *
//*------------------------------------------------------------------*
//* RETURN: TRUE if iterator points to a MapPlayer.                  *
//*------------------------------------------------------------------*

gboolean CMapPlayerList::Valid( std::list<CMapPlayer>::iterator& aPlayer )
{
  return ( aPlayer != end() );
}

//*-----------------------------------------------------------------*
//* This method returns iterators to all players in a map location. *
//*-----------------------------------------------------------------*
//* aLocation: Map location to be examined.                         *
//* aBuffer:   Buffer to receive copies of MapPlayer iterators.     *
//*-----------------------------------------------------------------*

void CMapPlayerList::Read( const CMapLocation& aLocation,
                           std::list<std::list<CMapPlayer>::iterator>& aBuffer )
{
  std::list<CMapPlayer>::iterator Player;

  for ( Player = begin(); Player != end(); ++ Player )
  {
    // If the player location is correct, add a copy of the player list
    // iterator to the provided buffer. 
    
    if ( (*Player).iLocation == aLocation )
      aBuffer.push_back( Player );
  }
  
  return;
}