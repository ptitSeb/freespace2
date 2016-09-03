/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Cmdline/cmdline.cpp $
 * $Revision: 315 $
 * $Date: 2010-02-09 12:11:54 +0100 (Tue, 09 Feb 2010) $
 * $Author: taylor $
 *
 * $Log$
 * Revision 1.7  2005/10/01 21:40:38  taylor
 * deal with OS X apps a little better, sets the path only based on ".app" in the name rather than the name itself
 * make sure a global cmdline.cfg file works with OS X when built as an app
 *
 * Revision 1.6  2003/06/22 12:50:11  taylor
 * fix memory error, code cleanup
 *
 * Revision 1.5  2003/05/06 07:20:14  taylor
 * implement command line options
 *
 * Revision 1.4  2002/06/21 03:34:05  relnev
 * implemented a stub and fixed a path
 *
 * Revision 1.3  2002/06/09 04:41:15  relnev
 * added copyright header
 *
 * Revision 1.2  2002/05/07 03:16:43  theoddone33
 * The Great Newline Fix
 *
 * Revision 1.1.1.1  2002/05/03 03:28:08  root
 * Initial import.
 *
 * 
 * 8     8/26/99 8:51p Dave
 * Gave multiplayer TvT messaging a heavy dose of sanity. Cheat codes.
 * 
 * 7     7/15/99 3:07p Dave
 * 32 bit detection support. Mouse coord commandline.
 * 
 * 6     7/13/99 1:15p Dave
 * 32 bit support. Whee!
 * 
 * 5     6/22/99 9:37p Dave
 * Put in pof spewing.
 * 
 * 4     1/12/99 5:45p Dave
 * Moved weapon pipeline in multiplayer to almost exclusively client side.
 * Very good results. Bandwidth goes down, playability goes up for crappy
 * connections. Fixed object update problem for ship subsystems.
 * 
 * 3     11/17/98 11:12a Dave
 * Removed player identification by address. Now assign explicit id #'s.
 * 
 * 2     10/07/98 10:52a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:48a Dave
 * 
 * 38    10/02/98 3:22p Allender
 * fix up the -connect option and fix the -port option
 * 
 * 37    9/15/98 4:04p Allender
 * added back in the -ip_addr command line switch because it needs to be
 * in the standalone server only executable
 * 
 * 36    9/14/98 11:52a Allender
 * don't use cfile
 * 
 * 35    9/14/98 11:28a Allender
 * support for server bashing of address when received from client.  Added
 * a cmdline.cfg file to process command line arguments from a file
 * 
 * 34    9/08/98 2:20p Allender
 * temporary code to force IP address to a specific value.
 * 
 * 33    8/20/98 5:30p Dave
 * Put in handy multiplayer logfile system. Now need to put in useful
 * applications of it all over the code.
 * 
 * 32    8/07/98 10:39a Allender
 * fixed debug standalone problem where stats would continually get sent
 * to tracker.  more debug code to help find stats problem
 * 
 * 31    7/24/98 11:14a Allender
 * start of new command line options for version 1.04
 * 
 * 30    5/21/98 1:50a Dave
 * Remove obsolete command line functions. Reduce shield explosion packets
 * drastically. Tweak PXO screen even more. Fix file xfer system so that
 * we can guarantee file uniqueness.
 * 
 * 29    5/18/98 9:10p Dave
 * Put in many new PXO features. Fixed skill level bashing in multiplayer.
 * Removed several old command line options. Put in network config files.
 * 
 * 28    5/09/98 7:16p Dave
 * Put in CD checking. Put in standalone host password. Made pilot into
 * popup scrollable.
 * 
 * 27    4/23/98 8:27p Allender
 * basic support for cutscene playback.  Into movie code in place.  Tech
 * room can view cutscenes stored in CDROM_dir variable
 * 
 * 26    4/09/98 5:43p Dave
 * Remove all command line processing from the demo. Began work fixing up
 * the new multi host options screen.
 * 
 * 25    4/02/98 11:40a Lawrance
 * check for #ifdef DEMO instead of #ifdef DEMO_RELEASE
 * 
 * 24    4/01/98 5:56p Dave
 * Fixed a messaging bug which caused msg_all mode in multiplayer not to
 * work. Compile out a host of multiplayer options not available in the
 * demo.
 * 
 * 23    3/14/98 2:48p Dave
 * Cleaned up observer joining code. Put in support for file xfers to
 * ingame joiners (observers or not). Revamped and reinstalled pseudo
 * lag/loss system.
 * 
 * 22    2/22/98 12:19p John
 * Externalized some strings
 * 
 * 21    1/31/98 4:32p Dave
 * Put in new support for VMT player validation, game logging in and game
 * logging out.
 * 
 * 20    12/10/97 4:45p Dave
 * Added in more detailed support for multiplayer packet lag/loss. Fixed
 * some multiplayer stuff. Added some controls to the standalone.
 * 
 * 19    12/09/97 6:14p Lawrance
 * add -nomusic flag
 * 
 * 18    12/01/97 5:10p Dave
 * Fixed a syntax bug.
 * 
 * 17    12/01/97 4:59p Dave
 * Synchronized multiplayer debris objects. Put in pilot popup in main
 * hall. Optimized simulated multiplayer lag module. Fixed a potential
 * file_xfer bug.
 * 
 * 16    11/28/97 7:04p Dave
 * Emergency checkin due to big system crash.
 * 
 * 15    11/28/97 5:06p Dave
 * Put in facilities for simulating multiplayer lag.
 * 
 * 14    11/24/97 5:42p Dave
 * Fixed a file xfer buffer free/malloc problem. Lengthened command line
 * switch string parse length.
 * 
 * 13    11/12/97 4:39p Dave
 * Put in multiplayer campaign support parsing, loading and saving. Made
 * command-line variables better named. Changed some things on the initial
 * pilot select screen.
 * 
 * 12    11/11/97 4:54p Dave
 * Put in support for single vs. multiplayer pilots. Put in initial player
 * selection screen (no command line option yet). Started work on
 * multiplayer campaign file save gaming.
 * 
 * 11    11/11/97 11:55a Allender
 * initialize network at beginning of application.  create new call to set
 * which network protocol to use
 * 
 * 10    9/18/97 10:12p Dave
 * Added -gimmemedals, which gives the current pilot all the medals in the
 * game (debug)
 * 
 * 9     9/18/97 9:20a Dave
 * Minor modifications
 * 
 * 8     9/15/97 11:40p Lawrance
 * remove demo granularity switch
 * 
 * 7     9/09/97 3:39p Sandeep
 * warning level 4 bugs
 * 
 * 6     9/03/97 5:03p Lawrance
 * add support for -nosound command line parm
 * 
 * 5     8/22/97 8:52a Dave
 * Removed a return statement which would have broken the parser out too
 * early.
 * 
 * 4     8/21/97 4:55p Dave
 * Added a switch for multiplayer chat streaming. Added a section for
 * global command line vars.
 * 
 * 3     8/06/97 2:26p Dave
 * Made the command line parse more robust. Made it easier to add and
 * process new command-line switches.
 * 
 * 2     8/04/97 3:13p Dave
 * Added command line functions. See cmdline.cpp for directions on adding
 * new switches
 * 
 * 1     8/04/97 9:58a Dave
 * 
 * $NoKeywords: $
 */

