//*------------------------------------*
//* Program: Enigma in the Wine Cellar *
//* Version: 3.0 for Linux OS          *
//* File:    Connection.h              *
//* Date:    July 22, 2016             *
//* Author:  Chris Sterne              *
//*                                    *
//* Connection class header.           *
//*------------------------------------*
 
#ifndef __CONNECTION_H__
#define __CONNECTION_H__

#include <gtkmm.h>
#include "MapControllerList.h"

class CConnection
{ 
  public:		
    // Public methods.
    
    CConnection();
    gboolean SetState( gboolean aState );
    gboolean GetState() const;
    std::string& Name();
    gboolean Connected();
    void Disconnect();
    void Connect( CMapControllerList& aControllers,
                  const std::string& aName );

  private:
    // Private data.
    
    std::string iName;                                // Connection signal name.
    std::list<CMapController>::iterator iController;  // Controller iterator.
    guint16 iSignal;                                  // Controller signal index.
    gboolean iState;                                  // Unconnected local state.
};

#endif // __CONNECTION_H__