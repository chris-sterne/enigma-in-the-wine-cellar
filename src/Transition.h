//*------------------------------------*
//* Program: Enigma in the Wine Cellar *
//* Version: 5.0 for Linux OS          *
//* File:    Transition.h              *
//* Date:    October 16, 2016          *
//* Author:  Chris Sterne              *
//*                                    *
//* Transition class header.           *
//*------------------------------------*
 
#ifndef __TRANSITION_H__
#define __TRANSITION_H__

#include <gtkmm.h>
#include "EnigmaWC.h"
#include "Map.h"
#include "FinePoint.h"

class CTransition
{
  public:    
    // Public methods.

    CTransition();
    guint8 GetTotal() const;
    guint8 GetStep() const;
    void SetDivision( gint64 aDivision );
    const CFinePoint& GetPoint() const;
    void Prepare( std::list<CMapPlayer>::iterator aPlayer );
    void Advance();
    void Clear();
   
  private:
    // Private data.

    gint64 iDivision;                    // Time duration of each step.
    guint8 iTotal;                       // Total transition steps.
    guint8 iStep;                        // Current transition step.    
    CFinePoint iBegin;                   // Path begin point.
    CFinePoint iEnd;                     // Path end point.
    CFinePoint iDelta;                   // Path point change.
    CFinePoint iPoint;                   // Current path point.
};

#endif // __TRANSITION_H__