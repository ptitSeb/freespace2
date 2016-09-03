/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/OsApi/OsApi.cpp $
 * $Revision: 307 $
 * $Date: 2010-02-08 09:09:13 +0100 (Mon, 08 Feb 2010) $
 * $Author: taylor $
 *
 * Low level Windows code
 *
 * $Log$
 * Revision 1.18  2005/10/01 21:49:11  taylor
 * don't use CTRL-Z for minimizing since it's an extrememly common key combo in the game
 *
 * Revision 1.17  2004/12/15 04:10:45  taylor
 * outwnd_unix.cpp from fs2_open for logging to file in debug mode
 * fixes for default function values
 * always use vm_* functions for sanity sake
 * make cfilearchiver 64-bit compatible
 * fix crash on exit from double free()
 * fix crash on startup from extra long GL extension string in debug
 *
 * Revision 1.16  2003/12/15 06:24:51  theoddone33
 * Bumpy ride... hang on.
 *
 * Revision 1.15  2003/08/03 15:56:59  taylor
 * simpler mouse usage; default ini settings in os_init(); cleanup
 *
 * Revision 1.14  2003/05/09 05:04:15  taylor
 * better window min/max/focus support
 *
 * Revision 1.13  2003/05/04 04:56:53  taylor
 * move SDL_Quit to os_deinit to fix fonttool segfault
 *
 * Revision 1.12  2003/02/20 17:41:07  theoddone33
 * Userdir patch from Taylor Richards
 *
 * Revision 1.11  2002/07/28 21:45:30  theoddone33
 * Add ctrl-z to iconify window
 *
 * Revision 1.10  2002/07/28 21:39:44  theoddone33
 * Add alt-enter to toggle fullscreen and ctrl-g to toggle mouse grabbing
 *
 * Revision 1.9  2002/06/16 23:59:31  relnev
 * untested joystick code
 *
 * Revision 1.8  2002/06/09 04:41:25  relnev
 * added copyright header
 *
 * Revision 1.7  2002/06/05 04:03:32  relnev
 * finished cfilesystem.
 *
 * removed some old code.
 *
 * fixed mouse save off-by-one.
 *
 * sound cleanups.
 *
 * Revision 1.6  2002/05/31 03:34:02  theoddone33
 * Fix Keyboard
 * Add titlebar
 *
 * Revision 1.5  2002/05/30 23:46:29  theoddone33
 * some minor key changes (not necessarily fixes)
 *
 * Revision 1.4  2002/05/30 16:50:24  theoddone33
 * Keyboard partially fixed
 *
 * Revision 1.3  2002/05/29 06:25:13  theoddone33
 * Keyboard input, mouse tracking now work
 *
 * Revision 1.2  2002/05/07 03:16:48  theoddone33
 * The Great Newline Fix
 *
 * Revision 1.1.1.1  2002/05/03 03:28:10  root
 * Initial import.
 * 
 * 
 * 7     6/30/99 5:53p Dave
 * Put in new anti-camper code.
 * 
 * 6     6/03/99 6:37p Dave
 * More TNT fun. Made perspective bitmaps more flexible.
 * 
 * 5     6/02/99 6:18p Dave
 * Fixed TNT lockup problems! Wheeeee!
 * 
 * 4     12/18/98 1:13a Dave
 * Rough 1024x768 support for Direct3D. Proper detection and usage through
 * the launcher.
 * 
 * 3     10/09/98 2:57p Dave
 * Starting splitting up OS stuff.
 * 
 * 2     10/08/98 2:38p Dave
 * Cleanup up OsAPI code significantly. Removed old functions, centralized
 * registry functions.
 * 
 * 118   7/10/98 5:04p Dave
 * Fix connection speed bug on standalone server.
 * 
 * 117   5/24/98 2:28p Hoffoss
 * Because we never really care about if the left or the right shift or
 * alt key was used, but rather than either shift or alt was used, made
 * both map to the left one.  Solves some problems, causes none.
 * 
 * 116   5/18/98 9:22p John
 * Took out the annoying tab check.
 * 
 * 115   5/18/98 11:17a John
 * Fixed some bugs with software window and output window.
 * 
 * 114   5/16/98 2:20p John
 * Changed the os_suspend and resume to use a critical section to prevent
 * threads from executing rather than just suspending the thread.  Had to
 * make sure gr_close was called before os_close.
 * 
 * 113   5/15/98 4:49p John
 * 
 * 112   5/15/98 3:36p John
 * Fixed bug with new graphics window code and standalone server.  Made
 * hwndApp not be a global anymore.
 * 
 * 111   5/14/98 5:42p John
 * Revamped the whole window position/mouse code for the graphics windows.
 * 
 * 110   5/04/98 11:08p Hoffoss
 * Expanded on Force Feedback code, and moved it all into Joy_ff.cpp.
 * Updated references everywhere to it.
 *
 * $NoKeywords: $
 */

