//*-----------------------------------------------------*
//* Program: Enigma in the Wine Cellar                  *
//* Version: 5.0 for Linux OS                           *
//* File:    ViewCone.cpp                               *
//* Date:    October 19, 2016                           *
//* Author:  Chris Sterne                               *
//*-----------------------------------------------------*
//* This class handles the drawing of the viewing cone. *
//*-----------------------------------------------------*

#include "ViewCone.h"

//*---------------------*
//* Local declarations. *
//*---------------------*

#define KIndoorR (GLfloat)40/255        // Indoor colour: Dark grey.
#define KIndoorG (GLfloat)40/255
#define KIndoorB (GLfloat)40/255

#define KOutdoorR (GLfloat)  0/255      // Outdoor colour: Light blue.
#define KOutdoorG (GLfloat)204/255
#define KOutdoorB (GLfloat)255/255

#define KInnerTubeOffset        0.45/2  // Offset for InnerTube in use.
#define KHydrogenBalloonOffset -0.52/2  // Offset for HydrogenBalloon in use.

// These matrices are used for translating an offset in the player's view
// space (Right, Up, Front) to an offset (East, Above, North) in map space.
// Each 3x3 matrix corresponds to a particular map direction, with Right
// matching East, Up matching Above, and Front matching North.
//
// Right  Up   Front 
// -----------------
// [ a     b     c ] -> East Offset
// [ d     e     f ] -> Above Offset
// [ g     h     i ] -> North Offset

static const gint8 KNone[9] =
{
   0,  0,  0,    // East
   0,  0,  0,    // Above
   0,  0,  0     // North
};

static const gint8 KNorth[9] =
{
   1,  0,  0,    // East
   0,  1,  0,    // Above
   0,  0,  1     // North
};

static const gint8 KSouth[9] =
{
  -1,  0,  0,    // East
   0,  1,  0,    // Above
   0,  0, -1     // North
};

static const gint8 KEast[9] =
{
   0,  0,  1,    // East
   0,  1,  0,    // Above
  -1,  0,  0     // North
};

static const gint8 KWest[9] =
{
   0,  0, -1,    // East
   0,  1,  0,    // Above
   1,  0,  0     // North
};

static const gint8 KAbove[9] =
{
   1,  0,  0,    // East
   0,  0,  1,    // Above
   0, -1,  0     // North
};

static const gint8 KBelow[9] =
{
   1,  0,  0,    // East
   0,  0, -1,    // Above
   0,  1,  0     // North
};

// This array contains the appropriate 3x3 map offset matrix to be used
// for a player's viewing direction (rotation) on any surface.

static const gint8* KOffsetMatrices[ (int)EnigmaWC::Direction::TOTAL ] =
{
  KNone, KNorth, KSouth, KEast, KWest, KAbove, KBelow, KNone
};

// This table contains all valid rotations on a surface.  It is used for
// randomizing a WaterLayer rotation.  The first index is the Waterlayer
// object Surface, while the second index is a Rotation selection value.

#define DNone  (guint8)EnigmaWC::Direction::ENone
#define DNorth (guint8)EnigmaWC::Direction::ENorth
#define DSouth (guint8)EnigmaWC::Direction::ESouth
#define DEast  (guint8)EnigmaWC::Direction::EEast
#define DWest  (guint8)EnigmaWC::Direction::EWest
#define DAbove (guint8)EnigmaWC::Direction::EAbove
#define DBelow (guint8)EnigmaWC::Direction::EBelow

static const guint8 KRotations[ (int)EnigmaWC::Direction::TOTAL ][ 4 ] =
//-----------------------------------------------
//      Rotations on Surface              Surface 
//-----------------------------------------------
{
  { DNone,  DNone,  DNone,  DNone },    // ENone
  { DAbove, DBelow, DEast,  DWest },    // ENorth
  { DAbove, DBelow, DEast,  DWest },    // ESouth    
  { DAbove, DBelow, DNorth, DSouth },   // EEast
  { DAbove, DBelow, DNorth, DSouth },   // EWest
  { DNorth, DSouth, DEast,  DWest },    // EAbove
  { DNorth, DSouth, DEast,  DWest },    // EBelow 
  { DNone,  DNone,  DNone,  DNone }     // ECenter
};

//*--------------------------*
//* C++ default constructor. *
//*--------------------------*

