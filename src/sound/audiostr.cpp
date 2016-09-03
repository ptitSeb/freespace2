/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Sound/AudioStr.cpp $
 * $Revision: 248 $
 * $Date: 2005-03-29 04:18:47 +0200 (Tue, 29 Mar 2005) $
 * $Author: taylor $
 *
 * Routines to stream large WAV files from disk
 *
 * $Log$
 * Revision 1.7  2005/03/29 02:18:47  taylor
 * Various 64-bit platform fixes
 * Fix compiler errors with MAKE_FS1 and fix gr_set_bitmap() too
 * Make sure that turrets can fire at asteroids for FS1 (needed for a couple missions)
 * Streaming audio support (big thanks to Pierre Willenbrock!!)
 * Removed dependance on strings.tbl for FS1 since we don't actually need it now
 *
 * Revision 1.6  2003/08/03 16:10:30  taylor
 * cleanup; compile warning fixes
 *
 * Revision 1.5  2002/06/09 04:41:26  relnev
 * added copyright header
 *
 * Revision 1.4  2002/06/05 04:03:33  relnev
 * finished cfilesystem.
 *
 * removed some old code.
 *
 * fixed mouse save off-by-one.
 *
 * sound cleanups.
 *
 * Revision 1.3  2002/05/27 18:42:50  theoddone33
 * Fix missing audiostr_* symbols
 *
 * Revision 1.2  2002/05/07 03:16:52  theoddone33
 * The Great Newline Fix
 *
 * Revision 1.1.1.1  2002/05/03 03:28:10  root
 * Initial import.
 *
 * 
 * 5     9/14/99 1:32a Jimb
 * Commented out Int3() that was hanging Jim's machine.  Happens before
 * sm2-07 command brief.
 * 
 * 4     7/14/99 12:09p Jefff
 * Make sure we're not servicing a bogus audiostream. Check for "used"
 * after the critical section lock.
 * 
 * 3     12/17/98 4:01p Andsager
 * up wavedata buffer size to 180000 to allow stereo 16b/22KHz streaming
 * 
 * 2     10/07/98 10:53a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:51a Dave
 * 
 * 53    6/28/98 6:35p Lawrance
 * move re-entrancy semaphore into audiostream class
 * 
 * 52    5/24/98 4:42p Dan
 * AL: Fix several bugs related to pausing and enabling/disabling event
 * music
 * 
 * 51    5/21/98 11:57a Lawrance
 * fix potential bug with transitions for music when in packfiles
 * 
 * 50    5/15/98 9:09p Lawrance
 * The last of the multi-threading fixes
 * 
 * 49    5/15/98 7:57p Duncan
 * AL: Fix race condition with music streaming
 * 
 * 48    5/15/98 10:13a Lawrance
 * remove unused audiostream member
 * 
 * 47    5/14/98 5:45p Lawrance2
 * Put critical section around audiostream destroying
 * 
 * 46    5/12/98 5:40p Lawrance
 * Add critical section code to the service buffer call.. since it is
 * possible to release buffers while in this call
 * 
 * 45    5/10/98 3:49p Sandeep
 * Fix problem with having the audio streaming while trying to close down
 * sound
 * 
 * 44    4/30/98 4:53p John
 * Restructured and cleaned up cfile code.  Added capability to read off
 * of CD-ROM drive and out of multiple pack files.
 * 
 * 43    4/26/98 3:30a Lawrance
 * Fix a couple of potential bugs
 * 
 * 42    4/21/98 10:18a Dan
 * 
 * 41    4/17/98 6:59a John
 * Changed code the used 'new' and 'delete' to use 'malloc' and 'free'
 * instead.  Had to manually can constructors/destructors.
 * 
 * 40    4/13/98 10:18a John
 * fixed warnings
 * 
 * 39    4/13/98 10:16a John
 * Switched gettime back to timer_get_milliseconds, which is now thread
 * safe.
 * 
 * 38    4/12/98 11:08p Lawrance
 * switch back to using gettime() in separate threads
 * 
 * 37    4/12/98 5:31p Lawrance
 * use timer_get_milliseconds() instead of gettime()
 * 
 * 36    4/06/98 12:36a Lawrance
 * Ensure all non-music ADPCM files get decompressed to 8 bit.
 * 
 * 35    4/03/98 4:56p Lawrance
 * Upu the max audio streams to 30
 * 
 * 34    3/31/98 4:50p Dan
 * AL: Clean up all audio streams if necessary in
 * event_music_level_close()
 * 
 * 33    3/23/98 4:12p Lawrance
 * Fix subtle bug with looping and fading out songs
 * 
 * 32    2/18/98 5:49p Lawrance
 * Even if the ADPCM codec is unavailable, allow game to continue.
 * 
 * 31    2/15/98 4:43p Lawrance
 * work on real-time voice
 * 
 * 30    1/19/98 11:37p Lawrance
 * Fixing Optimization build warnings
 * 
 * 29    1/17/98 4:41p Lawrance
 * Fix problem with multiple audio streams using the same buffers
 * 
 * 28    1/16/98 11:49a Lawrance
 * Use own internal timer for fading.
 * 
 * 27    12/28/97 12:43p John
 * Put in support for reading archive files; Made missionload use the
 * cf_get_file_list function.   Moved demos directory out of data tree.
 * 
 * 26    12/27/97 8:08p Lawrance
 * If an audiostream doesn't exists, it can't be playing
 * 
 * 25    12/18/97 3:30p Lawrance
 * Fix bug that sometimes caused music with no volume to not get stopped
 * properly.
 * 
 * 24    12/17/97 10:17p Allender
 * redid streadming code to use mmio* functions instead of cf* functions.
 * Our functions are not reentrant!
 * 
 * 23    12/10/97 10:04p Lawrance
 * modify what happens in Audio_stream constructor
 * 
 * 22    12/09/97 6:14p Lawrance
 * add -nomusic flag
 * 
 * 21    12/08/97 6:21p Lawrance
 * fix problems with signaling that end-of-file has been reached
 * 
 * 20    12/05/97 10:50a Lawrance
 * improve how silence bytes are written on transitions
 * 
 * 19    12/04/97 5:35p Lawrance
 * fix bug that may have caused errors when writing silence
 * 
 * 18    11/28/97 2:09p Lawrance
 * Overhaul how ADPCM conversion works... use much less memory... safer
 * too.
 * 
 * 17    10/03/97 8:24a Lawrance
 * When unpausing, be sure to retain looping status
 * 
 * 16    9/24/97 5:30p Lawrance
 * fix bug that was messing up streaming of 8 bit audio
 * 
 * 15    9/18/97 10:31p Lawrance
 * add functions to pause and unpause all audio streams
 * 
 * 14    9/09/97 3:39p Sandeep
 * warning level 4 bugs
 * 
 * $NoKeywords: $
 */


#include "pstypes.h"

#ifndef PLAT_UNIX
#define VC_EXTRALEAN
#define STRICT
#include <windows.h>
#include <mmsystem.h>
#include <mmreg.h>
#include <msacm.h>
#include "vdsound.h"
#else
#ifdef __APPLE__
#include <al.h>
#include <alc.h>
#include <alut.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
#endif	// __APPLE__
#endif
#include "audiostr.h"
#include "cfile.h"		// needed for cf_get_path
#include "timer.h"
#include "sound.h"		/* for Snd_sram */
#include "acm.h"
#include "ds.h"


#ifdef PLAT_UNIX
#ifndef NDEBUG
#define OpenAL_ErrorCheck(onerr)	do {		\
	int i = alGetError();			\
	if (i != AL_NO_ERROR) {			\
		while(i != AL_NO_ERROR) {	\
			nprintf(("Warning", "%s/%s:%d - OpenAL error %s\n", __FUNCTION__, __FILE__, __LINE__, alGetString(i))); \
			i = alGetError();	\
		}				\
		onerr;			\
	} 					\
} while (0);
#else
#define OpenAL_ErrorCheck(onerr)
#endif

#define MAX_AL_BUF 4

#define BYTE byte
#define LONG long
#define LPBYTE BYTE *

static int audiostr_read_word(SDL_RWops *rw, WORD *i)
{
	int rc = SDL_RWread( rw, i, 1, sizeof(WORD) );

	if (rc != sizeof(WORD)) {
		return 0;
	}

	*i = INTEL_SHORT(*i);

	return 1;
}

static int audiostr_read_dword(SDL_RWops *rw, DWORD *i)
{
	int rc = SDL_RWread( rw, i, 1, sizeof(DWORD) );

	if (rc != sizeof(DWORD))
		return 0;

	*i = INTEL_INT(*i);

	return 1;
}
#endif


// Constants
#ifndef SUCCESS
#define SUCCESS TRUE        // Error returns for all member functions
#define FAILURE FALSE
#endif // SUCCESS

typedef BOOL (*TIMERCALLBACK)(ptr_u);

#define BIGBUF_SIZE					180000			// This can be reduced to 88200 once we don't use any stereo
//#define BIGBUF_SIZE					88300			// This can be reduced to 88200 once we don't use any stereo
unsigned char *Wavedata_load_buffer = NULL;		// buffer used for cueing audiostreams
unsigned char *Wavedata_service_buffer = NULL;	// buffer used for servicing audiostreams

CRITICAL_SECTION Global_service_lock;

#define COMPRESSED_BUFFER_SIZE	88300
unsigned char *Compressed_buffer = NULL;				// Used to load in compressed data during a cueing interval
unsigned char *Compressed_service_buffer = NULL;	// Used to read in compressed data during a service interval

