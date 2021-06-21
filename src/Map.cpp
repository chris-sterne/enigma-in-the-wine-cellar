/*------------------------------------*
 * Program: Enigma in the Wine Cellar *
 * Version: 5.0 for Linux OS          *
 * File:    Map.cpp                   *
 * Date:    October 19, 2016          *
 * Author:  Chris Sterne              *
 *------------------------------------*
 * This class manages a game map.     *
 *------------------------------------*/

#include "EnigmaWC.h"
#include "Map.h"
#include "Connection.h"

//*----------------------*
//* Default constructor. *
//*----------------------*

CMap::CMap()
{	
  // Initialize instance data.

  Clear();
  return;
}

//*-----------------------------------------------------------*
//* This method returns a reference to the MapControllerList. *
//*-----------------------------------------------------------*

CMapControllerList& CMap::Controllers()
{
  return iControllers;
}

//*-------------------------------------------------------*
//* This method returns a reference to the MapObjectList. *
//*-------------------------------------------------------*

CMapObjectList& CMap::Objects()
{
  return iObjects;
}

//*-----------------------------------------------------------*
//* This method returns a reference to the MapTeleporterList. *
//*-----------------------------------------------------------*

CMapTeleporterList& CMap::Teleporters()
{
  return iTeleporters;
}

//*-----------------------------------------------------*
//* This method returns a reference to the MapItemList. *
//*-----------------------------------------------------*

CMapItemList& CMap::Items()
{
  return iItems;
}

//*-------------------------------------------------------*
//* This method returns a reference to the MapPlayerList. *
//*-------------------------------------------------------*

CMapPlayerList& CMap::Players()
{
  return iPlayers;
}

//*---------------------------------------------------------*
//* This method returns a reference to the map description. *
//*---------------------------------------------------------*

std::string& CMap::Description()
{
  return iDescription;
}

//*---------------------------------------------------------*
//* This method return the game map to the empty condition. *
//*---------------------------------------------------------*

void CMap::Clear()
{
  // Clear all data storage.

  iFileName.clear();
  iControllers.clear();
  iObjects.Clear();
  iTeleporters.Clear();
  iItems.Clear();
  iPlayers.Clear();
  iDescription.clear();

  // Initialize map variables.
  
  iSavable = FALSE;
  iBounded = FALSE;
  iLowerBounds.Clear();
  iUpperBounds.Clear();

  return;
}

//*----------------------------------------*
//* This method return the game map state. *
//*----------------------------------------*
//* RETURN: TRUE if game map loaded.       *
//*----------------------------------------*

gboolean CMap::GetLoaded()
{	
  // A lack of a game map filename is a sufficient indication
  // of an unloaded map.

  return ( iFileName.size() != 0 );
}

//*----------------------------------------*
//* This method return the Savable state.  *
//*----------------------------------------*
//* RETURN: TRUE if game map can be saved. *
//*----------------------------------------*

gboolean CMap::GetSavable()
{	
  return iSavable;
}

//*--------------------------------------------------------------------*
//* This private function returns the number of bytes in a string line *
//* of a line ending with the newline '\n' or \x00 characters.         *  
//*--------------------------------------------------------------------*
//* RETURN: Line length, or zero if no line is available.              *
//*--------------------------------------------------------------------*

guint FindLine( std::string& aSource, guint aIndex )
{
  // Count the number of characters before the line terminator.
  
  guint Index = aIndex;
  
  while (( Index < aSource.size() )
    && ( aSource.at( Index ) != '\n' )
    && ( aSource.at( Index ) != '\x00' ))
  {
    ++ Index;
  }

  // Include the line terminator in the final count, unless the end  of the
  // source string has been reached.

  if ( Index >= aSource.size() )
    return ( Index - aIndex );
  else
    return ( 1 + Index - aIndex );
}

//*----------------------------------------*
//* Restart a game map from initial state. *
//*----------------------------------------*

void CMap::Restart()
{
  iControllers.Restart();
  iPlayers.Restart();
  iItems.Restart();
  return;
}

//*--------------------------------------------*
//* Load a game map from internal saved state. *
//*--------------------------------------------*

void CMap::Load()
{
  if ( iSavable )
  {
    iControllers.Load();
    iPlayers.Load();
    iItems.Load();
  }
  
  return;
}

//*------------------------------------------*
//* Save a game map to internal saved state. *
//*------------------------------------------*

void CMap::Save()
{
  if ( iSavable )
  {
    iControllers.Save();
    iPlayers.Save();
    iItems.Save();
  }

  return;
}

//*-------------------------------------------------------------*
//* This private function extracts a MapObject from new version *
//* game map data.                                              *
//*-------------------------------------------------------------*
//* aFileData:    Keyvalue stream.                              *
//* aIndex:       KeyValue index.                               *
//* aControllers: Reference to map controller list.             *
//* aObject:      Reference to MapObject.                       *
//* RETURN:       MapObject filled with keyvalue information.   *
//*-------------------------------------------------------------*

void ExtractNewObject( const std::string& aFileData,
                       guint& aIndex,
                       CMapControllerList& aControllers,
                       CMapObject& aObject )
{
  // Clear all Object connections, but leave intact the remaining
  // information from a previous Object.  This allows minimizing
  // the number of keyvalue pairs uses for describing all Objects.
  
  aObject.iSense.Disconnect();
  aObject.iState.Disconnect();
  aObject.iVisibility.Disconnect();
  aObject.iPresence.Disconnect();
 
  aObject.iSense.SetState( FALSE );
  aObject.iState.SetState( TRUE );
  aObject.iVisibility.SetState( TRUE );
  aObject.iPresence.SetState( TRUE );
  
  // Record information from the header keyvalue.
  
  ++ aIndex;
  aObject.iID = (EnigmaWC::ID)aFileData.at( aIndex );
  ++ aIndex;
  
  // Record Object information from the keyvalue stream until an element
  // header or keyvalue sequence terminator is encountered.
 
  EnigmaWC::Key BankState = EnigmaWC::Key::ENone;
  EnigmaWC::Key Key;
  guint8 Value;
  guint16 HighValue;
  gboolean Done = FALSE;
  
  while ((( aFileData.size() - aIndex ) >= 2 ) && !Done )
  {
    Key = (EnigmaWC::Key)aFileData.at( aIndex );
    aIndex ++;

    Value = aFileData.at( aIndex );
    aIndex ++;
    
    switch( Key )
    {
      case EnigmaWC::Key::ESurface:      
        aObject.iSurface = (EnigmaWC::Direction)Value;
        break;
      
      case EnigmaWC::Key::ERotation: 
        aObject.iRotation = (EnigmaWC::Direction)Value;
        break;
      
      case EnigmaWC::Key::EAbove:        
        aObject.iLocation.iAbove = (guint16)Value;
        BankState = Key;
        break;
        
      case EnigmaWC::Key::ENorth:        
        aObject.iLocation.iNorth = (guint16)Value;
        BankState = Key;
        break;
        
      case EnigmaWC::Key::EEast:        
        aObject.iLocation.iEast = (guint16)Value;
        BankState = Key;
        break;
       
      case EnigmaWC::Key::EBank:
        // The value of a previous keyvalue is to have a high byte added.
        // Adding only one extra bank is supported, extending the value
        // to a maximum of 16 bits.
                
        HighValue = (guint16)( Value << 8 );
        
        if ( BankState == EnigmaWC::Key::EAbove )
          aObject.iLocation.iAbove |= HighValue;
        else if ( BankState == EnigmaWC::Key::ENorth )
          aObject.iLocation.iNorth |= HighValue;
        else if ( BankState == EnigmaWC::Key::EEast )
          aObject.iLocation.iEast |= HighValue;
                    
        break;

      case EnigmaWC::Key::ESense:
        // An object Sense connection name follows this keyvalue pair.  Value
        // has the name's length.  Connect the object to a controller.

        aObject.iSense.Connect( aControllers,
                                aFileData.substr( aIndex, Value ));
        aIndex += Value;
        break;
        
      case EnigmaWC::Key::EState:
        // An object State connection name follows this keyvalue pair.  Value
        // has the name's length.  Connect the object to a controller.
        
        aObject.iState.Connect( aControllers,
                                aFileData.substr( aIndex, Value ));
         
        aIndex += Value;
        break;
        
      case EnigmaWC::Key::EVisibility:
        // An object Visibility connection name follows this keyvalue pair.
        // Value has the name's length.  Connect the object to a controller.

        aObject.iVisibility.Connect( aControllers,
                                     aFileData.substr( aIndex, Value ));
        aIndex += Value;
        break;
        
      case EnigmaWC::Key::EPresence:
        // An object Presence connection name follows this keyvalue pair.
        // Value has the name's length.  Connect the object to a controller.
        
        aObject.iPresence.Connect( aControllers,
                                   aFileData.substr( aIndex, Value ));
        
        aIndex += Value;
        break;
      
      case EnigmaWC::Key::EObject:
      case EnigmaWC::Key::ETeleporter:
      case EnigmaWC::Key::EPlayer:
      case EnigmaWC::Key::EItem:
      case EnigmaWC::Key::EDescription:
      case EnigmaWC::Key::EController:
      case EnigmaWC::Key::EEnd:
        // An element keyvalue or array end keyvalue has been encountered.
        // Move the array index back to this keyvalue before exiting.
			
        aIndex -= 2;
        Done = TRUE;
        break;
       
      default:
        break;
    }
  }
  
  return;
}

