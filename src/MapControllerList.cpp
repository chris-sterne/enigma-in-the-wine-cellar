/*-----------------------------------------*
 * Program: Enigma in the Wine Cellar Map  *
 * Version: 3.0 for Linux OS               *
 * File:    MapControllerList.cpp          *
 * Date:    March 23, 2016                 *
 * Author:  Chris Sterne                   *
 *                                         *
 * MapControllerList class.                *
 *-----------------------------------------*/

#include "EnigmaWC.h"
#include "MapControllerList.h"

//*----------------------*
//* Default constructor. *
//*----------------------*

CMapControllerList::CMapControllerList()
{
  return;
}

//*-----------------------------------------------------------*
//* This method changes all controllers to their saved state. *
//*-----------------------------------------------------------*

void CMapControllerList::Load()
{
  std::list<CMapController>::iterator Controller;
  
  for ( Controller = begin(); Controller != end(); ++ Controller )
  {
    (*Controller).Load();
  }
  
  return;
}

//*-------------------------------------------------*
//* This method saves the state of all controllers. *
//*-------------------------------------------------*

void CMapControllerList::Save()
{
  std::list<CMapController>::iterator Controller;
  
  for ( Controller = begin(); Controller != end(); ++ Controller )
  {
    (*Controller).Save();
  }
  
  return;
}

//*-------------------------------------------------------------*
//* This method changes all controllers to their restart state. *
//*-------------------------------------------------------------*

void CMapControllerList::Restart()
{
  std::list<CMapController>::iterator Controller;
  
  for ( Controller = begin(); Controller != end(); ++ Controller )
  {
    (*Controller).Restart();
  }
  
  return;
}