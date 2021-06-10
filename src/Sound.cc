// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the Sound class implementation.  The Sound class plays a sound
// effect.
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

#include <queue>
#include <glibmm/refptr.h>
#include <glibmm/bytes.h>
#include <giomm/resource.h>
#define OV_EXCLUDE_STATIC_CALLBACKS    // Exclude static Vorbis callbacks.
#include <vorbis/vorbisfile.h>
#include <pulse/glib-mainloop.h>
#include <pulse/scache.h>
#include "Sound.h"

//-------------------
// Local declarations
//-------------------

static std::queue<std::string> name_queue;
void handle_name();

// PulseAudio sound server declarations.

static const pa_sample_format PULSEAUDIO_SAMPLE_FORMAT = PA_SAMPLE_S16LE;
static const uint32_t PULSEAUDIO_SAMPLE_RATE           = 44100;
static const uint8_t PULSEAUDIO_SAMPLE_CHANNELS        = 1;

static pa_sample_spec pulseaudio_sample_spec;
static pa_channel_map pulseaudio_map;

static pa_glib_mainloop* pulseaudio_mainloop = NULL;
static pa_context* pulseaudio_context        = NULL;
static pa_stream* pulseaudio_stream          = NULL;

void pulseaudio_context_notify_callback(pa_context *context,
                                        void *userdata);

void pulseaudio_stream_notify_callback(pa_stream *stream,
                                       void *userdata);

// Vorbis sound file decoder declarations.

static const int VORBIS_SAMPLE_ENDIAN     = 0;   // Little-endian format.
static const int VORBIS_SAMPLE_WORDLENGTH = 2;   // 16-bit data.
static const int VORBIS_SAMPLE_SIGNED     = 1;   // Signed data.

static OggVorbis_File vorbis_file;
static ov_callbacks vorbis_callbacks;
static guint8* vorbis_data;
static size_t vorbis_size;
static size_t vorbis_offset;
static int vorbis_userdata;

size_t vorbis_read_callback(void* dest,
                            size_t size,
                            size_t nmemb,
                            void* userdata);
                            
int vorbis_seek_callback(void *userdata,
                             ogg_int64_t offset,
                             int whence);

long vorbis_tell_callback(void *userdata);

//--------------------------------------------------
// This constructor creates an empty Sound instance.
//--------------------------------------------------

Enigma::Sound::Sound()
{
	// m_name will begin as an empty string.
}

//--------------------------------------------------------------------
// This method adds a sound sample name to a queue with names of sound
// samples waiting to be uploaded to the PulseAudio sound server.
//--------------------------------------------------------------------
// name: Sound resource name (eg. "Bump.ogg")
//--------------------------------------------------------------------

void Enigma::Sound::load(std::string name)
{	
	// Save the name for when the sample is to be played.
	
	m_name = name;
	
	// Add the new name to the queue.  Begin handling this new name if
	// the queue was originally empty (no other sound was being handled).
	
	name_queue.push(name);
	
	if (name_queue.size() == 1)
		handle_name();
}

//-----------------------------------------------------------
// This method asynchronously plays a sound sample previously
// uploaded to the PulseAudio sample cache.
//-----------------------------------------------------------

void Enigma::Sound::play()
{
	pa_context_play_sample(pulseaudio_context,
	                       m_name.c_str(),
	                       NULL,
			                   PA_VOLUME_INVALID,
			                   NULL,
			                   NULL);
}

//----------------------------------------------------------
// This static method initializes the Sound class resources.
//----------------------------------------------------------

void Enigma::Sound::initialize()
{
	// Initialize Vorbis file callback structure.
	
	vorbis_callbacks.read_func  = &vorbis_read_callback;
	vorbis_callbacks.seek_func  = &vorbis_seek_callback;
	vorbis_callbacks.close_func = NULL;
	vorbis_callbacks.tell_func  = &vorbis_tell_callback;
	
	// Initialize PulseAudio sample specifications and channel mapping.
	
	pulseaudio_sample_spec.format   = PULSEAUDIO_SAMPLE_FORMAT;
	pulseaudio_sample_spec.rate     = PULSEAUDIO_SAMPLE_RATE;
	pulseaudio_sample_spec.channels = PULSEAUDIO_SAMPLE_CHANNELS;
	
	pa_channel_map_init_mono(&pulseaudio_map);
	
	// Create a new Glib mainloop for PulseAudio if one does not exist.
	
	if (!pulseaudio_mainloop)
		pulseaudio_mainloop = pa_glib_mainloop_new(NULL);

	if (!pulseaudio_mainloop)
		return;
	
	// Create a common PulseAudio context if one does not exist.
	
	if (!pulseaudio_context)
	{
		pulseaudio_context =
			pa_context_new(pa_glib_mainloop_get_api(pulseaudio_mainloop), NULL);
	}
		
	if (!pulseaudio_context)
		destroy();

	// Begin the asynchronous process of connecting the context to the
	// PulseAudio server.  This will result in multiple calls to the
	// callback as the connection is being made.

	pa_context_set_state_callback(pulseaudio_context,
	                              pulseaudio_context_notify_callback,
	                              NULL);

	pa_context_connect(pulseaudio_context, NULL, PA_CONTEXT_NOFLAGS, NULL);
}