//*-----------------------------------------------------------------*
//* This private function extracts a MapTeleporter from new version *
//* game map data.                                                  *
//*-----------------------------------------------------------------*
//* aFileData:    Keyvalue stream.                                  *
//* aIndex:       KeyValue index.                                   *
//* aControllers: Reference to map controller list.                 *
//* aObject:      Reference to MapTeleporter.                       *
//* RETURN:       MapTeleporter filled with keyvalue information.   *
//*-----------------------------------------------------------------*

void ExtractNewTeleporter( const std::string& aFileData,
                           guint& aIndex,
                           CMapControllerList& aControllers,
                           CMapTeleporter& aObject )
{
  // Initialize all Object connections.
  
  aObject.iSense.Disconnect();
  aObject.iState.Disconnect();
  aObject.iVisibility.Disconnect();
  aObject.iPresence.Disconnect();
 
  aObject.iSense.SetState( FALSE );
  aObject.iState.SetState( TRUE );
  aObject.iVisibility.SetState( TRUE );
  aObject.iPresence.SetState( TRUE );

  // Clear the Teleporter arrival orientation so it uses the player's
  // current orientation in the game.  The presence of Teleporter arrival
  // keysvalues will change this default orientation.
  
  aObject.iSurfaceArrival  = EnigmaWC::Direction::ENone;
  aObject.iRotationArrival = EnigmaWC::Direction::ENone;

  aObject.iLocationArrival.iEast  = G_MAXUINT16;  
  aObject.iLocationArrival.iNorth = G_MAXUINT16;
  aObject.iLocationArrival.iAbove = G_MAXUINT16;

  // Record information from the header keyvalue.
  
  ++ aIndex;
  aObject.iID = (EnigmaWC::ID)aFileData.at( aIndex );
  ++ aIndex;
  
  // Record Object information from the keyvalue stream until an element
  // header or keyvalue sequence terminator is encountered.
 
  EnigmaWC::Key MemberState = EnigmaWC::Key::ENone;
  EnigmaWC::Key GroupState  = EnigmaWC::Key::ENone;
  EnigmaWC::Key Key;
  guint8 Value;
  guint16 HighValue;
  gboolean Done = FALSE;
  
  while ((( aFileData.size() - aIndex ) >= 2 ) && !Done )
  {
    Key = (EnigmaWC::Key)aFileData.at( aIndex );
    aIndex ++;

    Value = aFileData.at( aIndex );
    aIndex ++;

    switch( Key )
    {
      case EnigmaWC::Key::ESurface:
        // This key indicates a group of Surface values.

        aObject.iSurface = (EnigmaWC::Direction)Value;
        GroupState  = Key;
        MemberState = Key;
        break;
      
      case EnigmaWC::Key::ERotation:
        // This key indicates a group of Rotation values.
     
        aObject.iRotation = (EnigmaWC::Direction)Value;
        GroupState  = Key;
        MemberState = Key;
        break;
      
      case EnigmaWC::Key::EAbove:
        // This key indicates a group of Above location values.
      
        aObject.iLocation.iAbove = (guint16)Value;
        GroupState  = Key;
        MemberState = Key;
        break;
        
      case EnigmaWC::Key::ENorth:
        // This key indicates a group of North location values.
      
        aObject.iLocation.iNorth = (guint16)Value;
        GroupState  = Key;
        MemberState = Key;
        break;
        
      case EnigmaWC::Key::EEast:
        // This key indicates a group of East location values.
    
        aObject.iLocation.iEast = (guint16)Value;
        GroupState  = Key;
        MemberState = Key;
        break;

      case EnigmaWC::Key::EArrival:
        // This is the arrival value member for a group of values.
        // Select the appropriate destination.
        
        if ( GroupState == EnigmaWC::Key::ESurface )
          aObject.iSurfaceArrival = (EnigmaWC::Direction)Value;
        else if ( GroupState == EnigmaWC::Key::ERotation )
          aObject.iRotationArrival = (EnigmaWC::Direction)Value;
        else if ( GroupState == EnigmaWC::Key::EEast )
          aObject.iLocationArrival.iEast = (guint16)Value;
        else if ( GroupState == EnigmaWC::Key::ENorth )
          aObject.iLocationArrival.iNorth = (guint16)Value;
        else if ( GroupState == EnigmaWC::Key::EAbove )
          aObject.iLocationArrival.iAbove = (guint16)Value;
  
        MemberState = Key;
        break;
 
      case EnigmaWC::Key::EBank:
        // The value of a previous keyvalue is to have a high byte added.
        // Adding only one extra bank is supported, extending the value
        // to a maximum of 16 bits.
                
        HighValue = (guint16)( Value << 8 );
        
        if ( GroupState == EnigmaWC::Key::EEast )
        {
          if ( MemberState == EnigmaWC::Key::EEast )
            aObject.iLocation.iEast |= HighValue;
          else if ( MemberState == EnigmaWC::Key::EArrival )
            aObject.iLocationArrival.iEast |= HighValue;
        }
        else if ( GroupState == EnigmaWC::Key::ENorth )
        {
          if ( MemberState == EnigmaWC::Key::ENorth )
            aObject.iLocation.iNorth |= HighValue;
          else if ( MemberState == EnigmaWC::Key::EArrival )
            aObject.iLocationArrival.iNorth |= HighValue;
        }
        else if ( GroupState == EnigmaWC::Key::EAbove )
        {
          if ( MemberState == EnigmaWC::Key::EAbove )
            aObject.iLocation.iAbove |= HighValue;
          else if ( MemberState == EnigmaWC::Key::EArrival )
            aObject.iLocationArrival.iAbove |= HighValue;
        } 
                    
        break;

      case EnigmaWC::Key::ESense:
        // An object Sense connection name follows this keyvalue pair.  Value
        // has the name's length.  Connect the object to a controller.

        aObject.iSense.Connect( aControllers,
                                aFileData.substr( aIndex, Value ));
        aIndex += Value;
        break;
        
      case EnigmaWC::Key::EState:
        // An object State connection name follows this keyvalue pair.  Value
        // has the name's length.  Connect the object to a controller.

        aObject.iState.Connect( aControllers,
                                aFileData.substr( aIndex, Value ));
        aIndex += Value;
        break;
        
      case EnigmaWC::Key::EVisibility:
        // An object Visibility connection name follows this keyvalue pair.
        // Value has the name's length.  Connect the object to a controller.

        aObject.iVisibility.Connect( aControllers,
                                     aFileData.substr( aIndex, Value ));
        aIndex += Value;
        break;
        
      case EnigmaWC::Key::EPresence:
        // An object Presence connection name follows this keyvalue pair.
        // Value has the name's length.  Connect the object to a controller.
        
        aObject.iPresence.Connect( aControllers,
                                   aFileData.substr( aIndex, Value ));
        
        aIndex += Value;
        break;
      
      case EnigmaWC::Key::EObject:
      case EnigmaWC::Key::ETeleporter:
      case EnigmaWC::Key::EPlayer:
      case EnigmaWC::Key::EItem:
      case EnigmaWC::Key::EDescription:
      case EnigmaWC::Key::EController:
      case EnigmaWC::Key::EEnd:
        // An element keyvalue or array end keyvalue has been encountered.
        // Move the array index back to this keyvalue before exiting.
			
        aIndex -= 2;
        Done = TRUE;
      break;
       
      default:
        break;
    }
  }
  
  return;
}

//*-----------------------------------------------------------*
//* This private function extracts a MapItem from new version *
//* game map data.                                            *
//*-----------------------------------------------------------*
//* aFileData:    Keyvalue stream.                            *
//* aIndex:       KeyValue index.                             *
//* aControllers: Reference to map controller list.           *
//* aObject:      Reference to MapItem.                       *
//* aSavable:     Reference to Savable map control.           *
//* RETURN:       MapItem filled with keyvalue information.   *
//*-----------------------------------------------------------*