#include "pstypes.h"
#include "osapi.h"
#include "key.h"
#include "palman.h"
#include "mouse.h"
#include "outwnd.h"
#include "2d.h"
#include "cfile.h"
#include "sound.h"
#include "freespaceresource.h"
#include "managepilot.h"
#include "joy.h"
#include "joy_ff.h"
#include "gamesequence.h"
#include "freespace.h"
#include "osregistry.h"
#include "cmdline.h"
#ifdef HAVE_GLES
#include "eglport.h"
#endif

// ----------------------------------------------------------------------------------------------------
// OSAPI DEFINES/VARS
//

// os-wide globals
static int			fAppActive = 1;
//static int		main_window_inited = 0;		// not used (here)
//static char		szWinTitle[128];			// not used (here)
//static char		szWinClass[128];			// not used (here)
//static int		WinX, WinY, WinW, WinH;		// not used (grsoft.cpp) in UNIX build
static int			Os_inited = 0;

static CRITICAL_SECTION Os_lock;

int Os_debugger_running = 0;

// ----------------------------------------------------------------------------------------------------
// OSAPI FORWARD DECLARATIONS
//

#ifdef THREADED
	// thread handler for the main message thread
	DWORD win32_process(DWORD lparam);
#else
	DWORD win32_process1(DWORD lparam);
	DWORD win32_process1(DWORD lparam);
#endif

// Fills in the Os_debugger_running with non-zero if debugger detected.
void os_check_debugger();

// called at shutdown. Makes sure all thread processing terminates.
void os_deinit();


// ----------------------------------------------------------------------------------------------------
// OSAPI FUNCTIONS
//

// initialization/shutdown functions -----------------------------------------------

// detect users home directory
const char *detect_home(void)
{
	return (getenv("HOME"));
}

// If app_name is NULL or ommited, then TITLE is used
// for the app name, which is where registry keys are stored.
void os_init(const char *wclass, const char *title, const char *app_name, const char *version_string)
{
#ifndef NDEBUG
	outwnd_init(1);
#endif

	/* set some sane defaults since we don't have a laucher... */
	if (os_config_read_string(NULL, NOX("Videocard"), NULL) == NULL)
		os_config_write_string(NULL, NOX("Videocard"), NOX("OpenGL (640x480)"));
	
	if (os_config_read_string(NULL, NOX("NetworkConnection"), NULL) == NULL)
		os_config_write_string(NULL, NOX("NetworkConnection"), NOX("lan"));
	
	if (os_config_read_string(NULL, NOX("ConnectionSpeed"), NULL) == NULL)
		os_config_write_string(NULL, NOX("ConnectionSpeed"), NOX("Slow"));

	Os_inited = 1;

	Os_lock = SDL_CreateMutex();

	// check to see if we're running under msdev
	os_check_debugger();

	atexit(os_deinit);
}

// set the main window title
void os_set_title( const char *title )
{
	// the title is already set by SDL in gropengl.cpp
}

// call at program end
void os_cleanup()
{
	STUB_FUNCTION;
	
#ifndef NDEBUG
		outwnd_close();
#endif
}