#define AS_HIGHEST_MAX				999999999	// max uncompressed filesize supported is 999 meg

// Classes

// Timer
//
// Wrapper class for Windows multimedia timer services. Provides
// both periodic and one-shot events. User must supply callback
// for periodic events.
// 

class Timer
{
public:
    void constructor(void);
    void destructor(void);
    BOOL Create (UINT nPeriod, UINT nRes, ptr_u dwUser,  TIMERCALLBACK pfnCallback);
protected:
#ifndef PLAT_UNIX 
    static void CALLBACK TimeProc(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);
#else
    static Uint32 CALLBACK TimeProc(Uint32 interval, void *dwUser);
#endif
    TIMERCALLBACK m_pfnCallback;
    DWORD m_dwUser;
    UINT m_nPeriod;
    UINT m_nRes;
#ifndef PLAT_UNIX 
    UINT m_nIDTimer;
#else
    SDL_TimerID m_nIDTimer;
#endif
};


// Class

// WaveFile
//
// WAV file class (read-only).
//
// Public Methods:
//
// Public Data:
//   
//

class WaveFile
{
public:
	void Init(void);
	void Close(void);
	BOOL Open (LPSTR pszFilename);
	BOOL Cue (void);
	int	Read (BYTE * pbDest, UINT cbSize, int service=1);
	UINT GetNumBytesRemaining (void) { return (m_nDataSize - m_nBytesPlayed); }
	UINT GetUncompressedAvgDataRate (void) { return (m_nUncompressedAvgDataRate); }
	UINT GetDataSize (void) { return (m_nDataSize); }
	UINT GetNumBytesPlayed (void) { return (m_nBytesPlayed); }
	BYTE GetSilenceData (void);
	WAVEFORMATEX m_wfmt;					// format of wave file used by Direct Sound
	WAVEFORMATEX * m_pwfmt_original;	// foramt of wave file from actual wave source
	UINT m_total_uncompressed_bytes_read;
	UINT m_max_uncompressed_bytes_to_read;
	UINT	m_bits_per_sample_uncompressed;

protected:
	UINT m_data_offset;						// number of bytes to actual wave data
	int  m_data_bytes_left;
#ifndef PLAT_UNIX 
	HMMIO	cfp;
#else
	SDL_RWops *cfp;
#endif

	UINT m_wave_format;						// format of wave source (ie WAVE_FORMAT_PCM, WAVE_FORMAT_ADPCM)
	UINT m_nBlockAlign;						// wave data block alignment spec
	UINT m_nUncompressedAvgDataRate;		// average wave data rate
	UINT m_nDataSize;							// size of data chunk
	UINT m_nBytesPlayed;						// offset into data chunk
	BOOL m_abort_next_read;

#ifndef PLAT_UNIX 
	HACMSTREAM		m_hStream;
	int				m_hStream_open;
	WAVEFORMATEX	m_wfxDest;
#else
	void*		m_hStream;
	int				m_hStream_open;
	WAVEFORMATEX	m_wfxDest;
#endif
};

// Classes

// AudioStreamServices
//
// DirectSound apportions services on a per-window basis to allow
// sound from background windows to be muted. The AudioStreamServices
// class encapsulates the initialization of DirectSound services.
//
// Each window that wants to create AudioStream objects must
// first create and initialize an AudioStreamServices object. 
// All AudioStream objects must be destroyed before the associated 
// AudioStreamServices object is destroyed.
class AudioStreamServices
{
public:
    void Constructor(void);
    BOOL Initialize ();
#ifndef PLAT_UNIX
    LPDIRECTSOUND GetPDS (void) { return m_pds; }
protected:
    LPDIRECTSOUND m_pds;
#endif
};


// AudioStream
//
// Audio stream interface class for playing WAV files using DirectSound.
// Users of this class must create AudioStreamServices object before
// creating an AudioStream object.
//
// Public Methods:
//
// Public Data:
//

// status
#define ASF_FREE	0
#define ASF_USED	1

class AudioStream
{
public:
	AudioStream (void);
	~AudioStream (void);
	BOOL Create (LPSTR pszFilename, AudioStreamServices * pass);
	BOOL Destroy (void);
	void Play (long volume, int looping);
	int Is_Playing(){ return(m_fPlaying); }
	int Is_Paused(){ return(m_bIsPaused); }
	int Is_Past_Limit() { return m_bPastLimit; }
	void Stop (int paused=0);
	void Stop_and_Rewind (void);
	void Fade_and_Destroy (void);
	void Fade_and_Stop(void);
	void	Set_Volume(long vol);
	long	Get_Volume();
	void	Init_Data();
	void	Set_Byte_Cutoff(unsigned int num_bytes_cutoff);
	void  Set_Default_Volume(long converted_volume) { m_lDefaultVolume = converted_volume; }
	long	Get_Default_Volume() { return m_lDefaultVolume; }
	unsigned int Get_Bytes_Committed(void);
	int	Is_looping() { return m_bLooping; }
	int	status;
	int	type;
	UINT m_bits_per_sample_uncompressed;

protected:
	void Cue (void);
	BOOL WriteWaveData (UINT cbSize, UINT* num_bytes_written,int service=1);
	BOOL WriteSilence (UINT cbSize);
	DWORD GetMaxWriteSize (void);
	BOOL ServiceBuffer (void);
	static BOOL TimerCallback (ptr_u dwUser);
#ifdef PLAT_UNIX
	BOOL GetWritingDone (void);
#endif

	AudioStreamServices * m_pass;  // ptr to AudioStreamServices object
#ifndef PLAT_UNIX
	LPDIRECTSOUNDBUFFER m_pdsb;    // ptr to Direct Sound buffer
#else
        ALuint m_al_multichannel_id;   // name of openAL source
        ALuint m_al_buffer_ids[MAX_AL_BUF]; //names of buffers
#endif
	WaveFile * m_pwavefile;        // ptr to WaveFile object
	Timer m_timer;              // ptr to Timer object
	BOOL m_fCued;                  // semaphore (stream cued)
	BOOL m_fPlaying;               // semaphore (stream playing)
#ifndef PLAT_UNIX
	DSBUFFERDESC m_dsbd;           // Direct Sound buffer description
#endif
	LONG m_lInService;             // reentrancy semaphore
	UINT m_cbBufOffset;            // last write position
	UINT m_nBufLength;             // length of sound buffer in msec
	UINT m_cbBufSize;              // size of sound buffer in bytes
	UINT m_nBufService;            // service interval in msec
	UINT m_nTimeStarted;           // time (in system time) playback started

	BOOL	m_bLooping;						// whether or not to loop playback
	BOOL	m_bFade;							// fade out music 
	BOOL	m_bDestroy_when_faded;
	LONG  m_lVolume;						// volume of stream ( 0 -> -10 000 )
	LONG	m_lCutoffVolume;
	BOOL  m_bIsPaused;					// stream is stopped, but not rewinded
	UINT	m_silence_written;			// number of bytes of silence written to buffer
	UINT  m_bReadingDone;				// no more bytes to be read from disk, still have remaining buffer to play
	DWORD	m_fade_timer_id;				// timestamp so we know when to start fade
	DWORD	m_finished_id;					// timestamp so we know when we've played #bytes required
	BOOL	m_bPastLimit;					// flag to show we've played past the number of bytes requred
	LONG	m_lDefaultVolume;
#ifdef PLAT_UNIX
	int m_al_buffer_play;
#else
	HRESULT h_result;
#endif

	CRITICAL_SECTION write_lock;
};


// AudioStreamServices class implementation
//
////////////////////////////////////////////////////////////

// Constructor
void AudioStreamServices::Constructor(void)
{
#ifndef PLAT_UNIX 
    // Initialize member data
    m_pds = NULL;

    // It would seem to make sense to initialize DirectSound here,
    // but because there could be an error, it's best done in a
    // separate member function, ::Initialize.
#endif
}


#ifndef PLAT_UNIX 
extern LPDIRECTSOUND pDirectSound;		// From Sound.cpp
#endif


// Initialize
BOOL AudioStreamServices::Initialize ()
{
    
    BOOL fRtn = SUCCESS;    // assume success

#ifndef PLAT_UNIX 
    if (m_pds == NULL)  {
		m_pds = pDirectSound;
    }
#endif

    return (fRtn);
}



//
// AudioStream class implementation
//
////////////////////////////////////////////////////////////

// The following constants are the defaults for our streaming buffer operation.
const UINT DefBufferLength          = 2000; // default buffer length in msec
const UINT DefBufferServiceInterval = 250;  // default buffer service interval in msec

// Constructor
AudioStream::AudioStream (void)
{
#ifndef PLAT_UNIX
	InitializeCriticalSection( &write_lock );
#else
	write_lock = SDL_CreateMutex();
#endif
}


// Destructor
AudioStream::~AudioStream (void)
{
#ifndef PLAT_UNIX
	DeleteCriticalSection( &write_lock );
#else
	SDL_DestroyMutex( write_lock );
#endif
}