void ExtractNewItem( const std::string& aFileData,
                     guint& aIndex,
                     CMapControllerList& aControllers,
                     CMapItem& aObject,
                     gboolean& aSavable )
{
  // Clear all Object connections, but leave intact the remaining
  // information from a previous Object.  This allows minimizing
  // the number of keyvalue pairs uses for describing all Objects.
  
  aObject.iSense.Disconnect();
  aObject.iState.Disconnect();
  aObject.iVisibility.Disconnect();
  aObject.iPresence.Disconnect();
 
  aObject.iSense.SetState( FALSE );
  aObject.iState.SetState( TRUE );
  aObject.iVisibility.SetState( TRUE );
  aObject.iPresence.SetState( TRUE );
  
  // Record information from the header keyvalue.
  
  ++ aIndex;
  aObject.iID = (EnigmaWC::ID)aFileData.at( aIndex );
  ++ aIndex;
  
  // Record Object information from the keyvalue stream until an element
  // header or keyvalue sequence terminator is encountered.
 
  EnigmaWC::Key BankState  = EnigmaWC::Key::ENone;
  EnigmaWC::Key GroupState = EnigmaWC::Key::ENone;
  EnigmaWC::Key Key;
  guint8 Value;
  guint16 HighValue;
  gboolean Done = FALSE;
  
  while ((( aFileData.size() - aIndex ) >= 2 ) && !Done )
  {
    Key = (EnigmaWC::Key)aFileData.at( aIndex );
    aIndex ++;

    Value = aFileData.at( aIndex );
    aIndex ++;
    
    switch( Key )
    {
      case EnigmaWC::Key::ESurface:      
        aObject.iSurface = (EnigmaWC::Direction)Value;
        break;
      
      case EnigmaWC::Key::ERotation: 
        aObject.iRotation = (EnigmaWC::Direction)Value;
        break;

      case EnigmaWC::Key::ECategory:
        aObject.iCategory = (EnigmaWC::Category)Value;
        break;

      case EnigmaWC::Key::EEast:
        aObject.iLocation.iEast = (guint16)Value;
        BankState = Key;
        break;

      case EnigmaWC::Key::ENorth:        
        aObject.iLocation.iNorth = (guint16)Value;
        BankState = Key;
        break;
      
      case EnigmaWC::Key::EAbove:        
        aObject.iLocation.iAbove = (guint16)Value;
        BankState = Key;
        break;
      
      case EnigmaWC::Key::EBank:
        // The value of a previous keyvalue is to have a high byte added.
        // Adding only one extra bank is supported, extending the value
        // to a maximum of 16 bits.
                
        HighValue = (guint16)( Value << 8 );
        
        if ( BankState == EnigmaWC::Key::EAbove )
          aObject.iLocation.iAbove |= HighValue;
        else if ( BankState == EnigmaWC::Key::ENorth )
          aObject.iLocation.iNorth |= HighValue;
        else if ( BankState == EnigmaWC::Key::EEast )
          aObject.iLocation.iEast |= HighValue;
                    
        break;

      case EnigmaWC::Key::EActive:
        // This key indicates the arrival of a group of item Active
        // values, with the current value as the first member
        // in the group.
        
        aObject.iActive = (gboolean)Value;
        GroupState     = Key;
        break;
        
      case EnigmaWC::Key::ESelected:
        // This key indicates the arrival of a group of item Selected
        // values, with the current value as the first member
        // in the group.
        
        aObject.iSelected = (gboolean)Value;
        GroupState        = Key;
        break;

      case EnigmaWC::Key::EUsed:
        // This key indicates the arrival of a group of item Used
        // values, with the current value as the first member
        // in the group.
        
        aObject.iUsed = (gboolean)Value;
        GroupState    = Key;
        break;

      case EnigmaWC::Key::EOwner:
        // This key indicates the arrival of a group of item Owner
        // values, with the current value as the first member
        // in the group.
        
        aObject.iOwner = (guint8)Value;
        GroupState     = Key;
        break;
      
      case EnigmaWC::Key::ESaved:
        // This is the Saved value member in a group of values.
        // Select the appropriate destination.
        
        if ( GroupState == EnigmaWC::Key::EActive )
          aObject.iActiveSaved = (gboolean)Value;
        else if ( GroupState == EnigmaWC::Key::ESelected )
          aObject.iSelectedSaved = (gboolean)Value;
        else if ( GroupState == EnigmaWC::Key::EUsed )
          aObject.iUsedSaved = (gboolean)Value;
        else if ( GroupState == EnigmaWC::Key::EOwner )
          aObject.iOwnerSaved = (guint8)Value;
        
        // Set control flag allowing map to be saved.
        
        aSavable = TRUE;
        break;
      
      case EnigmaWC::Key::ERestart:
        // This is the Restart value member in a group of values.
        // Select the appropriate destination.

        if ( GroupState == EnigmaWC::Key::EActive )
          aObject.iActiveRestart = (gboolean)Value;
        else if ( GroupState == EnigmaWC::Key::ESelected )
          aObject.iSelectedRestart = (gboolean)Value;
        else if ( GroupState == EnigmaWC::Key::EUsed )
          aObject.iUsedRestart = (gboolean)Value;        
        else if ( GroupState == EnigmaWC::Key::EOwner )
          aObject.iOwnerRestart = (guint8)Value;
                  
        break;
      
      case EnigmaWC::Key::ESense:
        // An object Sense connection name follows this keyvalue pair.  Value
        // has the name's length.  Connect the object to a controller.

        aObject.iSense.Connect( aControllers,
                                aFileData.substr( aIndex, Value ));
        aIndex += Value;
        break;
        
      case EnigmaWC::Key::EState:
        // An object State connection name follows this keyvalue pair.  Value
        // has the name's length.  Connect the object to a controller.

        aObject.iState.Connect( aControllers,
                                aFileData.substr( aIndex, Value ));
        aIndex += Value;
        break;
        
      case EnigmaWC::Key::EVisibility:
        // An object Visibility connection name follows this keyvalue pair.
        // Value has the name's length.  Connect the object to a controller.

        aObject.iVisibility.Connect( aControllers,
                                     aFileData.substr( aIndex, Value ));
        aIndex += Value;
        break;
        
      case EnigmaWC::Key::EPresence:
        // An object Presence connection name follows this keyvalue pair.
        // Value has the name's length.  Connect the object to a controller.

        aObject.iPresence.Connect( aControllers,
                                   aFileData.substr( aIndex, Value ));
        aIndex += Value;
        break;
      
      case EnigmaWC::Key::EObject:
      case EnigmaWC::Key::ETeleporter:
      case EnigmaWC::Key::EPlayer:
      case EnigmaWC::Key::EItem:
      case EnigmaWC::Key::EDescription:
      case EnigmaWC::Key::EController:
      case EnigmaWC::Key::EEnd:
        // An element keyvalue or array end keyvalue has been encountered.
        // Move the array index back to this keyvalue before exiting.
			
        aIndex -= 2;
        Done = TRUE;
        break;
       
      default:
        break;
    }
  }
  
  return;
}

//*-------------------------------------------------------------*
//* This private function extracts a MapPlayer from new version *
//* game map data.                                              *
//*-------------------------------------------------------------*
//* aFileData:    Keyvalue stream.                              *
//* aIndex:       KeyValue index.                               *
//* aControllers: Reference to map controller list.             *
//* aObject:      Reference to MapPlayer.                       *
//* aSavable:     Reference to Savable map control.             *
//* RETURN:       MapPlayer filled with keyvalue information.   *
//*-------------------------------------------------------------*

