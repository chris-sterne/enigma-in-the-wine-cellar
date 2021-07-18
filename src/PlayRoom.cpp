// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2016, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the PlayRoom class implementation.  The PlayRoom class
// allows a player to move through map rooms and interact with objects within
// the room. 
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

#include <glibmm/i18n.h>
#include "EnigmaWC.h"
#include "PlayRoom.h"

//*---------------------*
//* Local declarations. *
//*---------------------*

#define KVisualDelay 333    // Transition delay (milliseconds).

static const char* KNoneText   = _("???");
static const char* KNorthText  = _("North");
static const char* KSouthText  = _("South");
static const char* KEastText   = _("East");
static const char* KWestText   = _("West");
static const char* KAboveText  = _("Above");
static const char* KBelowText  = _("Below");
static const char* KCenterText = _("Center");

static const char* KDirectionText[ (int)EnigmaWC::Direction::TOTAL ]
{
  KNoneText,    // ENone
  KNorthText,   // ENorth
  KSouthText,   // ESouth
  KEastText,    // EEast
  KWestText,    // EWest
  KAboveText,   // EAbove
  KBelowText,   // EBelow
  KCenterText   // ECenter
};

// Define Edger resurfacing stages.

#define KEdgerIdle   0
#define KEdgerSecond 1
#define KEdgerThird  2
#define KEdgerFinal  3

// Definitions to simplify the layout of tables.

#define DNone   (guint8)EnigmaWC::Direction::ENone
#define DNorth  (guint8)EnigmaWC::Direction::ENorth
#define DSouth  (guint8)EnigmaWC::Direction::ESouth
#define DEast   (guint8)EnigmaWC::Direction::EEast
#define DWest   (guint8)EnigmaWC::Direction::EWest
#define DAbove  (guint8)EnigmaWC::Direction::EAbove
#define DBelow  (guint8)EnigmaWC::Direction::EBelow
#define DCenter (guint8)EnigmaWC::Direction::ECenter

// This table is used to determine if a rotation is valid while on a surface.
// The first index selects the surface, and the second index is the rotation
// on the surface.  A value of 1 from the table means the rotation is valid.

static const guint8 KValidTable[ (int)EnigmaWC::Direction::TOTAL ]
                               [ (int)EnigmaWC::Direction::TOTAL ] =
//-----------------------------------------------------------------
// ENone ENorth ESouth EEast EWest EAbove EBelow ECenter
//-----------------------------------------------------------------
{
  { 0,     0,     0,     0,     0,     0,     0,     0 },  // DNone
  { 0,     0,     0,     1,     1,     1,     1,     0 },  // ENorth
  { 0,     0,     0,     1,     1,     1,     1,     0 },  // ESouth
  { 0,     1,     1,     0,     0,     1,     1,     0 },  // EEast
  { 0,     1,     1,     0,     0,     1,     1,     0 },  // EWest
  { 0,     1,     1,     1,     1,     0,     0,     0 },  // EAbove
  { 0,     1,     1,     1,     1,     0,     0,     0 },  // EBelow
  { 0,     0,     0,     0,     0,     0,     0,     0 }   // ECenter
};

// This table provides next rotation (directions) when turning on a surface.
// 
// Turn Left: Set the first index to the current surface, and the second
// index to the current rotation on the surface.  Read the new rotation
// from the table.
//
// Turn Right: Set the first index to the current rotation on the surface,
// and the second index to the current surface.  Read the new rotation
// from the table.

// NOTE: Unsupported rotations on surfaces are assigned ENone, which will
// likely inhibit any further motion if it appears.  It may be better to
// assign a valid but incorrect direction instead.
 
static const guint8 KRotateTable[ (int)EnigmaWC::Direction::TOTAL ]
                                [ (int)EnigmaWC::Direction::TOTAL ] =
//-----------------------------------------------------------------
//  ENone  ENorth  ESouth  EEast   EWest   EAbove  EBelow  ECenter
//-----------------------------------------------------------------
{
  { DNone, DNone,  DNone,  DNone,  DNone,  DNone,  DNone,  DNone   },  // DNone
  { DNone, DNone,  DNone,  DAbove, DBelow, DWest,  DEast,  DCenter },  // ENorth
  { DNone, DNone,  DNone,  DBelow, DAbove, DEast,  DWest,  DCenter },  // ESouth
  { DNone, DBelow, DAbove, DNone,  DNone,  DNorth, DSouth, DCenter },  // EEast
  { DNone, DAbove, DBelow, DNone,  DNone,  DSouth, DNorth, DCenter },  // EWest
  { DNone, DEast,  DWest,  DSouth, DNorth, DNone,  DNone,  DCenter },  // EAbove
  { DNone, DWest,  DEast,  DNorth, DSouth, DNone,  DNone,  DCenter },  // EBelow
  { DNone, DNorth, DSouth, DEast,  DWest,  DAbove, DBelow, DCenter }   // ECenter
};

// This table provides the opposite of a particular direction.

static const guint8 KOppositeTable[ (int)EnigmaWC::Direction::TOTAL ] =
//-----------------------------------------------------------------
// ENone  ENorth  ESouth  EEast  EWest  EAbove  EBelow  ECenter
//-----------------------------------------------------------------
{
  DNone,  DSouth, DNorth, DWest, DEast, DBelow, DAbove, DCenter
};

// These tables return rotations for use when resurfacing around
// a particular directional axis (East, Above, or North).

static const guint8 KResurfaceEastTable[ (int)EnigmaWC::Direction::TOTAL ] =
//-----------------------------------------------------------------
// ENone ENorth  ESouth  EEast  EWest  EAbove  EBelow  ECenter
//-----------------------------------------------------------------
{
  DNone, DBelow, DAbove, DEast, DWest, DNorth, DSouth, DCenter 
};

static const guint8 KResurfaceAboveTable[ (int)EnigmaWC::Direction::TOTAL ] =
//-----------------------------------------------------------------
// ENone  ENorth ESouth  EEast  EWest   EAbove  EBelow  ECenter
//-----------------------------------------------------------------
{
   DNone, DWest, DEast, DNorth, DSouth, DAbove, DBelow, DCenter 
};

static const guint8 KResurfaceNorthTable[ (int)EnigmaWC::Direction::TOTAL ] =
//-----------------------------------------------------------------
// ENone ENorth  ESouth  EEast   EWest   EAbove EBelow ECenter
//-----------------------------------------------------------------
{
  DNone, DNorth, DSouth, DAbove, DBelow, DWest, DEast, DCenter 
};

// Bit masks indicating directions in "map space".

#define MNone          0
#define MNorth  ( 1 << 0 )
#define MSouth  ( 1 << 1 )
#define MEast   ( 1 << 2 )
#define MWest   ( 1 << 3 )
#define MAbove  ( 1 << 4 )
#define MBelow  ( 1 << 5 )
#define MCenter ( 1 << 6 )

// The following array converts a "map space" direction
// into a bit mask.

static const guint8 KDirectionBits[ (int)EnigmaWC::Direction::TOTAL ] =
{
  MNone, MNorth, MSouth, MEast, MWest, MAbove, MBelow, MCenter
};

// The following array converts an opposite "map space" direction
// into a bit mask.

static const guint8 KOppositeBits[ (int)EnigmaWC::Direction::TOTAL ] =
{
  MNone, MSouth, MNorth, MWest, MEast, MBelow, MAbove, MCenter
};

//*----------------------*
//* Default constructor. *
//*----------------------*

CPlayRoom::CPlayRoom()
{
  // Construct slots for signal handlers.
	
  m_slot_delay =
    sigc::mem_fun( *this, &CPlayRoom::On_Continue );	
	
  m_slot_idle =
    sigc::mem_fun( *this, &CPlayRoom::On_Continue );
      
  // Clear instance data.

  iFreeMoving = FALSE;
  iEdging     = KEdgerIdle;
  return;
}

//*----------------------------------------------------------------------*
//* This method controls the display of multiple frame view transitions. *
//*----------------------------------------------------------------------*
//* aShow: TRUE to show transitions.                                     *
//*----------------------------------------------------------------------*

void CPlayRoom::SetViewTransitions( gboolean aShow )
{
	iViewTransitions = aShow;
	return;
}

//*----------------------------------------------*
//* This method enables or disables game sounds. *
//*----------------------------------------------*
//* aGameSounds: TRUE to play game sounds.       *
//*----------------------------------------------*

void CPlayRoom::SetSounds( gboolean aPlay )
{
  if ( aPlay )
  {
    // Game sounds are to be played.  Create a new Sounds object
    // unless one is already present.

    if ( !iSounds )
      iSounds = std::unique_ptr<CSounds>( new CSounds );
  }
  else
  {
    // Game sounds are not to be played.  Delete the Sounds object
    // if present.

    if ( iSounds )
      iSounds.reset( nullptr );
  }

  return;
}

//*-----------------------------------------------------*
//* This method controls the amount of information sent *
//* in the Orientation signal.                          *
//*-----------------------------------------------------*
//* aFullLocation: TRUE to send all information.        *
//*-----------------------------------------------------*

void CPlayRoom::SetFullOrientation( gboolean aFullOrientation )
{
  iFullOrientation = aFullOrientation;

  // Send the player's map orientation using the new format.
  
  Do_Orientation();
  return;
}

