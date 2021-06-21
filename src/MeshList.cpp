//*----------------------------------------------*
//* Program: Enigma in the Wine Cellar           *
//* Version: 5.0 for Linux OS                    *
//* File:    MeshList.h                          *
//* Date:    October 19, 2016                    *
//* Author:  Chris Sterne                        *
//*----------------------------------------------*
//* This class renders image meshes with OpenGL. *
//*----------------------------------------------*

#include "MeshList.h"
#include "Resources.h"

//*---------------------*
//* Local declarations. *
//*---------------------*

#define KPlayerLimit 0.65           // Distance limit from Player center.

#define KLightBeamOffsetX -0.090    // Offset from center of LightBeam mesh.
#define KLightBeamOffsetY  0.220
#define KLightBeamOffsetZ  0.123

#define KLightBeamWidth 0.55        // LightBeam width.

#define KFishOffsetX -0.087         // Offset from center of Fish mesh.
#define KFishOffsetY  0.715
#define KFishOffsetZ  1.213

#define KFishTurn      10           // Fish rotation increment.
#define KFishSwim      5            // Fish swim increment.
#define KFishSwimLeft  0            // Fish movement states.
#define KFishSwimRight 1
#define KFishTurnLeft  2
#define KFishTurnRight 3

#define KWallEyesOffsetX  0.165     // Offset from center of WallEyes mesh.
#define KWallEyesOffsetY  0.010
#define KWallEyesOffsetZ -0.025

#define KXYZ_NE 1
#define KXYZ_NW 2
#define KXYZ_NA 3
#define KXYZ_NB 4
#define KXYZ_SE 5
#define KXYZ_SW 6
#define KXYZ_SA 7
#define KXYZ_SB 8
#define KXYZ_EN 9
#define KXYZ_ES 10
#define KXYZ_EA 11
#define KXYZ_EB 12
#define KXYZ_WN 13
#define KXYZ_WS 14
#define KXYZ_WA 15
#define KXYZ_WB 16
#define KXYZ_AN 17
#define KXYZ_AS 18
#define KXYZ_AE 19
#define KXYZ_AW 20
#define KXYZ_BN 21
#define KXYZ_BS 22
#define KXYZ_BE 23
#define KXYZ_BW 24

// The following two tables are used for transforming an object's translation
// (X, Y, Z in model space) into a translation for the object if it lay only
// in the X-Y plane in the positive Z direction.  This transformation is used
// for applying a rotation to WallEyes, and to limit drawing of a LightBeam
// to a narrow viewing angle.  The first index is the object's surface, and
// the second is the object's rotation on the surface. 

static const guint8 KTransformIndexes[ (int)EnigmaWC::Direction::TOTAL ]
                                     [ (int)EnigmaWC::Direction::TOTAL ] =
{
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, KXYZ_NE, KXYZ_NW, KXYZ_NA, KXYZ_NB, 0 },
  { 0, 0, 0, KXYZ_SE, KXYZ_SW, KXYZ_SA, KXYZ_SB, 0 },
  { 0, KXYZ_EN, KXYZ_ES, 0, 0, KXYZ_EA, KXYZ_EB, 0 },
  { 0, KXYZ_WN, KXYZ_WS, 0, 0, KXYZ_WA, KXYZ_WB, 0 },
  { 0, KXYZ_AN, KXYZ_AS, KXYZ_AE, KXYZ_AW, 0, 0, 0 },
  { 0, KXYZ_BN, KXYZ_BS, KXYZ_BE, KXYZ_BW, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 }
};

