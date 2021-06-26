//*----------------------------------------------*
//* Program: Enigma in the Wine Cellar           *
//* Version: 5.0 for Linux OS                    *
//* File:    MeshList.h                          *
//* Date:    October 19, 2016                    *
//* Author:  Chris Sterne                        *
//*----------------------------------------------*
//* This class renders image meshes with OpenGL. *
//*----------------------------------------------*

#include <iostream>
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>
#include "Matrix4.h"
#include "Resources.h"
#include "MeshList.h"

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

static const std::string vertex_shader_code   = "VertexShader.glsl";
static const std::string fragment_shader_code = "FragmentShader.glsl";

// Perspective projection values.

static const GLfloat NEAR_CLIP     = 1.0f;
static const GLfloat FAR_CLIP      = 12.0f;
static const GLfloat FIELD_OF_VIEW = 60.0f;
static const GLfloat ASPECT_RATIO  = 1.5f;//1.0f;

static const GLfloat CAMERA_OFFSET = -1.0;//-1.15f;  // Camera offset.
static const GLfloat FULL_ROOM     = 2.0f;    // Width of a world room.
static const GLfloat HALF_ROOM     = 1.0f;    // Room center to outer walls

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
  
	ivPosition  = 0;
	ivColour    = 0;
	imTransform = 0;
  
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

	// Initialize object meshes if necessary.

	if ( !at( Index ).iInitialized )
	{
		// Load inactive and active object mesh data from resources.
		
		CResources Resources;
    Resources.LoadImageMesh( at( Index ), KMeshNames[ Index ] );
		
		if ( at( Index ).iInactive.iVertices.size() != 0 )
		{
			// Create server-side buffer object to contain vertex positions,
			// and copy the client-side data to this buffer. 

			glGenBuffers(1, &at( Index ).iInactive.iPositionBO);
			glBindBuffer(GL_ARRAY_BUFFER, at( Index ).iInactive.iPositionBO);
		
			glBufferData(GL_ARRAY_BUFFER,
				           at( Index ).iInactive.iVertices.size() * sizeof(GL_FLOAT),
				           at( Index ).iInactive.iVertices.data(),
				           GL_STATIC_DRAW);
		
			at( Index ).iInactive.iVertices.clear();
		}
		
		if ( at( Index ).iInactive.iColours.size() != 0 )
		{
			glGenBuffers(1, &at( Index ).iInactive.iColourBO);
			glBindBuffer(GL_ARRAY_BUFFER, at( Index ).iInactive.iColourBO);
	
			glBufferData(GL_ARRAY_BUFFER,
	    		         at( Index ).iInactive.iColours.size() * sizeof(GL_FLOAT),
	    		         at( Index ).iInactive.iColours.data(),
	    		         GL_STATIC_DRAW);
		
			at( Index ).iInactive.iColours.clear();
		}
		
		if ( at( Index ).iActive.iVertices.size() != 0 )
		{
			// Create server-side buffer object to contain vertex positions,
			// and copy the client-side data to this buffer. 

			glGenBuffers(1, &at( Index ).iActive.iPositionBO);
			glBindBuffer(GL_ARRAY_BUFFER, at( Index ).iActive.iPositionBO);
		
			glBufferData(GL_ARRAY_BUFFER,
				           at( Index ).iActive.iVertices.size() * sizeof(GL_FLOAT),
				           at( Index ).iActive.iVertices.data(),
				           GL_STATIC_DRAW);
				           
			at( Index ).iActive.iVertices.clear();
		}
		
		if ( at( Index ).iActive.iColours.size() != 0 )
		{
			glGenBuffers(1, &at( Index ).iActive.iColourBO);
			glBindBuffer(GL_ARRAY_BUFFER, at( Index ).iActive.iColourBO);
	
			glBufferData(GL_ARRAY_BUFFER,
	    		         at( Index ).iActive.iColours.size() * sizeof(GL_FLOAT),
	    		         at( Index ).iActive.iColours.data(),
	    		         GL_STATIC_DRAW);
		
			at( Index ).iActive.iColours.clear();
		}
		
		// Set flag to indicate object meshes have been initialized.
		
		at( Index ).iInitialized = true;
	}

	glEnable(GL_CULL_FACE);

	Enigma::Matrix4 matrix;
	matrix.identity();  

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
  }
  else
  {
    TranslateX = 0;
    TranslateY = 0;
    TranslateZ = 0;
  }

	if ( aObject.iID == EnigmaWC::ID::EPlayer )
  {
    // Do not draw a Player object if it is too close, since this would
    // show the Player mesh object from inside.

    GLfloat PlayerX =
    	TranslateX - (GLfloat)KObjectTranslateX[ ObjectSurface ];
    
    GLfloat PlayerY =
    	TranslateY - (GLfloat)KObjectTranslateY[ ObjectSurface ];
    
    GLfloat PlayerZ =
    	TranslateZ - (GLfloat)KObjectTranslateZ[ ObjectSurface ];

    if  (( PlayerX > -KPlayerLimit )
      && ( PlayerX < KPlayerLimit )
      && ( PlayerY > -KPlayerLimit )
      && ( PlayerY < KPlayerLimit )
      && ( PlayerZ > -KPlayerLimit )
      && ( PlayerZ < KPlayerLimit ))
    {
      return;
    }
  }
  else if ( aObject.iID == EnigmaWC::ID::EWallEyes )
  {
    // Rotate a WallEyes so it follows the player.
    // Translate the image mesh so the eyes are centered in the X-Y plane
    // before being rotated.
 
    matrix.translate(-KWallEyesOffsetX, -KWallEyesOffsetY, -KWallEyesOffsetZ);

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
  
  		matrix.rotate_x(RotateX);
  		matrix.rotate_y(RotateY);
    }
    
    // Translate the rotated image mesh back to its original location
    // in the X-Y plane.
    
    matrix.translate(KWallEyesOffsetX, KWallEyesOffsetY, KWallEyesOffsetZ);
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
    // Apply a small rotation around the center of a Fish object.
  
  	matrix.translate(-KFishOffsetX, -KFishOffsetY, 0);
    matrix.rotate_z((GLfloat)iFishTurn);
    matrix.translate(KFishOffsetX, KFishOffsetY, 0);
    
    // Apply a small rotation to a Fish object so it swims around Viewer.
  
    matrix.rotate_z((GLfloat)iFishSwim);
  }
  else if ( aObject.iID == EnigmaWC::ID::EWaterLayer )
  {    
    // Disable back-face culling for the WaterLayer so the underside will be
    // visible when the player is under it.

    glDisable(GL_CULL_FACE);
  }

	// All object meshes are drawn centered at the origin (0, 0, 0) on the
  // model space X-Y plane, rising up along the positive Z axis.  Rotate
  // the view so the object will be oriented properly on its surface.
  
  RotateZ = (GLfloat)KObjectRotateZ[ ObjectSurface ][ ObjectRotation ] * 90;
  matrix.rotate_z(RotateZ);
  
  // Rotate the view so the object is on its surface.  The object will
  // appear as it should to a viewer on the Below surface facing North.
  
  RotateX = (GLfloat)KObjectRotateX[ ObjectSurface ] * 90;
  RotateY = (GLfloat)KObjectRotateY[ ObjectSurface ] * 90;

  matrix.rotate_y(RotateY);          
  matrix.rotate_x(RotateX);

	// Apply all transations.
 	
 	matrix.translate(TranslateX, TranslateY, TranslateZ);

	// Rotate the translated view of the object so it appears correctly
  // to a viewer from the viewer's surface.

  RotateX = (GLfloat)KViewerRotateX[ ViewerSurface ] * -90;
  RotateY = (GLfloat)KViewerRotateY[ ViewerSurface ][ ViewerRotation ] * -90;
  RotateZ = (GLfloat)KViewerRotateZ[ ViewerSurface ] * -90;

  matrix.rotate_x(RotateX);
  matrix.rotate_z(RotateZ);
  matrix.rotate_y(RotateY);   

  // Apply player fine rotation offset.

  matrix.rotate_z((GLfloat)aViewer.iOffset.iRotateNorth * 90);
  matrix.rotate_y((GLfloat)aViewer.iOffset.iRotateAbove * -90); 
  matrix.rotate_x((GLfloat)aViewer.iOffset.iRotateEast * -90);

	// Move camera slightly away from viewer.

  matrix.translate(0, 0, CAMERA_OFFSET);
 	
  // Pass the transformation matrix to the mTransform uniform.

	glUniformMatrix4fv(imTransform, 1, GL_FALSE, matrix.array());
  
  // The object is now positioned properly in the viewer's space.
  // Choose object mesh data based on the object's state.

	GLuint PositionBO = 0;
	GLuint ColourBO   = 0;

	if ( aObject.iState.GetState() )
	{
		PositionBO = at( Index ).iActive.iPositionBO;
		ColourBO   = at( Index ).iActive.iColourBO;
	}
	else
	{
		PositionBO = at( Index ).iInactive.iPositionBO;
		ColourBO   = at( Index ).iInactive.iColourBO;
	}

	if (( PositionBO != 0 ) && ( ColourBO != 0 ))
	{
		// Attach vertex position buffer to the vPosition vertex attribute.

		glBindBuffer(GL_ARRAY_BUFFER, PositionBO);
		glVertexAttribPointer(ivPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
				
		// Calculate the total number of vertices to be rendered.
		
		GLint total;

		glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &total);
		total = total / (sizeof(GL_FLOAT) * 3);

		// Attach vertex colour buffer to the vColour vertex attribute.
		
		glBindBuffer(GL_ARRAY_BUFFER, ColourBO);
		glVertexAttribPointer(ivColour, 4, GL_FLOAT, GL_FALSE, 0, 0);
		
		// Draw the mesh.
		
		glDrawArrays(GL_TRIANGLES, 0, total);
	}
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

