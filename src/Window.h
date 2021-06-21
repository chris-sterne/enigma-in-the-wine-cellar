//*------------------------------------*
//* Program: Enigma in the Wine Cellar *
//* Version: 2.0 for Linux OS          *
//* File:    Window.h                  *
//* Date:    January 8, 2016           *
//* Author:  Chris Sterne              *
//*                                    *
//* Window class header.               *
//*------------------------------------*
 
#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <gtkmm.h>
#include "PlayerView.h"

class CWindow : public Gtk::ApplicationWindow
{
  public:
    // Public methods.
    
    CWindow();
};

#endif // __WINDOW_H__