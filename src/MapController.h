// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2016, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the MapController class header.  The MapController class
// executes a block of control code. 
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
 
#ifndef __MAPCONTROLLER_H__
#define __MAPCONTROLLER_H__

#include <gtkmm.h>

class CMapController
{
  public:  
    // Public methods.

    CMapController();
    void Initialize();
    guint16 GetSignalIndex( const std::string& aName );
    gboolean SetSignalState( guint16 aIndex, gboolean aState );
    gboolean GetSignalState( guint16 aIndex );
    void Run( const std::string& aCode );
    void CreateSignalCode( std::string& aCode );
    void Load();
    void Save();
    void Restart();
		
		// Public data.
	  
    std::string iName;                // Controller name.
    std::string iSignalNames;         // Packed array of signal names.
    std::string iSignals;             // Array of signal states.
    std::string iCurrentCode;         // Current state bytecode.
    std::string iSavedCode;           // Saved state bytecode.
    std::string iRestartCode;         // Restart state bytecode.
    std::string iMainCode;            // Main bytecode.
    Glib::Rand iRandom;               // Random number generator.
};

#endif // __MAPCONTROLLER_H__