//*---------------------------------------------*
//* This method sets the game map to be played. *
//*---------------------------------------------*
//* aMap: Shared pointer to game map.           *
//*---------------------------------------------*

void CPlayRoom::SetMap( std::shared_ptr<CMap> aMap )
{
  // Save pointer to the game map.
	
  iMap = aMap;

  // Return immediately if the game map has not been loaded.  (There will be
  // no map players).  Do_Orientation() will send a message mentioning this.
  
  if ( !iMap->GetLoaded() )
  {
    Do_Orientation();
    return;
  }

  // Prepare all players in the game map by regenerating state information
  // based on their current location.
  
  std::list<CMapPlayer>::iterator Player;
  
  for ( Player = iMap->Players().begin();
        Player != iMap->Players().end();
        ++ Player )
  {
    // Prepare the player's next orientation variables.
  
    (*Player).iSurfaceNext  = (*Player).iSurface;
    (*Player).iRotationNext = (*Player).iRotation;
    (*Player).iLocationNext = (*Player).iLocation;
    (*Player).iOutdoorNext  = (*Player).iOutdoor;
  
    // Prepare InnerTube and HydrogenBalloon usage.  These variables are
    // used to avoid having to search through the item list each time
    // to find if these items are being used.
    
    (*Player).iInnerTube =
      iMap->Items().GetSelected( EnigmaWC::ID::EInnerTube,
                                 (*Player).iNumber );
       
    (*Player).iHydrogenBalloon =
      iMap->Items().GetSelected( EnigmaWC::ID::EHydrogenBalloon,
                                 (*Player).iNumber );
  
    // Obtain information about objects in the players current location.
        
    (*Player).iStairsSurface      = EnigmaWC::Direction::ENone;
    (*Player).iStairsRotation     = EnigmaWC::Direction::ENone;
    (*Player).iStairsSurfaceNext  = EnigmaWC::Direction::ENone;
    (*Player).iStairsRotationNext = EnigmaWC::Direction::ENone;
    (*Player).iWaterLayer         = FALSE;
    (*Player).iWater              = FALSE;
    
    iObjects.clear();
    iMap->Objects().Read( (*Player).iLocation, iObjects );
    std::list<std::list<CMapObject>::iterator>::iterator Object;
  
    for ( Object = iObjects.begin();
          Object != iObjects.end();
          ++ Object )
    {
      if ( (*(*Object)).iPresence.GetState() )
      {
        switch( (*(*Object)).iID )
        {
          case EnigmaWC::ID::EWaterLayer:
            if ( (*(*Object)).iSurface == (*Player).iSurface )
              (*Player).iWaterLayer = TRUE;
                  
            break;
            
          case EnigmaWC::ID::EWater:
            (*Player).iWater = TRUE;
            break;
            
          case EnigmaWC::ID::EStairs:
            (*Player).iStairsSurface  = (*(*Object)).iSurface;
            (*Player).iStairsRotation = (*(*Object)).iRotation;
            break;
            
          case EnigmaWC::ID::EOutdoor:
            if ( (*(*Object)).iSurface == EnigmaWC::Direction::ECenter )
              (*Player).iOutdoor = TRUE;
            
            break;
            
          default:
            break;
        }
      }
    }
 
    // Prepare contact/anti-contact bits.

    (*Player).iContactBits = KDirectionBits[ (int)(*Player).iSurface ]
                            | MCenter;

    (*Player).iAntiContactBits = MNone;
    
    // Use the transition to find the player offset in the location.
    // This information is needed to properly render the active player
    // and other players being viewed by the active player.
    
    iTransition.Prepare( Player ); 
    (*Player).iOffset = iTransition.GetPoint();
  }

  // Get the active player, and prepare the room for this player.

  iPlayer = iMap->Players().GetActive();
  Prepare();
  return;
}

//*--------------------------------------------------------------------*
//* This method makes the next available player active and the current *
//* player idle.                                                       *
//*--------------------------------------------------------------------*

void CPlayRoom::SwitchPlayer()
{
  // Return immediately if the current active player is still busy.

  if ( !iActionQueue.empty() )
    return;

  // Get the next active player, and prepare the room for this player.

  iPlayer = iMap->Players().SwitchActive();
  Prepare();
  return;
}

//*------------------------------------------------------*
//* This method prepares the room for the active player. *
//*------------------------------------------------------*

void CPlayRoom::Prepare()
{
  // Clear room variables and storage.

  iActionQueue.clear();
  iFreeMoving  = FALSE;
  iEdging      = KEdgerIdle;

  // Initialize the list of objects in the player's room.
      
  iObjects.clear();
  iMap->Objects().Read( (*iPlayer).iLocation, iObjects );

  iTeleporters.clear();
  iMap->Teleporters().Read( (*iPlayer).iLocation, iTeleporters );

  iItems.clear();
  iMap->Items().Read( (*iPlayer).iLocation, iItems );
  iFoundItems.clear();

  iPlayers.clear();
  iMap->Players().Read( (*iPlayer).iLocation, iPlayers );

  // Start exploring the room.

  Explore( CPlayRoom::Action::ENone );
  return;
}

//*-------------------------------------------------------------*
//* This method makes the active player select an item for use. *
//*-------------------------------------------------------------*
//* aItem: Iterator to item to be selected for use.             *
//*-------------------------------------------------------------*

void CPlayRoom::SelectItem( std::list<CMapItem>::iterator aItem )
{
  // Return immediately if the current active player is still busy.

  if ( !iActionQueue.empty() )
    return;

  // Toggle the Selection state for the item and all similar ones owned
  // by the player.

  gboolean SelectedNext = !(*aItem).iSelected;
  
  // Clear the Selected state of all items owned by the player.  Only one
  // item is allowed to be selected at a time.
  
  iMap->Items().ClearSelected( (*aItem).iOwner );

  if ((((*aItem).iID == EnigmaWC::ID::EInnerTube )
    || ((*aItem).iID == EnigmaWC::ID::EHydrogenBalloon ))
    && (*iPlayer).iWater )
  {
    // An InnerTube or HydrogenBalloon cannot be selected if under Water
    // (cannot inflate these underwater).
    
    SelectedNext = FALSE;
  }
  
  // If necessary, set the Selected state of the item.
  
  if ( SelectedNext )
    iMap->Items().SetSelected( (*aItem).iID, (*aItem).iOwner, TRUE );   

  // Update the player's InnerTube and HydrogenBalloon usage.
 
  gboolean OldInnerTube       = (*iPlayer).iInnerTube;
  gboolean OldHydrogenBalloon = (*iPlayer).iHydrogenBalloon;
 
  (*iPlayer).iInnerTube =
    iMap->Items().GetSelected( EnigmaWC::ID::EInnerTube,
                               (*iPlayer).iNumber );

  (*iPlayer).iHydrogenBalloon =
    iMap->Items().GetSelected( EnigmaWC::ID::EHydrogenBalloon,
                               (*iPlayer).iNumber );

  // A change to InnerTube or HydrogenBalloon usage may affect
  // the player's stability, so the room must then be explored.
  
  if  (( OldInnerTube != (*iPlayer).iInnerTube )
    || ( OldHydrogenBalloon != (*iPlayer).iHydrogenBalloon ))
  {
    Explore( CPlayRoom::Action::ENone );
  }
  
  return;
}

//*---------------------------------------------------------------*
//* This private function returns the rotation on another surface *
//* when the player is resurfacing from a surface.                *
//*---------------------------------------------------------------*
//* aSurface1:  First surface.                                    *
//* aRotation1: Rotation on first surface.                        *
//* aSurface2:  Second surface.                                   *
//*---------------------------------------------------------------*

EnigmaWC::Direction Resurface( EnigmaWC::Direction aSurface1,
                               EnigmaWC::Direction aRotation1,
                               EnigmaWC::Direction aSurface2 )
{
  const guint8* Group;
  
  if  (( aSurface1 == aSurface2 )
    || ((guint8)aSurface1 == KOppositeTable[ (int)aSurface2 ] ))
  {
    // Keep the rotation unchanged when resurfacing onto the same or opposite
    // surfaces.  The rotation group to which these surfaces belong is
    // ambiguous.
  
    return aRotation1;
  }
  else if (( KResurfaceEastTable[ (int)aSurface1 ] != (guint8)aSurface1 )
        && ( KResurfaceEastTable[ (int)aSurface2 ] != (guint8)aSurface2 ))
  {
    // Select the East group of rotations.
    
    Group = KResurfaceEastTable;
  }
  else if (( KResurfaceAboveTable[ (int)aSurface1 ] != (guint8)aSurface1 )
        && ( KResurfaceAboveTable[ (int)aSurface2 ] != (guint8)aSurface2 ))
  {
    // Select the Above group of rotations.
    
    Group = KResurfaceAboveTable;
  }
  else if (( KResurfaceNorthTable[ (int)aSurface1 ] != (guint8)aSurface1 )
        && ( KResurfaceNorthTable[ (int)aSurface2 ] != (guint8)aSurface2 ))
  {
    // Select the North group of rotations.
    
    Group = KResurfaceNorthTable;
  }
  else
  {
    // Leave the rotation unchanged for surfaces (ECenter, ENone) that
    // do not belong to any rotation group.
    
    return aRotation1; 
  }

  // Search the rotation group array for the second surface and its
  // corresponding rotation.
  // NOTE: If the group array is faulty and the entries do not produce
  // a circular search, an endless loop will occur.  A limit counter
  // could be added. 

  while ( aSurface2 != aSurface1 )
  {  
    aSurface1  = (EnigmaWC::Direction)Group[ (int)aSurface1 ];
    aRotation1 = (EnigmaWC::Direction)Group[ (int)aRotation1 ];
  }

  return aRotation1;
}