void ExtractNewPlayer( const std::string& aFileData,
                       guint& aIndex,
                       CMapControllerList& aControllers,
                       CMapPlayer& aObject,
                       gboolean& aSavable )
{
  // Clear all Object connections, but leave intact the remaining
  // information from a previous Object.  This allows minimizing
  // the number of keyvalue pairs uses for describing all Objects.
  
  aObject.iSense.Disconnect();
  aObject.iState.Disconnect();
  aObject.iVisibility.Disconnect();
  aObject.iPresence.Disconnect();
 
  aObject.iSense.SetState( FALSE );
  aObject.iState.SetState( TRUE );
  aObject.iVisibility.SetState( TRUE );
  aObject.iPresence.SetState( TRUE );
  
  // Record information from the header keyvalue.
  
  ++ aIndex;
  aObject.iID = (EnigmaWC::ID)aFileData.at( aIndex );
  ++ aIndex;
  
  // Record Object information from the keyvalue stream until an element
  // header or keyvalue sequence terminator is encountered.
 
  EnigmaWC::Key GroupState  = EnigmaWC::Key::ENone;
  EnigmaWC::Key MemberState = EnigmaWC::Key::ENone;
  EnigmaWC::Key Key;
  guint8 Value;
  guint16 HighValue;
  gboolean Done = FALSE;
  
  while ((( aFileData.size() - aIndex ) >= 2 ) && !Done )
  {
    Key = (EnigmaWC::Key)aFileData.at( aIndex );
    aIndex ++;

    Value = aFileData.at( aIndex );
    aIndex ++;

    switch( Key )
    {
      case EnigmaWC::Key::ESurface:
        // This key indicates the arrival of a group of object surface
        // values, with the current value as the first member in
        // the group.    
        
        aObject.iSurface = (EnigmaWC::Direction)Value;
        GroupState  = Key;
        MemberState = Key;
        break;
      
      case EnigmaWC::Key::ERotation:
        // This key indicates the arrival of a group of object rotation
        // values, with the current value as the first member in
        // the group.
      
        aObject.iRotation = (EnigmaWC::Direction)Value;
        GroupState  = Key;
        MemberState = Key;
        break;

      case EnigmaWC::Key::EEast:        
        // This key indicates the arrival of a group of object East
        // locations, with the current value as the first member in
        // the group.
        
        aObject.iLocation.iEast = (guint16)Value;
        GroupState  = Key;
        MemberState = Key;
        break;

      case EnigmaWC::Key::ENorth:        
        // This key indicates the arrival of a group of object North
        // locations, with the current value as the first member in
        // the group.
        
        aObject.iLocation.iNorth     = (guint16)Value;
        aObject.iLocationNext.iNorth = (guint16)Value;
        GroupState  = Key;
        MemberState = Key;
        break;
      
      case EnigmaWC::Key::EAbove:
        // This key indicates the arrival of a group of object Above
        // locations, with the current value as the first member in
        // the group.
                
        aObject.iLocation.iAbove = (guint16)Value;
        GroupState  = Key;
        MemberState = Key;
        break;
      
      case EnigmaWC::Key::EBank:
        // The value of a previous keyvalue is to have a high byte added.
        // Adding only one extra bank is supported, extending the value
        // to a maximum of 16 bits.
                
        HighValue = (guint16)( Value << 8 );
        
        if ( GroupState == EnigmaWC::Key::EEast )
        {
          if ( MemberState == EnigmaWC::Key::EEast )
            aObject.iLocation.iEast |= HighValue;
          else if ( MemberState == EnigmaWC::Key::ESaved )
            aObject.iLocationSaved.iEast |= HighValue;
          else if ( MemberState == EnigmaWC::Key::ERestart )
            aObject.iLocationRestart.iEast |= HighValue;
        }
        else if ( GroupState == EnigmaWC::Key::ENorth )
        {
          if ( MemberState == EnigmaWC::Key::ENorth )
            aObject.iLocation.iNorth |= HighValue;
          else if ( MemberState == EnigmaWC::Key::ESaved )
            aObject.iLocationSaved.iNorth |= HighValue;
          else if ( MemberState == EnigmaWC::Key::ERestart )
            aObject.iLocationRestart.iNorth |= HighValue;
        }
        else if ( GroupState == EnigmaWC::Key::EAbove )
        {
          if ( MemberState == EnigmaWC::Key::EAbove )
            aObject.iLocation.iAbove |= HighValue;
          else if ( MemberState == EnigmaWC::Key::ESaved )
            aObject.iLocationSaved.iAbove |= HighValue;
          else if ( MemberState == EnigmaWC::Key::ERestart )
            aObject.iLocationRestart.iAbove |= HighValue;
        }

        break;

      case EnigmaWC::Key::EActive:
        // This key indicates the arrival of a group of Active
        // values, with the current value as the first member
        // in the group.
        
        aObject.iActive = (gboolean)Value;
        GroupState  = Key;
        MemberState = Key;
        break;

      case EnigmaWC::Key::EOutdoor:
        // This key indicates the arrival of a group of Outdoor
        // values, with the current value as the first member
        // in the group.
        
        aObject.iOutdoor = (gboolean)Value;
        GroupState  = Key;
        MemberState = Key;
        break;

      case EnigmaWC::Key::ESaved:
        // This is the Saved value member in a group of values.
        // Select the appropriate destination.
        
        if ( GroupState == EnigmaWC::Key::ESurface )
          aObject.iSurfaceSaved = (EnigmaWC::Direction)Value;
        else if ( GroupState == EnigmaWC::Key::ERotation )
          aObject.iRotationSaved = (EnigmaWC::Direction)Value;
        else if ( GroupState == EnigmaWC::Key::EEast )
          aObject.iLocationSaved.iEast = (guint16)Value;
        else if ( GroupState == EnigmaWC::Key::ENorth )
          aObject.iLocationSaved.iNorth = (guint16)Value;
        else if ( GroupState == EnigmaWC::Key::EAbove )
          aObject.iLocationSaved.iAbove = (guint16)Value;
        else if ( GroupState == EnigmaWC::Key::EActive )
          aObject.iActiveSaved = (gboolean)Value;
        else if ( GroupState == EnigmaWC::Key::EOutdoor )
          aObject.iOutdoorSaved = (gboolean)Value;
        
        MemberState = Key;
        
        // Set control flag allowing map to be saved.
        
        aSavable = TRUE;
        break;
      
      case EnigmaWC::Key::ERestart:
        // This is the Restart value member in a group of values.
        // Select the appropriate destination.
        
        if ( GroupState == EnigmaWC::Key::ESurface )
          aObject.iSurfaceRestart = (EnigmaWC::Direction)Value;
        else if ( GroupState == EnigmaWC::Key::ERotation )
          aObject.iRotationRestart = (EnigmaWC::Direction)Value;
        else if ( GroupState == EnigmaWC::Key::EEast )
          aObject.iLocationRestart.iEast = (guint16)Value;
        else if ( GroupState == EnigmaWC::Key::ENorth )
          aObject.iLocationRestart.iNorth = (guint16)Value;
        else if ( GroupState == EnigmaWC::Key::EAbove )
          aObject.iLocationRestart.iAbove = (guint16)Value;
        else if ( GroupState == EnigmaWC::Key::EActive )
          aObject.iActiveRestart = (gboolean)Value;
        else if ( GroupState == EnigmaWC::Key::EOutdoor )
          aObject.iOutdoorRestart = (gboolean)Value;
        
        MemberState = Key;
        break;
      
      case EnigmaWC::Key::ESense:
        // An object Sense connection name follows this keyvalue pair.  Value
        // has the name's length.  Connect the object to a controller.

        aObject.iSense.Connect( aControllers,
                                aFileData.substr( aIndex, Value ));
        aIndex += Value;
        break;
        
      case EnigmaWC::Key::EState:
        // An object State connection name follows this keyvalue pair.  Value
        // has the name's length.  Connect the object to a controller.

        aObject.iState.Connect( aControllers,
                                aFileData.substr( aIndex, Value ));
        aIndex += Value;
        break;
        
      case EnigmaWC::Key::EVisibility:
        // An object Visibility connection name follows this keyvalue pair.
        // Value has the name's length.  Connect the object to a controller.

        aObject.iVisibility.Connect( aControllers,
                                     aFileData.substr( aIndex, Value ));
        aIndex += Value;
        break;
        
      case EnigmaWC::Key::EPresence:
        // An object Presence connection name follows this keyvalue pair.
        // Value has the name's length.  Connect the object to a controller.

        aObject.iPresence.Connect( aControllers,
                                   aFileData.substr( aIndex, Value ));
        aIndex += Value;
        break;
      
      case EnigmaWC::Key::EObject:
      case EnigmaWC::Key::ETeleporter:
      case EnigmaWC::Key::EPlayer:
      case EnigmaWC::Key::EItem:
      case EnigmaWC::Key::EDescription:
      case EnigmaWC::Key::EController:
      case EnigmaWC::Key::EEnd:
        // An element keyvalue or array end keyvalue has been encountered.
        // Move the array index back to this keyvalue before exiting.
			
        aIndex -= 2;
        Done = TRUE;
      break;
       
      default:
        break;
    }
  }
  
  return;
}

//*---------------------------------------------------------------*
//* This private function extracts a Description from new version *
//* game map data.                                                *
//*---------------------------------------------------------------*
//* aFileData: Keyvalue stream.                                   *
//* aIndex:    KeyValue index.                                    *
//* aObject:   Reference to string buffer.                        *
//* RETURN:    String buffer filled with description text.        *
//*---------------------------------------------------------------*

void ExtractNewDescription( const std::string& aFileData,
                       guint& aIndex,
                       std::string& aDescription )
{
  // Skip over the header keyvalue.  At present, the language value
  // is ignored.
  
  aIndex += 2;
  
  // Record Description information from the keyvalue array until
  // an element header or keyvalue array terminator is encountered.
 
  EnigmaWC::Key BankState = EnigmaWC::Key::ENone;
  EnigmaWC::Key Key;
  guint8 Value;
  guint16 Length = 0;
  gboolean Done  = FALSE;
  
  while ((( aFileData.size() - aIndex ) >= 2 ) && !Done )
  {
    Key = (EnigmaWC::Key)aFileData.at( aIndex );
    aIndex ++;

    Value = aFileData.at( aIndex );
    aIndex ++;

    switch( Key )
    {
      case EnigmaWC::Key::ELength:      
        Length = (guint16)Value;
        BankState = Key;
        break;
    
      case EnigmaWC::Key::EData:
        // The keyvalue is followed by a block of description data.
        
        aDescription = aFileData.substr( aIndex, Length );
        aIndex += Length;
        break;
        
      case EnigmaWC::Key::EBank:
        // The value of a previous keyvalue is to have a high byte added.
        // Adding only one extra bank is supported, extending the value
        // to a maximum of 16 bits.

        if ( BankState == EnigmaWC::Key::ELength )
          Length |= (guint16)( Value << 8 );
                    
        break;
        
      case EnigmaWC::Key::EObject:
      case EnigmaWC::Key::ETeleporter:
      case EnigmaWC::Key::EPlayer:
      case EnigmaWC::Key::EItem:
      case EnigmaWC::Key::EDescription:
      case EnigmaWC::Key::EController:
      case EnigmaWC::Key::EEnd:
        // An element keyvalue or array end keyvalue has been encountered.
        // Move the array index back to this keyvalue before exiting.
			
        aIndex -= 2;
        Done = TRUE;
        break;
        
      default:
        break;
    }
  }
  
  return;
}

//*--------------------------------------------------------------*
//* This private function extracts a Controller from new version *
//* game map data.                                               *
//*--------------------------------------------------------------*
//* aFileData: Keyvalue stream.                                  *
//* aIndex:    KeyValue index.                                   *
//* aObject:   Reference to MapController.                       *
//* RETURN:    MapController filled with keyvalue information.   *
//*--------------------------------------------------------------*

