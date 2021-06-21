//*------------------------------------------------------*
//* Program: Enigma in the Wine Cellar                   *
//* Version: 5.0.1 for Linux OS                          *
//* File:    Connection.cpp                              *
//* Date:    November 21, 2016                           *
//* Author:  Chris Sterne                                *
//*------------------------------------------------------*
//* This class handles a connection to a map controller. *
//*------------------------------------------------------*

#include "Connection.h"

//*----------------------*
//* Default constructor. *
//*----------------------*

CConnection::CConnection()
{
  // Begin new connections as disconnected.
  
  Disconnect();
  return;
}

//*--------------------------------------------------------------------------*
//* This method establishes a connection to a fully-qualified map controller *
//* signal name (eg. "Controller:Signal").  Even if the connection could not *
//* be established, the signal name is saved for later retrieval when the    *
//* map is saved.                                                            *
//*--------------------------------------------------------------------------*
//* aControllers: List of map controllers.                                   *
//* aName:        Signal name.                                               *
//*--------------------------------------------------------------------------*

void CConnection::Connect( CMapControllerList& aControllers,
                           const std::string& aName )
{
  Disconnect();
  iName = aName;
  
  // Locate the character separating the controller and signal name.
  
  std::string::size_type Separator = aName.find( ':', 0 );

  if ( Separator == std::string::npos )
  {
    // The string name does not have a separator character.  If the name
    // consists only of the "T" character, set the local state to TRUE.
    // Otherwise, set the local state to FALSE (name either consists of
    // only the "F" character, or is missing its map controller name).
    
    if ( aName == "T" )
      iState = TRUE;
  }
  else
  {
    // The string name does have a separator character.  Search for the
    // controller name in the list.

    std::string String = aName.substr( 0, Separator ); 

    for ( iController = aControllers.begin();
          iController != aControllers.end();
          iController ++ )
    {
      if ( String == (*iController).iName )
        break;
    }
    
    if ( iController != aControllers.end() )
    {
      // The named map controller was found, and an iterator to it has been
      // stored.  Now find the named signal in this map controller.
      
      iSignal = (*iController).GetSignalIndex( aName.substr( Separator + 1,
                                               aName.size() - Separator )); 

      // If the signal name could not be found in the controller, the signal
      // index will be G_MAXUINT16 to indicate an unconnected connection.
    }
  }
  
  return;
}

//*---------------------------------------------------------------*
//* This method clears the signal name and returns the connection *
//* to a disconnected state.                                      *
//*---------------------------------------------------------------*

void CConnection::Disconnect()
{
  iName.clear();
  iSignal = G_MAXUINT16;
  iState  = FALSE;
  return;
}

//*----------------------------------------------------------------------*
//* This method returns TRUE if the signal is connected to a controller. *
//*----------------------------------------------------------------------*

gboolean CConnection::Connected()
{
  return ( iSignal != G_MAXUINT16 );
}

//*----------------------------------------------------------------*
//* This method returns a reference to the connection signal name. *
//*----------------------------------------------------------------*

std::string& CConnection::Name()
{
  return iName;
}

//*------------------------------------------------------*
//* This method sets the state of the connection signal. *
//*------------------------------------------------------*
//* aState: Connection state to be set.                  *
//* RETURN: TRUE if the signal state changed.            *
//*------------------------------------------------------*

gboolean CConnection::SetState( gboolean aState )
{
  gboolean Change;

  if ( iSignal != G_MAXUINT16 )
  {
    // The connection is connected to a controller.  Set the signal
    // state in the controller, which will return TRUE if the signal
    // state changed.
    
    Change = (*iController).SetSignalState( iSignal, aState );
  }
  else
  {
    // The connection is not connect to a controller.  Set the local
    // state and return TRUE if the signal state changed.
    
    Change = ( iState != aState );
    iState = aState;
  }
  
  return Change;
}

//*---------------------------------------------------------*
//* This method returns the state of the connection signal. *
//*---------------------------------------------------------*

gboolean CConnection::GetState() const
{
  // If the connection is connected to a controller, return the signal
  // state from the controller.  Otherwise, return the local state.

  if ( iSignal != G_MAXUINT16 )
    return (*iController).GetSignalState( iSignal );
  else
    return iState;
}