//*------------------------------------------------------------------------*
//* This method is the entry point for exploring a room.  Actions may be   *
//* handled right away, or added to a queue if another action is currently *
//* being handled.                                                         *
//*------------------------------------------------------------------------*
//* aAction: Initial action to perform during exploration.                 *
//*------------------------------------------------------------------------*

void CPlayRoom::Explore( CPlayRoom::Action aAction )
{  
  // Return immediately if a game map is not loaded (iPlayer will be invalid).

  if ( !iMap->GetLoaded() )
    return;
  
  if ( aAction == CPlayRoom::Action::EContinue )
  {
    // Delete the current action from the queue (has already been handled),
    // and read a new action from the queue.  Return if the queue is empty.
    
    if ( !iActionQueue.empty() )
      iActionQueue.pop_front();
      
    if ( iActionQueue.empty() )
      return;
    else
      aAction = iActionQueue.front();
  }
  else
  {
    if ( iActionQueue.empty() )
    {
      iActionQueue.push_back( aAction );    
    }
    else
    {
      iActionQueue.push_back( aAction );
      return;
    }
  }

  // Make a local copy of the player's orientation variables.  This is done
  // to simplify the orientation variable names.
  
  EnigmaWC::Direction Surface     = (*iPlayer).iSurface;
  EnigmaWC::Direction SurfaceNext = (*iPlayer).iSurfaceNext;
  
  EnigmaWC::Direction Rotation     = (*iPlayer).iRotation;
  EnigmaWC::Direction RotationNext = (*iPlayer).iRotationNext;
  
  CMapLocation Location     = (*iPlayer).iLocation;
  CMapLocation LocationNext = (*iPlayer).iLocationNext;

  // Handle all items found during the last room exploration.
  
  if ( !iFoundItems.empty() )
  {
    std::list<std::list<CMapItem>::iterator>::iterator Item =
      iFoundItems.begin();
 
    while ( Item != iFoundItems.end() )
    {
      // Mark the item has having been found.
    
      iMap->Items().SetFound( *Item, (*iPlayer).iNumber );
      Item = iFoundItems.erase( Item );
    }
  }

  gboolean SenseChange = FALSE;
  CSounds::ID SoundID  = CSounds::ID::ENone;
  std::list<std::list<CMapObject>::iterator>::iterator Object;

  if ( LocationNext != Location )
  {
    // The player has moved to a new room.  Apply Sense to objects in the old 
    // room.  This was not done when starting to move so as to avoid changing
    // the state of the room objects and causing an object to appear in the
    // view transition path (e.g. causing a WoodDoor to close so the player
    // passes through the closed door).  
  
    UpdateSense( SenseChange, SoundID );

    if ( iSounds )
      iSounds->Play( SoundID );
    
    // Remove the active player's contect with the old room, then update Sense
    // input from objects in the old room.  The old room may still be in 
    // contact with other players in it. 
    
    (*iPlayer).iContactBits     = MNone;
    (*iPlayer).iAntiContactBits = MNone;

    UpdateSense( SenseChange, SoundID );

    if ( iSounds )
      iSounds->Play( SoundID );
    
    // Update the player's location variables (local and player copies).

    (*iPlayer).iLocation = LocationNext;
    Location             = LocationNext;
 
    // Obtain a list of objects in the new room.

    iObjects.clear();
    iMap->Objects().Read( Location, iObjects );

    iTeleporters.clear();
    iMap->Teleporters().Read( Location, iTeleporters );

    iItems.clear();
    iMap->Items().Read( Location, iItems );
    
    iPlayers.clear();
    iMap->Players().Read( Location, iPlayers );

    // The first activity to do when entering a new room is to apply Sense
    // input to all present central Sensors.  This may change the state of
    // other objects in the room.
    
    (*iPlayer).iContactBits     = MCenter;
    (*iPlayer).iAntiContactBits = MNone;
    
    UpdateSense( SenseChange, SoundID );
    
    // Use Turners and Facers objects only if they are on the player's surface
    // or in the center of the room.  These invisible objects cause a immediate
    // change to the player's rotation, rather than being shown with a view 
    // transition.

    EnigmaWC::Direction ObjectSurface;
    EnigmaWC::Direction ObjectRotation;

    for ( Object = iObjects.begin();
          Object != iObjects.end();
          ++ Object )
    {
      ObjectSurface  = (*(*Object)).iSurface;
      ObjectRotation = (*(*Object)).iRotation;    

      switch ( (*(*Object)).iID )
      {
        case EnigmaWC::ID::ETurner:
          // Handle a Turner object on the player's surface or in the center
          // of the room.

          if  ( (*(*Object)).iPresence.GetState()
            && (( ObjectSurface == Surface )
            ||  ( ObjectSurface == EnigmaWC::Direction::ECenter )))
          {
            // Apply relative rotations to the left.  The Turner's
            // rotation is re-interpreted as a number of relative left
            // turns instead of a direction in map space.
            
            for ( int Counter = 0;
                  Counter < (int)ObjectRotation;
                  ++ Counter )
            {
              RotationNext = 
                (EnigmaWC::Direction)KRotateTable[ (int)Surface ]
                                                 [ (int)RotationNext ];
            }
            
            // Immediately update the player's rotation (local and player
            // copies).
            
            Rotation                 = RotationNext;
            (*iPlayer).iRotation     = Rotation;
            (*iPlayer).iRotationNext = RotationNext;
            
            // Apply a Sense to the Turner object.
            
            (*(*Object)).iSense.SetState( TRUE );
          }
          
          break;

        case EnigmaWC::ID::EFacer:
          // Handle a Facer object on the player's surface or in the center
          // of the room.

          if  ( (*(*Object)).iPresence.GetState()
            && (( ObjectSurface == Surface )
            ||  ( ObjectSurface == EnigmaWC::Direction::ECenter ))
            && ( KValidTable[ (int)Surface ][ (int)ObjectRotation ] == 1 ))
          {
            // Change the player's rotation to that of the Facer.  This rotation
            // has been confirmed to be valid on the player's surface. 
              
            RotationNext = ObjectRotation;
            
            // Immediately update the player's rotation (local and player
            // copies).
            
            Rotation                 = RotationNext;
            (*iPlayer).iRotation     = Rotation;
            (*iPlayer).iRotationNext = RotationNext;
            
            // Apply Sense to the Facer object.
          
            (*(*Object)).iSense.SetState( TRUE );
          }
          
          break;

        default:
          break;
      }
    }
  }
  
  // Complete the update of the player's orientation variables (local and
  // player copies).
      
  (*iPlayer).iSurface  = SurfaceNext;
  Surface              = SurfaceNext;       
  (*iPlayer).iRotation = RotationNext;
  Rotation             = RotationNext;
  (*iPlayer).iOutdoor  = (*iPlayer).iOutdoorNext;

  // Convert the current action (view space) into motion (map space).
  
  EnigmaWC::Direction Motion = EnigmaWC::Direction::ENone;
  
  switch( aAction )
  {
    case CPlayRoom::Action::ETurnLeft:
      RotationNext =
        (EnigmaWC::Direction)KRotateTable[ (int)Surface ][ (int)Rotation ];
      break;

    case CPlayRoom::Action::ETurnRight:
      RotationNext =
        (EnigmaWC::Direction)KRotateTable[ (int)Rotation ][ (int)Surface ];
      break;

    case CPlayRoom::Action::EMoveFront:
      Motion = (*iPlayer).iRotation;
      break;
  
    case CPlayRoom::Action::EMoveBack:
      Motion =
        (EnigmaWC::Direction)KOppositeTable[ (int)Rotation ];
      break;

    case CPlayRoom::Action::EMoveLeft:
      Motion =
        (EnigmaWC::Direction)KRotateTable[ (int)Surface ][ (int)Rotation ];
      break;

    case CPlayRoom::Action::EMoveRight:
      Motion =
        (EnigmaWC::Direction)KRotateTable[ (int)Rotation ][ (int)Surface ];
      break;

    default:
      break;
  }

  // Generate bit masks describing the player's positions (view space).  If the
  // player moves, the motion direction is designated as the front.  Otherwise,
  // the player's front is the player's rotation while at rest.

  EnigmaWC::Direction Direction;

  if ( Motion != EnigmaWC::Direction::ENone )
    Direction = Motion;
  else
    Direction = Rotation;

  guint8 FrontBit = KDirectionBits[ (int)Direction ];
  guint8 BackBit  = KOppositeBits[ (int)Direction ];
  guint8 DownBit  = KDirectionBits[ (int)Surface ];
  guint8 UpBit    = KOppositeBits[ (int)Surface ];
  
  Direction =
    (EnigmaWC::Direction)KRotateTable[ (int)Surface ][ (int)Direction ];
  
  guint8 LeftBit  = KDirectionBits[ (int)Direction ];
  guint8 RightBit = KOppositeBits[ (int)Direction ];
  
  // Create a description of the room from the objects.  These bit variables
  // simplfy questions about object presence and orientation.

  guint8 WallBits          = MNone;
  guint8 MoverBits         = MNone;
  guint8 FlipperBits       = MNone;
  guint8 StairsBits        = MNone;
  guint8 StairsTopBits     = MNone;
  guint8 LadderBits        = MNone;
  guint8 FrontLadderBits   = MNone;
  guint8 BackLadderBits    = MNone;
  guint8 UpLadderBits      = MNone;
  guint8 DownLadderBits    = MNone;
  guint8 LeftLadderBits    = MNone;
  guint8 RightLadderBits   = MNone;
  guint8 HandHoldBits      = MNone;
  guint8 FrontHandHoldBits = MNone;
  guint8 BackHandHoldBits  = MNone;
  guint8 UpHandHoldBits    = MNone;
  guint8 LeftHandHoldBits  = MNone;
  guint8 RightHandHoldBits = MNone;
  guint8 CatWalkBits       = MNone;
  guint8 OutdoorBits       = MNone;
  guint8 IndoorBits        = MNone;
  
  EnigmaWC::Direction Surfacer = EnigmaWC::Direction::ENone;
  EnigmaWC::Direction Edger    = EnigmaWC::Direction::ENone;
  
  gboolean Water      = FALSE;
  gboolean WaterLayer = FALSE;

  // Clear room information recorded by the player.

  (*iPlayer).iWater              = FALSE;
  (*iPlayer).iStairsSurface      = EnigmaWC::Direction::ENone;
  (*iPlayer).iStairsRotation     = EnigmaWC::Direction::ENone;
  (*iPlayer).iStairsSurfaceNext  = EnigmaWC::Direction::ENone;
  (*iPlayer).iStairsRotationNext = EnigmaWC::Direction::ENone;
  
  guint8 ObjectSurfaceBit;

  EnigmaWC::Direction ObjectSurface;
  EnigmaWC::Direction ObjectRotation;
    
  for ( Object = iObjects.begin();
        Object != iObjects.end();
        ++ Object )
  {
    if ( (*(*Object)).iPresence.GetState() )
    {
      // The object is present, so it can affect the player.

      ObjectSurface  = (*(*Object)).iSurface;
      ObjectRotation = (*(*Object)).iRotation;

      switch ( (*(*Object)).iID )
      {
        case EnigmaWC::ID::EBlockWall:
        case EnigmaWC::ID::EStoneWall:
        case EnigmaWC::ID::EWoodWall:
        case EnigmaWC::ID::EEarthWall:
        case EnigmaWC::ID::EBlocker:
          // Note the presence of a wall object.
          
          WallBits |= KDirectionBits[ (int)ObjectSurface ];
          break;

        case EnigmaWC::ID::EFence:
          // A Fence acts like a wall, but is perpendicular to its surface.
          // Using its rotation instead of its surface will convert it into
          // a wall.
          
          WallBits |= KDirectionBits[ (int)ObjectRotation ];
          break;
        
        case EnigmaWC::ID::EWoodDoor:
          // An active WoodDoor object acts like a wall.
          
          if ( (*(*Object)).iState.GetState() )
            WallBits |= KDirectionBits[ (int)ObjectSurface ];

          break;

        case EnigmaWC::ID::ELadder:
        case EnigmaWC::ID::ELadderTop:
          // Note the presence of a Ladder or LadderTop object.  A Ladder has
          // two ends, while a LadderTop has only one.  Record the direction
          // of these ends in the player's view space.
 
          ObjectSurfaceBit = KDirectionBits[ (int)ObjectSurface ];
          LadderBits       = KDirectionBits[ (int)ObjectRotation ]; 
          
          if ( (*(*Object)).iID == EnigmaWC::ID::ELadder )
            LadderBits |= KOppositeBits[ (int)ObjectRotation ];

          if ( ObjectSurfaceBit == FrontBit )
            FrontLadderBits |= LadderBits;
          else if ( ObjectSurfaceBit == BackBit )
            BackLadderBits |= LadderBits;
          else if ( ObjectSurfaceBit == RightBit )
            RightLadderBits |= LadderBits;
          else if ( ObjectSurfaceBit == LeftBit )
            LeftLadderBits |= LadderBits;          
          else if ( ObjectSurfaceBit == UpBit )
            UpLadderBits |= LadderBits;
          else if ( ObjectSurfaceBit == DownBit )
            DownLadderBits |= LadderBits;

          break;
          
        case EnigmaWC::ID::ETree:
        case EnigmaWC::ID::ETreeTop:
          // Note the presence of a Tree or TreeTop object with a TRUE state.
          // These behave like ladders, but are perpendicular to its surface.
          // Exchanging a Tree surface and rotation allows the Ladder/LadderTop
          // bits to be used.
 
          if ( (*(*Object)).iState.GetState() )
          {
            ObjectSurfaceBit = KDirectionBits[ (int)ObjectRotation ];
            LadderBits       = KDirectionBits[ (int)ObjectSurface ]; 
          
            if ( (*(*Object)).iID == EnigmaWC::ID::ETree )
              LadderBits |= KOppositeBits[ (int)ObjectSurface ];

            if ( ObjectSurfaceBit == FrontBit )
              FrontLadderBits |= LadderBits;
            else if ( ObjectSurfaceBit == BackBit )
              BackLadderBits |= LadderBits;
            else if ( ObjectSurfaceBit == RightBit )
              RightLadderBits |= LadderBits;
            else if ( ObjectSurfaceBit == LeftBit )
              LeftLadderBits |= LadderBits;          
            else if ( ObjectSurfaceBit == UpBit )
              UpLadderBits |= LadderBits;
            else if ( ObjectSurfaceBit == DownBit )
              DownLadderBits |= LadderBits;
          }

          break;
          
        case EnigmaWC::ID::EHandHold:
          // Note the presence of a HandHold object.
          
          ObjectSurfaceBit = KDirectionBits[ (int)ObjectSurface ];
          HandHoldBits     = KDirectionBits[ (int)ObjectRotation ];
   
          if ( ObjectSurfaceBit == FrontBit )
            FrontHandHoldBits |= HandHoldBits;
          else if ( ObjectSurfaceBit == BackBit )
            BackHandHoldBits |= HandHoldBits;
          else if ( ObjectSurfaceBit == RightBit )
            RightHandHoldBits |= HandHoldBits;
          else if ( ObjectSurfaceBit == LeftBit )
            LeftHandHoldBits |= HandHoldBits;          
          else if ( ObjectSurfaceBit == UpBit )
            UpHandHoldBits |= HandHoldBits;

          break;
        
       case EnigmaWC::ID::EVine:
          // a Vine object above acts like four HandHolds above.
          
          ObjectSurfaceBit = KDirectionBits[ (int)ObjectSurface ];
          HandHoldBits     = KDirectionBits[ (int)ObjectRotation ];
          
          if ( ObjectSurfaceBit == UpBit )
            UpHandHoldBits = ( FrontBit | BackBit | LeftBit | RightBit );

          break;
        
        case EnigmaWC::ID::EStairs:
          // Note the presence of a Stairs object on the player's surface.
          // Stairs in this orientation can be climbed.
                   
          if ( ObjectSurface == Surface )
            StairsBits = KDirectionBits[ (int)ObjectRotation ];
          
          // Stairs also act like a two-sided wall.
          
          WallBits |= KDirectionBits[ (int)ObjectSurface ];
          WallBits |= KDirectionBits[ (int)ObjectRotation ];
          
          // Save the surface of these Stairs in the active player.
          // This information is used for planning transition paths.
          
          (*iPlayer).iStairsSurface  = ObjectSurface;
          (*iPlayer).iStairsRotation = ObjectRotation;

          break;
          
        case EnigmaWC::ID::EStairsTop:
          // Note the presence of a StairsTop object on the player's surface.
          // A StairsTop in this orientation can be climbed.  To simplify
          // the conditional for handling motion with this object, its rotation
          // is reversed.
          
          if ( ObjectSurface == Surface )
            StairsTopBits = KOppositeBits[ (int)ObjectRotation ];

          break;
          
        case EnigmaWC::ID::ECatWalk:
          // Note the presence of a CatWalk on the player's surface.
          
          if ( ObjectSurface == Surface )
          {
            CatWalkBits |= KDirectionBits[ (int)ObjectRotation ];
            CatWalkBits |= KOppositeBits[ (int)ObjectRotation ];
          }
          
          break;
          
        case EnigmaWC::ID::EMover:
          // Accumulate motion due to a Mover.
        
          MoverBits |= KDirectionBits[ (int)ObjectRotation ];
          break;

        case EnigmaWC::ID::EFlipper:
          // Note the position of a Flipper.  This object is only active when
          // on the player's surface.

          FlipperBits |= KDirectionBits[ (int)ObjectSurface ];
          break;
          
        case EnigmaWC::ID::ESurfacer:
          // Note the position of a Surfacer on a front wall near the player's
          // floor.  This will be active if the player is moving.  Surfacers in
          // other positions are not active.

          if  (( KDirectionBits[ (int)ObjectRotation ] == DownBit )
            && ( KDirectionBits[ (int)ObjectSurface ] == FrontBit ))
          {
            Surfacer = ObjectSurface;
          }

          break;

        case EnigmaWC::ID::EEdger:
          // Note the presence of an Edger on the front surface near the floor,
          // or on the floor in the direction of motion.  This will be active
          // if the player is moving.  Edgers in other positions are not active.          
          
          if (( KDirectionBits[ (int)ObjectRotation ] == DownBit )
            && ( KDirectionBits[ (int)ObjectSurface ] == FrontBit ))
          { 
            Edger = ObjectSurface;
          }
          else if (( KDirectionBits[ (int)ObjectRotation ] == FrontBit )
            && ( KDirectionBits[ (int)ObjectSurface ] == DownBit ))
          {
            Edger = ObjectRotation;
          }        
        
          break;

        case EnigmaWC::ID::EFern:
          // Note the presence of a Fern.  This object ran parallel to its
          // surface (similar to a CatWalk) in the original maps.  In new maps,
          // a fern now grows perpendicular to its surface.
          
          if ( ObjectRotation == Surface )
          {
            // The Fern runs underneath the player, behaving like a CatWalk. 
            
            CatWalkBits = KDirectionBits[ (int)ObjectSurface ]
                        | KOppositeBits[ (int)ObjectSurface ];
          }
          else
          {
            // The Fern runs alongside other walls, behaving like a pair of
            // HandHolds on these walls.
          
            HandHoldBits = KDirectionBits[ (int)ObjectSurface ]
                         | KOppositeBits[ (int)ObjectSurface ];
                         
            ObjectSurfaceBit = KDirectionBits[ (int)ObjectRotation ];
            
            if ( ObjectSurfaceBit == FrontBit )
              FrontHandHoldBits = HandHoldBits;
            else if ( ObjectSurfaceBit == BackBit )
              BackHandHoldBits = HandHoldBits;
            else if ( ObjectSurfaceBit == RightBit )
              RightHandHoldBits = HandHoldBits;
            else if ( ObjectSurfaceBit == LeftBit )
              LeftHandHoldBits = HandHoldBits;          
            else if ( ObjectSurfaceBit == UpBit )
              UpHandHoldBits = HandHoldBits;
          }
          
          break;
  
        case EnigmaWC::ID::EOutdoor:
          // Note the presence of an Outdoor object.
          
          OutdoorBits |= KDirectionBits[ (int)ObjectSurface ];
          break;
          
        case EnigmaWC::ID::EIndoor:
          // Note the presence of an Indoor object.
          
          IndoorBits |= KDirectionBits[ (int)ObjectSurface ];
          break;
          
        case EnigmaWC::ID::EWaterLayer:
          // Note the presence of an active Waterlayer with the same
          // surface as the player.  
          
          WaterLayer = ( ObjectSurface == Surface );
          
          // Any Waterlayer, regardless of its surface, will act like
          // a wall if the player is using an InnerTube or HydrogenBalloon.
          
          if ( (*iPlayer).iInnerTube || (*iPlayer).iHydrogenBalloon )
            WallBits |= KDirectionBits[ (int)ObjectSurface ];

          break;
          
         case EnigmaWC::ID::EWater:
          // Note the presence of Water surrounding the player.
          
          Water = TRUE;
          
          // Save Water presence in the active player.  This information
          // may influence items being selected for use.
          
          (*iPlayer).iWater = Water;
          break;

        default:
          break;
      }
    }
  } 

  // Search for an active teleporter in the room.

  gboolean Teleporting = FALSE;
  
  std::list<std::list<CMapTeleporter>::iterator>::iterator Teleporter;

  for ( Teleporter = iTeleporters.begin();
        Teleporter != iTeleporters.end();
        ++ Teleporter )
  {
    // If a teleporter is present and is on the player's surface, it can affect
    // the player.  Exit the loop with an interator to this teleporter.
    
    if ( (*(*Teleporter)).iPresence.GetState()
      && (*(*Teleporter)).iState.GetState()
      && ( (*(*Teleporter)).iSurface == Surface ))
    {
      Teleporting = TRUE;
      break;
    }
  }
  
  // Note walls due to nearby map boundaries.
  
  if ( Location.iAbove >= iMap->UpperBounds().iAbove )
    WallBits |= MAbove;
  
  if ( Location.iAbove <= iMap->LowerBounds().iAbove )
    WallBits |= MBelow;
    
  if ( Location.iNorth >= iMap->UpperBounds().iNorth )
    WallBits |= MNorth;
  
  if ( Location.iNorth <= iMap->LowerBounds().iNorth )
    WallBits |= MSouth;

  if ( Location.iEast >= iMap->UpperBounds().iEast )
    WallBits |= MEast;

  if ( Location.iEast <= iMap->LowerBounds().iEast )
    WallBits |= MWest;

  // Update contact bits for all players in the room.  These bits combine
  // to form a total room contact.
  
  std::list<std::list<CMapPlayer>::iterator>::iterator Player;
  
  for ( Player = iPlayers.begin();
        Player != iPlayers.end();
        ++ Player )
  {
    (*(*Player)).iContactBits
      = KDirectionBits[ (int)(*(*Player)).iSurface ] | MCenter;

    (*(*Player)).iAntiContactBits = MNone;
  }
  
  // [ Motion: First Phase ]
  // Now that a description of the room has been created, determine how the
  // player will move and change contact within the room.  Before moving, the
  // player starts on a neutral surface.

  guint8 NeutralBit;

  if ( ((*iPlayer).iInnerTube || (*iPlayer).iHydrogenBalloon ) && Water )
    NeutralBit = UpBit;
  else
    NeutralBit = DownBit;

  guint8 MotionBit     = KDirectionBits[ (int)Motion ];
  guint8 TranslateBits = MNone;
  gboolean VisualDelay = FALSE;
  
  // Motion may add additional contact directions.  The conditional
  // statements are arranged from the highest priority to the lowest.  Each
  // one (except the fall-through final one) considers a situation where the
  // player does not move normally.

  if ( MoverBits )
  { 
    // Apply offsets due to Movers.  Cancel any FreeMoving, Edger resurfacing,
    // and Teleporting in progress.  If transitions are enabled, a visual
    // delay will be added before starting to move.

    TranslateBits = MoverBits;
    VisualDelay   = TRUE;
    Teleporting   = FALSE;
    iFreeMoving   = FALSE;
    iEdging       = KEdgerIdle;
  }
  else if ( Teleporting )
  {
    // A teleporter is present on the player's surface.  Update those parts
    // of the players next orientation that should change.

    if ( (*(*Teleporter)).iSurfaceArrival != EnigmaWC::Direction::ENone )
      SurfaceNext = (*(*Teleporter)).iSurfaceArrival;     
   
    if ( (*(*Teleporter)).iRotationArrival != EnigmaWC::Direction::ENone )
      RotationNext = (*(*Teleporter)).iRotationArrival;  
    
    if ( (*(*Teleporter)).iLocationArrival.iEast != G_MAXUINT16 )
      LocationNext.iEast = (*(*Teleporter)).iLocationArrival.iEast;
    
    if ( (*(*Teleporter)).iLocationArrival.iNorth != G_MAXUINT16 )
      LocationNext.iNorth = (*(*Teleporter)).iLocationArrival.iNorth;
      
    if ( (*(*Teleporter)).iLocationArrival.iAbove != G_MAXUINT16 )
      LocationNext.iAbove = (*(*Teleporter)).iLocationArrival.iAbove;
    
    // Play a sound and add a visual delay if the teleporter is visible.
    
    if ( (*(*Teleporter)).iVisibility.GetState() )
    {
      SoundID     = CSounds::ID::ETeleport;
      VisualDelay = TRUE;
    } 

    // Cancel any FreeMoving, and Edger resurfacing in progress.
    
    iFreeMoving = FALSE;
    iEdging     = KEdgerIdle;
  }
  else if ( (*iPlayer).iHydrogenBalloon
         && ( MotionBit != MNone ))
  {
    // The player is moving while using a HydrogenBalloon.  Only vertical
    // movement is allowed.

    if ( MotionBit & KDirectionBits[ (int)Rotation] )
      TranslateBits |= UpBit;
    else if ( MotionBit & KOppositeBits[ (int)Rotation ] )
      TranslateBits |= DownBit;
      
    iFreeMoving = FALSE;
  }
  else if ( iFreeMoving
      && (( WallBits & NeutralBit )
      || ( LadderBits
         | StairsBits
         | StairsTopBits
         | HandHoldBits
         | CatWalkBits )))
  {
    // The player was FreeMoving, but has encountered a platform.  Create a
    // false "wall" for the player to bump into, producing a "bump" sound played
    // later.
    
    WallBits     |= NeutralBit;
    TranslateBits = MotionBit | NeutralBit;
    iFreeMoving   = FALSE;
  }
  else if ( iEdging == KEdgerSecond )
  {
    // An Edger resurfacing is in progress, which is currently in the
    // second stage (second attempt to find a surface).
    
    if ( WallBits & KDirectionBits[ (int)iOldSurface ] )
    {
      // There is a wall in front of the player, so resurface the
      // player onto this instead.
          
      SurfaceNext  = iOldSurface;
      RotationNext = Resurface( Surface, Rotation, SurfaceNext );
      iEdging      = KEdgerIdle;
    }
    else
    {
      // There is no wall in front of the player, so lower the player
      // past the edge and along-side a possible new surface.
          
      TranslateBits |= KDirectionBits[ (int)iOldSurface ];
      iEdging        = KEdgerFinal;
      VisualDelay    = TRUE;
    }
  }
  else if ( iEdging == KEdgerFinal )
  {
    // An Edger resurfacing is in progress, which is currently in the
    // final stage (final attempt to find a surface).

    if (!( WallBits & KDirectionBits[ (int)Surface ]))
    {
      // A surface is still not available, so attempt to resurface the player
      // onto the underside of the player's original surface.
       
      SurfaceNext  = (EnigmaWC::Direction)KOppositeTable[ (int)iOldSurface ];
      RotationNext = Resurface( Surface, Rotation, SurfaceNext );
          
      // Move the player along-side the underside surface.
          
      TranslateBits |= KDirectionBits[ (int)Surface ];
      VisualDelay    = TRUE;
    }

    // End the Edger resurfacing process.  If the underside of the player's
    // original surface is later found to be no longer present, the player
    // will start falling.
        
    iEdging = KEdgerIdle;
  }
  else if ( FlipperBits & DownBit )
  {
    // There is a Flipper on the player's floor, so move the player to the
    // ceiling.  A sound will be played later.
    
    SurfaceNext  = (EnigmaWC::Direction)KOppositeTable[ (int)Surface ];
    RotationNext = Resurface( Surface, Rotation, SurfaceNext );
    SoundID      = CSounds::ID::EFlipSurface;
    iFreeMoving  = FALSE;
  }
  else if (( MotionBit != MNone )
        && ( Surfacer != EnigmaWC::Direction::ENone ))
  {
    // The player is moving towards a wall with a Surfacer near the floor.
    // Move the player onto the wall.  A sound will be played later.
          
    SurfaceNext  = Surfacer;
    RotationNext = Resurface( Surface, Rotation, SurfaceNext );
    SoundID      = CSounds::ID::EResurface;
    iFreeMoving  = FALSE;  
  }
  else if (( MotionBit != MNone )
    && ( Edger != EnigmaWC::Direction::ENone ))
  {
    // The player is moving towards a wall with an Edger near the floor.
    // Begin searching for a new surface for the player.  A sound will be
    // played later.

    if ( WallBits & MotionBit )
    {
      // A wall is present in front of the player in the direction
      // of motion.  Resurface the player onto this wall (behave
      // like a Surfacer).
          
      SurfaceNext = Edger;
      iEdging     = KEdgerIdle;
    }
    else
    {
      // A surface is not present in front of the player.  Begin an 
      // attempt to resurface the player onto the surface opposite to
      // that of motion, which is the surface around the edge.

      SurfaceNext    = (EnigmaWC::Direction)KOppositeTable[ (int)Motion ];
      TranslateBits |= MotionBit;
      iOldSurface    = Surface;       
      iEdging        = KEdgerSecond;
    }
    
    RotationNext = Resurface( Surface, Rotation, SurfaceNext );
    SoundID      = CSounds::ID::EResurface;
    iFreeMoving  = FALSE;
  }
  else if ( StairsBits & MotionBit )
  {
    // The player is moving towards Stairs oriented for climbing.  Climb "up"
    // in the player's "view space".
    
    TranslateBits |= UpBit;
  }
  else if ( StairsTopBits & MotionBit )
  {
    // The player is moving away from a StairsTop oriented for climbing.
    // Climb "down" in the player's "view space".
    
    TranslateBits |= DownBit;
  }
  else if (( MotionBit != MNone )
       && (( UpLadderBits | UpHandHoldBits ) & FrontBit )
       && (( FrontLadderBits | FrontHandHoldBits ) & UpBit ))
  {
    // The player is moving towards an intersection of a parallel upper ladder
    // end or HandHold and a vertical front ladder end or HandHold.  The player
    // climbs "down" the vertical ladder or HandHold (similar to having the
    // player climb "up" a vertical ladder at a floor).

    TranslateBits |= DownBit;
  }
  else if (( MotionBit != MNone )
       && (( FrontLadderBits | FrontHandHoldBits ) & UpBit ))
  {
    // The player is moving towards a surface with an upper ladder end or
    // HandHold.  The direction of motion is "up" in the player's "view space".
    
    TranslateBits |= UpBit;
  }
  else if (( MotionBit != MNone )
       && (( BackLadderBits | BackHandHoldBits ) & DownBit )
       && !( WallBits & DownBit )
       && !( CatWalkBits & FrontBit ) 
       && (( UpLadderBits | UpHandHoldBits ) == 0 )
       && (( DownLadderBits & ( FrontBit | BackBit )) != ( FrontBit
                                                         | BackBit )))
  {
    // The player is backing away from a vertical ladder end or HandHold near
    // the floor, there is no floor under the player or complete parallel
    // ladder below (i.e. no barrier to downward motion), and there are no
    // ladders or HandHolds above.  The player moves "down" in a controlled
    // manner.
    
    TranslateBits |= DownBit;
  }
  else if (( MotionBit != MNone )
       && ( LadderBits != 0 )
       && !( WallBits & DownBit )
       && !( CatWalkBits & FrontBit ) 
       && ( FrontLadderBits == 0 )
       && !(( UpLadderBits | DownLadderBits ) & ( LeftBit
                                                | RightBit
                                                | FrontBit ))
       && !(( LeftLadderBits | RightLadderBits ) & ( UpBit
                                                   | DownBit
                                                   | FrontBit )))
  {
    // The player is moving and Ladders are present, but there are no Ladder
    // ends positioned to allow the player to move forward, and there is no
    // floor below.  The player falls "down".  There is no visual delay before
    // falling.
    
    TranslateBits |= DownBit;
    iFreeMoving    = TRUE;
  }
  else if (( MotionBit != MNone )
       && ( LadderBits == 0 )
       && ( CatWalkBits != 0 )
       && ( HandHoldBits == 0 )
       && !( WallBits & DownBit )
       && !( CatWalkBits & FrontBit ))
  {
    // The player is moving with CatWalks only, but there are no CatWalks
    // in the direction of the player's motion, and there is no floor below.
    // The player falls "down".  There is no visual delay before falling.  
  
    TranslateBits |= DownBit;
    iFreeMoving    = TRUE;
  }
  else if (( MotionBit != MNone )
       && ( LadderBits == 0 )
       && ( CatWalkBits == 0 )
       && ( HandHoldBits != 0 )
       && !( WallBits & DownBit )
       && !(( UpHandHoldBits
            | LeftHandHoldBits
            | RightHandHoldBits ) & FrontBit ))
  {
    // The player is moving with HandHolds only, but there are no HandHolds
    // in the direction of the player's motion, and there is no floor below.
    // The player falls "down".  There is no visual delay before falling.  
  
    TranslateBits |= DownBit;
    iFreeMoving    = TRUE;
  }
  else if ( !( WallBits & NeutralBit )
       && !( (*iPlayer).iHydrogenBalloon && !Water )
       && (( LadderBits
           | StairsBits
           | StairsTopBits
           | HandHoldBits
           | CatWalkBits ) == 0 ))
  {
    // The player is unsupported, so begins falling towards the neutral surface
    // (view space).  Any motion action is ignored.  If transitions are
    // disabled, a visual delay will be added before moving to the next room.
  
    TranslateBits = NeutralBit;
    iFreeMoving   = TRUE;
    VisualDelay   = TRUE;
  }
  else
  {
    // There are no special situations in this room, so the player moves
    // horizontally based on the original motion direction.

    TranslateBits = MotionBit;
  }

  // Add any additional contact directions due to motion.

  (*iPlayer).iContactBits |= TranslateBits;

 // [ Motion: Last Phase ]
  // The previous phase of motion determination may have changed the direction
  // of the original motion.  This phase handles the updated motion.
  // The translation bits are converted into a new location, and a test is made
  // for walls and Outdoor/Indoor surface crossings.
  
  gboolean Blocked        = FALSE;
  (*iPlayer).iOutdoorNext = (*iPlayer).iOutdoor;
    
  if ( TranslateBits & MEast )
  {				
    if ( WallBits & MEast )
      Blocked = TRUE;
    else
    {
      // The player will be passing through the East surface surface.
      
      ++ LocationNext.iEast;
      
      // Update the player's Outdoor state if an Outdoor/Indoor object
      // surface will be crossed.
      
      if ( OutdoorBits & MEast )
        (*iPlayer).iOutdoorNext = TRUE;
      else if ( IndoorBits & MEast )
        (*iPlayer).iOutdoorNext = FALSE;
    }
  }
  else if ( TranslateBits & MWest )
  {		
    if ( WallBits & MWest )
      Blocked = TRUE;
    else
    {
      // The player will be passing through the West surface surface.
    
      -- LocationNext.iEast;
      
      // Update the player's Outdoor state if an Outdoor/Indoor object
      // surface will be crossed.
      
      if ( OutdoorBits & MWest )
        (*iPlayer).iOutdoorNext = TRUE;
      else if ( IndoorBits & MWest )
        (*iPlayer).iOutdoorNext = FALSE;
    }  
  }

  if ( TranslateBits & MNorth )
  {				
    if ( WallBits & MNorth )
      Blocked = TRUE;
    else
    {
      // The player will be passing through the North surface surface.
    
      ++ LocationNext.iNorth;
      
      // Update the player's Outdoor state if an Outdoor/Indoor object
      // surface will be crossed.
      
      if ( OutdoorBits & MNorth )
        (*iPlayer).iOutdoorNext = TRUE;
      else if ( IndoorBits & MNorth )
        (*iPlayer).iOutdoorNext = FALSE;
    }
  }
  else if ( TranslateBits & MSouth )
  {		
    if ( WallBits & MSouth )
      Blocked = TRUE;
    else
    {
      // The player will be passing through the South surface surface.
     
      -- LocationNext.iNorth;
      
      // Update the player's Outdoor state if an Outdoor/Indoor object
      // surface will be crossed.
      
      if ( OutdoorBits & MSouth )
        (*iPlayer).iOutdoorNext = TRUE;
      else if ( IndoorBits & MSouth )
        (*iPlayer).iOutdoorNext = FALSE;
    }
  }

  if ( TranslateBits & MAbove )
  {			
    if ( WallBits & MAbove )
      Blocked = TRUE;
    else
    {
      // The player will be passing through the Above surface surface.
      
      ++ LocationNext.iAbove;
    
      // Update the player's Outdoor state if an Outdoor/Indoor object
      // surface will be crossed.
      
      if ( OutdoorBits & MAbove )
        (*iPlayer).iOutdoorNext = TRUE;
      else if ( IndoorBits & MAbove )
        (*iPlayer).iOutdoorNext = FALSE;
    }
    
  }
  else if ( TranslateBits & MBelow )
  {
    if ( WallBits & MBelow )
      Blocked = TRUE;
    else
    {
      // The player will be passing through the Below surface surface.
     
      -- LocationNext.iAbove;
      
      // Update the player's Outdoor state if an Outdoor/Indoor object
      // surface will be crossed.
      
      if ( OutdoorBits & MBelow )
        (*iPlayer).iOutdoorNext = TRUE;
      else if ( IndoorBits & MBelow )
        (*iPlayer).iOutdoorNext = FALSE;
    }
  }
  
  if ( OutdoorBits & MCenter )
    (*iPlayer).iOutdoorNext = TRUE;
  else if ( IndoorBits & MCenter )
    (*iPlayer).iOutdoorNext = FALSE;
  
  // Move all found items to another list for handling later.  The final motion
  // offset is taken into account here, which may differ from the initial one.
 
  guint8 ContactBits = (*iPlayer).iContactBits
                     & ~(*iPlayer).iAntiContactBits;

  std::list<std::list<CMapItem>::iterator>::iterator Item = iItems.begin();
  std::list<std::list<CMapItem>::iterator>::iterator NextItem;
 
  while ( Item != iItems.end() )
  {
    if ( !iMap->Items().GetFound( *Item )
      && (*(*Item)).iPresence.GetState() )
    {
      ObjectSurfaceBit = KDirectionBits[ (int)(*(*Item)).iSurface ];
      
      if ( ContactBits & ObjectSurfaceBit )
      {
        // The player is in contact with an item.
        
        NextItem = Item;
        ++ NextItem;
        iFoundItems.splice( iFoundItems.end(), iItems, Item );
        Item = NextItem;
        
        // Play a sound.
        
        SoundID = CSounds::ID::EItemFound;
      }
      else
      {
        // Keep other unfound items in the list.
      
        ++ Item;
      }
    }
    else
    {
      // Erase items already found.
 
      Item = iItems.erase( Item );
    }   
  }
  
  // Apply Sense to objects in contact with the player, but only if the player
  // is remaining in the room.  Add a visual delay if this causes a visible
  // mechanism to changes (all visual mechanisms also have a mechanism sound).
  
  if ((*iPlayer).iLocation == LocationNext )
  {
    CSounds::ID MechanismSoundID;
    UpdateSense( SenseChange, MechanismSoundID );
  
    if ( MechanismSoundID != CSounds::ID::ENone )
    {
      SoundID     = MechanismSoundID;
      VisualDelay = TRUE;
    }
  }
  
  // Play a sound if the player enters or exits a WaterLayer, but only if
  // there are no pending sounds from a previous mechanism.  Update the
  // player's WaterLayer state.  
  
  if ( SoundID == CSounds::ID::ENone )
  {
    if ( !WaterLayer && (*iPlayer).iWaterLayer )
      SoundID = CSounds::ID::EExitWater;
    else if ( WaterLayer && !(*iPlayer).iWaterLayer )
      SoundID = CSounds::ID::EEnterWater;
  }
  
  (*iPlayer).iWaterLayer = WaterLayer;
  
  // If sounds are enabled, play any pending sounds resulting from room
  // exploration.

  if ( iSounds )
  {
    if ( SoundID != CSounds::ID::ENone )
      iSounds->Play( SoundID);
    else if ( Blocked )
      iSounds->Play( CSounds::ID::EBump );
  }
  
  // The player has finished exploring the room during this exploration phase,
  // and accumulated activities for completion during the next phase.  Proceed
  // to the next phase after either showing a view transition, pausing briefly
  // for a visual delay, or continuing right away.
 
  (*iPlayer).iSurfaceNext  = SurfaceNext;
  (*iPlayer).iRotationNext = RotationNext;
  (*iPlayer).iLocationNext = LocationNext;     
   
  if ( (*iPlayer).iLocation != (*iPlayer).iLocationNext ) 
  {
    // The player is moving to a different room. Append a Continue action
    // at the head of the queue for handling next.
    
    iActionQueue.front() = CPlayRoom::Action::EContinue;
    iActionQueue.push_front( CPlayRoom::Action::ENone );
    
    // If Stairs are present in the player's next location, find the surface
    // of these stairs.  This allows planning a transition path to avoid them.

    std::list<std::list<CMapObject>::iterator> ObjectsNext;
    iMap->Objects().Read( (*iPlayer).iLocationNext, ObjectsNext );
      
    for ( Object = ObjectsNext.begin();
          Object != ObjectsNext.end();
          ++ Object )
    {
      if ( ((*(*Object)).iID == EnigmaWC::ID::EStairs )
         && (*(*Object)).iPresence.GetState() )
      {
        // Stairs are present in the next location.
        // Save the surface of these stairs.
      
        (*iPlayer).iStairsSurfaceNext  = (*(*Object)).iSurface;
        (*iPlayer).iStairsRotationNext = (*(*Object)).iRotation;
        break;
      }
    } 
    
    // Continue room exploration.
    
    if ( iViewTransitions && !Teleporting )
      Do_Transition_Continue();
    else if ( VisualDelay )
      Do_Delay_Continue();
    else
      Do_Continue();  
  }  
  else if (( (*iPlayer).iSurface != (*iPlayer).iSurfaceNext )
        || ( (*iPlayer).iRotation != (*iPlayer).iRotationNext ))
  {
    // The player is remaining in the room, but changing surface or rotation.
    // Append a Continue action at the head of the queue for handling next.
    
    iActionQueue.front() = CPlayRoom::Action::EContinue;
    iActionQueue.push_front( CPlayRoom::Action::ENone );
    
    // Continue room exploration.

    if ( iViewTransitions && !Teleporting )
      Do_Transition_Continue();
    else if ( VisualDelay )
      Do_Delay_Continue();
    else
      Do_Continue();
  }
  else if ( !iFoundItems.empty()
          || VisualDelay
          || SenseChange )   
  {
    // The player is remaining in the room and not changing orientation, but
    // an item has been found, a visual delay was requested, or Sense to a
    // connected object changed.  Append a Continue action at the head of the
    // queue for handling next.
    
    iActionQueue.front() = CPlayRoom::Action::EContinue;
    iActionQueue.push_front( CPlayRoom::Action::ENone ); 
      
    // Continue room exploration. 

    if ( VisualDelay )
      Do_Delay_Continue();
    else
      Do_Continue();
  }
  else
  {
    // No changes have occurred during room exploration.
    // Continue room exploration.
    
    Do_Continue();
  }
  
  return;
}