void ExtractNewController( const std::string& aFileData,
                           guint& aIndex,
                           CMapController& aController )
{
  // A controller name follows the new element keyvalue pair.
  // Prepare the controller to receive new information.
 
  ++ aIndex;
  guint8 Value = aFileData.at( aIndex );
  ++ aIndex;
  
  aController.iName = aFileData.substr( aIndex, Value );
  aController.iSignalNames.resize(0);
  aController.iRestartCode.resize(0);
  aController.iMainCode.resize(0);
  
  aIndex += Value;
  
  // Record Controller information from the keyvalue array until
  // an element header or keyvalue array terminator is encountered.
 
  EnigmaWC::Key BankState = EnigmaWC::Key::ENone;
  EnigmaWC::Key DataState = EnigmaWC::Key::ENone;
  EnigmaWC::Key Key;
  guint16 Length = 0;
  gboolean Done  = FALSE;
  
  while ((( aFileData.size() - aIndex ) >= 2 ) && !Done )
  {
    Key = (EnigmaWC::Key)aFileData.at( aIndex );
    aIndex ++;

    Value = aFileData.at( aIndex );
    aIndex ++;

    switch( Key )
    {
      case EnigmaWC::Key::ECode:
      case EnigmaWC::Key::ECurrent:
      case EnigmaWC::Key::ESaved:      
      case EnigmaWC::Key::ERestart:
      case EnigmaWC::Key::ESignal:
        // These keys indicates a controller bytecode block or signal
        // name block.
      
        DataState = Key;
        break;
    
      case EnigmaWC::Key::ELength:      
        Length = (guint16)Value;
        BankState = Key;
        break;
        
      case EnigmaWC::Key::EBank:
        // The value of a previous keyvalue is to have a high byte added.
        // Adding only one extra bank is supported, extending the value
        // to a maximum of 16 bits.

        if ( BankState == EnigmaWC::Key::ELength )
          Length |= (guint16)( Value << 8 );
                    
        break;
    
      case EnigmaWC::Key::EData:
        // The keyvalue is followed by a block of data.

        if ( DataState == EnigmaWC::Key::ECode )
          aController.iMainCode.assign( aFileData, aIndex, Length );
        else if ( DataState == EnigmaWC::Key::ECurrent )
          aController.iCurrentCode.assign( aFileData, aIndex, Length );
        else if ( DataState == EnigmaWC::Key::ESaved )
          aController.iSavedCode.assign( aFileData, aIndex, Length );
        else if ( DataState == EnigmaWC::Key::ERestart )
          aController.iRestartCode.assign( aFileData, aIndex, Length );
        else if ( DataState == EnigmaWC::Key::ESignal )
          aController.iSignalNames.assign( aFileData, aIndex, Length );
        
        // Move the array index over the data block.
        
        aIndex += Length;
        break;
    
      case EnigmaWC::Key::EObject:
      case EnigmaWC::Key::ETeleporter:
      case EnigmaWC::Key::EPlayer:
      case EnigmaWC::Key::EItem:
      case EnigmaWC::Key::EDescription:
      case EnigmaWC::Key::EController:
      case EnigmaWC::Key::EEnd:
        // An element keyvalue or array end keyvalue has been encountered.
        // Move the array index back to this keyvalue before exiting.
			
        aIndex -= 2;
        Done = TRUE;
        break;
        
      default:
        break;
    }
  }
}

//*---------------------------------------------------------------*
//* This private method adjusts the map boundary.  This boundary  *
//* is only as large as is needed for all objects, and is mainly  *
//* a guard against being able to "fall" too far.                 *
//*---------------------------------------------------------------*
//* aLocation: Location of map object to use for adjustment.      *
//*---------------------------------------------------------------*

void CMap::AdjustBoundary( const CMapLocation& aLocation )
{
  if ( iBounded )
  {
    // A boundary has been set, so expand it as needed.
    
    if ( aLocation.iEast > iUpperBounds.iEast )
      iUpperBounds.iEast = aLocation.iEast;
    else if ( aLocation.iEast < iLowerBounds.iEast )
      iLowerBounds.iEast = aLocation.iEast;
      
    if ( aLocation.iNorth > iUpperBounds.iNorth )
      iUpperBounds.iNorth = aLocation.iNorth;
    else if ( aLocation.iNorth < iLowerBounds.iNorth )
      iLowerBounds.iNorth = aLocation.iNorth;
      
    if ( aLocation.iAbove > iUpperBounds.iAbove )
      iUpperBounds.iAbove = aLocation.iAbove;
    else if ( aLocation.iAbove < iLowerBounds.iAbove )
      iLowerBounds.iAbove = aLocation.iAbove;
  }
  else
  {
    // No boundary has been set, so use the provided map location
    // to set both the upper and lower boundaries.  This will result
    // in a boundary with only one map room.
    
    iLowerBounds = aLocation;
    iUpperBounds = aLocation;
    iBounded     = TRUE;
  }
  
  return;
}

//*---------------------------------------------------------------*
//* This method returns a reference to the map's upper boundary.  *
//*---------------------------------------------------------------*

const CMapLocation& CMap::UpperBounds()
{
  return iUpperBounds;
}

//*---------------------------------------------------------------*
//* This method returns a reference to the map's lower boundary.  *
//*---------------------------------------------------------------*

const CMapLocation& CMap::LowerBounds()
{
  return iLowerBounds;
}

//*-------------------------------*
//* Load a game map from a file.  *
//*-------------------------------*
//* aFileName: Game map filename. *
//*-------------------------------*

