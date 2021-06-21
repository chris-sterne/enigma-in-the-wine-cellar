/*-------------------------------------------------*
 * Program: Enigma in the Wine Cellar              *
 * Version: 5.0 for Linux OS                       *
 * File:    Sounds.cpp          	                 *
 * Date:    October 17, 2016                       *
 * Author:  Chris Sterne                           *
 *                                                 *
 * Sounds class.                                   *
 *-------------------------------------------------*
 * This class plays room sounds using the canberra *
 * dynamic shared library.                         *
 *-------------------------------------------------*/
 
#include "Sounds.h"

//*---------------------*
//* Local declarations. *
//*---------------------*

static const char* FilenameArray[ CSounds::ID::TOTAL ] =
{
  NULL,
  "./Sounds/DoorOpen.ogg",
  "./Sounds/DoorClose.ogg",
  "./Sounds/RingPull.ogg",
  "./Sounds/RingRelease.ogg",
  "./Sounds/PadButtonPress.ogg",
  "./Sounds/PadButtonRelease.ogg",
  "./Sounds/LockOn.ogg",
  "./Sounds/LockOff.ogg",
  "./Sounds/EnterWater.ogg",
  "./Sounds/ExitWater.ogg",
  "./Sounds/Bump.ogg",
  "./Sounds/Purr.ogg",
  "./Sounds/Teleport.ogg",
  "./Sounds/Resurface.ogg",
  "./Sounds/FlipSurface.ogg",
  "./Sounds/ItemFound.ogg",
  "./Sounds/StoneButtonPress.ogg",
  "./Sounds/StoneButtonRelease.ogg",
  "./Sounds/TreeGrow.ogg"
};

//*----------------------*
//* Default constructor. *
//*----------------------*

CSounds::CSounds()
{
	iBusy = FALSE;

	// Open the sound library and initialize some library function
	// pointers.

	ica_handle  = dlopen( "libcanberra.so", RTLD_LAZY );
	ica_context = NULL;

	if ( ica_handle )
	{
		dlerror();

		*(void **)(&ica_context_create) =
			dlsym( ica_handle, "ca_context_create" );

		*(void **)(&ica_context_destroy) =
			dlsym( ica_handle, "ca_context_destroy" );

		*(void **)(&ica_proplist_create) =
			dlsym( ica_handle, "ca_proplist_create" );

		*(void **)(&ica_proplist_destroy) =
			dlsym( ica_handle, "ca_proplist_destroy" );

		*(void **)(&ica_proplist_sets) =
			dlsym( ica_handle, "ca_proplist_sets" );
		
		*(void **)(&ica_context_play) =
			dlsym( ica_handle, "ca_context_play" );

		*(void **)(&ica_context_play_full) =
			dlsym( ica_handle, "ca_context_play_full" );

		*(void **)(&ica_context_cancel) =
			dlsym( ica_handle, "ca_context_cancel" );
		
		*(void **)(&ica_context_open) =
			dlsym( ica_handle, "ca_context_open" );

		// Create a sound library context.
		
		(*ica_context_create)( &ica_context );
		(*ica_context_open)( ica_context );
	}

	return;
}

//*---------------------*
//* Default destructor. *
//*---------------------*

CSounds::~CSounds()
{
	// Destroy the sound library context and close the library.
	// Library handle and context pointers are cleared to indicate
	// the library is no longer loaded.

	if ( ica_handle )
	{
		if ( ica_context)
		{
			// Cancel any sound being played.

			if ( iBusy )
				(*ica_context_cancel)(ica_context, 1 );

			// Delete the sound context.
			
			ica_context_destroy( ica_context );
			ica_context = NULL;
		}

		dlclose( ica_handle );
		ica_handle = NULL;
	}

	return;
}

//*--------------------------------------------------*
//* Function called when sound playback is complete. *
//*--------------------------------------------------*

void Playback_Callback( ca_context *c, uint32_t id, int error_code, void* user_data )
{
	// Set iBusy = FALSE to indicate the sound has finished playing,
	// which then allows a new sound to be played.

	CSounds* Sounds = (CSounds*)user_data;
	Sounds->iBusy = FALSE;
	
	return;
}

//*-----------------------------------------------------------------*
//* This method plays a sound.  If another sound is being played,   *
//* the new request is ignored (From observation, if a new sound is *
//* started while an old one is in progress, static noise can be    *
//* heard).                                                         *
//*-----------------------------------------------------------------*
//* aSound: ID of sound to play.                                    *
//*-----------------------------------------------------------------*

void CSounds::Play( CSounds::ID aID )
{
	if ( ica_handle
	  && !iBusy
	  && ( aID != CSounds::ID::ENone )
	  && ( (int)aID < (int)CSounds::ID::TOTAL ))
	{
    ca_proplist* PropertyList;

    if ( (*ica_proplist_create)(&PropertyList) == 0 )
    {
      // Add entries to the property list.
		
      (*ica_proplist_sets)(PropertyList,
                           CA_PROP_MEDIA_FILENAME,
                           FilenameArray[ (int)aID ] );

      // Play the sound.
					
      int Result = (*ica_context_play_full)(ica_context,
                   1,
                   PropertyList,
                   &Playback_Callback,
                   *this );

      // Set the "busy" flag if the sound playing started successfully.
					
      if ( Result == CA_SUCCESS )
        iBusy = TRUE;
						
      (*ica_proplist_destroy)(PropertyList);
    }
  }

  return;
}