//*---------------------------------------------------------------*
//* This method update Sense input to all room objects.           *
//*---------------------------------------------------------------*
//* aChange: TRUE if the Sense of a connected object changed.     *
//* aSound:  Mechanism sound ID, or ENone if no mechanism change. *
//*---------------------------------------------------------------*

void CPlayRoom::UpdateSense( gboolean& aChange, CSounds::ID& aSound )
{
  // All players in the room contribute to the application of Sense
  // to the room.  Obtain a total contact contribution from all players. 

  guint8 ContactBits     = MNone;
  guint8 AntiContactBits = MNone;  

  std::list<std::list<CMapPlayer>::iterator>::iterator Player;
  
  for ( Player = iPlayers.begin();
        Player != iPlayers.end();
        ++ Player )  
  {
    ContactBits     |= (*(*Player)).iContactBits;
    AntiContactBits |= (*(*Player)).iAntiContactBits;
  }
  
  ContactBits &= ~AntiContactBits;
    
  // Update Sense to all room objects.
 
  gboolean OldState;
  gboolean NewState;
  aSound  = CSounds::ID::ENone;
  aChange = FALSE;
  guint8 Contact;
  
  std::list<std::list<CMapObject>::iterator>::iterator Object;
  
  for ( Object = iObjects.begin();
        Object != iObjects.end();
        ++ Object )
  {
    // Update Sense to a connected and present object. 
    
    if ( (*(*Object)).iSense.Connected()
      && (*(*Object)).iPresence.GetState() )
    {    
      OldState = (*(*Object)).iState.GetState();

      // Starting in v4.0, a Tree object is oriented perpendicular to its
      // surface, unlike a Ladder which runs parallel to its surface.  Applying
      // Sense to a Tree now requires its Rotation instead of its Surface.
      // All other objects used their Surface when applying Sense.
    
      if ( (*(*Object)).iID == EnigmaWC::ID::ETree )
      {
        Contact =
          ContactBits & KDirectionBits[ (int)(*(*Object)).iRotation ];
      }
      else
      {
        Contact =
          ContactBits & KDirectionBits[ (int)(*(*Object)).iSurface ];
      }
     
      // Apply Sense to the object if the player has the required key,
      // or if the object does not require a key.
 
      if ( iMap->Items().UseKey( *Object, (*iPlayer).iNumber )) 
      {
        // NOTE: The SetState() function must be called.  To avoid having this
        // function short-circuited if aChange is already TRUE, it must appear
        // first in the Boolean OR expression.
        
        if ( Contact )
          aChange = (*(*Object)).iSense.SetState( TRUE ) || aChange;
        else
          aChange = (*(*Object)).iSense.SetState( FALSE ) || aChange;
      }

      // If the Sense change to a visible mechanism also changes its State,
      // select an appropriate sound to be played.
      
      NewState = (*(*Object)).iState.GetState();
      
      if ( NewState != OldState )
      {
        switch ( (*(*Object)).iID )
        {
          case EnigmaWC::ID::EStoneButton:
             // Select a StoneButton sound.

            if ( NewState )
              aSound = CSounds::ID::EStoneButtonPress;
            else
              aSound = CSounds::ID::EStoneButtonRelease;
          
            break;
            
          case EnigmaWC::ID::EPadButton:
             // Select a PadButton sound.

            if ( NewState )
              aSound = CSounds::ID::EPadButtonPress;
            else
              aSound = CSounds::ID::EPadButtonRelease;
          
            break;
            
          case EnigmaWC::ID::EWoodDoor:
             // Select a WoodDoor sound.

            if ( NewState )
              aSound = CSounds::ID::EDoorClose;
            else
              aSound = CSounds::ID::EDoorOpen;
          
            break;

          case EnigmaWC::ID::EPullRing:
             // Select a WoodDoor sound.

            if ( NewState )
              aSound = CSounds::ID::ERingPull;
            else
              aSound = CSounds::ID::ERingRelease;
          
            break;
            
          case EnigmaWC::ID::EWallEyes:
             // Select a WallEyes sound.

            if ( NewState )
              aSound = CSounds::ID::EPurr;
          
            break;

          case EnigmaWC::ID::ECubeLock:
          case EnigmaWC::ID::ESphereLock:
          case EnigmaWC::ID::EWaterLock:
          case EnigmaWC::ID::EAppleLock:
          case EnigmaWC::ID::EOrangeLock:
          case EnigmaWC::ID::EWineLock:
             // Select a lock sound.

            if ( NewState )
              aSound = CSounds::ID::ELockOn;
            else
              aSound = CSounds::ID::ELockOff;
          
            break;
    
          case EnigmaWC::ID::ETree:
             // Select a WoodDoor sound.

            if ( NewState )
              aSound = CSounds::ID::ETreeGrow;
          
            break;
    
          default:
            break;
        }
      }
    }
  }
  
  return;
}