void CMap::LoadFile( const std::string& aFileName )
{
  // Prepare an empty game map.
	
  Clear();
  
  // Load game map file.  Return if unsuccessful.

  std::string FileData;
  
  try
  {
    FileData = Glib::file_get_contents( aFileName );
  }
    catch( Glib::Error error )
  {
    return;
  }
  
  // Return immediately if the first line of the game map file does not have
  // the proper indentification code.

  if ( !FileData.compare( "ewc\n" ) )
    return;

  // Confirm that the file header has the correct ending.

  guint Index = 0;
  guint Size  = 0;
  guint FoundHeader;
  
  do
  {
    Size        = FindLine( FileData, Index );
    FoundHeader = FileData.compare( Index, Size, "end_header\n" );
    Index += Size;
  }
  while (( Size != 0 ) && ( FoundHeader != 0 ));
  
	// Return immediately if the end of the header was not found.  Otherwise,
  // the index now points to the beginning of a KeyValue array (key and
  // value byte pairs) describing the map.
  
  if ( FoundHeader != 0 )
    return;

  // Initialize a MapObject to receive keyvalue array information.

  CMapObject Object;
  
  Object.iID       = EnigmaWC::ID::ENone;
  Object.iSurface  = EnigmaWC::Direction::ENone;
  Object.iRotation = EnigmaWC::Direction::ENone;

  Object.iLocation.Clear();
  
  // Initialize a MapTeleporter to receive keyvalue array information.

  CMapTeleporter Teleporter;
  
  Teleporter.iID       = EnigmaWC::ID::ENone;
  Teleporter.iSurface  = EnigmaWC::Direction::ENone;
  Teleporter.iRotation = EnigmaWC::Direction::ENone;

  Teleporter.iLocation.Clear();
  
  Teleporter.iSurfaceArrival  = EnigmaWC::Direction::ENone;
  Teleporter.iRotationArrival = EnigmaWC::Direction::ENone;

  Teleporter.iLocationArrival.Clear();
  
  // Initialize a MapItem to receive keyvalue array information.
  
  CMapItem Item;
  
  Item.iID       = EnigmaWC::ID::ENone;
  Item.iSurface  = EnigmaWC::Direction::ENone;
  Item.iRotation = EnigmaWC::Direction::ENone;
  Item.iCategory = EnigmaWC::Category::ENone;

  Item.iLocation.Clear();  
  
  Item.iActive        = FALSE;
  Item.iActiveSaved   = Item.iActive;
  Item.iActiveRestart = Item.iActive;

  Item.iSelected        = FALSE;
  Item.iSelectedSaved   = Item.iSelected;
  Item.iSelectedRestart = Item.iSelected;
  
  Item.iUsed        = FALSE;
  Item.iUsedSaved   = Item.iUsed;
  Item.iUsedRestart = Item.iUsed;

  Item.iOwner        = 0;
  Item.iOwnerSaved   = Item.iOwner;
  Item.iOwnerRestart = Item.iOwner;

  // Initialize a MapPlayer to receive keyvalue array information.

  CMapPlayer Player;
  
  Player.iID           = EnigmaWC::ID::ENone;
  Player.iSurface      = EnigmaWC::Direction::ENone;
  Player.iRotation     = EnigmaWC::Direction::ENone;
  Player.iSurfaceNext  = EnigmaWC::Direction::ENone;
  Player.iRotationNext = EnigmaWC::Direction::ENone;

  Player.iLocation.Clear();  
  Player.iLocationSaved.Clear();
  Player.iLocationRestart.Clear();
  Player.iLocationNext.Clear();

  Player.iNumber = 0;
   
  Player.iActive        = FALSE;
  Player.iActiveSaved   = Player.iActive;
  Player.iActiveRestart = Player.iActive;
    
  Player.iOutdoor        = FALSE;
  Player.iOutdoorSaved   = Player.iOutdoor;
  Player.iOutdoorRestart = Player.iOutdoor;
  
  Player.iContactBits     = 0;
  Player.iAntiContactBits = 0;
  
  // Read information from the keyvalue array.
  
  gboolean ValidData = TRUE;
  gboolean Done      = FALSE;
  EnigmaWC::Key Key;
  guint8 Value;
  
  while ((( FileData.size() - Index ) >= 2 ) && !Done )
  {
    // A complete keyvalue pair is available.  Read the keyvalue,
    // but keep the keyvalue array index on the element header. 
  
    Key   = (EnigmaWC::Key)FileData.at( Index );
    Value = (guint8)FileData.at( Index + 1 );
    
    switch( Key )
    {      
      case EnigmaWC::Key::EObject:
        // An Object element header has been encountered.
        
        ExtractNewObject( FileData, Index, iControllers, Object );
        
        if  (( (int)Object.iID < (int)EnigmaWC::ID::TOTAL )
          && ( (int)Object.iSurface < (int)EnigmaWC::Direction::TOTAL )
          && ( (int)Object.iRotation < (int)EnigmaWC::Direction::TOTAL ))
        {            
          // The MapObject has been filled with valid data.  Add a new
          // object to the list.

          iObjects.push_back( Object );
          AdjustBoundary( Object.iLocation );
        }
        else
        {
          // The MapObject has some invalid data.  Force an exit with an
          // invalid data error.
        
          ValidData = FALSE;
          Done      = TRUE;
        }
          
        break;
        
      case EnigmaWC::Key::ETeleporter:
        // An Object element header has been encountered.
        
        ExtractNewTeleporter( FileData, Index, iControllers, Teleporter );
        
        if  (( (int)Teleporter.iID < (int)EnigmaWC::ID::TOTAL )
          && ( (int)Teleporter.iSurface < (int)EnigmaWC::Direction::TOTAL )
          && ( (int)Teleporter.iRotation < (int)EnigmaWC::Direction::TOTAL )
          && ( (int)Teleporter.iSurfaceArrival < (int)EnigmaWC::Direction::TOTAL )
          && ( (int)Teleporter.iRotationArrival < (int)EnigmaWC::Direction::TOTAL ))
        {            
          // The MapTeleporter has been filled with valid data.  Add a new
          // teleporter to the list.

          iTeleporters.push_back( Teleporter );
          AdjustBoundary( Teleporter.iLocation );
          AdjustBoundary( Teleporter.iLocationArrival );
        }
        else
        {
          // The MapObject has some invalid data.  Force an exit with an
          // invalid data error.
        
          ValidData = FALSE;
          Done      = TRUE;
        }
          
        break;
        
      case EnigmaWC::Key::EItem:
        // An Item element header has been encountered.
        
        ExtractNewItem( FileData, Index, iControllers, Item, iSavable );
        
        if  (( (int)Item.iID < (int)EnigmaWC::ID::TOTAL )
          && ( (int)Item.iSurface < (int)EnigmaWC::Direction::TOTAL )
          && ( (int)Item.iRotation < (int)EnigmaWC::Direction::TOTAL ))
        {            
          // The MapItem has been filled with valid data.  Add a new
          // item to the list.

          iItems.push_back( Item );
          AdjustBoundary( Item.iLocation );
        }
        else
        {
          // The MapItem has some invalid data.  Force an exit with an
          // invalid data error.
        
          ValidData = FALSE;
          Done      = TRUE;
        }
          
        break;
        
      case EnigmaWC::Key::EPlayer:
        // A Player element header has been encountered.
        
        ExtractNewPlayer( FileData, Index, iControllers, Player, iSavable );
        
        if  (( (int)Player.iID < (int)EnigmaWC::ID::TOTAL )
          && ( (int)Player.iSurface < (int)EnigmaWC::Direction::TOTAL )
          && ( (int)Player.iRotation < (int)EnigmaWC::Direction::TOTAL ))
        {            
          // The MapPlayer has been filled with valid data.  Add a new
          // player to the list.

          iPlayers.push_back( Player );
          AdjustBoundary( Player.iLocationRestart );
          ++ Player.iNumber;
        }
        else
        {
          // The MapItem has some invalid data.  Force an exit with an
          // invalid data error.
        
          ValidData = FALSE;
          Done      = TRUE;
        }
          
        break;        

      case EnigmaWC::Key::EDescription:
        // A Description element header has been encountered.
        
        ExtractNewDescription( FileData, Index, iDescription );
        break;
        
      case EnigmaWC::Key::EController:
        // A Controller element header has been encountered.  Create a new
        // controller at the end of the controller list.
        
        iControllers.emplace_back();
        ExtractNewController( FileData, Index, iControllers.back() );
        iControllers.back().Initialize();
        break;
			
			case EnigmaWC::Key::EEnd:
			  // The end of the keyvalue array has been found.  Ensure it has
			  // the correct value, and is last in the keyvalue array.
            
        if (( Value != 0 ) || (( FileData.size() - Index ) != 2 ))
          ValidData = FALSE;
			  
			  Done = TRUE;
			  break;
			
      default:
        // An unrecognized element header has been encountered.  Force
        // an exit with an invalid data error.
        
        ValidData = FALSE;
        Done      = TRUE;
        break;
    }
  }
  
  // If the game map file had valid data, an ending KeyValue, and there is
  // at least one player (requirement for minimal map), save the game map
  // name for use when saving the map in the future.  Otherwise, clear all
  // saved data (game map file may be faulty).
  
  if ( ValidData && !iPlayers.empty() )
    iFileName = aFileName;
  else
    Clear();

  return;
}

//*-------------------------------------------------------------*
//* This private function writes a KeyValue with a 16-bit value *
//* to a buffer.                                                *
//*-------------------------------------------------------------*
//* aBuffer: Destination buffer.                                *
//* aKey:    KeyValue key.                                      *
//* aValue:  16-bit value.                                      *
//*-------------------------------------------------------------*

void WriteKeyValue_16Bit( std::string& aBuffer,
                          EnigmaWC::Key aKey,
                          guint16 aValue )
{
  aBuffer.push_back( (guchar)aKey );
  aBuffer.push_back( (guchar)aValue );

  // Write a bank keyvalue if the level exceeds that for a byte.
			
  if ( aValue > G_MAXUINT8 )
  {
    aBuffer.push_back( (guchar)EnigmaWC::Key::EBank );
    aBuffer.push_back( (guchar)(aValue >> 8) );
  }
  
  return;
}

//*-------------------------------------------------------------*
//* This private function writes a KeyValue with an 8-bit value *
//* to a buffer.                                                *
//*-------------------------------------------------------------*
//* aBuffer: Destination buffer.                                *
//* aKey:    KeyValue key.                                      *
//* aValue:  8-bit value.                                       *
//*-------------------------------------------------------------*

void WriteKeyValue_8Bit( std::string& aBuffer,
                         EnigmaWC::Key aKey,
                         guint8 aValue )
{
  aBuffer.push_back( (guchar)aKey );
  aBuffer.push_back( (guchar)aValue );  
  return;
}

//*--------------------------------------------------------------*
//* This private function writes a KeyValue with a boolean value *
//* to a buffer.                                                 *
//*--------------------------------------------------------------*
//* aBuffer: Destination buffer.                                 *
//* aKey:    KeyValue key.                                       *
//* aValue:  boolean value.                                      *
//*--------------------------------------------------------------*

void WriteKeyValue_Boolean( std::string& aBuffer,
                            EnigmaWC::Key aKey,
                            gboolean aValue )
{
  aBuffer.push_back( (guchar)aKey );
  
  if ( aValue )
    aBuffer.push_back( (guchar)1 );
  else
    aBuffer.push_back( (guchar)0 );
    
  return;
}

//*-------------------------------------------------------------*
//* This private function writes a KeyValue followed by a short *
//* string (255 character maximum) to a buffer.  This function  *
//* is used for new version maps.                               *
//*-------------------------------------------------------------*
//* aBuffer: Destination buffer.                                *
//* aKey:    KeyValue key.                                      *
//* aString: Reference to string to be written.                 *
//*-------------------------------------------------------------*

void WriteKeyValue_String( std::string& aBuffer,
                           EnigmaWC::Key aKey,
                           std::string& aString )
{
  // Write KeyValue if the string is not empty.
  
  if ( aString.size() )
  {
    aBuffer.push_back( (guchar)aKey );
    aBuffer.push_back( (guint8)aString.size() );                          
    aBuffer.append( aString );
  }
    
  return;
}

//*-----------------------------------------------------------*
//* This private function writes KeyValues for all signal     *
//* connections.  This function is used for new version maps. *
//*-----------------------------------------------------------*
//* aBuffer: Destination buffer.                              *
//* aObject: MapObject with connections.                      *
//*-----------------------------------------------------------*