CViewCone::CViewCone()
{  
  // Initialize instance data.

  iDepth = 0;
  
  // Prepare the Outdoor sky objects MapObject.
  
  iSkyObjects.iID       = EnigmaWC::ID::ESkyObjects;
  iSkyObjects.iSurface  = EnigmaWC::Direction::ESouth;
  iSkyObjects.iRotation = EnigmaWC::Direction::EAbove;
  
  iSkyObjects.iLocation.Clear();
   
  iSkyObjects.iState.SetState( TRUE );
  iSkyObjects.iVisibility.SetState( TRUE );
  iSkyObjects.iPresence.SetState( TRUE );

  // Prepare MapObject being used by player.
  
  iUseObject.iID       = EnigmaWC::ID::ENone;
  iUseObject.iSurface  = EnigmaWC::Direction::ENone;
  iUseObject.iRotation = EnigmaWC::Direction::ENone;
  
  iUseObject.iLocation.Clear();
   
  iUseObject.iState.SetState( TRUE );
  iUseObject.iVisibility.SetState( TRUE );
  iUseObject.iPresence.SetState( TRUE );

  return;
}

//*-------------------------------------------------*
//* This method sets the depth of the viewing cone. *
//*-------------------------------------------------*
//* aDepth: Depth of viewing code.                  *
//*-------------------------------------------------*

void CViewCone::SetDepth( gint aDepth )
{
	iDepth      = aDepth;
	iRenderTime = 0;
	return;
}

//*-----------------------------------------------------------------------*
//* This method sets the game map to be viewed.  The game map must be set *
//* before calling any method that will access the map.                   *
//*-----------------------------------------------------------------------*
//* aMap: Shared pointer to game map.                                     *
//*-----------------------------------------------------------------------*

void CViewCone::SetMap( std::shared_ptr<CMap> aMap )
{	
  // Save game map information.
	
  iMap    = aMap; 
  iPlayer = iMap->Players().GetActive();
	return;
}

//*---------------------------------------------------------------*
//* This method gets the time taken to render the previous frame. *
//*---------------------------------------------------------------*

gint64 CViewCone::GetRenderTime()
{
  return iRenderTime;
}

//*-------------------------------------------------------------*
//* This private function adds a translation along a particular *
//* direction to an offset.  It is used for shifting an item    *
//* being used by a player into its proper position.            * 
//*-------------------------------------------------------------*
//* aDirection:   Direction to apply translation.               *
//* aOffset:      Reference to offset for updating.             *
//* aTranslation: Translation amount.                           *
//*-------------------------------------------------------------*       

void TranslateUseObject( EnigmaWC::Direction aSurface,
                         CFinePoint& aOffset,
                         gfloat aTranslation )
{
  switch ( aSurface )
  {  
    case EnigmaWC::Direction::ENorth:
      aOffset.iNorth += aTranslation;
      break;
      
    case EnigmaWC::Direction::ESouth:
      aOffset.iNorth -= aTranslation;
      break;
      
    case EnigmaWC::Direction::EEast:
      aOffset.iEast += aTranslation;
      break;
    
    case EnigmaWC::Direction::EWest:
      aOffset.iEast -= aTranslation;
      break;
    
    case EnigmaWC::Direction::EAbove:
      aOffset.iAbove += aTranslation;
      break;
      
    case EnigmaWC::Direction::EBelow:
      aOffset.iAbove -= aTranslation;
      break; 
     
    default:
      break;
  }
  
  return;
}

//*----------------------------------------------------------------------*
//* This method renders the viewing cone.  The view transition sequencer *
//* is driven by queued rendering requests until the transition is done, *
//* so the transition rate depends on the GTK widget frame clock.        *
//*----------------------------------------------------------------------*

