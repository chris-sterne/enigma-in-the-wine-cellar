/*------------------------------------*
 * Program: Enigma in the Wine Cellar *
 * Version: 5.0 for Linux OS          *
 * File:    Sounds.h                  *
 * Date:    October 17, 2016          *
 * Author:  Chris Sterne              *
 *                                    *
 * Sounds class header.               *
 *------------------------------------*/

#ifndef __SOUNDS_H__
#define __SOUNDS_H__

#include <gtkmm.h>
#include <dlfcn.h>
#include <canberra.h>

class CSounds
{
  public:
    // Public declarations.
		
    enum ID
    {
      ENone = 0,
      EDoorOpen,
      EDoorClose,
      ERingPull,
      ERingRelease,
      EPadButtonPress,
      EPadButtonRelease,
      ELockOn,
      ELockOff,
      EEnterWater,
      EExitWater,
      EBump,
      EPurr,
      ETeleport,
      EResurface,
      EFlipSurface,
      EItemFound,
      EStoneButtonPress,
      EStoneButtonRelease,
      ETreeGrow,
      TOTAL
    };
		
    // Public methods.

    CSounds();
    ~CSounds();
    void Play( CSounds::ID aID );

    // Public data.

    gboolean iBusy;        // TRUE if a sound is being played.
		
  private:
    // Private data for canberra sound library.

    void *ica_handle;
    ca_context* ica_context;
    int (*ica_context_create)(ca_context**);
    int (*ica_context_destroy)(ca_context*);
    int (*ica_context_open)(ca_context*);
    int (*ica_proplist_create)(ca_proplist**);
    int (*ica_proplist_destroy)(ca_proplist*);
    int (*ica_proplist_sets)(ca_proplist*, const char* key, const char* value);
    int (*ica_context_cancel)(ca_context*, uint32_t);

    int (*ica_context_play)(ca_context*, uint32_t,
                            const char*, const char*,
                            const char *);

    int (*ica_context_play_full)(ca_context *c,
                                 uint32_t id,
                                 ca_proplist *p,
                                 ca_finish_callback_t cb,
                                 CSounds& aSounds );
};

#endif /* SOUNDS_H_ */