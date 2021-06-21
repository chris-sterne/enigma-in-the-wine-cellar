//*----------------------------------------*
//* Program: Enigma in the Wine Cellar     *
//* Version: 5.0 for Linux OS              *
//* File:    Transition.h                  *
//* Date:    October 20, 2016              *
//* Author:  Chris Stene                   *
//*----------------------------------------*
//* This class describes a transition path *
//* between two player orientations.       *
//*----------------------------------------*

#include "Transition.h"

//*---------------------*
//* Local declarations. *
//*---------------------*

#define KDuration         333000    // Transition duration (microseconds).
#define KMinimumDivision  16000     // Lower limit on transition step time.

#define KStairsOffsetAbove 0.35     // Offsets to avoid Stairs mesh.
#define KStairsOffsetFront 0.20

// This table is used for rotating a viewer around the East axis.

static const gint8 KRotateEast[ (int)EnigmaWC::Direction::TOTAL ]
                              [ (int)EnigmaWC::Direction::TOTAL ] =
//-----------------------------------------------------------
//  .   n   s   e   w   a   b   c    <-- Rotation on Surface.
//-----------------------------------------------------------
{  
  { 0,  0,  0,  0,  0,  0,  0,  0 },    // ENone.
  { 0,  0,  0,  2,  2,  1, -1,  0 },    // On ENorth surface.
  { 0,  0,  0,  0,  0,  1, -1,  0 },    // On ESouth surface.
  { 0,  1, -1,  0,  0,  1, -1,  0 },    // On EEast surface.
  { 0,  1, -1,  0,  0,  1, -1,  0 },    // On EWest surface.
  { 0,  2,  2,  2,  2,  0,  0,  0 },    // On EAbove surface.
  { 0,  0,  0,  0,  0,  0,  0,  0 },    // On EBelow surface.
  { 0,  0,  0,  0,  0,  0,  0,  0 }     // On ECenter surface.
};

// This table is used for rotating a viewer around the Above axis.

static const gint8 KRotateAbove[ (int)EnigmaWC::Direction::TOTAL ]
                               [ (int)EnigmaWC::Direction::TOTAL ] =
//-----------------------------------------------------------
//  .   n   s   e   w   a   b   c    <-- Rotation on Surface.
//-----------------------------------------------------------
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

// This table is used for rotating an viewer around the North axis.

static const gint8 KRotateNorth[ (int)EnigmaWC::Direction::TOTAL ]
                               [ (int)EnigmaWC::Direction::TOTAL ] =
//----------------------------------------------------------
// .   n   s   e   w   a   b   c    <-- Rotation on Surface.
//----------------------------------------------------------
{  
  { 0,  0,  0,  0,  0,  0,  0,  0 },    // ENone.
  { 0,  0,  0,  1, -1,  0,  0,  0 },    // On ENorth surface.
  { 0,  0,  0, -1,  1,  2,  2,  0 },    // On ESouth surface.  
  { 0, -1,  1,  0,  0, -1,  1,  0 },    // On EEast surface.
  { 0,  1, -1,  0,  0,  1, -1,  0 },    // On EWest surface.
  { 0,  2,  2,  2,  2,  0,  0,  0 },    // On EAbove surface.
  { 0,  0,  0,  0,  0,  0,  0,  0 },    // On EBelow surface.
  { 0,  0,  0,  0,  0,  0,  0,  0 }     // On ECenter surface.
};

//*----------------------*
//* Default constructor. *
//*----------------------*

CTransition::CTransition()
{
  Clear();
  return;
}

//*--------------------------------------------------*
//* This method clears all pending transition steps. *
//*--------------------------------------------------*

void CTransition::Clear()
{
  iStep = 0;
  return;
}

//*-------------------------------------------------*
//* This method returns the total transition steps. *
//*-------------------------------------------------*

guint8 CTransition::GetTotal() const
{
  return iTotal;
}

//*-----------------------------------------------------*
//* This method returns the remaining transition steps. *
//*-----------------------------------------------------*

guint8 CTransition::GetStep() const
{
  return iStep;
}

//*------------------------------------------------------------------*
//* This method returns a reference to the current transition point. *
//*------------------------------------------------------------------*

const CFinePoint& CTransition::GetPoint() const
{
  return iPoint;
}

//*--------------------------------------------------------------*
//* This method sets the time duration for each transition step. *
//*--------------------------------------------------------------*