static const gint8 KTransformMatrices[25][9] =
{
  {  0,  0,  0,  0,  0,  0,  0,  0,  0 },    // No change.
  {  0, -1,  0, -1,  0,  0,  0,  0,  1 },    // On North rotated East.
  {  0,  1,  0,  1,  0,  0,  0,  0,  1 },    // On North rotated West.
  { -1,  0,  0,  0,  1,  0,  0,  0, -1 },    // On North rotated Above.
  {  1,  0,  0,  0, -1,  0,  0,  0, -1 },    // On North rotated Below.
  {  0, -1,  0,  1,  0,  0,  0,  0,  1 },    // On South rotated East.
  {  0,  1,  0, -1,  0,  0,  0,  0,  1 },    // On South rotated West.
  {  1,  0,  0,  0,  1,  0,  0,  0,  1 },    // On South rotated Above.
  { -1,  0,  0,  0, -1,  0,  0,  0,  1 },    // On South rotated Below.
  {  0, -1,  0,  0,  0, -1,  1,  0,  0 },    // On East rotated North.
  {  0,  1,  0,  0,  0,  1,  1,  0,  0 },    // On East rotated South.
  {  0,  0, -1,  0,  1,  0,  1,  0,  0 },    // On East rotated Above.
  {  0,  0,  1,  0, -1,  0,  1,  0,  0 },    // On East rotated Below.
  {  0,  1,  0,  0,  0, -1, -1,  0,  0 },    // On West rotated North.
  {  0, -1,  0,  0,  0,  1, -1,  0,  0 },    // On West rotated South.
  {  0,  0,  1,  0,  1,  0, -1,  0,  0 },    // On West rotated Above.
  {  0,  0, -1,  0, -1,  0, -1,  0,  0 },    // On West rotated Below.
  {  1,  0,  0,  0,  0, -1,  0,  1,  0 },    // On Above rotated North.
  { -1,  0,  0,  0,  0,  1,  0,  1,  0 },    // On Above rotated South.
  {  0,  0,  1,  1,  0,  0,  0,  1,  0 },    // On Above rotated East.
  {  0,  0, -1, -1,  0,  0,  0,  1,  0 },    // On Above rotated West.
  { -1,  0,  0,  0,  0, -1,  0, -1,  0 },    // On Below rotated North.
  {  1,  0,  0,  0,  0,  1,  0, -1,  0 },    // On Below rotated South.
  {  0,  0, -1,  1,  0,  0,  0, -1,  0 },    // On Below rotated East.
  {  0,  0,  1, -1,  0,  0,  0, -1,  0 }     // On Below rotated West.
};

// This table is used for rotating a viewer on the initial Below surface
// facing North around the East axis onto another surface.  The rotations
// are in units of quarter-turns (+/- 90 degrees) around the positive
// X axis.

static const gint8 KViewerRotateX[ (int)EnigmaWC::Direction::TOTAL ] =
//----------------------------------------------
// .   n   s   e   w   a   b   c    <-- Surface
//----------------------------------------------
{
   0,  1, -1,  0,  0,  0,  0,  0
};

// This table is used for rotating a viewer on a surface around an axis
// perpendicular to its surface.  The rotations are in units of quarter-turns
// (+/- 90 degrees) around the positive Y axis.

static const gint8 KViewerRotateY[ (int)EnigmaWC::Direction::TOTAL ]
                                 [ (int)EnigmaWC::Direction::TOTAL ] =
//----------------------------------------------------------
//  .   n   s   e   w   a   b   c    <-- Rotation on Surface
//----------------------------------------------------------
{  
  { 0,  0,  0,  0,  0,  0,  0,  0 },    // ENone.
  { 0,  0,  0, -1,  1,  0, -2,  0 },    // On ENorth surface.
  { 0,  0,  0, -1,  1,  2,  0,  0 },    // On ESouth surface.  
  { 0,  0, -2,  0,  0, -1,  1,  0 },    // On EEast surface.
  { 0,  0,  2,  0,  0,  1, -1,  0 },    // On EWest surface.
  { 0,  0, -2,  1, -1,  0,  0,  0 },    // On EAbove surface.
  { 0,  0,  2, -1,  1,  0,  0,  0 },    // On EBelow surface.
  { 0,  0,  0,  0,  0,  0,  0,  0 }     // On ECenter surface.
};