void WriteKeyValue_Connections( std::string& aBuffer,
                                CMapObject& aObject )
{
  // Add a Sense signal connection if present.
  
  if ( aObject.iSense.Name().size() )
  {
    WriteKeyValue_String( aBuffer,
                          EnigmaWC::Key::ESense,
                          aObject.iSense.Name() );
  }
  
  // Add a State signal connection if present.
  
  if ( aObject.iState.Name().size() )
  {
    WriteKeyValue_String( aBuffer,
                          EnigmaWC::Key::EState,
                          aObject.iState.Name() );
  }
  
  // Add a Visibility signal connection if present.
  
  if ( aObject.iVisibility.Name().size() )
  {
    WriteKeyValue_String( aBuffer,
                          EnigmaWC::Key::EVisibility,
                          aObject.iVisibility.Name() );
  }
  
  // Add a Presence signal connection if present.
  
  if ( aObject.iPresence.Name().size() )
  {
    WriteKeyValue_String( aBuffer,
                          EnigmaWC::Key::EPresence,
                          aObject.iPresence.Name() );
  }

  return;
}

//*-------------------------------------------------------------*
//* This private function writes KeyValues for a block of data. *
//* connections.  This function is used for new version maps.   *
//*-------------------------------------------------------------*
//* aBuffer: Destination buffer.                                *
//* aKey:    Header key.                                        *
//* aValue:  Header value.                                      *
//* aData:   Data buffer.                                       *
//*-------------------------------------------------------------*

void WriteKeyValue_Data( std::string& aBuffer,
                         EnigmaWC::Key aKey,
                         guint8 aValue,
                         const std::string& aData )
{
  WriteKeyValue_8Bit( aBuffer,
                      aKey,
                      aValue );  

  WriteKeyValue_16Bit( aBuffer,
                       EnigmaWC::Key::ELength,
                       (guint16)aData.size() );
    
  WriteKeyValue_8Bit( aBuffer,
                      EnigmaWC::Key::EData,
                      0 );
                        
  aBuffer.append( aData );

  return;
}

//*-------------------------------------------------------------*
//* This method saves the game map to a file using the filename *
//* set when the game was last loaded from the file.            *
//*-------------------------------------------------------------*