//*---------------------------------------------------------------------*
//* This method transitions to a new orientation before continuing room *
//* exploration.                                                        *
//*---------------------------------------------------------------------*

void CPlayRoom::Do_Transition_Continue()
{
  // Prepare the transition path, then advance to the first step
  // of the transition.
 
  iTransition.Prepare( iPlayer );
  iTransition.Advance();
  (*iPlayer).iOffset = iTransition.GetPoint();
  
  // Update the player's current view to advance the transition using
  // On_View_Done().

  Do_View( 1 );
  Do_Orientation();                                         
  return;
}

//*---------------------------------------------------------------------*
//* This method adds a visual delay before continuing room exploration. *
//*---------------------------------------------------------------------*

void CPlayRoom::Do_Delay_Continue()
{
  // Update the transition to find the player offset in the current
  // location, then clear the transition so it has no effect in
  // On_View_Done().

  iTransition.Prepare( iPlayer ); 
  (*iPlayer).iOffset = iTransition.GetPoint();
  iTransition.Clear();

  // Update the player's current view and orientation.

  Do_View( 1 );
  Do_Orientation();
  
  // Allow control to pass back to the Glib Main Loop before continuing
  // room exploration after a short visual delay.
  
  Glib::signal_timeout().connect_once( m_slot_delay,
                                       KVisualDelay,
                                       Glib::PRIORITY_DEFAULT );
  return; 
}