// These tables are used for rotating a viewer on the initial Below surface
// facing North around the North axis onto another surface.  The rotations
// are in units of quarter-turns (+/- 90 degrees) around the positive Z axis.

static const gint8 KViewerRotateZ[ (int)EnigmaWC::Direction::TOTAL ] =
//----------------------------------------------
// .   n   s   e   w   a   b   c    <-- Surface
//----------------------------------------------
{
   0,  0,  0,  1, -1, -2,  0,  0
};

// These tables are used for rotating an object on the initial North
// surface facing Above onto another surface.  The rotations are in
// units of quarter-turns (+/- 90 degrees).

static const gint8 KObjectRotateX[ (int)EnigmaWC::Direction::TOTAL ] =
//----------------------------------------------
// .   n   s   e   w   a   b   c    <-- Surface
//----------------------------------------------
{
   0,  0,  0,  0,  0,  1, -1,  0
};

static const gint8 KObjectRotateY[ (int)EnigmaWC::Direction::TOTAL ] =
//----------------------------------------------
// .   n   s   e   w   a   b   c    <-- Surface
//----------------------------------------------
{
   0,  0,  2, -1,  1,  0,  0,  0
};

// This table is used for rotating an object on a surface around
// an axis perpendicular to the surface.

static const gint8 KObjectRotateZ[ (int)EnigmaWC::Direction::TOTAL ]
                                 [ (int)EnigmaWC::Direction::TOTAL ] =

//-------------------------------------------------
//  .   n   s   e   w   a   b   c    <-- Direction
//-------------------------------------------------
{  
  { 0,  0,  0,  0,  0,  0,  0,  0 },    // ENone.
  { 0,  0,  0, -1,  1,  0,  2,  0 },    // On ENorth surface.
  { 0,  0,  0,  1, -1,  0,  2,  0 },    // On ESouth surface.
  { 0,  1, -1,  0,  0,  0,  2,  0 },    // On EEast surface.
  { 0, -1,  1,  0,  0,  0,  2,  0 },    // On EWest surface.
  { 0,  2,  0, -1,  1,  0,  0,  0 },    // On EAbove surface.
  { 0,  0,  2, -1,  1,  0,  0,  0 },    // On EBelow surface.
  { 0,  0,  0,  0,  0,  0,  0,  0 }     // On ECenter surface.
};

// These arrays are used for translating an object from its initial location
// of (0, 0, 0) to its proper location in a room.

static const gint8 KObjectTranslateX[ (int)EnigmaWC::Direction::TOTAL ] =
//--------------------------------------------------------
// .   n   s   e   w   a   b   c    <-- Surface Direction
//--------------------------------------------------------
{
   0,  0,  0,  1, -1,  0,  0,  0
};

static const gint8 KObjectTranslateY[ (int)EnigmaWC::Direction::TOTAL ] =
//--------------------------------------------------------
// .   n   s   e   w   a   b   c    <-- Surface Direction
//--------------------------------------------------------
{
   0,  0,  0,  0,  0,  1, -1,  0
};

static const gint8 KObjectTranslateZ[ (int)EnigmaWC::Direction::TOTAL ] =
//--------------------------------------------------------
// .   n   s   e   w   a   b   c    <-- Surface Direction
//--------------------------------------------------------
{
   0, -1,  1,  0,  0,  0,  0,  0
};

// This array contains the root filenames for the image meshes.  The ordering
// and total number of entries correspond to those in the EnigmaWC::ID list.
// Objects without image meshes have a NULL name.