#include <string.h>
#include <stdlib.h>
#include <new>
#include "cmdline.h"
#include "linklist.h"
#include "systemvars.h"
#include "multi.h"
#include "cfile.h"
#include "osapi.h"
#include "osregistry.h"
#include "version.h"

// variables
class cmdline_parm {
public:
	cmdline_parm *next, *prev;
	const char *name;						// name of parameter, must start with '-' char
#ifdef PLAT_UNIX
	const char *name_s;					// single letter argument
#endif
	const char *help;						// help text for this parameter
	char *args;						// string value for parameter arguements (NULL if no arguements)
	int name_found;				// true if parameter on command line, otherwise false

#ifndef PLAT_UNIX
	cmdline_parm(const char *name, const char *help);
#else
	cmdline_parm(const char *name, const char *name_s, const char *help);
#endif
	~cmdline_parm();
	int found();
	int get_int();
	float get_float();
	char *str();
};

// here are the command line parameters that we will be using for FreeSpace
#ifndef PLAT_UNIX
cmdline_parm standalone_arg("-standalone", NULL);
cmdline_parm nosound_arg("-nosound", NULL);
cmdline_parm nomusic_arg("-nomusic", NULL);
cmdline_parm startgame_arg("-startgame", NULL);
cmdline_parm gamename_arg("-gamename", NULL);
cmdline_parm gamepassword_arg("-password", NULL);
cmdline_parm gameclosed_arg("-closed", NULL);
cmdline_parm gamerestricted_arg("-restricted", NULL);
cmdline_parm allowabove_arg("-allowabove", NULL);
cmdline_parm allowbelow_arg("-allowbelow", NULL);
cmdline_parm port_arg("-port", NULL);
cmdline_parm connect_arg("-connect", NULL);
cmdline_parm multilog_arg("-multilog", NULL);
cmdline_parm server_firing_arg("-oldfire", NULL);
cmdline_parm client_dodamage("-clientdamage", NULL);
cmdline_parm pof_spew("-pofspew", NULL);
cmdline_parm d3d_32bit("-32bit", NULL);
cmdline_parm mouse_coords("-coords", NULL);
cmdline_parm timeout("-timeout", NULL);
cmdline_parm d3d_window("-window", NULL);
#else
cmdline_parm standalone_arg("--standalone", "-d", NULL);
cmdline_parm nosound_arg("--nosound", "-s", NULL);
cmdline_parm nomusic_arg("--nomusic", "--nomusic", NULL);
cmdline_parm startgame_arg("--startgame", "-S", NULL);
cmdline_parm gamename_arg("--gamename", "-N", NULL);
cmdline_parm gamepassword_arg("--password", "-p", NULL);
cmdline_parm gameclosed_arg("--closed", "-c", NULL);
cmdline_parm gamerestricted_arg("--restricted", "-r", NULL);
cmdline_parm allowabove_arg("--allowabove", "-a", NULL);
cmdline_parm allowbelow_arg("--allowbelow", "-b", NULL);
cmdline_parm port_arg("--port", "-o", NULL);
cmdline_parm connect_arg("--connect", "-C", NULL);
cmdline_parm multilog_arg("--multilog", "-m", NULL);
cmdline_parm server_firing_arg("--oldfire", "-F", NULL);
cmdline_parm client_dodamage("--clientdamage", "-D", NULL);
cmdline_parm pof_spew("--pofspew", "-P", NULL);
cmdline_parm mouse_coords("-coords", "-M", NULL);
cmdline_parm timeout("--timeout", "-t", NULL);
cmdline_parm d3d_window("--window", "-w", NULL);
cmdline_parm d3d_fullscreen("--fullscreen", "-f", NULL);
cmdline_parm help("--help", "-h", NULL);
cmdline_parm no_grab("--nograb", "-g", NULL);
cmdline_parm fs_version("--version", "-v", NULL);
cmdline_parm no_movies("--nomovies", "-n", NULL);
#ifdef PANDORA
cmdline_parm no_shouldermb("--noshouldermb", "-B", NULL);
#endif
#endif