void CViewCone::Render()
{  
  if ( !iMap->GetLoaded() )
  {
    // A game map has not been loaded, there will be no active player.
    // Fill the view with a background colour, but do no further
    // rendering.
	
    glClearColor( KIndoorR, KIndoorG, KIndoorB, -1 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  }
  else
  {
    // Record the current time to determine the view rendering time later.
 
    iRenderTime = g_get_monotonic_time();
    
    // Render a background based on the player's environment.
	
    if ( (*iPlayer).iOutdoor )
    {
      glClearColor( KOutdoorR, KOutdoorG, KOutdoorB, -1 );    
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
      iMeshList.Render( iSkyObjects, *iPlayer );
      glClear( GL_DEPTH_BUFFER_BIT ); 
    }
    else
    {
      glClearColor( KIndoorR, KIndoorG, KIndoorB, -1 );
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    }

    // Layer any Indoor/Outdoor environment regions on the background.
  
    std::list<std::list<CMapObject>::iterator>::iterator MapObject;

    if ( !iEnvironments.empty() )
    {
      // Reverse the Depth buffer range to layer far environment regions on top
      // of near environment regions to create a "tunnel" effect with different
      // environments.
  
      glDepthRange( 1, 0 );
    
      // Clear the Stencil buffer. 
    
      glEnable(GL_STENCIL_TEST);
      glStencilMask(0xFF);
      glClear(GL_STENCIL_BUFFER_BIT);
      glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);  

      gboolean DrawSkyObjects = FALSE;
  
      for ( MapObject = iEnvironments.begin();
            MapObject != iEnvironments.end();
            ++ MapObject )
      {      
        if ( (*(*MapObject)).iID == EnigmaWC::ID::EOutdoor )
        {
          // Set Stencil bits within the Outdoor region, which will allow
          // SkyObjects to be visible within an Outdoor region.
        
          glStencilFunc(GL_ALWAYS, 0x01, 0xFF);     
          iMeshList.Render( *(*MapObject), *iPlayer );
          DrawSkyObjects = TRUE;
        }
        else
        {
          // Clear Stencil bits within the Indoor region, which will prevent
          // SkyObjects from being visible within an Indoor region.
      
          glStencilFunc(GL_ALWAYS, 0x00, 0xFF);
          iMeshList.Render( *(*MapObject), *iPlayer );
        }
      }

      // Return the Depth buffer range to normal, and clear the Depth Buffer.
      // All other objects will then be layered on top of the environment
      // regions.
    
      glDepthRange( 0, 1 );
      glClear( GL_DEPTH_BUFFER_BIT );
    
      // Render the SkyObjects.  The Stencil buffer will restrict rendering
      // to the Outdoor regions.  
    
      if ( DrawSkyObjects )
      {
        glStencilFunc(GL_EQUAL, 1, 0xFF);
        iMeshList.Render( iSkyObjects, *iPlayer );
      }
    
      // Clear the Depth buffer, and disable the Stencil buffer test.  All other
      // objects will then be layered on top of the environment regions.  
    
      glClear( GL_DEPTH_BUFFER_BIT );
      glDisable( GL_STENCIL_TEST );
    }

    // Render all objects in the appropriate object buffer.

    for ( MapObject = iObjects.begin();
          MapObject != iObjects.end();
          ++ MapObject )
    {    
      // Render an object.

      iMeshList.Render( *(*MapObject), *iPlayer );
    }

    std::list<std::list<CMapTeleporter>::iterator>::iterator MapTeleporter;
  
    for ( MapTeleporter = iTeleporters.begin();
          MapTeleporter != iTeleporters.end();
          ++ MapTeleporter )
    {
      // Render a teleporter.

      iMeshList.Render( *(*MapTeleporter), *iPlayer );
    }

    std::list<std::list<CMapItem>::iterator>::iterator MapItem;
    
    for ( MapItem = iItems.begin();
          MapItem != iItems.end();
          ++ MapItem )
    {
      // Render an item if it has not yet been found.

      if ( (*(*MapItem)).iActive )
        iMeshList.Render( *(*MapItem), *iPlayer );
    }

    CFinePoint Offset;

    // Draw all players except the active one.  This player is not visible
    // to itself, and its item use visuals are different.  As well, the
    // map location of the active player is not updated until the end of
    // a transition, so it will not be present in the end view buffer.
    
    std::list<std::list<CMapPlayer>::iterator>::iterator Player;

    for ( Player = iPlayers.begin();
          Player != iPlayers.end();
          ++ Player )
    {
      if ( iPlayer != *Player )
      {
        // Adjust the active player's offset so the other player has its
        // offset applied properly its location.
        
        Offset = (*iPlayer).iOffset;
        (*iPlayer).iOffset -= (*(*Player)).iOffset;        
        iMeshList.Render( *(*Player), *iPlayer );
        (*iPlayer).iOffset = Offset;
        
        if ( (*(*Player)).iInnerTube )
        {
          // The other player is using an InnerTube.  Prepare this object
          // for use in the other player's location.
          
          iUseObject.iID       = EnigmaWC::ID::EInnerTube;
          iUseObject.iLocation = (*(*Player)).iLocation;
          iUseObject.iSurface  = (*(*Player)).iSurface;
          iUseObject.iRotation = (*(*Player)).iRotation;
        
          // Adjust the active player's offset so the InnerTube is placed
          // in the correct position under the other player.
          
          Offset = (*iPlayer).iOffset;
          
          TranslateUseObject( (*(*Player)).iSurface,
                              (*iPlayer).iOffset,
                              KInnerTubeOffset );
                              
          (*iPlayer).iOffset -= (*(*Player)).iOffset;
          iMeshList.Render( iUseObject, *iPlayer );
          (*iPlayer).iOffset = Offset;
        } 
        
        if ( (*(*Player)).iHydrogenBalloon )
        {
          // The other player is using a HydrogenBalloon.  Prepare this object
          // for use in the other player's location.
          
          iUseObject.iID       = EnigmaWC::ID::EHydrogenBalloon;
          iUseObject.iLocation = (*(*Player)).iLocation;
          iUseObject.iSurface  = (*(*Player)).iSurface;
          iUseObject.iRotation = (*(*Player)).iRotation;
        
          // Adjust the active player's offset so the HydrogenBalloon is placed
          // in the correct position in front of the other player.
          
          Offset = (*iPlayer).iOffset;
          
          TranslateUseObject( (*(*Player)).iRotation,
                              (*iPlayer).iOffset,
                              KHydrogenBalloonOffset );
                              
          (*iPlayer).iOffset -= (*(*Player)).iOffset;
          iMeshList.Render( iUseObject, *iPlayer );
          (*iPlayer).iOffset = Offset;
        }      
      }
    }

    // Draw InnerTube use by the active player.

    if ( (*iPlayer).iInnerTube )
    {
      iUseObject.iID       = EnigmaWC::ID::EInnerTube;
      iUseObject.iLocation = (*iPlayer).iLocation;
      iUseObject.iSurface  = (*iPlayer).iSurface;
      iUseObject.iRotation = (*iPlayer).iRotation;
              
      Offset = (*iPlayer).iOffset;
      (*iPlayer).iOffset.Clear();
      
      TranslateUseObject( (*iPlayer).iSurface,
                          (*iPlayer).iOffset,
                          KInnerTubeOffset );
      
      iMeshList.Render( iUseObject, *iPlayer );
      (*iPlayer).iOffset = Offset;
    }

    // Draw HydrogenBalloon use by the active player.

    if ( (*iPlayer).iHydrogenBalloon )
    {
      iUseObject.iID       = EnigmaWC::ID::EHydrogenBalloon;
      iUseObject.iLocation = (*iPlayer).iLocation;
      iUseObject.iSurface  = (*iPlayer).iSurface;
      iUseObject.iRotation = (*iPlayer).iRotation;
              
      Offset = (*iPlayer).iOffset;
      (*iPlayer).iOffset.Clear();
      iMeshList.Render( iUseObject, *iPlayer );
      (*iPlayer).iOffset = Offset;
    }
  }

  // The orientation of Fish objects is advanced after rendering each frame.
  
  iMeshList.Animate();

  // Update the view rendering time.

  iRenderTime = g_get_monotonic_time() - iRenderTime;
  return;
}