static const char* KMeshNames[ (int)EnigmaWC::ID::TOTAL ] =
{
  NULL,               // ENone
  "WineCellar",       // EWineCellar
  "BlockWall",        // EBlockWall
  "StoneWall",        // EStoneWall
  "Ladder",           // ELadder
  "LadderTop",        // ELadderTop
  "Player",           // EPlayer
  NULL,               // EBlocker
  NULL,               // EMover
  NULL,               // ETurner
  NULL,               // EFacer
  "Surfacer",         // ESurfacer
  "Mushrooms",        // EMushrooms
  "CorkScrew",        // ECorkScrew
  "WineBottle",       // EWineBottle
  "Bread",            // EBread
  "Cheese",           // ECheese
  "Knife",            // EKnife
  "Apple",            // EApple	
  "WineGlass",        // EWineGlass
  "PlaceMat",         // EPlaceMat
  "Grapes",           // EGrapes
  "Skull",            // ESkull
  "EdgeDrain",        // EEdgeDrain
  "AirVent",          // EAirVent
  "EasterEgg",        // EEasterEgg
  "Flipper",          // EFlipper
  "Edger",            // EEdger
  "Stairs",           // EStairs
  NULL,               // EStairsTop
  NULL,               // ESensor
  "HandHold",         // EHandHold
  "Moss0",            // EMoss0
  "Moss90",           // EMoss90
  "Moss180",          // EMoss180
  "Moss270",          // EMoss270
  "Moss360",          // EMoss360
  "AirBalloon",       // EAirBalloon
  "Plate",            // EPlate
  "Fork",             // EFork
  "Cake",             // ECake
  "StonePost",        // EStonePost
  "StoneButton",      // EStoneButton
  "WallEyes",         // EWallEyes
  "Outdoor",          // EOutdoor
  "Indoor",           // EIndoor
  "SkyObjects",       // ESkyObjects
  "ArchWay",          // EArchWay
  "WoodDoor",         // EWoodDoor
  "WoodWall",         // EWoodWall
  "PullRing",         // EPullRing
  "CubeLock",         // ECubeLock
  "SphereLock",       // ESphereLock
  "CubeKey",          // ECubeKey
  "SphereKey",        // ESphereKey
  "WaterLayer",       // EWaterLayer
  "LightBeam",        // ELightBeam
  "Egg",              // EEgg
  "Web",              // EWeb
  "Orange",           // EOrange
  "Fish",             // EFish
  "WaterBottle",      // EWaterBottle
  "Fertilizer",       // EFertilizer
  "Tree",             // ETree
  NULL,               // ETreeTop
  "CatWalk",          // ECatWalk
  "Teleporter",       // ETeleporter
  "InnerTube",        // EInnerTube
  "HydrogenBalloon",  // EHydrogenBalloon
  "Weed",             // EWeed
  "Fern",             // EFern
  "Vine",             // EVine
  "WaterLock",        // EWaterLock
  "AppleLock",        // EAppleLock
  "OrangeLock",       // EOrangeLock
  "WineLock",         // EWineLock
  "PadButton",        // EPadButton
  "Fence",            // EFence
  "EarthWall",        // EEarthWall
  NULL                // EWater
};

//*----------------------*
//* Default constructor. *
//*----------------------*

CMeshList::CMeshList()
{	
  // Fill the image mesh array with empty meshes.  They will be filled
  // later when needed.

  CMesh EmptyMesh;  
  assign( (int)EnigmaWC::ID::TOTAL, EmptyMesh );

  // Initialize instance values.
  
  iFishSwim  = 0;
  iFishTurn  = 0;
  iFishState = KFishSwimLeft;
  return;
}

//*---------------------------------------------------------*
//* This method renders an object image as a triangle mesh. *
//*---------------------------------------------------------*
//* aObject: Object being viewed.                           *
//* aCamera: Player viewing the object.                     *
//*---------------------------------------------------------*