//*-------------------------------------------*
//* This method continues exploring the room. *
//*-------------------------------------------*

void CPlayRoom::Do_Continue()
{
  // Update the transition to find the player offset for the current
  // location, then clear the transition so it has no effect in
  // On_View_Done().

  iTransition.Prepare( iPlayer ); 
  (*iPlayer).iOffset = iTransition.GetPoint();
  iTransition.Clear();
  
  // Update the player's current view and orientation.
  
  Do_View( 1 );
  Do_Orientation();
  
  // Allow control to pass back to the Glib Main Loop before continuing
  // room exploration.
 
  Glib::signal_idle().connect_once( m_slot_idle,
                                    Glib::PRIORITY_HIGH_IDLE );
  return; 
}

//*---------------------------------------------------------*
//* This method emits a signal to update the player's view. *
//*---------------------------------------------------------*
//* aControl: View control.                                 *
//*---------------------------------------------------------*

void CPlayRoom::Do_View( guint8 aControl )
{
  // Update the player's view.
  
  m_signal_view.emit( aControl );
  return; 
}

//*------------------------------------------------------------------*
//* This method is a signal handler that continues room exploration. *
//*------------------------------------------------------------------*

void CPlayRoom::On_Continue()
{  
  // The current action has been completed.  Obtain the next one from the
  // queue to continue room exploration.
 
  Explore( CPlayRoom::Action::EContinue );
  return; 
}