//*-----------------------------------------------------------------*
//* This method fills all viewing points with objects from the map. *
//*-----------------------------------------------------------------*
//* aSelect: Selects player view.                                   *
//*-----------------------------------------------------------------*

void CViewCone::Fill( guint8 aSelect )
{
  // Return immediately if the buffer is to remain unchanged.
  
  if ( aSelect == 0 )
    return;
  
  CMapLocation ViewLocation; 
  const gint8* Offsets;
  
  // Update the active player in case this player has changed.
    
  iPlayer = iMap->Players().GetActive();
  
  if ( aSelect == 1 )
  {
    // Select the view from the player's current orientation, and also
    // the matrix for generating map locations from viewing cone offsets.
    
    ViewLocation = (*iPlayer).iLocation;
    Offsets      = KOffsetMatrices[ (int)(*iPlayer).iRotation ];
  }
  else
  {
    // Select the view from the player's next orientation, and also
    // the matrix for generating map locations from viewing cone offsets.
  
    ViewLocation = (*iPlayer).iLocationNext;
    Offsets      = KOffsetMatrices[ (int)(*iPlayer).iRotationNext ];
  } 
  
  // Clear old objects from the buffer.

  iEnvironments.clear();
  iObjects.clear();
  iTeleporters.clear();
  iItems.clear();
  iPlayers.clear();
  
  gint32 MapEast;
  gint32 MapNorth;
  gint32 MapAbove;

  // Fill all ViewPoints with object from the game map.  The closest ViewPoints
  // are filled first (they will be rendered first) in order to fill the OpenGL
  // Z-Buffer quickly during rendering, hopefully improving performance during
  // the rasterization process.

  CMapLocation MapLocation; 
  gint8 ViewFront;
  gint8 ViewRight;
  gint8 ViewUp;

  gint8 Limit = iDepth - 1;

  for ( ViewFront = 0; ViewFront < iDepth; ++ ViewFront )
  {
    for ( ViewRight = -Limit; ViewRight <= Limit; ++ ViewRight )
    {  
      for ( ViewUp = -Limit; ViewUp <= Limit; ++ ViewUp )
      {   
        // Determine the map location of the ViewPoint in map space
        // from the ViewPoint location in the player's view space.
        
        MapEast = (guint32)ViewLocation.iEast
                + ViewRight * Offsets[ 0 ]
                + ViewUp * Offsets[ 1 ]
                + ViewFront * Offsets[ 2 ];

        MapAbove = (guint32)ViewLocation.iAbove
                 + ViewRight * Offsets[ 3 ]
                 + ViewUp * Offsets[ 4 ]
                 + ViewFront * Offsets[ 5 ];
        
        MapNorth = (guint32)ViewLocation.iNorth
                 + ViewRight * Offsets[ 6 ]
                 + ViewUp * Offsets[ 7 ]
                 + ViewFront * Offsets[ 8 ];

	      // Skip all ViewPoints that lie beyond the limits of a guint16
	      // value (outside map boundary limits).

        if  (( MapEast >= 0 ) && ( MapEast <= G_MAXUINT16 )
          && ( MapNorth >= 0 ) && ( MapNorth <= G_MAXUINT16 )
          && ( MapAbove >= 0 ) && ( MapAbove <= G_MAXUINT16 ))
        {
          MapLocation.iEast  = (guint16)MapEast;
          MapLocation.iNorth = (guint16)MapNorth;
          MapLocation.iAbove = (guint16)MapAbove;

          iMap->Objects().Read( MapLocation, iObjects );
          iMap->Teleporters().Read( MapLocation, iTeleporters );
          iMap->Items().Read( MapLocation, iItems );
          iMap->Players().Read( MapLocation, iPlayers );
        }
      }
    }
  }

  std::list<std::list<CMapObject>::iterator>::iterator Object;
  std::list<std::list<CMapObject>::iterator>::iterator NextObject;
  Object = iObjects.begin();
  
  while ( Object != iObjects.end() )
  {
    if  (((*(*Object)).iID == EnigmaWC::ID::EOutdoor )
      || ((*(*Object)).iID == EnigmaWC::ID::EIndoor ))
    {
      // Move Outdoor and Indoor objects into an environment object
      // buffer.  This avoids a search for these objects in a larger object
      // buffer for each view transitions frame.
      
      NextObject = Object;
      ++ NextObject;
      iEnvironments.splice( iEnvironments.end(), iObjects, Object );
      Object = NextObject;
    }
    else if ( (*(*Object)).iID == EnigmaWC::ID::EWaterLayer )
    {
      // Randomize rotation of WaterLayer objects in buffer.  This is done
      // only when the buffer is filled, since randomizing each time a
      // WaterLayer object is drawn during a transition would be visually
      // too busy.
      
      (*(*Object)).iRotation =
        (EnigmaWC::Direction)KRotations[ (int)(*(*Object)).iSurface ]
                                       [ iRandom.get_int_range( 0, 3 ) ];
      ++ Object;
    }
    else
      ++ Object;
  }

  return;
}

//----------------------------------------------------------------------
// This method initializes the ViewCone within an active OpenGL context.
//----------------------------------------------------------------------

void CViewCone::Initialize()
{
	iMeshList.Initialize();
}