void CMeshList::Render( const CMapObject& aObject,
                        const CMapPlayer& aViewer )
{
  // Exit immediately if the object is not visible or present.
  
  if ( !aObject.iVisibility.GetState()
    || !aObject.iPresence.GetState() )
  {
    return;
  }
    
  // Exit immediately if the object ID exceeds the mesh and mesh name
  // array size.
 
  guint Index = (guint)aObject.iID;

  if ( Index >= size() )
    return;

  // Exit immediately if the mesh name is NULL (object has no mesh).
  
  if ( KMeshNames[ Index ] == NULL )
    return;
  
  // If necessary, load the mesh data from the resources.

  if  (( at( Index ).iInactive.iVertices.size() == 0 )
    && ( at( Index ).iActive.iVertices.size() == 0 ))
  {    
    CResources Resources;
    Resources.LoadImageMesh( at( Index ), KMeshNames[ Index ] );
  }
  
  // Exit immediately if the object mesh is still empty (possibly due to
  // an error in the resource data).

  if  (( at( Index ).iInactive.iVertices.size() == 0 )
    && ( at( Index ).iActive.iVertices.size() == 0 ))
  {
    return;
  }

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glEnable(GL_CULL_FACE);

  GLfloat TranslateX;
  GLfloat TranslateY;
  GLfloat TranslateZ;
  
  GLfloat RotateX;
  GLfloat RotateY;
  GLfloat RotateZ;
  
  int ObjectSurface  = (int)aObject.iSurface;
  int ObjectRotation = (int)aObject.iRotation;
  int ViewerSurface  = (int)aViewer.iSurface;
  int ViewerRotation = (int)aViewer.iRotation;

  // [ Transformation #6 ]
  // Apply player fine rotation offset.

  glRotatef( (GLfloat)aViewer.iOffset.iRotateEast * -90, 1, 0, 0 );
  glRotatef( (GLfloat)aViewer.iOffset.iRotateAbove * -90, 0, 1, 0 );
  glRotatef( (GLfloat)aViewer.iOffset.iRotateNorth * 90, 0, 0, 1 );

  // [ Transformation #4 ]
  // Rotate the translated view of the object so it appears correctly
  // to a viewer from the viewer's surface.

  RotateX = (GLfloat)KViewerRotateX[ ViewerSurface ] * -90; 
  RotateY = (GLfloat)KViewerRotateY[ ViewerSurface ][ ViewerRotation ] * -90;  
  RotateZ = (GLfloat)KViewerRotateZ[ ViewerSurface ] * -90;

  glRotatef( RotateY, 0, 1, 0 );
  glRotatef( RotateZ, 0, 0, 1 );
  glRotatef( RotateX, 1, 0, 0 );
  
  // [ Transformation #3 ]
  // Translate the object within model space.  SkyObjects remain in a fixed
  // location around the viewer.
  
  if ( aObject.iID != EnigmaWC::ID::ESkyObjects )
  { 
    TranslateX = (GLfloat)KObjectTranslateX[ ObjectSurface ]
               + ((gfloat)aViewer.iLocation.iEast  
               - aObject.iLocation.iEast
               + aViewer.iOffset.iEast ) * -2;
               
    TranslateY = (GLfloat)KObjectTranslateY[ ObjectSurface ]
               + ((gfloat)aViewer.iLocation.iAbove  
               - aObject.iLocation.iAbove
               + aViewer.iOffset.iAbove ) * -2;
               
    TranslateZ = (GLfloat)KObjectTranslateZ[ ObjectSurface ]
               + ((gfloat)aViewer.iLocation.iNorth  
               - aObject.iLocation.iNorth
               + aViewer.iOffset.iNorth ) * 2; 

    glTranslatef( TranslateX, TranslateY, TranslateZ );
  }
  else
  {
    TranslateX = 0;
    TranslateY = 0;
    TranslateZ = 0;
  }
  
  // [ Transformation #2 ]
  // Rotate the view so the object is on its surface.  The object will
  // appear as it should to a viewer on the Below surface facing North.
  
  RotateX = (GLfloat)KObjectRotateX[ ObjectSurface ] * 90;
  RotateY = (GLfloat)KObjectRotateY[ ObjectSurface ] * 90;
          
  glRotatef( RotateX, 1, 0, 0 );
  glRotatef( RotateY, 0, 1, 0 );

  // [ Transformation #1 ]
  // All object meshes are drawn centered at the origin (0, 0, 0) on the
  // model space X-Y plane, rising up along the positive Z axis.  Rotate
  // the view so the object will be oriented properly on its surface.
  
  RotateZ = (GLfloat)KObjectRotateZ[ ObjectSurface ][ ObjectRotation ] * 90;

  glRotatef( RotateZ, 0, 0, 1 );

  if ( aObject.iID == EnigmaWC::ID::EPlayer )
  {
    // Do not draw a Player object if it is too close, since this would
    // show the Player mesh object from inside.

    TranslateX -= (GLfloat)KObjectTranslateX[ ObjectSurface ];
    TranslateY -= (GLfloat)KObjectTranslateY[ ObjectSurface ];
    TranslateZ -= (GLfloat)KObjectTranslateZ[ ObjectSurface ];

    if  (( TranslateX > -KPlayerLimit )
      && ( TranslateX < KPlayerLimit )
      && ( TranslateY > -KPlayerLimit )
      && ( TranslateY < KPlayerLimit )
      && ( TranslateZ > -KPlayerLimit )
      && ( TranslateZ < KPlayerLimit ))
    {
      return;
    }
  }
  else if ( aObject.iID == EnigmaWC::ID::EWallEyes )
  {
    // Rotate a WallEyes so it follows the player.
    
    // [ WallEyes Transformation #2 ]
    // Translate the rotated image mesh back to its original location
    // in the X-Y plane.
    
    glTranslatef( KWallEyesOffsetX, KWallEyesOffsetY, KWallEyesOffsetZ );

    // [ WallEyes Transformation #1 ]
    // Transform the translation values into those for a WallEyes in the
    // X-Y plane (eyes are in-line along the X-axis).Then rotate the image
    // mesh about the X and Y axis so the Walleyes face the viewer.
    
    GLfloat DeltaX;
    GLfloat DeltaY;
    GLfloat DeltaZ;  

    guint8 Index = KTransformIndexes[ ObjectSurface ][ ObjectRotation ];
    
    DeltaX = TranslateX * KTransformMatrices[ Index ][ 0 ]
           + TranslateY * KTransformMatrices[ Index ][ 1 ]
           + TranslateZ * KTransformMatrices[ Index ][ 2 ]
           - KWallEyesOffsetX;

    DeltaY = TranslateX * KTransformMatrices[ Index ][ 3 ]
           + TranslateY * KTransformMatrices[ Index ][ 4 ]
           + TranslateZ * KTransformMatrices[ Index ][ 5 ]
           - KWallEyesOffsetY;
           
    DeltaZ = TranslateX * KTransformMatrices[ Index ][ 6 ]
           + TranslateY * KTransformMatrices[ Index ][ 7 ]
           + TranslateZ * KTransformMatrices[ Index ][ 8 ]
           - KWallEyesOffsetZ;
    
    if ( DeltaZ != 0 )
    {
      RotateX = (GLfloat)atan( DeltaY / DeltaZ ) * ( 180.0 / 3.1416 ); 
      RotateY = (GLfloat)atan( DeltaX / DeltaZ ) * ( 180.0 / 3.1416 );
  
      glRotatef( RotateY, 0, 1, 0 );
      glRotatef( RotateX, 1, 0, 0 );
    }
    
    // [ WallEyes Transformation #0 ]
    // Translate the image mesh so the eyes are centered in the X-Y plane
    // before being rotated.
 
    glTranslatef( -KWallEyesOffsetX, -KWallEyesOffsetY, -KWallEyesOffsetZ );
  }
  else if ( aObject.iID == EnigmaWC::ID::ELightBeam )
  {
    // Do not draw a LightBeam too far out of line with the viewer.
    
    GLfloat DeltaX;
    GLfloat DeltaY;

    guint8 Index = KTransformIndexes[ ObjectSurface ][ ObjectRotation ];
    
    DeltaX = TranslateX * KTransformMatrices[ Index ][ 0 ]
           + TranslateY * KTransformMatrices[ Index ][ 1 ]
           + TranslateZ * KTransformMatrices[ Index ][ 2 ]
           - KLightBeamOffsetX;

    DeltaY = TranslateX * KTransformMatrices[ Index ][ 3 ]
           + TranslateY * KTransformMatrices[ Index ][ 4 ]
           + TranslateZ * KTransformMatrices[ Index ][ 5 ]
           - KLightBeamOffsetY;
           
    if  (( DeltaX > KLightBeamWidth )
      || ( DeltaX < -KLightBeamWidth )
      || ( DeltaY > KLightBeamWidth )
      || ( DeltaY < -KLightBeamWidth ))
    {
      return;
    }
  }
  else if ( aObject.iID == EnigmaWC::ID::EFish )
  {
    // Apply a small rotation to a Fish object so it swims around Viewer.
  
    glRotatef( (GLfloat)iFishSwim, 0, 0, 1 );

    // Apply a small rotation around the center of a Fish object.
  
    glTranslatef( KFishOffsetX, KFishOffsetY, 0 );
    glRotatef( (GLfloat)iFishTurn, 0, 0, 1 );
    glTranslatef( -KFishOffsetX, -KFishOffsetY, 0 );
  }
  else if ( aObject.iID == EnigmaWC::ID::EWaterLayer )
  {    
    // Disable back-face culling for the WaterLayer so the underside will be
    // visible when the player is under it.

    glDisable(GL_CULL_FACE);
  }
  
  // The object is now positioned properly in the viewer's space.
  // Set the object mesh information before drawing.

  if ( aObject.iState.GetState() )
  {
    // Draw active state mesh.
    
    glVertexPointer( 3, GL_FLOAT, 0, at( Index ).iActive.iVertices.data() );
    glColorPointer( 3, GL_FLOAT, 0, at( Index ).iActive.iColours.data() );
	
    glDrawElements( GL_TRIANGLES,
                    at( Index ).iActive.iFaces.size(),
                    GL_UNSIGNED_INT,
                    at( Index ).iActive.iFaces.data() );
  }
  else
  {
    // Draw inactive state mesh.
  
    glVertexPointer( 3, GL_FLOAT, 0, at( Index ).iInactive.iVertices.data() );
    glColorPointer( 3, GL_FLOAT, 0, at( Index ).iInactive.iColours.data() );
	
    glDrawElements( GL_TRIANGLES,
                    at( Index ).iInactive.iFaces.size(),
                    GL_UNSIGNED_INT,
                    at( Index ).iInactive.iFaces.data() );
  }
  
  return;
}