// window management -----------------------------------------------------------------

// Returns 0 if app is not the foreground app.
int os_foreground()
{
	return fAppActive;
}

// Returns the handle to the main window
uint os_get_window()
{
//	STUB_FUNCTION;	// not used/needed with UNIX builds?
	return 0;
}


// process management -----------------------------------------------------------------

// Sleeps for n milliseconds or until app becomes active.
void os_sleep(int ms)
{
	usleep(ms*1000);
}

// Used to stop message processing
void os_suspend()
{
	ENTER_CRITICAL_SECTION(&Os_lock);	
}

// resume message processing
void os_resume()
{
	LEAVE_CRITICAL_SECTION(&Os_lock);	
}


// ----------------------------------------------------------------------------------------------------
// OSAPI FORWARD DECLARATIONS
//

// Fills in the Os_debugger_running with non-zero if debugger detected.
void os_check_debugger()
{
}

// called at shutdown. Makes sure all thread processing terminates.
void os_deinit()
{
	SDL_DestroyMutex(Os_lock);

	#ifdef HAVE_GLES
	EGL_Close();
	#endif
	SDL_Quit();
}

extern int SDLtoFS2[SDLK_LAST];
void os_poll()
{
	SDL_Event e;

	while (SDL_PollEvent (&e)) {
		switch (e.type) {
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				switch(e.button.button) {
					case SDL_BUTTON_RIGHT:
						mouse_mark_button(MOUSE_RIGHT_BUTTON, e.button.state);
						break;
					case SDL_BUTTON_MIDDLE:
						mouse_mark_button(MOUSE_MIDDLE_BUTTON, e.button.state);
						break;
					case SDL_BUTTON_LEFT:
						mouse_mark_button(MOUSE_LEFT_BUTTON, e.button.state);
						break;
				}
				break;
			case SDL_KEYDOWN:
				if ((e.key.keysym.mod & KMOD_ALT) &&
				    (e.key.keysym.sym == SDLK_RETURN))
				{
					if (!Cmdline_no_grab && !(SDL_GetVideoSurface()->flags & SDL_FULLSCREEN))
						SDL_WM_GrabInput (SDL_GRAB_ON);
					SDL_WM_ToggleFullScreen (SDL_GetVideoSurface());
					break;
				}
				if ((e.key.keysym.mod & KMOD_CTRL) &&
				    (e.key.keysym.sym == SDLK_g))
				{
					/* DDOI - ignore grab changes when fullscreen */
					if (!(SDL_GetVideoSurface()->flags & SDL_FULLSCREEN))
					{
						if (SDL_WM_GrabInput(SDL_GRAB_QUERY)==SDL_GRAB_ON)
							SDL_WM_GrabInput (SDL_GRAB_OFF);
						else
							SDL_WM_GrabInput (SDL_GRAB_ON);
					}
					break;
				}

				/* this is a very common key combo in the game so don't use this to iconify
				if ((e.key.keysym.mod & KMOD_CTRL) &&
				    (e.key.keysym.sym == SDLK_z))
				{
					SDL_WM_IconifyWindow();
					break;
				} */

				if (SDLtoFS2[e.key.keysym.sym])
					key_mark (SDLtoFS2[e.key.keysym.sym], 1, 0);
				break;
			case SDL_KEYUP:
				if (SDLtoFS2[e.key.keysym.sym])
					key_mark (SDLtoFS2[e.key.keysym.sym], 0, 0);
				break;
			case SDL_ACTIVEEVENT:
				if (e.active.state & SDL_APPACTIVE) {
					fAppActive = e.active.gain;
					gr_activate(fAppActive);
				}
				if (e.active.state & SDL_APPINPUTFOCUS) {
					gr_activate(e.active.gain);
				}
				break;
			default:
				break;
		}
	}
	
    extern void joy_read();
    joy_read();
}

void debug_int3()
{
	STUB_FUNCTION;
}
