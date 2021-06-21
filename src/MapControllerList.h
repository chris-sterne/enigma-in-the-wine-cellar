/*------------------------------------*
 * Program: Enigma in the Wine Cellar *
 * Version: 3.0 for Linux OS          *
 * File:    MapControllerList.h       *
 * Date:    March 23, 2016            *
 * Author:  Chris Sterne              *
 *                                    *
 * MapControllerList class header.    *
 *------------------------------------*/
 
#ifndef __MAPCONTROLLERLIST_H__
#define __MAPCONTROLLERLIST_H__

#include <gtkmm.h>
#include "MapController.h"

class CMapControllerList : public std::list<CMapController>
{
  public:
    // Public methods.

    class CSignalIndex
    {
      public:
        guint16 iController;   // Index to controller.
        guint16 iSignal;       // Index to signal in controller.
    };

    CMapControllerList();
    void Load();
    void Save();
    void Restart();
};

#endif // __MAPCONTROLLERLIST_H__