//*--------------------------------------------*
//* This method updates mesh animation values. *
//*--------------------------------------------*

void CMeshList::Animate()
{
  // Advance a Fish object to a new position.
  
  switch ( iFishState )
  {
    case KFishSwimLeft:
      iFishSwim += KFishSwim;
  
      if ( iFishSwim > 360 )
      {
        iFishSwim  = 0;
        iFishTurn  = 0;
        iFishState = KFishTurnRight;
      }
    
      break;
      
    case KFishSwimRight:
      iFishSwim -= KFishSwim;
  
      if ( iFishSwim < -360 )
      {
        iFishSwim  = 0;
        iFishTurn  = -180;
        iFishState = KFishTurnLeft;
      }
    
      break;
      
    case KFishTurnLeft:
      iFishTurn += KFishTurn;
  
      if ( iFishTurn > 0 )
      {
        iFishSwim  = 0;
        iFishTurn  = 0;
        iFishState = KFishSwimLeft;
      }
    
      break;
      
    case KFishTurnRight:
      iFishTurn -= KFishTurn;
  
      if ( iFishTurn < -180 )
      {
        iFishSwim  = 0;
        iFishTurn  = -180;
        iFishState = KFishSwimRight;
      }
      
      break;
        
    default:
      break;
  }

  return;
}