void AudioStream::Init_Data ()
{
	m_bLooping = 0;
	m_bFade = FALSE;
	m_fade_timer_id = 0;
	m_finished_id = 0;
	m_bPastLimit = FALSE;
	
	m_bDestroy_when_faded = FALSE;
	m_lVolume = 0;
	m_lCutoffVolume = -10000;
	m_bIsPaused = FALSE;
	m_silence_written = 0;
	m_bReadingDone = FALSE;

	m_pwavefile = NULL;
#ifndef PLAT_UNIX
	m_pdsb = NULL;
#else
	memset(m_al_buffer_ids, 0, sizeof(m_al_buffer_ids));
	m_al_multichannel_id = 0;
	m_al_buffer_play = 0;
#endif
	m_fPlaying = m_fCued = FALSE;
	m_lInService = FALSE;
	m_cbBufOffset = 0;
	m_nBufLength = DefBufferLength;
	m_cbBufSize = 0;
	m_nBufService = DefBufferServiceInterval;
	m_nTimeStarted = 0;
}

// Create
BOOL AudioStream::Create (LPSTR pszFilename, AudioStreamServices * pass)
{
	BOOL fRtn = SUCCESS;    // assume success

	Assert(pszFilename);
	Assert(pass);

	m_pass = pass;
	Init_Data();

	if (pszFilename && m_pass) {
		// Create a new WaveFile object
	
		m_pwavefile = (WaveFile *)malloc(sizeof(WaveFile));
		Assert(m_pwavefile);

		if (m_pwavefile) {
			// Call constructor
			m_pwavefile->Init();
			// Open given file
			m_pwavefile->m_bits_per_sample_uncompressed = m_bits_per_sample_uncompressed;
			if (m_pwavefile->Open (pszFilename)) {
				// Calculate sound buffer size in bytes
#ifndef PLAT_UNIX
				// Buffer size is average data rate times length of buffer
				// No need for buffer to be larger than wave data though
				m_cbBufSize = (m_pwavefile->GetUncompressedAvgDataRate () * m_nBufLength) / 1000;
				nprintf(("SOUND", "SOUND => Stream buffer created using %d bytes\n", m_cbBufSize));
				// m_cbBufSize = (m_cbBufSize > m_pwavefile->GetDataSize ()) ? m_pwavefile->GetDataSize () : m_cbBufSize;

				//nprintf(("Sound", "SOUND => average data rate = %d\n\r", m_pwavefile->GetUncompressedAvgDataRate ()));
				//nprintf(("Sound", "SOUND => m_cbBufSize = %d\n\r", m_cbBufSize));

				// Create sound buffer
				HRESULT hr;
				memset (&m_dsbd, 0, sizeof (DSBUFFERDESC));
				m_dsbd.dwSize = sizeof (DSBUFFERDESC);
				m_dsbd.dwBufferBytes = m_cbBufSize;
				m_dsbd.lpwfxFormat = &m_pwavefile->m_wfmt;
				m_dsbd.dwFlags = DSBCAPS_STATIC | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_LOCSOFTWARE;

				hr = (m_pass->GetPDS ())->CreateSoundBuffer (&m_dsbd, &m_pdsb, NULL);
				if (hr == DS_OK) {
					// Cue for playback
					Cue ();
					Snd_sram += m_cbBufSize;
				}
				else {
					// Error, unable to create DirectSound buffer
					nprintf(("Sound", "SOUND => Error, unable to create DirectSound buffer\n\r"));
					if (hr == DSERR_BADFORMAT) {
						nprintf(("Sound", "SOUND => Bad format (probably ADPCM)\n\r"));
					}

					fRtn = FAILURE;
				}
#else

				m_cbBufSize = 
				        (m_nBufLength/1000)*
				        (m_pwavefile->m_wfmt.wBitsPerSample/8) *
				        m_pwavefile->m_wfmt.nChannels*
   				        m_pwavefile->m_wfmt.nSamplesPerSec;
//				nprintf(("SOUND", "SOUND => Stream buffer created using %d bytes\n", m_cbBufSize));
				// Create sound buffer
				alGenBuffers(MAX_AL_BUF, m_al_buffer_ids);
				
				OpenAL_ErrorCheck(return FAILURE);

				alGenSources(1, &m_al_multichannel_id);
				
				OpenAL_ErrorCheck(return FAILURE);

				alSourcef(m_al_multichannel_id,AL_ROLLOFF_FACTOR,0);

				OpenAL_ErrorCheck(return FAILURE);

				alSourcef(m_al_multichannel_id,AL_SOURCE_RELATIVE,AL_TRUE);

				OpenAL_ErrorCheck(return FAILURE);

				ALfloat posv[] = { 0,0,0 };
				alSourcefv(m_al_multichannel_id,AL_POSITION,posv);

				OpenAL_ErrorCheck(return FAILURE);

				alSourcef(m_al_multichannel_id,AL_GAIN,1);

				OpenAL_ErrorCheck(return FAILURE);

				// Cue for playback
				Cue ();
				Snd_sram += m_cbBufSize;
#endif
			}
			else {
				// Error opening file
				nprintf(("SOUND", "SOUND => Failed to open wave file: %s\n\r", pszFilename));
				m_pwavefile->Close();
				free(m_pwavefile);
				m_pwavefile = NULL;
				fRtn = FAILURE;
			}   
		}
		else {
			// Error, unable to create WaveFile object
			nprintf(("Sound", "SOUND => Failed to create WaveFile object %s\n\r", pszFilename));
			fRtn = FAILURE;
		}
	}
	else {
		// Error, passed invalid parms
		fRtn = FAILURE;
	}

	return (fRtn);
}


// Destroy
BOOL AudioStream::Destroy (void)
{
	BOOL fRtn = SUCCESS;

#ifndef PLAT_UNIX
	EnterCriticalSection(&write_lock);
#else
	SDL_LockMutex(write_lock);
#endif
	
	// Stop playback
	Stop ();

	// Release DirectSound buffer
#ifndef PLAT_UNIX
	if (m_pdsb) {
		m_pdsb->Release ();
		m_pdsb = NULL;
		Snd_sram -= m_cbBufSize;
	}
#else
	alDeleteBuffers(MAX_AL_BUF,m_al_buffer_ids);
	alDeleteSources(1,&m_al_multichannel_id);
#endif

	// Delete WaveFile object
	if (m_pwavefile) {
		m_pwavefile->Close();
		free(m_pwavefile);
		m_pwavefile = NULL;
	}

	status = ASF_FREE;

#ifndef PLAT_UNIX
	LeaveCriticalSection(&write_lock);
#else
	SDL_UnlockMutex(write_lock);
#endif

	return fRtn;
}

// WriteWaveData
//
// Writes wave data to sound buffer. This is a helper method used by Create and
// ServiceBuffer; it's not exposed to users of the AudioStream class.
BOOL AudioStream::WriteWaveData (UINT size, UINT *num_bytes_written, int service)
{
#ifndef PLAT_UNIX
	HRESULT hr;
	LPBYTE lpbuf1 = NULL;
	LPBYTE lpbuf2 = NULL;
	DWORD dwsize1 = 0;
	DWORD dwsize2 = 0;
	DWORD dwbyteswritten1 = 0;
	DWORD dwbyteswritten2 = 0;
#endif
	BOOL fRtn = SUCCESS;
	unsigned char	*uncompressed_wave_data;

	*num_bytes_written = 0;

	if ( size == 0 || m_bReadingDone ) {
		return fRtn;
	}

#ifndef PLAT_UNIX
	if ( !m_pdsb || !m_pwavefile ) {
		return fRtn;
	}

	if ( service ) {
		EnterCriticalSection(&Global_service_lock);
	}
#else		    
	if ( (m_al_buffer_ids[0] == 0) || !m_pwavefile ) {
		return fRtn;
	}

	if ( service ) {
		SDL_LockMutex(Global_service_lock);
	}
#endif
		    
	if ( service ) {
		uncompressed_wave_data = Wavedata_service_buffer;
	} else {
		uncompressed_wave_data = Wavedata_load_buffer;
	}

	int num_bytes_read = 0;

#ifndef PLAT_UNIX
    // Lock the sound buffer
	hr = m_pdsb->Lock (m_cbBufOffset, size, (void**)(&lpbuf1), &dwsize1, (void**)(&lpbuf2), &dwsize2, 0);
	if (hr == DS_OK) {
		// Write data to sound buffer. Because the sound buffer is circular, we may have to
		// do two write operations if locked portion of buffer wraps around to start of buffer.
		Assert(lpbuf1);
		
		num_bytes_read = m_pwavefile->Read(uncompressed_wave_data, dwsize1+dwsize2,service);
#else
		num_bytes_read = m_pwavefile->Read(uncompressed_wave_data, m_cbBufSize ,service);
#endif
		if ( num_bytes_read == -1 ) {
			// means nothing left to read!
			num_bytes_read = 0;
			m_bReadingDone = 1;
		}

		if ( num_bytes_read > 0 ) {
#ifndef PLAT_UNIX
			if ( (unsigned int)num_bytes_read > dwsize1 ) {
				dwbyteswritten1 = dwsize1;
				dwbyteswritten2 = num_bytes_read - dwsize1;

				memcpy(lpbuf1, uncompressed_wave_data, dwsize1);
				Assert(lpbuf2);
				memcpy(lpbuf2, uncompressed_wave_data+dwsize1, num_bytes_read-dwsize1);
			} else {
				dwbyteswritten1 = num_bytes_read;
				dwbyteswritten2 = 0;
				memcpy(lpbuf1, uncompressed_wave_data, num_bytes_read);
			}
#else
			nprintf(("SOUND", "SOUND ==> Queueing %d bytes of Data\n", num_bytes_read));
			// Lock the sound buffer
			ALenum format = AL_FORMAT_MONO8;

			if (m_pwavefile->m_wfmt.nChannels == 1) {
				if (m_pwavefile->m_wfmt.wBitsPerSample == 8) 
					format = AL_FORMAT_MONO8;
				else if (m_pwavefile->m_wfmt.wBitsPerSample == 16) 
					format = AL_FORMAT_MONO16;
			} else if (m_pwavefile->m_wfmt.nChannels == 2) {
				if (m_pwavefile->m_wfmt.wBitsPerSample == 8) 
					format = AL_FORMAT_STEREO8;
				else if (m_pwavefile->m_wfmt.wBitsPerSample == 16) 
					format = AL_FORMAT_STEREO16;
			}

			ALuint bid = 0;

			alSourceUnqueueBuffers(m_al_multichannel_id,1,&bid);
				
			alGetError();
			
			alBufferData(m_al_buffer_ids[m_al_buffer_play],format,uncompressed_wave_data,num_bytes_read,m_pwavefile->m_wfmt.nSamplesPerSec);
			
			OpenAL_ErrorCheck(return FAILURE);
			
			alSourceQueueBuffers(m_al_multichannel_id,1,&m_al_buffer_ids[m_al_buffer_play]);
			
			OpenAL_ErrorCheck(return FAILURE);
			
			m_al_buffer_play++;

			if (m_al_buffer_play>=MAX_AL_BUF)
				m_al_buffer_play = 0;
			
			m_cbBufOffset = (m_cbBufOffset + num_bytes_read) % m_cbBufSize;
			*num_bytes_written = num_bytes_read;
#endif
		}

#ifndef PLAT_UNIX			
		// Update our buffer offset and unlock sound buffer
		m_cbBufOffset = (m_cbBufOffset + dwbyteswritten1 + dwbyteswritten2) % m_cbBufSize;
		*num_bytes_written = dwbyteswritten1 + dwbyteswritten2;
		m_pdsb->Unlock (lpbuf1, dwsize1, lpbuf2, dwsize2);
    }
	else {
		// Error locking sound buffer
		nprintf(("SOUND", "SOUND ==> Error, unable to lock sound buffer in AudioStr\n"));
		fRtn = FAILURE;
	}
#endif

	if ( service ) {
#ifndef PLAT_UNIX
		LeaveCriticalSection(&Global_service_lock);
#else
		SDL_UnlockMutex(Global_service_lock);
#endif
	}
    
	return (fRtn);
}

