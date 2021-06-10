// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file contains the main entry point for the application.
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

#include "Application.h"
#include <glibmm/i18n.h>
#include <glibmm/miscutils.h>

//-------------------
// Local declarations
//-------------------

static const std::string program_name     = "Enigma in the Wine Cellar";
static const std::string application_name =_("Enigma in the Wine Cellar");

//--------------------------------------------------
// This is the main entry point for the application.
//--------------------------------------------------

int main(int argc, char *argv[])
{
	// Set the program name early so it is available if g_warning()
	// or g_errror() macros are used in class constructors.
	
	Glib::set_prgname(program_name);
	Glib::set_application_name(application_name);

	// Create and run an instance of the application.
 
  Glib::RefPtr<Enigma::Application> application(new Enigma::Application);
	return application->run(argc, argv);
}