//*------------------------------------------------------------------*
//* This method is called after the player's view has been rendered. *
//* A transition in progress is advanced.                            * 
//*------------------------------------------------------------------*
//* aRenderTime: View rendering time.                                *
//*------------------------------------------------------------------*

void CPlayRoom::On_View_Done( gint64 aRenderTime )
{
  // Update the transition division time.

  iTransition.SetDivision( aRenderTime );
  
  // Handle a view transition if one is in progress.
  
  if ( iTransition.GetStep() > 1 )
  {
    // A transition is in progress, but is not on the final step.  Advance
    // the transition to the next step.
  
    iTransition.Advance();
    (*iPlayer).iOffset = iTransition.GetPoint();
    
    if ( iTransition.GetStep() == ( iTransition.GetTotal() / 2 ) )
    {
      // The transition is at its mid-point.  Switch to view from the player's
      // end orientation.  The player's Outdoor state is updated in case
      // an Outdoor/Indoor surface is being crossed.
      
      (*iPlayer).iOutdoor = (*iPlayer).iOutdoorNext;
      Do_View( 2 );
    }
    else
    {
      // Update the player's view using the current orientation.
      
      Do_View( 0 );
    }
  }  
  else if ( iTransition.GetStep() == 1 )
  {
    // A transition in progress is on the final step.  Advance past the final
    // step, then continue exploring the room.  An update of the player's
    // view will be done later.

    iTransition.Advance();
    (*iPlayer).iOffset.Clear();
    Explore( CPlayRoom::Action::EContinue );
  }

  return; 
}