//----------------------------------------------------
// This static method frees the Sound class resources.
//----------------------------------------------------

void Enigma::Sound::destroy()
{	
	// Disconnect the PulseAudio context from the server.

	if (pulseaudio_context)
	{
		pa_context_disconnect(pulseaudio_context);
		pulseaudio_context = NULL;
	}
	
	// Free the PulseAudio mainloop.
	
	if (pulseaudio_mainloop)
	{
		pa_glib_mainloop_free(pulseaudio_mainloop);
		pulseaudio_mainloop = NULL;
	}
}

//--------------------------------------------------------------------------
// This private function begins an asynchronous process to decode a Vorbis
// Ogg sound file in resources and upload it to the PulseAudio sample cache.
//--------------------------------------------------------------------------

void handle_name()
{
	// Return if there is no PulseAudio context or the context is not
	// ready to execute operations.

	if  ((!pulseaudio_context)
	  || (pa_context_get_state(pulseaudio_context) != PA_CONTEXT_READY))
	{
		return;
	}

	if (!name_queue.empty())
	{
		// Get the front sound name from the queue.  It will be left in the queue
		// to indicate a sound is currently being handed.
		
		std::string name = name_queue.front();
		
		// Extract the sound data from the resource bundle.  This data is stored
		// in Vorbis Ogg format, so a decoder will be used to convert it into PCM
		// sound data.

		std::string path = G_RESOURCE_PREFIX;
		path += name;
		
		Glib::RefPtr<const Glib::Bytes> byte_array =
			Gio::Resource::lookup_data_global(path, Gio::RESOURCE_LOOKUP_FLAGS_NONE);

		// Initialize Vorbis resource data information.

		vorbis_offset = 0;
		vorbis_size   = 0;
		vorbis_data   = (guint8*)byte_array->get_data(vorbis_size);

		// Open the Vorbis file with callbacks.  Although the callback userdata
		// field is not used, setting this field to NULL in ov_open_callbacks()
		// results in an open failure with error -132 (OV_ENOTVORBIS).  Therefore,
		// a dummy vorbis_userdata pointer is provided.

		if (ov_open_callbacks(&vorbis_userdata,
		                      &vorbis_file,
		                      NULL,
		                      0,
		                      vorbis_callbacks) == 0)
		{
			// Calculate the total byte length of the PCM sample.
						
			ogg_int64_t total_pcm_bytes =
				ov_pcm_total(&vorbis_file, -1) * VORBIS_SAMPLE_WORDLENGTH;
		
			// Create a PulseAudio stream.
			
			pulseaudio_stream = pa_stream_new(pulseaudio_context,
					                              name.c_str(),
					                              &pulseaudio_sample_spec,
					                              &pulseaudio_map);

			if (pulseaudio_stream)
			{
			 	// Set the callback used when stream data can be written, then begin
				// an asynchronous process of connecting the stream to upload sample
				// data to the PulseAudio server cache.
				
			 	pa_stream_set_state_callback(pulseaudio_stream,
						                         pulseaudio_stream_notify_callback,
						                         NULL);
				
				pa_stream_connect_upload(pulseaudio_stream, total_pcm_bytes);
			}
			else
			{
				// A PulseAudio stream could not be created.  Clear the Vorbis file
				// to free its resources.
				
				ov_clear(&vorbis_file);
			}
		}
	}
}

//----------------------------------------------------------------------------
// This callback function is called when the PulseAudio context changes state.
//----------------------------------------------------------------------------
// context:  PulseAudio context.
// userdata: Pointer to user data (unused).
//----------------------------------------------------------------------------

void pulseaudio_context_notify_callback(pa_context *context, void *userdata)
{
	switch (pa_context_get_state(context))
	{
		case PA_CONTEXT_READY:
			// The PulseAudio context is ready to execute operations.
			// Handle any sound name waiting in the queue.

			handle_name();
			break;
		
		default:
			break;
	}
}

//---------------------------------------------------------------------------
// This callback function is called when the PulseAudio stream changes state.
//---------------------------------------------------------------------------
// stream:   PulseAudio stream.
// userdata: Pointer to user data (unused).
//---------------------------------------------------------------------------