int Cmdline_multi_stream_chat_to_file = 0;
int Cmdline_freespace_no_sound = 0;
int Cmdline_freespace_no_music = 0;
int Cmdline_gimme_all_medals = 0;
int Cmdline_use_last_pilot = 0;
int Cmdline_multi_protocol = -1;
int Cmdline_cd_check = 1;
int Cmdline_start_netgame = 0;
int Cmdline_closed_game = 0;
int Cmdline_restricted_game = 0;
int Cmdline_network_port = -1;
char *Cmdline_game_name = NULL;
char *Cmdline_game_password = NULL;
char *Cmdline_rank_above= NULL;
char *Cmdline_rank_below = NULL;
char *Cmdline_connect_addr = NULL;
int Cmdline_multi_log = 0;
int Cmdline_server_firing = 0;
int Cmdline_client_dodamage = 0;
int Cmdline_spew_pof_info = 0;
int Cmdline_force_32bit = 0;
int Cmdline_mouse_coords = 0;
int Cmdline_timeout = -1;
#ifdef PLAT_UNIX
int Cmdline_no_grab = 0;
int Cmdline_play_movies = 1;
int Cmdline_fullscreen = 0;
#ifdef PANDORA
int Cmdline_noshouldermb = 0;
#endif
#endif

int Cmdline_window = 0;