//----------------------------------------------------------------------
// This method initializes the MeshList within an active OpenGL context.
//----------------------------------------------------------------------

void CMeshList::Initialize()
{
	// Create a shader program.
	
	GLuint program = glCreateProgram();
	
	// Read vertex shader code from the resource bundle.
	
	Glib::RefPtr<const Glib::Bytes> byte_array;
	GLchar* code;
	gsize size;
	std::string path;

	path = "/org/game/EnigmaWC/Shaders/";
	path += vertex_shader_code;
	
	byte_array =
		Gio::Resource::lookup_data_global(path, Gio::RESOURCE_LOOKUP_FLAGS_NONE);
	
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	code                 = (GLchar*)byte_array->get_data(size);
	
	glShaderSource(vertex_shader, 1, &code, NULL);
	
	// Compile vertex shader code and attach it to the shader program.
	
	glCompileShader(vertex_shader);
	glAttachShader(program, vertex_shader);
	
	// Read fragment shader code from the resource bundle.

	path = "/org/game/EnigmaWC/Shaders/";
	path += fragment_shader_code;

	byte_array =
		Gio::Resource::lookup_data_global(path, Gio::RESOURCE_LOOKUP_FLAGS_NONE);
		                                  
	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	code                   = (GLchar*)byte_array->get_data(size);
	
	glShaderSource(fragment_shader, 1, &code, NULL);
	
	// Compile vertex shader code and attach it to the shader program.
	
	glCompileShader(fragment_shader);
	glAttachShader(program, fragment_shader);

	// Link shader program containing the two compiled shaders, then check	
	// the program link status.  If linking was not successful, output
	// linking log as an error message.
	
	glLinkProgram(program);
	
	GLint status = GL_FALSE;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	
	if (status == GL_FALSE)
	{
		std::cerr << "Failed linking of shader program." << std::endl;
		
		GLint length;
		glGetShaderiv(program, GL_INFO_LOG_LENGTH, &length);

		if (length > 0)
		{
			GLchar log[length];
			glGetProgramInfoLog(program, sizeof(log), NULL, log);
			std::cerr << log << std::endl;
		}
		else
			std::cerr << "No linking log available." << std::endl;
	}

	// The linked program is now ready to use.
	
	glUseProgram(program);
	
	// Record the shader attribute and uniform locations assigned by the linker.
	
	ivPosition   = glGetAttribLocation(program, "vPosition");
	ivColour     = glGetAttribLocation(program, "vColour");
	imTransform  = glGetUniformLocation(program, "mTransform");
	
	// Unbind and delete the shaders since they are no longer needed
	// once the program has been linked.
	
	glDetachShader(program, vertex_shader);
	glDetachShader(program, fragment_shader);
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
		
	// Create and bind a common Vertex Array Object to use when rendering
	// all Mesh objects.
	
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	// Enable all vertex attributes.
	
	glEnableVertexAttribArray(ivPosition);
	glEnableVertexAttribArray(ivColour);
	
	// Set a perspective projection matrix.  This remains constant
	// for all rendering.

	GLint mProjection = glGetUniformLocation(program, "mProjection");	
	
	Enigma::Matrix4 matrix;
	matrix.perspective(FIELD_OF_VIEW, ASPECT_RATIO,
	                   NEAR_CLIP, FAR_CLIP);

	glUniformMatrix4fv(mProjection, 1, GL_FALSE, matrix.array());
}
