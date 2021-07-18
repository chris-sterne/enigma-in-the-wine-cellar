// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2016, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the Connection class header.  The Connection class handles
// a connection to a map controller.
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