#ifndef PLAT_UNIX
static cmdline_parm Parm_list(NULL, NULL);
#else
static cmdline_parm Parm_list(NULL, NULL, NULL);
void print_instructions();
#endif
static int Parm_list_inited = 0;

//	Return true if this character is an extra char (white space and quotes)
int is_extra_space(char ch)
{
	return ((ch == ' ') || (ch == '\t') || (ch == 0x0a) || (ch == '\'') || (ch == '\"'));
}


// eliminates all leading and trailing extra chars from a string.  Returns pointer passed in.
char *drop_extra_chars(char *str)
{
	int s, e;

	s = 0;
	while (str[s] && is_extra_space(str[s]))
		s++;

	e = strlen(str) - 1;
	while (e > s) {
		if (!is_extra_space(str[e])){
			break;
		}

		e--;
	}

	if (e > s){
		memmove(str, str + s, e - s + 1);
	}

	str[e - s + 1] = 0;
	return str;
}


// internal function - copy the value for a parameter agruement into the cmdline_parm arg field
#ifdef PLAT_UNIX
void parm_stuff_args(cmdline_parm *parm, char *cmdline, bool single)
#else
void parm_stuff_args(cmdline_parm *parm, char *cmdline)
#endif
{
	char buffer[1024] = { 0 };
	char *dest = buffer;

#ifdef PLAT_UNIX
	if (single) {
		cmdline += strlen(parm->name_s);
	} else {
		cmdline += strlen(parm->name);
	}
#else
	cmdline += strlen(parm->name);
#endif

	while ((*cmdline != 0) && (*cmdline != '-') && (dest-buffer > 1)) {
		*dest++ = *cmdline++;
	}

	drop_extra_chars(buffer);

	// mwa 9/14/98 -- made it so that newer command line arguments found will overwrite
	// the old arguments
//	Assert(parm->args == NULL);
	if ( parm->args != NULL ) {
		delete [] parm->args;
		parm->args = NULL;
	}

	int size = strlen(buffer);
	if (size > 0) {
		try {
			parm->args = new char[size+1];
			memset(parm->args, 0, size+1);
			strcpy(parm->args, buffer);
		} catch (std::bad_alloc) {
			parm->args = NULL;
		}
	}
}


// internal function - parse the command line, extracting parameter arguements if they exist
// cmdline - command line string passed to the application
void os_parse_parms(char *cmdline)
{
	// locate command line parameters
	cmdline_parm *parmp;
	char *cmdline_offset = NULL;

	if ( !cmdline || (strlen(cmdline) <= 1) ) {
		return;
	}

	for (parmp = GET_FIRST(&Parm_list); parmp !=END_OF_LIST(&Parm_list); parmp = GET_NEXT(parmp) ) {
		cmdline_offset = strstr(cmdline, parmp->name);

#ifdef PLAT_UNIX
		bool single = false;

		// if a match isn't found check for single args
		// tack a space on the single args so they don't get mixed up with double args
		if (!cmdline_offset) {
			char single_tmp[32] = { 0 };
			strcpy(single_tmp, parmp->name_s);
			strcat(single_tmp, " ");
			cmdline_offset = strstr(cmdline, single_tmp);
			single = true;
		}

		if (cmdline_offset) {
			parmp->name_found = 1;
			parm_stuff_args(parmp, cmdline_offset, single);
		}
#else
		if (cmdline_offset) {
			parmp->name_found = 1;
			parm_stuff_args(parmp, cmdline_offset);
		}
#endif


	}
}