void CTransition::SetDivision( gint64 aDivision )
{  
  iDivision = aDivision; 
  return;
}

//*-------------------------------------------------------------*
//* This private function adds a fine translation to an offset. *
//*-------------------------------------------------------------*
//* aOffset:    Reference to offset for updating.               *
//* aDirection: Direction to apply translation.                 *
//* aTranslate: Fine translation.                               *
//*-------------------------------------------------------------*       

void FineTranslate( CFinePoint& aOffset,
                    EnigmaWC::Direction aDirection,
                    gfloat aTranslate )
{
  switch ( aDirection )
  {  
    case EnigmaWC::Direction::ENorth:
      aOffset.iNorth -= aTranslate;
      break;
      
    case EnigmaWC::Direction::ESouth:
      aOffset.iNorth += aTranslate;
      break;
      
    case EnigmaWC::Direction::EEast:
      aOffset.iEast -= aTranslate;
      break;
    
    case EnigmaWC::Direction::EWest:
      aOffset.iEast += aTranslate;
      break;
    
    case EnigmaWC::Direction::EAbove:
      aOffset.iAbove -= aTranslate;
      break;
      
    case EnigmaWC::Direction::EBelow:
      aOffset.iAbove += aTranslate;
      break; 
     
    default:
      break;
  }
  
  return;
}

//*-------------------------------------------------------------------*
//* This method prepares a transition path based on saved information *
//* about the player's orientation.                                   *
//*-------------------------------------------------------------------*
//* aPlayer: Player undergoing a transition to a new orientation.     *
//*-------------------------------------------------------------------*