void CMap::SaveFile()
{
  // Return immediately if no game map is loaded. 

  if ( !GetLoaded() )
    return;

  // Prepare the game map header.

  std::string FileData =
"ewc\n\
format binary_byte 1.0\n\
comment Enigma in the Wine Cellar 3.0 game map, created by EnigmaWC 3.0\n";

  FileData.append( "element object " );
  FileData.append( std::to_string( iObjects.size() ));
  FileData.push_back( '\n' );

  FileData.append( "element player " );
  FileData.append( std::to_string( iPlayers.size() ));
  FileData.push_back( '\n' );

  FileData.append( "element item " );
  FileData.append( std::to_string( iItems.size() ));
  FileData.push_back( '\n' );

  FileData.append( "element description 1" );
  FileData.push_back( '\n' );
  
  FileData.append( "element controller " );
  FileData.append( std::to_string( iControllers.size() ));
  FileData.push_back( '\n' );

  FileData.append( "end_header\n" );

  //*------------------------------------------*
  //* Write KeyValues for all map controllers. *
  //*------------------------------------------*
  
  std::list<CMapController>::iterator Controller;
  
  for ( Controller = iControllers.begin();
        Controller != iControllers.end();
        ++ Controller )
  {
		// Add header for a map controller.
		
		WriteKeyValue_String( FileData,
                          EnigmaWC::Key::EController,
                          (*Controller).iName );
    
    // Write Main bytecode block.

    WriteKeyValue_Data( FileData,
                        EnigmaWC::Key::ECode,
                        0,
                        (*Controller).iMainCode );    

    // Write Current bytecode block.  This block must first be updated
    // from the current state of all map controller signals.
    
    (*Controller).CreateSignalCode( (*Controller).iCurrentCode );
    
    WriteKeyValue_Data( FileData,
                        EnigmaWC::Key::ECurrent,
                        0,
                        (*Controller).iCurrentCode );
    
    // Write Saved bytecode block.
    
    if ( iSavable )
    {    
      WriteKeyValue_Data( FileData,
                          EnigmaWC::Key::ESaved,
                          0,
                          (*Controller).iSavedCode );
    }
    
    // Write Restart bytecode block.
    
    WriteKeyValue_Data( FileData,
                        EnigmaWC::Key::ERestart,
                        0,
                        (*Controller).iRestartCode );
    
    // Add header for a packed array of controller signal names.

    WriteKeyValue_Data( FileData,
                        EnigmaWC::Key::ESignal,
                        0,
                        (*Controller).iSignalNames );
  }

  //*---------------------------------------------*
  //* Write KeyValues for all structural objects. *
  //*---------------------------------------------*

  // Set the initial East, North, and Above values to their maximum,
  // forcing an addition of location keyvalues for the first structural
  // object (the first structural object cannot use the maximum value). 
	
  guint16 East  = G_MAXUINT16; 
  guint16 North = G_MAXUINT16;
  guint16 Above = G_MAXUINT16;
	
  std::list<CMapObject>::iterator Object;

  for ( Object = iObjects.begin(); Object != iObjects.end(); ++ Object )
  {
    // Add the header for a simple object.
		
    WriteKeyValue_8Bit( FileData,
                        EnigmaWC::Key::EObject,
                        (guint8)(*Object).iID );

    // Add a new East keyvalue if an object with a different value
    // has been encountered.
		
    if ( (*Object).iLocation.iEast != East )
    {
      East = (*Object).iLocation.iEast;

      WriteKeyValue_16Bit( FileData,
                           EnigmaWC::Key::EEast,
                           East );
    }

    // Add a new North keyvalue if an object with a different value
    // has been encountered.
		
    if ( (*Object).iLocation.iNorth != North )
    {
      North = (*Object).iLocation.iNorth;
      
      WriteKeyValue_16Bit( FileData,
                           EnigmaWC::Key::ENorth,
                           North );
    }

    // Add a new Above keyvalue if an object with a different value
    // has been encountered.
		
    if ( (*Object).iLocation.iAbove != Above )
    {			
      Above = (*Object).iLocation.iAbove;

      WriteKeyValue_16Bit( FileData,
                           EnigmaWC::Key::EAbove,
                           Above );
    }

    // Add a surface keyvalue.

    WriteKeyValue_8Bit( FileData,
                        EnigmaWC::Key::ESurface,
                        (guint8)(*Object).iSurface );

    // Add a rotation keyvalue.

    WriteKeyValue_8Bit( FileData,
                        EnigmaWC::Key::ERotation,
                        (guint8)(*Object).iRotation );
  
      // Add connection keyvalues.
  
    WriteKeyValue_Connections( FileData, (*Object) );
  }

  //*---------------------------------------------*
  //* Write KeyValues for all teleporter objects. *
  //*---------------------------------------------*

  std::list<CMapTeleporter>::iterator Teleporter;

  for ( Teleporter = iTeleporters.begin();
        Teleporter != iTeleporters.end();
        ++ Teleporter )
	{
		// Add header for a teleporter object.
		
		WriteKeyValue_8Bit( FileData,
                        EnigmaWC::Key::ETeleporter,
                        (guint8)(*Teleporter).iID );
  
    // Write departure surface keyvalue, and add arrival surface
    // if it is not to be the player's current surface.
    
    WriteKeyValue_8Bit( FileData,
                        EnigmaWC::Key::ESurface,
                        (guint8)(*Teleporter).iSurface );
  
    if ( (*Teleporter).iSurfaceArrival != EnigmaWC::Direction::ENone )
    {
      WriteKeyValue_8Bit( FileData,
                          EnigmaWC::Key::EArrival,
                          (guint8)(*Teleporter).iSurfaceArrival );
    }
    
    // Write departure rotation keyvalue, and add arrival rotation
    // if it is not to be the player's current rotation.
    
    WriteKeyValue_8Bit( FileData,
                        EnigmaWC::Key::ERotation,
                        (guint8)(*Teleporter).iRotation );
  
    if ( (*Teleporter).iRotationArrival != EnigmaWC::Direction::ENone )
    {
      WriteKeyValue_8Bit( FileData,
                          EnigmaWC::Key::EArrival,
                          (guint8)(*Teleporter).iRotationArrival );
    }
    
    // Write departure East location keyvalue, and add arrival East location
    // if it is not to be the player's current East location.
  
    WriteKeyValue_16Bit( FileData,
                         EnigmaWC::Key::EEast,
                         (*Teleporter).iLocation.iEast );
                         
    if ( (*Teleporter).iLocationArrival.iEast != G_MAXUINT16 )
    {
      WriteKeyValue_16Bit( FileData,
                           EnigmaWC::Key::EArrival,
                           (*Teleporter).iLocationArrival.iEast );
    }
    
    // Write departure North location keyvalue, and add arrival North location
    // if it is not to be the player's current North location.
  
    WriteKeyValue_16Bit( FileData,
                         EnigmaWC::Key::ENorth,
                         (*Teleporter).iLocation.iNorth );
                         
    if ( (*Teleporter).iLocationArrival.iNorth != G_MAXUINT16 )
    {
      WriteKeyValue_16Bit( FileData,
                           EnigmaWC::Key::EArrival,
                           (*Teleporter).iLocationArrival.iNorth );
    }
    
    // Write departure Above location keyvalue, adding arrival Above location
    // if it is not to be the player's current Above location.
  
    WriteKeyValue_16Bit( FileData,
                         EnigmaWC::Key::EAbove,
                         (*Teleporter).iLocation.iAbove );

    if ( (*Teleporter).iLocationArrival.iAbove != G_MAXUINT16 )
    {           
      WriteKeyValue_16Bit( FileData,
                           EnigmaWC::Key::EArrival,
                           (*Teleporter).iLocationArrival.iAbove );
    }

    // Add connection keyvalues.
  
    WriteKeyValue_Connections( FileData, (*Teleporter) );
  }
  
  //*-----------------------------------------*
  //* Write KeyValues for all player objects. *
  //*-----------------------------------------*
  
  std::list<CMapPlayer>::iterator Player;
  
  for ( Player = iPlayers.begin(); Player != iPlayers.end(); ++ Player )
	{
		// Add header for a player object.
		
		WriteKeyValue_8Bit( FileData,
                        EnigmaWC::Key::EPlayer,
                        (guint8)(*Player).iID );
		
		// Write current, saved, and restart surface keyvalues.
    
    WriteKeyValue_8Bit( FileData,
                        EnigmaWC::Key::ESurface,
                        (guint8)(*Player).iSurface );

    if ( iSavable )
    {
      WriteKeyValue_8Bit( FileData,
                          EnigmaWC::Key::ESaved,
                          (guint8)(*Player).iSurfaceSaved );
    }
                        
    WriteKeyValue_8Bit( FileData,
                        EnigmaWC::Key::ERestart,
                        (guint8)(*Player).iSurfaceRestart );

    // Write current, saved, and restart rotation keyvalues.
    
    WriteKeyValue_8Bit( FileData,
                        EnigmaWC::Key::ERotation,
                        (guint8)(*Player).iRotation );

    if ( iSavable )
    {
      WriteKeyValue_8Bit( FileData,
                          EnigmaWC::Key::ESaved,
                          (guint8)(*Player).iRotationSaved );
    }
                        
    WriteKeyValue_8Bit( FileData,
                        EnigmaWC::Key::ERestart,
                        (guint8)(*Player).iRotationRestart );
				
		// Write current, saved, and restart East locations.
		
    WriteKeyValue_16Bit( FileData,
                         EnigmaWC::Key::EEast,
                         (*Player).iLocation.iEast );

    if ( iSavable )
    {
      WriteKeyValue_16Bit( FileData,
                           EnigmaWC::Key::ESaved,
                           (*Player).iLocationSaved.iEast );
    }
                         
    WriteKeyValue_16Bit( FileData,
                         EnigmaWC::Key::ERestart,
                         (*Player).iLocationRestart.iEast );

		// Write current, saved, and restart North locations.

    WriteKeyValue_16Bit( FileData,
                         EnigmaWC::Key::ENorth,
                         (*Player).iLocation.iNorth );

    if ( iSavable )
    {
      WriteKeyValue_16Bit( FileData,
                           EnigmaWC::Key::ESaved,
                           (*Player).iLocationSaved.iNorth );
    }
                         
    WriteKeyValue_16Bit( FileData,
                         EnigmaWC::Key::ERestart,
                         (*Player).iLocationRestart.iNorth );

    // Write current, saved, and restart Above locations.

    WriteKeyValue_16Bit( FileData,
                         EnigmaWC::Key::EAbove,
                         (*Player).iLocation.iAbove );

    if ( iSavable )
    {
      WriteKeyValue_16Bit( FileData,
                           EnigmaWC::Key::ESaved,
                           (*Player).iLocationSaved.iAbove );
    }
                         
    WriteKeyValue_16Bit( FileData,
                         EnigmaWC::Key::ERestart,
                         (*Player).iLocationRestart.iAbove );

		// Add current, saved, and restart Active state keyvalues.

    WriteKeyValue_Boolean( FileData,
                           EnigmaWC::Key::EActive,
                           (*Player).iActive );
                           
    if ( iSavable )
    {
      WriteKeyValue_Boolean( FileData,
                             EnigmaWC::Key::ESaved,
                             (*Player).iActiveSaved );
    } 

		// Add current, saved, and restart Outdoor state keyvalues.

    WriteKeyValue_Boolean( FileData,
                           EnigmaWC::Key::EOutdoor,
                           (*Player).iOutdoor );
                           
    if ( iSavable )
    {
      WriteKeyValue_Boolean( FileData,
                             EnigmaWC::Key::ESaved,
                             (*Player).iOutdoorSaved );
    } 
                           
    WriteKeyValue_Boolean( FileData,
                           EnigmaWC::Key::ERestart,
                           (*Player).iOutdoorRestart );

    // Add connection keyvalues.
  
    WriteKeyValue_Connections( FileData, (*Player) );
	}

  //*---------------------------------------*
  //* Write KeyValues for all item objects. *
  //*---------------------------------------*

  std::list<CMapItem>::iterator Item;

  for ( Item = iItems.begin(); Item != iItems.end(); ++ Item )
  {
    // Add header for an item object.
		
		WriteKeyValue_8Bit( FileData,
                        EnigmaWC::Key::EItem,
                        (guint8)(*Item).iID );

   // Write surface.

    WriteKeyValue_8Bit( FileData,
                        EnigmaWC::Key::ESurface,
                        (guint8)(*Item).iSurface );
    
    // Write rotation.

    WriteKeyValue_8Bit( FileData,
                        EnigmaWC::Key::ERotation,
                        (guint8)(*Item).iRotation );

    // Write location.
		
    WriteKeyValue_16Bit( FileData,
                         EnigmaWC::Key::EEast,
                         (*Item).iLocation.iEast );

    WriteKeyValue_16Bit( FileData,
                         EnigmaWC::Key::ENorth,
                         (*Item).iLocation.iNorth );

    WriteKeyValue_16Bit( FileData,
                         EnigmaWC::Key::EAbove,
                         (*Item).iLocation.iAbove );

    // Write category.

    WriteKeyValue_8Bit( FileData,
                        EnigmaWC::Key::ECategory,
                        (guint8)(*Item).iCategory );
                        
    // Add current, saved, and restart Active state keyvalues.

    WriteKeyValue_Boolean( FileData,
                           EnigmaWC::Key::EActive,
                           (*Item).iActive );
                           
    if ( iSavable )
    {
      WriteKeyValue_Boolean( FileData,
                             EnigmaWC::Key::ESaved,
                             (*Item).iActiveSaved );
    }
                           
    WriteKeyValue_Boolean( FileData,
                           EnigmaWC::Key::ERestart,
                           (*Item).iActiveRestart );

    // Add current, saved, and restart Selected state keyvalues.

    WriteKeyValue_Boolean( FileData,
                           EnigmaWC::Key::ESelected,
                           (*Item).iSelected );

    if ( iSavable )
    {
      WriteKeyValue_Boolean( FileData,
                             EnigmaWC::Key::ESaved,
                             (*Item).iSelectedSaved );
    }
                           
    WriteKeyValue_Boolean( FileData,
                           EnigmaWC::Key::ERestart,
                           (*Item).iSelectedRestart );

    // Add current, saved, and restart Used state keyvalues.

    WriteKeyValue_Boolean( FileData,
                           EnigmaWC::Key::EUsed,
                           (*Item).iUsed );
                           
    if ( iSavable )
    {
      WriteKeyValue_Boolean( FileData,
                             EnigmaWC::Key::ESaved,
                             (*Item).iUsedSaved );
    }
                           
    WriteKeyValue_Boolean( FileData,
                           EnigmaWC::Key::ERestart,
                           (*Item).iUsedRestart );

    // Add current, saved, and restart owner keyvalues if the item
    // was also found.

    if ( !(*Item).iActive )
    {
      WriteKeyValue_8Bit( FileData,
                          EnigmaWC::Key::EOwner,
                          (*Item).iOwner );
    }
                 
    if ( !(*Item).iActiveSaved && iSavable )
    {         
      WriteKeyValue_8Bit( FileData,
                          EnigmaWC::Key::ESaved,
                          (*Item).iOwnerSaved );
    }
    
    if ( !(*Item).iActiveRestart )
    { 
      WriteKeyValue_8Bit( FileData,
                          EnigmaWC::Key::ERestart,
                          (*Item).iOwnerRestart );
    }
    
    // Add connection keyvalues.
  
    WriteKeyValue_Connections( FileData, (*Item) );
	}

  //*------------------------------------------*
  //* Write KeyValues for the map description. *
  //*------------------------------------------*

  WriteKeyValue_Data( FileData,
                      EnigmaWC::Key::EDescription,
                      (guint8)EnigmaWC::Language::EEnglish,
                      iDescription );

  //*----------------------------------*
  //* Write KeyValue array terminator. *
  //*----------------------------------*

  WriteKeyValue_8Bit( FileData,
                      EnigmaWC::Key::EEnd,
                      0 );

  // Write the file data to the file.

  try
  {
    Glib::file_set_contents( iFileName,
                             FileData.data(),
                             FileData.size() );
  }
    catch( Glib::Error error )
  {
    return;
  }

  return;
}