#ifndef PLAT_UNIX 

// WriteSilence
//
// Writes silence to sound buffer. This is a helper method used by
// ServiceBuffer; it's not exposed to users of the AudioStream class.
BOOL AudioStream::WriteSilence (UINT size)
{
	HRESULT hr;
	LPBYTE lpbuf1 = NULL;
	LPBYTE lpbuf2 = NULL;
	DWORD dwsize1 = 0;
	DWORD dwsize2 = 0;
	DWORD dwbyteswritten1 = 0;
	DWORD dwbyteswritten2 = 0;
	BOOL fRtn = SUCCESS;

	// Lock the sound buffer
	hr = m_pdsb->Lock (m_cbBufOffset, size, (void**)(&lpbuf1), &dwsize1, (void**)(&lpbuf2), &dwsize2, 0);
	if (hr == DS_OK) {

		// Get silence data for this file format. Although word sizes vary for different
		// wave file formats, ::Lock will always return pointers on word boundaries.
		// Because silence data for 16-bit PCM formats is 0x0000 or 0x00000000, we can
		// get away with writing bytes and ignoring word size here.
		BYTE bSilence = m_pwavefile->GetSilenceData ();
        
		// Write silence to sound buffer. Because the sound buffer is circular, we may have to
		// do two write operations if locked portion of buffer wraps around to start of buffer.
		memset (lpbuf1, bSilence, dwsize1);
		dwbyteswritten1 = dwsize1;
            
     // Second write required?
		if (lpbuf2) {
			memset (lpbuf2, bSilence, dwsize2);
			dwbyteswritten2 = dwsize2;
		}
            
		// Update our buffer offset and unlock sound buffer
		m_cbBufOffset = (m_cbBufOffset + dwbyteswritten1 + dwbyteswritten2) % m_cbBufSize;
//		m_pdsb->Unlock (lpbuf1, dwbyteswritten1, lpbuf2, dwbyteswritten2);
		m_pdsb->Unlock (lpbuf1, dwsize1, lpbuf2, dwsize2);
	}
	else {
		// Error locking sound buffer
		nprintf(("SOUND", "SOUND ==> Error, unable to lock sound buffer in AudioStr\n"));
		fRtn = FAILURE;
	}

	return (fRtn);
}

#endif


// GetMaxWriteSize
//
// Helper function to calculate max size of sound buffer write operation, i.e. how much
// free space there is in buffer.
DWORD AudioStream::GetMaxWriteSize (void)
{
#ifndef PLAT_UNIX
	DWORD dwWriteCursor, dwPlayCursor, dwMaxSize;

	// Get current play position
	if (m_pdsb->GetCurrentPosition (&dwPlayCursor, &dwWriteCursor) == DS_OK) {
		if (m_cbBufOffset <= dwPlayCursor) {
			// Our write position trails play cursor
			dwMaxSize = dwPlayCursor - m_cbBufOffset;
		}

		else  {// (m_cbBufOffset > dw7Cursor)
			// Play cursor has wrapped
			dwMaxSize = m_cbBufSize - m_cbBufOffset + dwPlayCursor;
		}
	}
	else {
		// GetCurrentPosition call failed
		Int3();
		dwMaxSize = 0;
	}
#else
	DWORD dwMaxSize = m_cbBufSize;
	ALint n,q;

	alGetSourcei(m_al_multichannel_id,AL_BUFFERS_PROCESSED,&n);

	OpenAL_ErrorCheck(return 0);

	alGetSourcei(m_al_multichannel_id,AL_BUFFERS_QUEUED,&q);

	OpenAL_ErrorCheck(return 0);

	if (!n && q>=MAX_AL_BUF) //all buffers queued
		dwMaxSize = 0;
#endif

//	nprintf(("Alan","Max write size: %d\n", dwMaxSize));
	return (dwMaxSize);
}

#ifdef PLAT_UNIX
// GetWritingDone
//
// Helper function to check if all data is written
BOOL AudioStream::GetWritingDone (void)
{
	ALint n;

	alGetSourcei(m_al_multichannel_id,AL_BUFFERS_PROCESSED,&n);

	OpenAL_ErrorCheck(return 1);

	return (n == MAX_AL_BUF); //both buffers processed
	       
}
#endif