// validate the command line parameters.  Display an error if an unrecognized parameter is located.
void os_validate_parms(char *cmdline)
{
	cmdline_parm *parmp;
	char seps[] = " ,\t\n";
	char *token;
	int parm_found;

	if ( !cmdline || (strlen(cmdline) <= 1) ) {
		return;
	}

   token = strtok(cmdline, seps);
   while(token != NULL) {
		if (token[0] == '-') {
			parm_found = 0;
			for (parmp = GET_FIRST(&Parm_list); parmp !=END_OF_LIST(&Parm_list); parmp = GET_NEXT(parmp) ) {
#ifndef PLAT_UNIX
				if (!stricmp(parmp->name, token)) {
#else
				// make sure to do a case sensitive check here
				if (!strcmp(parmp->name, token) || !strcmp(parmp->name_s, token)) {
#endif
					parm_found = 1;
					break;
				}
			}

			if (parm_found == 0) {
#ifndef PLAT_UNIX
				Error(LOCATION,"Unrecogzined command line parameter %s", token);
#else
				print_instructions();
#endif
			}
		}

		token = strtok(NULL, seps);
	}
}


// Call once to initialize the command line system
//
// cmdline - command line string passed to the application
void os_init_cmdline(char *cmdline)
{
	FILE *fp;

	// read the cmdline.cfg file from the data folder, and pass the command line arguments to
	// the the parse_parms and validate_parms line.  Read these first so anything actually on
	// the command line will take precedence
#ifdef PLAT_UNIX
	char cmdname[MAX_PATH] = { 0 };

	snprintf(cmdname, MAX_PATH, "%s/%s/Data/cmdline.cfg", detect_home(), Osreg_user_dir);
	fp = fopen(cmdname, "rt");
	
	if (!fp) {
		// if not already found check exec directory
#if defined(__APPLE__) && !defined(MACOSX)
		// we may end up doing this twice but it would largely get skipped the second time anyway
		char *c = NULL;
		c = strstr(full_path, ".app");
		
		if ( c != NULL) {
			while (c && (*c != '/'))
				c--;
			
			*c = '\0';
		}

		snprintf(cmdname, MAX_PATH, "%s/Data/cmdline.cfg", full_path);

		fp = fopen(cmdname, "rt");
#else
		fp = fopen("Data/cmdline.cfg", "rt");
#endif // __APPLE__ && !__MACOSX__
	}
#else
	fp = fopen("data\\cmdline.cfg", "rt");
#endif

	// if the file exists, get a single line, and deal with it
	if ( fp ) {
		char buf[1024] = { 0 }, *p;

		while (fgets(buf, sizeof(buf), fp) != NULL) {
			// replace the newline character with a NUL:
			if ( (p = strrchr(buf, '\n')) != NULL ) {
				*p = '\0';
			}

#ifdef PLAT_UNIX
			// make sure that we have a trailing space for option finding to work
			// properly with single args
			strcat(buf, " ");
#endif

			os_parse_parms(buf);
			os_validate_parms(buf);
		}

		fclose(fp);
	}



	os_parse_parms(cmdline);
	os_validate_parms(cmdline);

}

#ifdef PLAT_UNIX
// help for available cmdline options
void print_instructions()
{
	printf("http://icculus.org/freespace2\n");
	printf("Support - FAQ: http://icculus.org/lgfaq\n");
	printf("          Web: http://bugzilla.icculus.org\n\n");

	printf("Usage: freespace2 [options]\n");
	printf("     [-h | --help]           Show this help message\n");
	printf("     [-v | --version]        Show game version\n");
	printf("     [-s | --nosound]        Do no access the sound card\n");
	printf("     [-f | --fullscreen]     Run the game fullscreen\n");
	printf("     [-w | --window]         Run the game in a window\n");
	printf("     [-g | --nograb]         Do not automatically grab mouse\n");
	printf("     [-n | --nomovies]       Do not play movies\n");
	printf("     [-d | --standalone]     Run as a dedicated server\n");
	printf("     [-S | --startgame]      Start a multiplayer game\n");
	printf("     [-N | --gamename]       Name of the multiplayer game\n");
	printf("     [-p | --password]       Use this password to connect\n");
	printf("     [-c | --closed]         Closed multiplayer game\n");
	printf("     [-r | --restricted]     Restricted multiplayer game\n");
	printf("     [-a | --allowabove]     Only allow above certain rank\n");
	printf("     [-b | --allowbelow]     Only allow below certain rank\n");
	printf("     [-o | --port]           Port to use for multiplayer games\n");
	printf("     [-C | --connect]        Connect to particular IP address\n");
	printf("     [-m | --multilog]       Log multiplayer events\n");
	printf("     [-F | --oldfire]        Server side firing\n");
	printf("     [-D | --clientdamage]   Client does damage\n");
	printf("     [-t | --timeout]        Multiplayer game timeout\n");
	printf("     [-P | --pofspew]        Save model info to pofspew.txt\n");
	printf("     [-M | --coords]         Show coordinates of the mouse cursor\n\n");
#ifdef PANDORA
	printf("     [-B | --noshouldermb]   No Shoulder Mouse Buttons\n\n");
#endif
	printf("Freespace 2 v%d.%02d -- Linux Client v%d.%02d\n\n", FS_VERSION_MAJOR, FS_VERSION_MINOR, FS_UNIX_VERSION_MAJOR, FS_UNIX_VERSION_MINOR);

	exit(0);
}
#endif

// arg constructor
// name_ - name of the parameter, must start with '-' character
// help_ - help text for this parameter
#ifndef PLAT_UNIX
cmdline_parm::cmdline_parm(const char *name_, const char *help_)
#else
cmdline_parm::cmdline_parm(const char *name_, const char *name_s_, const char *help_)
#endif
{
	name = name_;
#ifdef PLAT_UNIX
	name_s = name_s_;
#endif
	help = help_;
	args = NULL;
	name_found = 0;

	if (Parm_list_inited == 0) {
		list_init(&Parm_list);
		Parm_list_inited = 1;
	}

	if (name != NULL) {
		list_append(&Parm_list, this);
	}
}


// destructor - frees any allocated memory
cmdline_parm::~cmdline_parm()
{
	if (args) {
		delete [] args;
		args = NULL;
	}
}


// returns - true if the parameter exists on the command line, otherwise false
int cmdline_parm::found()
{
	return name_found;
}


// returns - the interger representation for the parameter arguement
int cmdline_parm::get_int()
{
	Assert(args);
	return atoi(args);
}


// returns - the float representation for the parameter arguement
float cmdline_parm::get_float()
{
	Assert(args);
	return (float)atof(args);
}


// returns - the string value for the parameter arguement
char *cmdline_parm::str()
{
	Assert(args);
	return args;
}

// external entry point into this modules
int parse_cmdline(char *cmdline)
{
	os_init_cmdline(cmdline);

	// is this a standalone server??
	if (standalone_arg.found()) {
		Is_standalone = 1;
	}

	// run with no sound
	if ( nosound_arg.found() ) {
		Cmdline_freespace_no_sound = 1;
	}

	// run with no music
	if ( nomusic_arg.found() ) {
		Cmdline_freespace_no_music = 1;
	}

	// should we start a network game
	if ( startgame_arg.found() ) {
		Cmdline_use_last_pilot = 1;
		Cmdline_start_netgame = 1;
	}

	// closed network game
	if ( gameclosed_arg.found() ) {
		Cmdline_closed_game = 1;
	}

	// restircted network game
	if ( gamerestricted_arg.found() ) {
		Cmdline_restricted_game = 1;
	}

	// get the name of the network game
	if ( gamename_arg.found() ) {
		Cmdline_game_name = gamename_arg.str();

#ifdef PLAT_UNIX
		// if there wasn't an argument then complain and exit
		if ( !(strlen(Cmdline_game_name) > 0) ) {
			fprintf(stderr, "ERROR: The --gamename (-N) option requires an additional argument!\n");
			exit(0);
		}
#endif

		// be sure that this string fits in our limits
		if ( strlen(Cmdline_game_name) > MAX_GAMENAME_LEN ) {
			Cmdline_game_name[MAX_GAMENAME_LEN-1] = '\0';
		}
	}

	// get the password for a pssword game
	if ( gamepassword_arg.found() ) {
		Cmdline_game_password = gamepassword_arg.str();

#ifdef PLAT_UNIX
		// if there wasn't an argument then complain and exit
		if ( !(strlen(Cmdline_game_password) > 0) ) {
			fprintf(stderr, "ERROR: The --password (-p) option requires an additional argument!\n");
			exit(0);
		}
#endif

		// be sure that this string fits in our limits
		if ( strlen(Cmdline_game_password) > MAX_PASSWD_LEN ) {
			Cmdline_game_password[MAX_PASSWD_LEN-1] = '\0';
		}
	}

	// set the rank above/below arguments
	if ( allowabove_arg.found() ) {
		Cmdline_rank_above = allowabove_arg.str();
	}
	if ( allowbelow_arg.found() ) {
		Cmdline_rank_below = allowbelow_arg.str();
	}

	// get the port number for games
	if ( port_arg.found() ) {
		Cmdline_network_port = port_arg.get_int();

#ifdef PLAT_UNIX
		// if there wasn't an argument then complain and exit
		if ( !Cmdline_network_port ) {
			fprintf(stderr, "ERROR: The --port (-P) option requires an additional argument!\n");
			exit(0);
		}
#endif
	}

	// the connect argument specifies to join a game at this particular address
	if ( connect_arg.found() ) {
		Cmdline_use_last_pilot = 1;
		Cmdline_connect_addr = connect_arg.str();

#ifdef PLAT_UNIX
		// if there wasn't an argument then complain and exit
		if ( !(strlen(Cmdline_connect_addr) > 0) ) {
			fprintf(stderr, "ERROR: The --connect (-C) option requires an additional argument!\n");
			exit(0);
		}
#endif
	}

	// see if the multilog flag was set
	if ( multilog_arg.found() ){
		Cmdline_multi_log = 1;
	}	

	// maybe use old-school server-side firing
	if (server_firing_arg.found() ){
		Cmdline_server_firing = 1;
	}

	// maybe use old-school client damage
	if(client_dodamage.found()){
		Cmdline_client_dodamage = 1;
	}	

	// spew pof info
	if(pof_spew.found()){
		Cmdline_spew_pof_info = 1;
	}

#ifndef PLAT_UNIX
	// 32 bit
	if(d3d_32bit.found()){
		Cmdline_force_32bit = 1;
	}
#endif

	// mouse coords
	if(mouse_coords.found()){
		Cmdline_mouse_coords = 1;
	}

	// net timeout
	if(timeout.found()){
		Cmdline_timeout = timeout.get_int();
	}

	// d3d windowed
	if(d3d_window.found()){
		Cmdline_window = 1;
	}

#ifdef PLAT_UNIX
	// run fullscreen
	if(d3d_fullscreen.found()){
		Cmdline_fullscreen = 1;
	}

	// help!!
	if(help.found()){
		print_instructions();
	}

	// no key/mouse grab
	if(no_grab.found()){
		Cmdline_no_grab = 1;
	}

	// play movies?
	if(no_movies.found()){
		Cmdline_play_movies = 0;
	}
#ifdef PANDORA
	// play movies?
	if(no_shouldermb.found()){
		Cmdline_noshouldermb = 1;
	}
#endif

	// display game version
	if(fs_version.found()){
		printf("Freespace 2 version:  %d.%02d\n", FS_VERSION_MAJOR, FS_VERSION_MINOR);
		printf("Linux client version:  %d.%02d\n", FS_UNIX_VERSION_MAJOR, FS_UNIX_VERSION_MINOR);
		exit(0);
	}
#endif

	return 1;
}
