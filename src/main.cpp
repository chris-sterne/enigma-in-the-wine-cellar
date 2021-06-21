//*------------------------------------------------*
//* Program: Enigma in the Wine Cellar             *
//* Version: 2.0 for Linux OS                      *
//* File:    main.cpp                              *
//* Date:    January 7, 2016                       *
//* Author:  Chris Sterne                          *
//*------------------------------------------------*
//* Main entry point for program.  The application *
//* object is created and run.                     *
//*------------------------------------------------*

#include <gtkmm.h>
#include "Application.h"

int main( int argc, char *argv[] )
{
	// Create and run an instance of the application.
	
	std::unique_ptr< CApplication > Application( new CApplication );  
	return Application->run( argc, argv );
}