// ServiceBuffer
//
// Routine to service buffer requests initiated by periodic timer.
//
// Returns TRUE if buffer serviced normally; otherwise returns FALSE.
#define FADE_VOLUME_INTERVAL	 	 					400		// 100 == 1db
#define VOLUME_ATTENUATION_BEFORE_CUTOFF			3000		//  12db 
BOOL AudioStream::ServiceBuffer (void)
{
	long	vol;
	int	fRtn = TRUE;

	if ( status != ASF_USED )
		return FALSE;

#ifndef PLAT_UNIX
	EnterCriticalSection(&write_lock);
#else
	SDL_LockMutex(write_lock);
#endif

	// status may have changed, so lets check once again
	if ( status != ASF_USED ){
#ifndef PLAT_UNIX
		LeaveCriticalSection(&write_lock);
#else
		SDL_UnlockMutex(write_lock);
#endif
		return FALSE;
	}

	// Check for reentrance
#ifndef PLAT_UNIX
	if (InterlockedExchange (&m_lInService, TRUE) == FALSE) {
#endif
		if ( m_bFade == TRUE ) {
			if ( m_lCutoffVolume == -10000 ) {
				vol = Get_Volume();
//				nprintf(("Alan","Volume is: %d\n",vol));
				m_lCutoffVolume = max(vol - VOLUME_ATTENUATION_BEFORE_CUTOFF, -10000);
			}

			vol = Get_Volume();
			vol = vol - FADE_VOLUME_INTERVAL;	// decrease by 1db
//			nprintf(("Alan","Volume is now: %d\n",vol));
			Set_Volume(vol);

//			nprintf(("Sound","SOUND => Volume for stream sound is %d\n",vol));
//			nprintf(("Alan","Cuttoff Volume is: %d\n",m_lCutoffVolume));
			if ( vol < m_lCutoffVolume ) {
				m_bFade = 0;
				m_lCutoffVolume = -10000;
				if ( m_bDestroy_when_faded == TRUE ) {
#ifndef PLAT_UNIX
					LeaveCriticalSection(&write_lock);
#else
					SDL_UnlockMutex(write_lock);
#endif
					Destroy();	
					// Reset reentrancy semaphore
#ifndef PLAT_UNIX
					InterlockedExchange (&m_lInService, FALSE);
#endif
					return FALSE;
				}
				else {
					Stop_and_Rewind();
					// Reset reentrancy semaphore
#ifndef PLAT_UNIX
					LeaveCriticalSection(&write_lock);
					InterlockedExchange (&m_lInService, FALSE);
#else
					SDL_UnlockMutex(write_lock);
#endif
					return TRUE;
				}
			}
		}

		// All of sound not played yet, send more data to buffer
		DWORD dwFreeSpace = GetMaxWriteSize ();

		// Determine free space in sound buffer
		if (dwFreeSpace) {

			// Some wave data remains, but not enough to fill free space
			// Send wave data to buffer, fill remainder of free space with silence
			uint num_bytes_written;

			if (WriteWaveData (dwFreeSpace, &num_bytes_written) == SUCCESS) {
//				nprintf(("Alan","Num bytes written: %d\n", num_bytes_written));

				if ( m_pwavefile->m_total_uncompressed_bytes_read >= m_pwavefile->m_max_uncompressed_bytes_to_read ) {
					m_fade_timer_id = timer_get_milliseconds() + 1700;		// start fading 1.7 seconds from now
					m_finished_id = timer_get_milliseconds() + 2000;		// 2 seconds left to play out buffer
					m_pwavefile->m_max_uncompressed_bytes_to_read = AS_HIGHEST_MAX;
				}

				if ( (m_fade_timer_id>0) && ((uint)timer_get_milliseconds() > m_fade_timer_id) ) {
					m_fade_timer_id = 0;
					Fade_and_Stop();
				}

				if ( (m_finished_id>0) && ((uint)timer_get_milliseconds() > m_finished_id) ) {
					m_finished_id = 0;
					m_bPastLimit = TRUE;
				}

#ifndef PLAT_UNIX
				if ( (num_bytes_written < dwFreeSpace) && m_bReadingDone ) {
					int num_bytes_silence;
					num_bytes_silence = dwFreeSpace - num_bytes_written;

					if ( num_bytes_silence > 0 ) {

						m_silence_written += num_bytes_silence;
						if (WriteSilence (num_bytes_silence) == FAILURE)	{
							fRtn = FALSE;
							Int3();
						}

						if ( m_silence_written >= m_cbBufSize ) {
							m_silence_written = 0;
#else
				if ( m_bReadingDone && GetWritingDone()) {
#endif

							if ( m_bDestroy_when_faded == TRUE ) {
#ifndef PLAT_UNIX
								LeaveCriticalSection(&write_lock);
#else
								SDL_UnlockMutex(write_lock);
#endif
								Destroy();
								// Reset reentrancy semaphore
#ifndef PLAT_UNIX
								InterlockedExchange (&m_lInService, FALSE);
#endif
								return FALSE;
							}

							// All of sound has played, stop playback or loop again
							if ( m_bLooping && !m_bFade) {
								Play(m_lVolume, m_bLooping);
							}
							else {
								Stop_and_Rewind();
							}
#ifndef PLAT_UNIX
						}
					}
#endif
				}
			}
			else {
				// Error writing wave data
				fRtn = FALSE;
				Int3(); 
			}
		}
#ifndef PLAT_UNIX
        // Reset reentrancy semaphore
        InterlockedExchange (&m_lInService, FALSE);
    } else {
		// Service routine reentered. Do nothing, just return
		fRtn = FALSE;
    }

	LeaveCriticalSection(&write_lock);
#endif

#ifdef PLAT_UNIX
	SDL_UnlockMutex(write_lock);
#endif

	return (fRtn);
}

// Cue
void AudioStream::Cue (void)
{
	UINT num_bytes_written;

	if (!m_fCued) {
		m_bFade = FALSE;
		m_fade_timer_id = 0;
		m_finished_id = 0;
		m_bPastLimit = FALSE;
		m_lVolume = 0;
		m_lCutoffVolume = -10000;

		m_bDestroy_when_faded = FALSE;

		// Reset buffer ptr
		m_cbBufOffset = 0;

		// Reset file ptr, etc
		m_pwavefile->Cue ();

#ifndef PLAT_UNIX
		// Reset DirectSound buffer
		m_pdsb->SetCurrentPosition (0);
#else
		// Unqueue all buffers
		alSourceUnqueueBuffers(m_al_multichannel_id,MAX_AL_BUF,m_al_buffer_ids);
#endif

		// Fill buffer with wave data
		WriteWaveData (m_cbBufSize, &num_bytes_written,0);

		m_fCued = TRUE;
	}
}


// Play
void AudioStream::Play (long volume, int looping)
{
#ifndef PLAT_UNIX
	if (m_pdsb) {
#else
	if (m_al_buffer_ids[0] != 0) {
#endif
		// If playing, stop
		if (m_fPlaying) {
			if ( m_bIsPaused == FALSE)
			Stop_and_Rewind();
		}

		// Cue for playback if necessary
		if (!m_fCued) {
			Cue ();
		}

		if ( looping )
			m_bLooping = 1;
		else
			m_bLooping = 0;

#ifndef PLAT_UNIX
		// Begin DirectSound playback
		HRESULT hr = m_pdsb->Play (0, 0, DSBPLAY_LOOPING);
		if (hr == DS_OK) {
#else
		alSourcePlay(m_al_multichannel_id);
		
		OpenAL_ErrorCheck(return);
#endif
			m_nTimeStarted = timer_get_milliseconds();
			Set_Volume(volume);
			// Kick off timer to service buffer
			m_timer.constructor();

			m_timer.Create (m_nBufService, m_nBufService, ptr_u (this), TimerCallback);

			// Playback begun, no longer cued
			m_fPlaying = TRUE;
			m_bIsPaused = FALSE;
#ifndef PLAT_UNIX
		}
		else {
			// If the buffer was lost, try to restore it
			if ( hr == DSERR_BUFFERLOST ) {
				hr = m_pdsb->Restore();
				if ( hr == DS_OK ) {
					hr = m_pdsb->Play (0, 0, DSBPLAY_LOOPING);
				}
				else {
					nprintf(("Sound", "Sound => Lost a buffer, tried restoring but got %s\n", get_DSERR_text(hr) ));
					Int3();	// get Alan, he wants to see this
				}
			}

			if ( hr != DS_OK ) {
				nprintf(("Sound", "Sound => Play failed with return value %s\n", get_DSERR_text(hr) ));
			}
		}
#endif
	}
}

// Timer callback for Timer object created by ::Play method.
BOOL AudioStream::TimerCallback (ptr_u dwUser)
{
    // dwUser contains ptr to AudioStream object
    AudioStream * pas = (AudioStream *) dwUser;

    return (pas->ServiceBuffer ());
}

void AudioStream::Set_Byte_Cutoff(unsigned int byte_cutoff)
{
	if ( m_pwavefile == NULL )
		return;

	m_pwavefile->m_max_uncompressed_bytes_to_read = byte_cutoff;
}

unsigned int AudioStream::Get_Bytes_Committed(void)
{
	if ( m_pwavefile == NULL )
		return 0;

	return m_pwavefile->m_total_uncompressed_bytes_read;
}


// Fade_and_Destroy
void AudioStream::Fade_and_Destroy (void)
{
	m_bFade = TRUE;
	m_bDestroy_when_faded = TRUE;
}

// Fade_and_Destroy
void AudioStream::Fade_and_Stop (void)
{
	m_bFade = TRUE;
	m_bDestroy_when_faded = FALSE;
}


// Stop
void AudioStream::Stop(int paused)
{
	if (m_fPlaying) {
		// Stop DirectSound playback
#ifndef PLAT_UNIX
		m_pdsb->Stop ();
#else
		if (paused) {
			alSourcePause(m_al_multichannel_id);
		} else {
			alSourceStop(m_al_multichannel_id);
		}
#endif
		m_fPlaying = FALSE;
		m_bIsPaused = paused;

		// Delete Timer object
		m_timer.destructor();
	}
}

// Stop_and_Rewind
void AudioStream::Stop_and_Rewind (void)
{
	if (m_fPlaying) {
		// Stop DirectSound playback
#ifndef PLAT_UNIX
		m_pdsb->Stop ();
#else
		alSourceStop(m_al_multichannel_id);
		OpenAL_ErrorCheck(return);
#endif

		// Delete Timer object
		m_timer.destructor();

		m_fPlaying = FALSE;
	}

	m_fCued = FALSE;	// this will cause wave file to start from beginning
	m_bReadingDone = FALSE;
}

// Set_Volume
void AudioStream::Set_Volume(long vol)
{
	if ( vol < -10000 )
		vol = -10000;
	
	if ( vol > 0 )
		vol = 0;

	Assert( vol >= -10000 && vol <= 0 );

#ifndef PLAT_UNIX
	h_result = m_pdsb->SetVolume(vol);
#else
	ALfloat alvol = (vol != -10000) ? pow(10.0, (float)vol / (-600.0 / log10(.5))): 0.0;

	alSourcef(m_al_multichannel_id,AL_GAIN,alvol);
#endif

	m_lVolume = vol;

#ifndef PLAT_UNIX
	if ( h_result != DS_OK )
		nprintf(("Sound","SOUND => SetVolume() failed with code '%s'\n", get_DSERR_text(h_result) ));
#endif
}


// Get_Volume
long AudioStream::Get_Volume()
{
	return m_lVolume;
}

// constructor
void Timer::constructor(void)
{
	m_nIDTimer = NULL;
#ifdef PLAT_UNIX
	SDL_InitSubSystem(SDL_INIT_TIMER);
#endif
}


// Destructor
void Timer::destructor(void)
{
	if (m_nIDTimer) {
#ifndef PLAT_UNIX
		timeKillEvent (m_nIDTimer);
#else
		SDL_RemoveTimer (m_nIDTimer);
#endif
		m_nIDTimer = NULL;
	}
}


// Create
BOOL Timer::Create (UINT nPeriod, UINT nRes, ptr_u dwUser, TIMERCALLBACK pfnCallback)
{
	BOOL bRtn = SUCCESS;    // assume success

	Assert(pfnCallback);
	Assert(nPeriod > 10);
	Assert(nPeriod >= nRes);

	m_nPeriod = nPeriod;
	m_nRes = nRes;
	m_dwUser = dwUser;
	m_pfnCallback = pfnCallback;

#ifndef PLAT_UNIX
	if ((m_nIDTimer = timeSetEvent (m_nPeriod, m_nRes, TimeProc, (DWORD) this, TIME_PERIODIC)) == NULL) {
#else
	if ((m_nIDTimer = SDL_AddTimer (m_nPeriod, TimeProc, (void*) this)) == NULL) {
	    nprintf(("SOUND", "SOUND ==> Error, unable to create timer\n"));
#endif
	  bRtn = FAILURE;
	}

	return (bRtn);
}


// Timer proc for multimedia timer callback set with timeSetTime().
//
// Calls procedure specified when Timer object was created. The 
// dwUser parameter contains "this" pointer for associated Timer object.
// 
#ifndef PLAT_UNIX
void CALLBACK Timer::TimeProc(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
#else
Uint32 CALLBACK Timer::TimeProc(Uint32 interval, void *dwUser)
#endif
{
    // dwUser contains ptr to Timer object
    Timer * ptimer = (Timer *) dwUser;

    // Call user-specified callback and pass back user specified data
    (ptimer->m_pfnCallback) (ptimer->m_dwUser);
#ifdef PLAT_UNIX
    if (ptimer->m_nPeriod)
	return interval;
    else {
	SDL_RemoveTimer(ptimer->m_nIDTimer);
	ptimer->m_nIDTimer = NULL;
	return 0;
    }
#endif
}


// WaveFile class implementation
//
////////////////////////////////////////////////////////////

// Constructor
void WaveFile::Init(void)
{
	// Init data members
	m_data_offset = 0;
	cfp = NULL;
	m_pwfmt_original = NULL;
	m_nBlockAlign= 0;
	m_nUncompressedAvgDataRate = 0;
	m_nDataSize = 0;
	m_nBytesPlayed = 0;
	m_total_uncompressed_bytes_read = 0;
	m_max_uncompressed_bytes_to_read = AS_HIGHEST_MAX;

	m_hStream_open = 0;
	m_abort_next_read = FALSE;
}

// Destructor
void WaveFile::Close(void)
{
	// Free memory
	if (m_pwfmt_original) {
		free(m_pwfmt_original);
		m_pwfmt_original = NULL;
	}

	if ( m_hStream_open ) {
		ACM_stream_close((void*)m_hStream);
		m_hStream_open = 0;
	}

	// Close file
	if (cfp) {
#ifndef PLAT_UNIX
		mmioClose( cfp, 0 );
#else
		SDL_RWclose(cfp);
#endif
		cfp = NULL;
	}
}


// Open
BOOL WaveFile::Open (LPSTR pszFilename)
{
	int done = FALSE;
	WORD cbExtra = 0;
	BOOL fRtn = SUCCESS;    // assume success
	PCMWAVEFORMAT pcmwf;
	char fullpath[_MAX_PATH];

	m_total_uncompressed_bytes_read = 0;
	m_max_uncompressed_bytes_to_read = AS_HIGHEST_MAX;

	int FileSize, FileOffset;

	if ( !cf_find_file_location(pszFilename, CF_TYPE_ANY, fullpath, &FileSize, &FileOffset ))	{
		goto OPEN_ERROR;
	}

#ifndef PLAT_UNIX
	cfp = mmioOpen(fullpath, NULL, MMIO_ALLOCBUF | MMIO_READ);
#else
	cfp = SDL_RWFromFile(fullpath, "rb");
#endif

	if ( cfp == NULL ) {
		goto OPEN_ERROR;
	}

	// Skip the "RIFF" tag and file size (8 bytes)
	// Skip the "WAVE" tag (4 bytes)
#ifndef PLAT_UNIX
	mmioSeek( cfp, 12+FileOffset, SEEK_SET );
#else
	SDL_RWseek( cfp, 12+FileOffset, SEEK_SET );
#endif
	// Now read RIFF tags until the end of file
	uint tag, size, next_chunk;

	while(done == FALSE)	{
#ifndef PLAT_UNIX
		if ( mmioRead(cfp, (char *)&tag, sizeof(uint)) != sizeof(uint) )
#else
		if ( SDL_RWread(cfp, &tag, 1, sizeof(uint)) != sizeof(uint) )
#endif
			break;

		tag = INTEL_INT( tag );

#ifndef PLAT_UNIX
		if ( mmioRead(cfp, (char *)&size, sizeof(uint)) != sizeof(uint) )
#else
		if ( SDL_RWread(cfp, &size, 1, sizeof(uint)) != sizeof(uint) )
#endif
			break;

		size = INTEL_INT( size );

#ifndef PLAT_UNIX
		next_chunk = mmioSeek( cfp, 0, SEEK_CUR );
#else
		next_chunk = SDL_RWtell( cfp );
#endif
		next_chunk += size;

		switch( tag )	{
		case 0x20746d66:		// The 'fmt ' tag
#ifndef PLAT_UNIX
			mmioRead( cfp, (char *)&pcmwf, sizeof(PCMWAVEFORMAT) );
			if ( pcmwf.wf.wFormatTag != WAVE_FORMAT_PCM ) {
				mmioRead( cfp, (char *)&cbExtra, sizeof(short) );
			}
#else
			audiostr_read_word(cfp, &pcmwf.wf.wFormatTag);
			audiostr_read_word(cfp, &pcmwf.wf.nChannels);
			audiostr_read_dword(cfp, &pcmwf.wf.nSamplesPerSec);
			audiostr_read_dword(cfp, &pcmwf.wf.nAvgBytesPerSec);
			audiostr_read_word(cfp, &pcmwf.wf.nBlockAlign);
			audiostr_read_word(cfp, &pcmwf.wBitsPerSample);

			if ( pcmwf.wf.wFormatTag != WAVE_FORMAT_PCM ) {
				audiostr_read_word(cfp, &cbExtra);
			}
#endif

			// Allocate memory for WAVEFORMATEX structure + extra bytes
			if ( (m_pwfmt_original = (WAVEFORMATEX *) malloc ( sizeof(WAVEFORMATEX)+cbExtra )) != NULL ){
				Assert(m_pwfmt_original != NULL);
				// Copy bytes from temporary format structure
				memcpy (m_pwfmt_original, &pcmwf, sizeof(pcmwf));
				m_pwfmt_original->cbSize = cbExtra;

				// Read those extra bytes, append to WAVEFORMATEX structure
				if (cbExtra != 0) {
#ifndef PLAT_UNIX
					mmioRead( cfp, (char *)((ubyte *)(m_pwfmt_original) + sizeof(WAVEFORMATEX)), cbExtra );
#else
					SDL_RWread( cfp, ((ubyte *)(m_pwfmt_original) + sizeof(WAVEFORMATEX)), 1, cbExtra );
#endif
				}
			}
			else {
				Int3();		// malloc failed
				goto OPEN_ERROR;
			}	
			break;

		case 0x61746164:		// the 'data' tag
			m_nDataSize = size;	// This is size of data chunk.  Compressed if ADPCM.
			m_data_bytes_left = size;
#ifndef PLAT_UNIX
			m_data_offset = mmioSeek( cfp, 0, SEEK_CUR);
#else
			m_data_offset = SDL_RWtell( cfp );
#endif
			done = TRUE;
			break;

		default:	// unknown, skip it
			break;
		}	// end switch

#ifndef PLAT_UNIX
		mmioSeek( cfp, next_chunk, SEEK_SET );
#else
		SDL_RWseek( cfp, next_chunk, SEEK_SET );
#endif
	}

  	// At this stage, examine source format, and set up WAVEFORATEX structure for DirectSound.
	// Since DirectSound only supports PCM, force this structure to be PCM compliant.  We will
	// need to convert data on the fly later if our souce is not PCM
	switch ( m_pwfmt_original->wFormatTag ) {
		case WAVE_FORMAT_PCM:
			m_wave_format = WAVE_FORMAT_PCM;
			m_wfmt.wBitsPerSample = m_pwfmt_original->wBitsPerSample;
			break;

		case WAVE_FORMAT_ADPCM:
			m_wave_format = WAVE_FORMAT_ADPCM;
			m_wfmt.wBitsPerSample = 16;
			m_bits_per_sample_uncompressed = 16;
			break;

		default:
			nprintf(("SOUND", "SOUND => Not supporting %d format for playing wave files\n"));
			//Int3();
			goto OPEN_ERROR;
			break;

	} // end switch
            
	// Set up the WAVEFORMATEX structure to have the right PCM characteristics
	m_wfmt.wFormatTag = WAVE_FORMAT_PCM;
	m_wfmt.nChannels = m_pwfmt_original->nChannels;
	m_wfmt.nSamplesPerSec = m_pwfmt_original->nSamplesPerSec;
	m_wfmt.cbSize = 0;
	m_wfmt.nBlockAlign = (unsigned short)(( m_wfmt.nChannels * m_wfmt.wBitsPerSample ) / 8);
	m_wfmt.nAvgBytesPerSec = m_wfmt.nBlockAlign * m_wfmt.nSamplesPerSec;

	// Init some member data from format chunk
	m_nBlockAlign = m_pwfmt_original->nBlockAlign;
	m_nUncompressedAvgDataRate = m_wfmt.nAvgBytesPerSec;

	// Cue for streaming
	Cue ();
 
	// Successful open
	goto OPEN_DONE;
    
OPEN_ERROR:
	// Handle all errors here
	nprintf(("SOUND","SOUND ==> Could not open wave file %s for streaming\n",pszFilename));

	fRtn = FAILURE;
	if (cfp != NULL) {
		// Close file
#ifndef PLAT_UNIX
		mmioClose( cfp, 0 );
#else
		SDL_RWclose( cfp );
#endif
		cfp = NULL;
	}
	if (m_pwfmt_original)
	{
		free(m_pwfmt_original);
		m_pwfmt_original = NULL;
	}

OPEN_DONE:
	return (fRtn);
}


// Cue
//
// Set the file pointer to the start of wave data
//
BOOL WaveFile::Cue (void)
{
	BOOL fRtn = SUCCESS;    // assume success
	int rval;

	m_total_uncompressed_bytes_read = 0;
	m_max_uncompressed_bytes_to_read = AS_HIGHEST_MAX;

#ifndef PLAT_UNIX
	rval = mmioSeek( cfp, m_data_offset, SEEK_SET );
#else
	rval = SDL_RWseek( cfp, m_data_offset, SEEK_SET );
#endif
	if ( rval == -1 ) {
		fRtn = FAILURE;
	}

	m_data_bytes_left = m_nDataSize;
	m_abort_next_read = FALSE;

	return fRtn;
}


// Read
//
// Returns number of bytes actually read.
// 
//	Returns -1 if there is nothing more to be read.  This function can return 0, since
// sometimes the amount of bytes requested is too small for the ACM decompression to 
// locate a suitable block
int WaveFile::Read(BYTE *pbDest, UINT cbSize, int service)
{
	unsigned char	*dest_buf=NULL, *uncompressed_wave_data;
	int				rc, uncompressed_bytes_written;
	unsigned int	src_bytes_used, convert_len, num_bytes_desired=0, num_bytes_read;

//	nprintf(("Alan","Reqeusted: %d\n", cbSize));


	if ( service ) {
		uncompressed_wave_data = Wavedata_service_buffer;
	} else {
		uncompressed_wave_data = Wavedata_load_buffer;
	}

	switch ( m_wave_format ) {
		case WAVE_FORMAT_PCM:
			num_bytes_desired = cbSize;
			dest_buf = pbDest;
			break;

		case WAVE_FORMAT_ADPCM:
			if ( !m_hStream_open ) {
				if ( !ACM_stream_open(m_pwfmt_original, &m_wfxDest, (void**)&m_hStream, m_bits_per_sample_uncompressed)  ) {
					m_hStream_open = 1;
				} else {
					Int3();
					return -1;
				}
			}

			num_bytes_desired = cbSize;
	
			if ( service ) {
				dest_buf = Compressed_service_buffer;
			} else {
				dest_buf = Compressed_buffer;
			}

			if ( num_bytes_desired <= 0 ) {
				num_bytes_desired = 0;
//				nprintf(("Alan","No bytes required for ADPCM time interval\n"));
			} else {
				num_bytes_desired = ACM_query_source_size((void*)m_hStream, cbSize);
//				nprintf(("Alan","Num bytes desired: %d\n", num_bytes_desired));
			}
			break;

		default:
			nprintf(("SOUND", "SOUND => Not supporting %d format for playing wave files\n"));
			Int3();
			return -1;
			break;

	} // end switch
                
	num_bytes_read = 0;
	convert_len = 0;
	src_bytes_used = 0;

	// read data from disk
	if ( m_data_bytes_left <= 0 ) {
		num_bytes_read = 0;
		uncompressed_bytes_written = 0;
		return -1;
	}

	if ( m_data_bytes_left > 0 && num_bytes_desired > 0 ) {
		int actual_read;

		if ( num_bytes_desired <= (unsigned int)m_data_bytes_left ) {
			num_bytes_read = num_bytes_desired;
		}
		else {
			num_bytes_read = m_data_bytes_left;
		}

#ifndef PLAT_UNIX
		actual_read = mmioRead( cfp, (char *)dest_buf, num_bytes_read );
#else
		actual_read = SDL_RWread( cfp, dest_buf, 1, num_bytes_read );
#endif
		if ( (actual_read <= 0) || (m_abort_next_read) ) {
			num_bytes_read = 0;
			uncompressed_bytes_written = 0;
			return -1;
		}

		if ( num_bytes_desired >= (unsigned int)m_data_bytes_left ) {
			m_abort_next_read = 1;			
		}

		num_bytes_read = actual_read;
	}

	// convert data if necessary, to PCM
	if ( m_wave_format == WAVE_FORMAT_ADPCM ) {
		if ( num_bytes_read > 0 ) {
				rc = ACM_convert((void*)m_hStream, dest_buf, num_bytes_read, uncompressed_wave_data, BIGBUF_SIZE, &convert_len, &src_bytes_used);
				if ( rc == -1 ) {
					goto READ_ERROR;
				}
				if ( convert_len == 0 ) {
					Int3();
					goto READ_ERROR;
				}
		}

		Assert(src_bytes_used <= num_bytes_read);
		if ( src_bytes_used < num_bytes_read ) {
			// seek back file pointer to reposition before unused source data
#ifndef PLAT_UNIX
			mmioSeek(cfp, src_bytes_used - num_bytes_read, SEEK_CUR);
#else
			SDL_RWseek( cfp, src_bytes_used - num_bytes_read, SEEK_CUR );
#endif
		}

		// Adjust number of bytes left
		m_data_bytes_left -= src_bytes_used;
		m_nBytesPlayed += src_bytes_used;
		uncompressed_bytes_written = convert_len;

		// Successful read, keep running total of number of data bytes read
		goto READ_DONE;
	}
	else {
		// Successful read, keep running total of number of data bytes read
		// Adjust number of bytes left
		m_data_bytes_left -= num_bytes_read;
		m_nBytesPlayed += num_bytes_read;
		uncompressed_bytes_written = num_bytes_read;
		goto READ_DONE;
	}
    
READ_ERROR:
	num_bytes_read = 0;
	uncompressed_bytes_written = 0;

READ_DONE:
	m_total_uncompressed_bytes_read += uncompressed_bytes_written;
//	nprintf(("Alan","Read: %d\n", uncompressed_bytes_written));
	return (uncompressed_bytes_written);
}


#ifndef PLAT_UNIX
// GetSilenceData
//
// Returns 8 bits of data representing silence for the Wave file format.
//
// Since we are dealing only with PCM format, we can fudge a bit and take
// advantage of the fact that for all PCM formats, silence can be represented
// by a single byte, repeated to make up the proper word size. The actual size
// of a word of wave data depends on the format:
//
// PCM Format       Word Size       Silence Data
// 8-bit mono       1 byte          0x80
// 8-bit stereo     2 bytes         0x8080
// 16-bit mono      2 bytes         0x0000
// 16-bit stereo    4 bytes         0x00000000
//
BYTE WaveFile::GetSilenceData (void)
{
	BYTE bSilenceData = 0;

	// Silence data depends on format of Wave file
	if (m_pwfmt_original) {
		if (m_wfmt.wBitsPerSample == 8) {
			// For 8-bit formats (unsigned, 0 to 255)
			// Packed DWORD = 0x80808080;
			bSilenceData = 0x80;
		}
		else if (m_wfmt.wBitsPerSample == 16) {
			// For 16-bit formats (signed, -32768 to 32767)
			// Packed DWORD = 0x00000000;
			bSilenceData = 0x00;
		}
		else {
			Int3();
			return -1;
		}
	}
	else {
		Int3();
		return -1;
	}

	return (bSilenceData);
}

#endif

AudioStreamServices * m_pass = NULL;   // ptr to AudioStreamServices object

#define MAX_AUDIO_STREAMS	30
AudioStream Audio_streams[MAX_AUDIO_STREAMS];

int Audiostream_inited = 0;

void audiostream_init()
{
	if ( Audiostream_inited == 1 )
		return;
		
	int i;

	if ( !ACM_is_inited() ) {
		return;
	}

	// Create and initialize AudioStreamServices object.
	// This must be done once and only once for each window that uses
	// streaming services.
	m_pass = (AudioStreamServices *)malloc(sizeof(AudioStreamServices));

	if (m_pass)	{
		m_pass->Constructor();
		m_pass->Initialize();
#ifndef PLAT_UNIX
		if ( !pDirectSound ) {
			return;
		}
#endif
	}

	// Allocate memory for the buffer which holds the uncompressed wave data that is streamed from the
	// disk during a load/cue
	if ( Wavedata_load_buffer == NULL ) {
		Wavedata_load_buffer = (unsigned char*)malloc(BIGBUF_SIZE);
		Assert(Wavedata_load_buffer != NULL);
	}

	// Allocate memory for the buffer which holds the uncompressed wave data that is streamed from the
	// disk during a service interval
	if ( Wavedata_service_buffer == NULL ) {
		Wavedata_service_buffer = (unsigned char*)malloc(BIGBUF_SIZE);
		Assert(Wavedata_service_buffer != NULL);
	}

	// Allocate memory for the buffer which holds the compressed wave data that is read from the hard disk
	if ( Compressed_buffer == NULL ) {
		Compressed_buffer = (unsigned char*)malloc(COMPRESSED_BUFFER_SIZE);
		Assert(Compressed_buffer != NULL);
	}

	if ( Compressed_service_buffer == NULL ) {
		Compressed_service_buffer = (unsigned char*)malloc(COMPRESSED_BUFFER_SIZE);
		Assert(Compressed_service_buffer != NULL);
	}

	for ( i = 0; i < MAX_AUDIO_STREAMS; i++ ) {
		Audio_streams[i].Init_Data();
		Audio_streams[i].status = ASF_FREE;
		Audio_streams[i].type = ASF_NONE;
	}

#ifndef PLAT_UNIX
	InitializeCriticalSection( &Global_service_lock );
#else
	Global_service_lock = SDL_CreateMutex( );
#endif

	Audiostream_inited = 1;
}

// Close down the audiostream system.  Must call audiostream_init() before any audiostream functions can
// be used.
void audiostream_close()
{
	if ( Audiostream_inited == 0 )
		return;

	int i;

	for ( i = 0; i < MAX_AUDIO_STREAMS; i++ ) {
		if ( Audio_streams[i].status == ASF_USED ) {
			Audio_streams[i].status = ASF_FREE;
			Audio_streams[i].Destroy();
		}
	}

	// Destroy AudioStreamServices object
	if (m_pass)	{
		free(m_pass);
		m_pass = NULL;
	}

	// free global buffers
	if ( Wavedata_load_buffer ) {
		free(Wavedata_load_buffer);
		Wavedata_load_buffer = NULL;
	}

	if ( Wavedata_service_buffer ) {
		free(Wavedata_service_buffer);
		Wavedata_service_buffer = NULL;
	}

	if ( Compressed_buffer ) {
		free(Compressed_buffer);
		Compressed_buffer = NULL;
	}

	if ( Compressed_service_buffer ) {
		free(Compressed_service_buffer);
		Compressed_service_buffer = NULL;
	}

#ifndef PLAT_UNIX
	DeleteCriticalSection( &Global_service_lock );
#else
	SDL_DestroyMutex( Global_service_lock );
#endif
	Audiostream_inited = 0;
}

// Open a digital sound file for streaming
//
// input:	filename	=>	disk filename of sound file
//				type		=> what type of audio stream do we want to open:
//									ASF_SOUNDFX
//									ASF_EVENTMUSIC
//									ASF_VOICE
//	
// returns:	success => handle to identify streaming sound
//				failure => -1
int audiostream_open( char * filename, int type )
{

	int i, rc;
	if (!Audiostream_inited || !snd_is_inited())
		return -1;

	for ( i = 0; i < MAX_AUDIO_STREAMS; i++ ) {
		if ( Audio_streams[i].status == ASF_FREE ) {
			Audio_streams[i].status = ASF_USED;
			Audio_streams[i].type = type;
			break;
		}
	}

	if ( i == MAX_AUDIO_STREAMS ) {
		nprintf(("Sound", "SOUND => No more audio streams available!\n"));
		return -1;
	}

	switch(type) {
	case ASF_VOICE:
	case ASF_SOUNDFX:
		Audio_streams[i].m_bits_per_sample_uncompressed = 8;
		break;
	case ASF_EVENTMUSIC:
		Audio_streams[i].m_bits_per_sample_uncompressed = 16;
		break;
	default:
		Int3();
		return -1;
	}
	
	rc = Audio_streams[i].Create(filename, m_pass);
	if ( rc == 0 ) {
		Audio_streams[i].status = ASF_FREE;
		return -1;
	}
	else
		return i;
}


void audiostream_close_file(int i, int fade)
{
	if (!Audiostream_inited)
		return;

	if ( i == -1 )
		return;

	Assert( i >= 0 && i < MAX_AUDIO_STREAMS );

	if ( Audio_streams[i].status == ASF_USED ) {
		if ( fade == TRUE ) {
			Audio_streams[i].Fade_and_Destroy();
		}
		else {
			Audio_streams[i].Destroy();
		}
	}
}

void audiostream_close_all(int fade)
{
	int i;

	for ( i = 0; i < MAX_AUDIO_STREAMS; i++ ) {
		if ( Audio_streams[i].status == ASF_FREE )
			continue;

		audiostream_close_file(i, fade);
	}
}

extern int ds_convert_volume(float volume);

void audiostream_play(int i, float volume, int looping)
{
	if (!Audiostream_inited)
		return;

	if ( i == -1 )
		return;

	Assert(looping >= 0);
	Assert( i >= 0 && i < MAX_AUDIO_STREAMS );

	// convert from 0->1 to -10000->0 for volume
	int converted_volume;
	if ( volume == -1 ) {
		converted_volume = Audio_streams[i].Get_Default_Volume();
	}
	else {
		Assert(volume >= 0.0f && volume <= 1.0f );
		converted_volume = ds_convert_volume(volume);
	}

	Assert( Audio_streams[i].status == ASF_USED );
	Audio_streams[i].Set_Default_Volume(converted_volume);
	Audio_streams[i].Play(converted_volume, looping);
}

void audiostream_stop(int i, int rewind, int paused)
{
	if (!Audiostream_inited) return;

	if ( i == -1 )
		return;

	Assert( i >= 0 && i < MAX_AUDIO_STREAMS );
	Assert( Audio_streams[i].status == ASF_USED );

	if ( rewind )
		Audio_streams[i].Stop_and_Rewind();
	else
		Audio_streams[i].Stop(paused);
}

int audiostream_is_playing(int i)
{
	if ( i == -1 )
		return 0;

	Assert( i >= 0 && i < MAX_AUDIO_STREAMS );
	if ( Audio_streams[i].status != ASF_USED )
		return 0;

	return Audio_streams[i].Is_Playing();
}


void audiostream_set_volume_all(float volume, int type)
{
	int i;

	for ( i = 0; i < MAX_AUDIO_STREAMS; i++ ) {
		if ( Audio_streams[i].status == ASF_FREE )
			continue;

		if ( Audio_streams[i].type == type ) {
			int converted_volume;
			converted_volume = ds_convert_volume(volume);
			Audio_streams[i].Set_Volume(converted_volume);
		}
	}
}


void audiostream_set_volume(int i, float volume)
{
	if ( i == -1 )
		return;

	Assert( i >= 0 && i < MAX_AUDIO_STREAMS );
	Assert( volume >= 0 && volume <= 1);

	if ( Audio_streams[i].status == ASF_FREE )
		return;

	int converted_volume;
	converted_volume = ds_convert_volume(volume);
	Audio_streams[i].Set_Volume(converted_volume);
}


int audiostream_is_paused(int i)
{
	if ( i == -1 )
		return 0;

	Assert( i >= 0 && i < MAX_AUDIO_STREAMS );
	if ( Audio_streams[i].status == ASF_FREE )
		return -1;

	BOOL is_paused;
	is_paused = Audio_streams[i].Is_Paused();
	return is_paused;
}


void audiostream_set_byte_cutoff(int i, unsigned int cutoff)
{
	if ( i == -1 )
		return;

	Assert( i >= 0 && i < MAX_AUDIO_STREAMS );
	Assert( cutoff > 0 );

	if ( Audio_streams[i].status == ASF_FREE )
		return;

	Audio_streams[i].Set_Byte_Cutoff(cutoff);
}


unsigned int audiostream_get_bytes_committed(int i)
{
	if ( i == -1 )
		return 0;

	Assert( i >= 0 && i < MAX_AUDIO_STREAMS );

	if ( Audio_streams[i].status == ASF_FREE )
		return 0;

	unsigned int num_bytes_committed;
	num_bytes_committed = Audio_streams[i].Get_Bytes_Committed();
	return num_bytes_committed;
}

int audiostream_done_reading(int i)
{
	if ( i == -1 )
		return 0;

	Assert( i >= 0 && i < MAX_AUDIO_STREAMS );

	if ( Audio_streams[i].status == ASF_FREE )
		return 0;

	int done_reading;
	done_reading = Audio_streams[i].Is_Past_Limit();
	return done_reading;
}


int audiostream_is_inited()
{
	return Audiostream_inited;
}

// pause a single audio stream, indentified by handle i.
void audiostream_pause(int i)
{
	if ( i == -1 )
		return;

	Assert( i >= 0 && i < MAX_AUDIO_STREAMS );
	if ( Audio_streams[i].status == ASF_FREE )
		return;

	if ( audiostream_is_playing(i) == TRUE ) {
		audiostream_stop(i, 0, 1);
	}
}

// pause all audio streams that are currently playing.
void audiostream_pause_all()
{
	int i;

	for ( i = 0; i < MAX_AUDIO_STREAMS; i++ ) {
		if ( Audio_streams[i].status == ASF_FREE )
			continue;

		audiostream_pause(i);
	}
}

// unpause the audio stream identified by handle i.
void audiostream_unpause(int i)
{
	if ( i == -1 )
		return;

	int is_looping;

	Assert( i >= 0 && i < MAX_AUDIO_STREAMS );
	if ( Audio_streams[i].status == ASF_FREE )
		return;

	if ( audiostream_is_paused(i) == TRUE ) {
		is_looping = Audio_streams[i].Is_looping();
		audiostream_play(i, -1.0f, is_looping);
	}
}

// unpause all audio streams that are currently paused
void audiostream_unpause_all()
{
	int i;

	for ( i = 0; i < MAX_AUDIO_STREAMS; i++ ) {
		if ( Audio_streams[i].status == ASF_FREE )
			continue;

		audiostream_unpause(i);
	}
}

