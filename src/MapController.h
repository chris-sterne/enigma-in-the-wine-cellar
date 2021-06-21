/*------------------------------------*
 * Program: Enigma in the Wine Cellar *
 * Version: 3.0 for Linux OS          *
 * File:    MapController.h           *
 * Date:    September 4, 2016         *
 * Author:  Chris Sterne              *
 *                                    *
 * MapController class header.        *
 *------------------------------------*/
 
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