//*------------------------------------------------------------*
//* This method emits a signal containing a string with player *
//* orientation (Location, Surface, and Rotation).             *
//*------------------------------------------------------------*

void CPlayRoom::Do_Orientation()
{
  // Return immediately of the game map has not been loaded
  // (iPlayer will be invalid).
  
  if ( !iMap->GetLoaded() )
  {
    m_signal_orientation.emit( _("No game map") );
    return;
  }
  
  // Obtain the compass and floor text string from the player's surface
  // and rotation.
	
  const char* RotationText = KDirectionText[ (int)(*iPlayer).iRotation ];
	const char* SurfaceText  = KDirectionText[ (int)(*iPlayer).iSurface ];
	
  if ( iFullOrientation )
  {
    // Emit signal with player's full map orientation.

    Glib::ustring String =
      Glib::ustring::compose(
        _("East %1, North %2, Above %3\nPlayer %4 on %5 facing %6"),
        (*iPlayer).iLocation.iEast,
        (*iPlayer).iLocation.iNorth,
        (*iPlayer).iLocation.iAbove,
        (*iPlayer).iNumber + 1,
        SurfaceText,
        RotationText );
        
     m_signal_orientation.emit( String );
  }
  else
  {
    // Emit signal with only player's compass direction (rotation).

     m_signal_orientation.emit( RotationText );
  }

  return; 
}

//*-------------------------------------------------------------------*
//* This method returns the PlayRoom single view frame signal server. *
//*-------------------------------------------------------------------*

CPlayRoom::type_signal_view CPlayRoom::signal_view()
{
  return m_signal_view;
}

//*-------------------------------------------------------------*
//* This method returns the PlayRoom orientation signal server. *
//*-------------------------------------------------------------*

CPlayRoom::type_signal_orientation CPlayRoom::signal_orientation()
{
  return m_signal_orientation;
}