void pulseaudio_stream_notify_callback(pa_stream *stream, void *userdata)
{
	switch (pa_stream_get_state(stream))
	{
		case PA_STREAM_READY:
			{
				// The PulseAudio stream is ready to accept operations.  Decode all
				// Vorbis file data and write it to the PulseAudio stream.  The number
				// of bytes read per iteration (2048, 512, and 256 have been observed)
				// will generally be less than the buffer size (4096 is recommended),
				// which is treated as a limit rather than a request.

				char buffer[4096];
				int bitstream = 0;
				size_t length;
				size_t writable;
				char* byte;

				do
				{
					length = ov_read(&vorbis_file,
									         buffer,
									         sizeof(buffer),
									         VORBIS_SAMPLE_ENDIAN,
									         VORBIS_SAMPLE_WORDLENGTH,
									         VORBIS_SAMPLE_SIGNED,
									         &bitstream);
					
					// It is expected that the total writable PulseAudio stream data
					// always equals the total Vorbis file data yet to be read.  If
					// this is not the case, the sample will be clipped.

					writable = pa_stream_writable_size(stream); 
					
					if (writable < length)
						length = writable;
					
					pa_stream_write(stream, buffer, length, NULL, 0, PA_SEEK_RELATIVE);
				}
				while (length > 0);
				
				// All Vorbis file data has been decoded and copied to the PulseAudio
				// server cache.  Clear the Vorbis file structure to free its
				// resources.
		
				ov_clear(&vorbis_file);
				
				// Finish the stream upload.  From observations of the stream state
				// changes and the PulseAudio sourcecode, this also disconnects the
				// stream (stream notify callback will be called with the stream
				// state as PA_STREAM_TERMINATED).
		
				pa_stream_finish_upload(stream);
			}
			
			break;			
			
		case PA_STREAM_TERMINATED:
			// The PulseAudio stream has terminated cleanly.  Remove the current
			// sound name from the queue since it is finished being handled.

			if (!name_queue.empty())
				name_queue.pop();
	
			// Unset the stream state callback.  Doing so may be unnecessary, but
			// it does prevent an unexpected callback from occurring.  It will be
			// set when another sound is to be uploaded.
			
			pa_stream_set_state_callback(stream, NULL, NULL);
	
			// Handle the next sound name in the queue.
	
			handle_name();
			break;
			
		default:
			break;
	}
}

//---------------------------------------------------------------------
// This callback function is called when a Vorbis read request is made.
//---------------------------------------------------------------------
// dest:     Destination for data that was read.
// size:     Byte size of data items.
// nmemb:    Number of items to be read.
// userdata: Pointer to user data (unused).
// RETURN:   Number of items read, or 0 to indicate the end of items.
//---------------------------------------------------------------------

size_t vorbis_read_callback(void* dest,
                            size_t size,
                            size_t nmemb,
                            void* userdata)
{	
	// Determine the actual number of bytes of source data that can be read.
	// This may be less than that requested if it is not available.
	
	size_t actual    = size * nmemb;
	size_t available = vorbis_size - vorbis_offset;

	if (actual > available)
		actual = available;

	// Copy data from the source to the destination, and advance the source
	// data offset.
	
	memcpy(dest, vorbis_data + vorbis_offset, actual);
	vorbis_offset += actual;
	
	// Return the number of items read.
	
	if (actual > 0)
		return (actual / size);
	else
		return 0;
}

//---------------------------------------------------------------------
// This callback function is called when a Vorbis seek request is made. 
//---------------------------------------------------------------------
// userdata: Pointer to user data (unused).
// offset:   Offset to be added to relative position.
// whence:   Relative position to use when adding offset.
// RETURN:   0 if successful, or -1 if offset could not be set.
//---------------------------------------------------------------------

int vorbis_seek_callback(void *userdata, ogg_int64_t offset, int whence)
{	
	int result = 0;

	switch (whence)
	{
		case SEEK_SET:
			// Add offset relative to the beginning of the source data.
		
			vorbis_offset = offset;
			break;
		
		case SEEK_CUR:
			// Add offset relative to the current offset.
			
			vorbis_offset += offset;
			break;
		
		case SEEK_END:
			// Add offset relative the offset one past the last source byte.
		
			vorbis_offset = vorbis_size + offset;
			break;
		
		default:
			// Return failure code for unrecognized offset references.
		
			result = -1;
			break;
	}

	return result;
}

//---------------------------------------------------------------------
// This callback function is called when a Vorbis tell request is made. 
//---------------------------------------------------------------------
// userdata: Pointer to user data (unused).
// RETURN:   Byte offset of next data byte that would will be read.
//---------------------------------------------------------------------

long vorbis_tell_callback(void *userdata)
{	
	// Return offset of next byte that will be read.  If the offset is already
	// on the last source byte, the returned value will be the total size of
	// the source data.

	if (vorbis_offset < vorbis_size)
		return vorbis_offset + 1;
	else
		return vorbis_size;
}