void CTransition::Prepare( std::list<CMapPlayer>::iterator aPlayer )
{  
  // Simple transition frames may take much less time to render than the time
  // between the widget's frame clock (1/60 seconds), so the transition
  // ends up taking a very long time.  Set a minimum division time to the
  // total duration is equal to KDuration.
  
  if ( iDivision < KMinimumDivision )
    iDivision = KMinimumDivision;
  
  // Determine the total number of transition steps to make.  The minimum
  // total is 2 steps, which will appear as a single intermediate step
  // between the begin and end orientations.
 
  iTotal = (guint8)( KDuration / iDivision ) + 2;
  iStep  = iTotal;

  // Get information about the begin and end locations.
  
  CMapLocation BeginLocation              = (*aPlayer).iLocation;
  EnigmaWC::Direction BeginSurface        = (*aPlayer).iSurface;
  EnigmaWC::Direction BeginRotation       = (*aPlayer).iRotation;
  EnigmaWC::Direction BeginStairsSurface  = (*aPlayer).iStairsSurface;
  EnigmaWC::Direction BeginStairsRotation = (*aPlayer).iStairsRotation;

  CMapLocation EndLocation               = (*aPlayer).iLocationNext;
  EnigmaWC::Direction EndSurface         = (*aPlayer).iSurfaceNext;
  EnigmaWC::Direction EndRotation        = (*aPlayer).iRotationNext;
  EnigmaWC::Direction EndStairsSurface   = (*aPlayer).iStairsSurfaceNext;
  EnigmaWC::Direction EndStairsRotation  = (*aPlayer).iStairsRotationNext;
  
  // Clear all offsets.
  
  iBegin.Clear();
  iEnd.Clear();
  
  // Plan a translation path to avoid room objects at the begin and end
  // locations.  At present, this only considers Stairs.

  FineTranslate( iBegin, BeginStairsSurface, KStairsOffsetAbove );
  FineTranslate( iBegin, BeginStairsRotation, KStairsOffsetFront );
  
  if ( EndLocation != BeginLocation )
  {
    // The player is moving to a different location.  Determine the relative
    // translation offsets in units of 1 room.

    iEnd.iEast  = EndLocation.iEast - BeginLocation.iEast;
    iEnd.iNorth = EndLocation.iNorth - BeginLocation.iNorth;
    iEnd.iAbove = EndLocation.iAbove - BeginLocation.iAbove;  
    
    FineTranslate( iEnd, EndStairsSurface, KStairsOffsetAbove );
    FineTranslate( iEnd, EndStairsRotation, KStairsOffsetFront );
  }
  else
  {
    // The player is not moving to a different location, so both the begin
    // and end offsets are the same.
    
    iEnd.iEast  = iBegin.iEast;
    iEnd.iNorth = iBegin.iNorth;
    iEnd.iAbove = iBegin.iAbove;
  }
  
  // Determine the rotation offsets in units of 90 degrees.  The result will
  // be a rotation around only one axis, since rotations around multiple axes
  // are visually too complicated.
  
  gint RotateBegin;
  gint RotateEnd;
  gint RotateDelta;
  
  if ( EndSurface == BeginSurface )
  {
    // There is no change to the surface, but the rotation may change.
    // This occurs if the viewer simply turns on a surface.

    RotateBegin = KRotateAbove[ (int)BeginSurface ][ (int)BeginRotation ];
    RotateEnd   = KRotateAbove[ (int)BeginSurface ][ (int)EndRotation ]; 
    RotateDelta = RotateEnd - RotateBegin;
  
    // Change rotational changes of +/- 3 into +/- 1, which is equivalent
    // but requires less visual change between each transition step.
    
    if ( RotateDelta == -3 )
      iEnd.iRotateAbove = (gfloat)1;
    else if ( RotateDelta == 3 )
      iEnd.iRotateAbove = (gfloat)-1;
    else
      iEnd.iRotateAbove = (gfloat)RotateDelta;
  }
  else if ( EndRotation == BeginRotation )
  {
    // There is no change to the rotation, but the surface may change.
    // This occurs if the viewer moves left or right with a viewing
    // direction (rotation) parallel to a map space axis.
  
    RotateBegin = KRotateNorth[ (int)BeginSurface ][ (int)BeginRotation ];
    RotateEnd   = KRotateNorth[ (int)EndSurface ][ (int)BeginRotation ];            
    RotateDelta = RotateEnd - RotateBegin;
  
    // Change rotational changes of +/- 3 into +/- 1, which is equivalent
    // but requires less visual change between each transition step.
    
    if ( RotateDelta == -3 )
      iEnd.iRotateNorth = (gfloat)1;
    else if ( RotateDelta == 3 )
      iEnd.iRotateNorth = (gfloat)-1;
    else
      iEnd.iRotateNorth = (gfloat)RotateDelta;
  }
  else
  {
    // Both the surface and rotation may change.  This occurs if the
    // viewer moves Front or Back with a viewing direction that is
    // towards the Front.

    RotateBegin = KRotateEast[ (int)BeginSurface ][ (int)BeginRotation ];
    RotateEnd   = KRotateEast[ (int)EndSurface ][ (int)EndRotation ]; 
    RotateDelta = RotateEnd - RotateBegin;
  
    // Change rotational changes of +/- 3 into +/- 1, which is equivalent
    // but requires less visual change between each transition step.
    
    if ( RotateDelta == -3 )
      iEnd.iRotateEast = (gfloat)1;
    else if ( RotateDelta == 3 )
      iEnd.iRotateEast = (gfloat)-1;
    else
      iEnd.iRotateEast = (gfloat)RotateDelta;
  }

  // Set the current point to the begin point.

  iPoint = iBegin;
 
  // Calculate a fractional change for all transition variables.
    
  iDelta.iEast        = ( iEnd.iEast - iBegin.iEast ) / iTotal;
  iDelta.iNorth       = ( iEnd.iNorth - iBegin.iNorth ) / iTotal;   
  iDelta.iAbove       = ( iEnd.iAbove - iBegin.iAbove ) / iTotal;
  iDelta.iRotateEast  = ( iEnd.iRotateEast - iBegin.iRotateEast ) / iTotal;
  iDelta.iRotateNorth = ( iEnd.iRotateNorth - iBegin.iRotateNorth ) / iTotal;
  iDelta.iRotateAbove = ( iEnd.iRotateAbove - iBegin.iRotateAbove ) / iTotal;

  return;
}

//*----------------------------------------------------------*
//* This method advances one step along the transition path. *
//*----------------------------------------------------------*

void CTransition::Advance()
{
  if ( iStep > 0 )
  {  
    // Advance the transition variables.

    iPoint.iEast        += iDelta.iEast;
    iPoint.iNorth       += iDelta.iNorth;
    iPoint.iAbove       += iDelta.iAbove;
    iPoint.iRotateEast  += iDelta.iRotateEast;
    iPoint.iRotateNorth += iDelta.iRotateNorth;
    iPoint.iRotateAbove += iDelta.iRotateAbove;
    
    // Decrement the step counter.
  
    -- iStep;
  }

  return;
}