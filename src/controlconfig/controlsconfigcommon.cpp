/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/ControlConfig/ControlsConfigCommon.cpp $
 * $Revision: 311 $
 * $Date: 2010-02-08 09:09:26 +0100 (Mon, 08 Feb 2010) $
 * $Author: taylor $
 *
 * C module for keyboard, joystick and mouse configuration common stuff (between Fred and FreeSpace)
 *
 * $Log$
 * Revision 1.5  2003/06/03 04:00:39  taylor
 * Polish language support (Janusz Dziemidowicz)
 *
 * Revision 1.4  2003/05/25 02:30:42  taylor
 * Freespace 1 support
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
 * 14    11/01/99 3:36p Jefff
 * had scan code texts for y and z in german swapped
 * 
 * 13    11/01/99 2:16p Jefff
 * minor key name change in german
 * 
 * 12    10/29/99 6:10p Jefff
 * squashed the damned y/z german issues once and for all
 * 
 * 11    10/28/99 11:16p Jefff
 * Changed some german key names.  Made key translations always use the
 * english table.
 * 
 * 10    10/28/99 2:05a Jefff
 * revised some german key names.  changed some y/z switch stuff.
 * 
 * 9     10/25/99 5:39p Jefff
 * swap init binding for y and z keys in German builds
 * 
 * 8     9/01/99 2:56p Jefff
 * a few control key description changes
 * 
 * 7     8/22/99 5:53p Dave
 * Scoring fixes. Added self destruct key. Put callsigns in the logfile
 * instead of ship designations for multiplayer players.
 * 
 * 6     8/19/99 10:59a Dave
 * Packet loss detection.
 * 
 * 5     8/01/99 12:39p Dave
 * Added HUD contrast control key (for nebula).
 * 
 * 4     7/23/99 2:57p Andsager
 * fix translate_key_to_index to work with localization
 * 
 * 3     11/05/98 4:18p Dave
 * First run nebula support. Beefed up localization a bit. Removed all
 * conditional compiles for foreign versions. Modified mission file
 * format.
 * 
 * 2     10/07/98 10:52a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:48a Dave
 * 
 * 41    6/19/98 3:51p Lawrance
 * localize control text
 * 
 * 40    6/17/98 11:05a Lawrance
 * localize the control config strings
 * 
 * 39    5/24/98 2:28p Hoffoss
 * Because we never really care about if the left or the right shift or
 * alt key was used, but rather than either shift or alt was used, made
 * both map to the left one.  Solves some problems, causes none.
 * 
 * 38    5/22/98 11:21a Hoffoss
 * Added bank when pressed back in.
 * 
 * 37    5/15/98 8:37p Lawrance
 * Add 'equalize recharge rates' and 'target ship that sent last
 * transmission' key bindings
 * 
 * 36    5/13/98 7:15p Hoffoss
 * Fixed remaining bugs with axis binding.
 * 
 * 35    5/13/98 1:17a Hoffoss
 * Added joystick axes configurability.
 * 
 * 34    5/12/98 11:25a Hoffoss
 * Disabled bank when pressed action.
 * 
 * 33    5/09/98 10:48p Lawrance
 * change text for view bindings
 * 
 * 32    5/09/98 4:52p Lawrance
 * Implement padlock view (up/rear/left/right)
 * 
 * 31    4/30/98 9:43p Hoffoss
 * Added new actions for free look movement which are default bounded to
 * the hat.
 * 
 * 30    4/25/98 1:25p Lawrance
 * Add time compression keys to key config
 * 
 * 29    4/25/98 12:43p Allender
 * added new shortcut key for attack my subsystem
 * 
 * 28    4/18/98 5:00p Dave
 * Put in observer zoom key. Made mission sync screen more informative.
 * 
 * 27    4/17/98 1:24a Lawrance
 * fix spelling error
 * 
 * 26    4/15/98 11:06a Lawrance
 * fix bug with a multi key showing up in demo, remove obsolete bindings
 * from demo and full version
 * 
 * 25    4/10/98 12:47p Allender
 * changed working on replay popup.  Don't reference repair in comm menu.
 * Added Shift-R for repair me
 * 
 * 24    4/08/98 11:11a Hoffoss
 * Fixed some bugs that showed up due to fixing other bugs the other day
 * with controls.
 * 
 * 23    4/07/98 4:06p Lawrance
 * Change binding next for bomb targeting
 * 
 * 22    4/07/98 1:52p Lawrance
 * fix error in binding text for energy keys 
 * 
 * 21    4/06/98 11:17a Hoffoss
 * Fixed num lock/pause interplay bug.
 * 
 * 20    4/01/98 6:46p Lawrance
 * change text for Alt+J
 * 
 * 19    3/31/98 4:15p Hoffoss
 * Disabled the show objectives action.
 * 
 * 18    3/25/98 2:16p Dave
 * Select random default image for newly created pilots. Fixed several
 * multi-pause messaging bugs. Begin work on online help for multiplayer
 * keys.
 * 
 * 17    3/19/98 5:04p Dave
 * Put in support for targeted multiplayer text and voice messaging (all,
 * friendly, hostile, individual).
 * 
 * 16    3/17/98 11:25a Hoffoss
 * Fixed some of the action's names.
 * 
 * 15    3/17/98 10:48a Hoffoss
 * Allowed a special hack for "bank while pressed" action to use alt and
 * shift keys standalone.
 * 
 * 14    2/28/98 9:47p Lawrance
 * change some binding text
 * 
 * 13    2/28/98 7:02p Lawrance
 * overhaul on-line help
 * 
 * 12    2/26/98 12:33a Lawrance
 * Added back slew mode,  lots of changes to external and chase views.
 * 
 * 11    2/23/98 11:25a Sandeep
 * added default keys
 * 
 * 10    2/22/98 6:27p Lawrance
 * External mode takes precedence over chase mode.  Fix bug with repair
 * ships docking in chase mode.
 * 
 * 9     2/22/98 12:19p John
 * Externalized some strings
 * 
 * 8     2/02/98 6:59p Lawrance
 * Adding new targeting keys (bomb, uninspected cargo, new ship, live
 * turrets).
 * 
 * 7     1/27/98 4:24p Allender
 * moved hotkey selection to F3 instead of X.  Made palette flash
 * disappear in multiplayer pause menu
 * 
 * 6     1/22/98 4:53p Hoffoss
 * Made training messages/directives display a joystick button in place of
 * a keypress if there is no keypress bound to the action.
 * 
 * 5     1/22/98 4:15p Hoffoss
 * Added code to allow popup to tell player he needs to bind a key for the
 * training mission.
 * 
 * 4     1/02/98 4:42p Allender
 * removed unused key bindings from control config list
 * 
 * 3     12/30/97 4:47p Allender
 * work with ignore my target command.  Added new keyboard hotkey.  Made
 * it work globally
 * 
 * 2     12/24/97 3:37p Hoffoss
 * Moved control config stuff to seperate library to Fred can access it as
 * well.
 * 
 * 1     12/24/97 3:27p Hoffoss
 * 
 * 1     12/24/97 11:58a Hoffoss
 * 
 * 98    12/22/97 2:15p Hoffoss
 * Fixed bug where joystick axis lines weren't being displayed.
 * 
 * 97    12/16/97 2:44p Hoffoss
 * Added clear button to control config screen.
 * 
 * 96    12/12/97 3:07p Hoffoss
 * Changed how deleting bindings work.  Each control of an action can be
 * deleted independently or both at once.
 * 
 * 95    12/07/97 2:36p John
 * Made warp out be Alt+J instead of J
 * 
 * 94    12/03/97 4:59p Hoffoss
 * Added reset sound and change control config sounds around.
 * 
 * 93    12/03/97 4:16p Hoffoss
 * Changed sound stuff used in interface screens for interface purposes.
 * 
 * 92    12/01/97 5:25p Hoffoss
 * Routed interface sound playing through a special function that will
 * only allow one instance of the sound to play at a time, avoiding
 * over-mixing problems.
 * 
 * 91    12/01/97 3:38p Hoffoss
 * Changed placement of 'More' indicator.
 * 
 * 90    11/25/97 3:49p Hoffoss
 * Changed subhilighting to happen when mouse is over line, rather than
 * mouse button down but not up again yet.
 * 
 * 89    11/24/97 10:20p Lawrance
 * Add key 'KEY_N' to target next ship on monitoring view
 * 
 * 88    11/24/97 6:15p Lawrance
 * fix button scroll problem
 * 
 * 87    11/23/97 6:15p Hoffoss
 * Make exiting the controls config screen save the pilot, so changes keep
 * even if programs crashes or something.
 * 
 * 86    11/21/97 5:57p Hoffoss
 * Fixed bug where timef type controls weren't checking joystick buttons.
 * 
 * 85    11/21/97 4:06p Hoffoss
 * Fixed bug with enabling of axis are reversed.
 * 
 * 84    11/20/97 4:47p Hoffoss
 * Swapped throttle and rudder, which apparently were backwards.
 * 
 * 83    11/20/97 4:00p Lawrance
 * set z to a valid value
 * 
 * 82    11/20/97 3:52p Hoffoss
 * Made "None" appear if no control is bound to function.
 * 
 * 81    11/20/97 2:10p Hoffoss
 * Added defaults for  joystick buttons 2 and 3.
 * 
 * 80    11/20/97 1:08a Lawrance
 * add support for 'R' key - target closest friendly repair ship
 * 
 * 79    11/19/97 8:33p Hoffoss
 * New controls config screen baby!
 * 
 * 78    11/17/97 7:09p Hoffoss
 * Made chase view control a trigger type.
 * 
 * 77    11/17/97 6:40p Lawrance
 * Changed 'I' key to toggle of extended target info, moved target closest
 * locked missile to 'L'
 * 
 * 76    11/17/97 3:26p Jasen
 * Adjusted coordinates for the help button .ani location
 * 
 * 75    11/16/97 3:51p Hoffoss
 * Added more button to config screen when scrolling allowed.
 * 
 * 74    11/16/97 3:20p Hoffoss
 * Added various suggestions from the Todolist.
 * 
 * 73    11/14/97 4:33p Mike
 * Change Debug key to backquote (from F11).
 * Balance a ton of subsystems in ships.tbl.
 * Change "Heavy Laser" to "Disruptor".
 * 
 * 72    11/12/97 2:54p Hoffoss
 * Made modifiers non-stand alone, added support for red tabs on
 * conflicts.
 * 
 * 71    10/30/97 6:01p Hoffoss
 * Changed screen to utilize the new text color standards.
 * 
 * 70    10/29/97 7:25p Hoffoss
 * Added crude support for UI button double click checking.
 * 
 * 69    10/29/97 6:32p Hoffoss
 * Changed some interface apperances.
 * 
 * 68    10/29/97 4:54p Hoffoss
 * Changed scan_code_text element for Enter to 'Enter' instead of 'a' with
 * a caret over it (what's that all about?)
 * 
 * 67    10/28/97 10:42a Hoffoss
 * Fixed handling of continuous controls that have modifiers to the key.
 * 
 * 66    10/28/97 10:03a Hoffoss
 * Fixed bug with continuous type controls not registering if pressed and
 * released too quickly.  Also some other little changes.
 * 
 * 65    10/28/97 12:12a Lawrance
 * remove unused keys (Alt-H and Alt-F)
 * 
 * 64    10/27/97 12:23p Hoffoss
 * Improved warning system and fixed coloring bug.
 * 
 * 63    10/27/97 11:39a Hoffoss
 * Added control conflicts checking.
 * 
 * 62    10/26/97 3:20p Hoffoss
 * Added many missing features to control config screen.
 * 
 * 61    10/25/97 5:41p Hoffoss
 * More functionality added to the controls config screen.
 * 
 * 60    10/24/97 11:00p Hoffoss
 * Controls config screen much improved.
 * 
 * 59    10/22/97 4:50p Hoffoss
 * Disabled throttle by default.
 * 
 * 58    10/22/97 11:00a Hoffoss
 * Changed VIEW_SLEW and VIEW_EXTERNAL to be continuous rather than
 * trigger actions.
 * 
 * 57    10/21/97 7:05p Hoffoss
 * Overhauled the key/joystick control structure and usage throughout the
 * entire FreeSpace code.  The whole system is very different now.
 * 
 * 56    10/18/97 7:19p Hoffoss
 * Added timestamp recording when a key is pressed.
 * 
 * 55    10/13/97 4:33p Hoffoss
 * Made training messages go away after time.
 * 
 * 54    10/13/97 3:24p Hoffoss
 * Made it so training message text can be arbitrarily bolded.
 * 
 * 53    9/24/97 4:52p Hoffoss
 * Changed training message key token handling, and implemented a new
 * training message system method to test out for a while.
 * 
 * 52    9/14/97 10:24p Lawrance
 * add damage screen popup window
 * 
 * 51    9/10/97 6:02p Hoffoss
 * Added code to check for key-pressed sexp operator in FreeSpace as part
 * of training mission stuff.
 * 
 * 50    9/09/97 3:39p Sandeep
 * warning level 4 bugs
 * $NoKeywords: $
 *
*/

#include "controlsconfig.h"
#include "key.h"
#include "joy.h"
#include "localize.h"

#define TARGET_TAB			0
#define SHIP_TAB				1
#define WEAPON_TAB			2
#define COMPUTER_TAB			3

int Failed_key_index;

// assume control keys are used as modifiers until we find out 
int Shift_is_modifier;
int Ctrl_is_modifier;
int Alt_is_modifier;

int Axis_enabled[JOY_NUM_AXES] = { 1, 1, 1, 0, 0, 0 };
int Axis_enabled_defaults[JOY_NUM_AXES] = { 1, 1, 1, 0, 0, 0 };
int Invert_axis[JOY_NUM_AXES] = { 0, 0, 0, 0, 0, 0 };
int Invert_axis_defaults[JOY_NUM_AXES] = { 0, 0, 0, 0, 0, 0 };

// arrays which hold the key mappings.  The array index represents a key-independent action.
//
//XSTR:OFF
config_item Control_config[CCFG_MAX + 1] = {
	// targeting a ship
	{                           KEY_T,				-1, TARGET_TAB,	"Target Next Ship", CC_TYPE_TRIGGER, -1, -1, 0 },
	{             KEY_SHIFTED | KEY_T,				-1, TARGET_TAB,	"Target Previous Ship", CC_TYPE_TRIGGER, -1, -1, 0 },
	{                           KEY_H,				2,  TARGET_TAB,	"Target Next Closest Hostile Ship", CC_TYPE_TRIGGER, -1, -1, 0 },
	{	           KEY_SHIFTED | KEY_H,				-1, TARGET_TAB,	"Target Previous Closest Hostile Ship", CC_TYPE_TRIGGER, -1, -1, 0 },
	{ KEY_ALTED |               KEY_H,				-1, TARGET_TAB,	"Toggle Auto Targeting", CC_TYPE_TRIGGER, -1, -1, 0 },
	{                           KEY_F,				-1, TARGET_TAB,	"Target Next Closest Friendly Ship", CC_TYPE_TRIGGER, -1, -1, 0 },
	{             KEY_SHIFTED | KEY_F,				-1, TARGET_TAB,	"Target Previous Closest Friendly Ship", CC_TYPE_TRIGGER, -1, -1, 0 },
	{                           KEY_Y,				4,  TARGET_TAB,	"Target Ship in Reticle", CC_TYPE_TRIGGER, -1, -1, 0 },
	{                           KEY_G,				-1, TARGET_TAB,	"Target Target's Nearest Attacker", CC_TYPE_TRIGGER, -1, -1, 0 },
	{ KEY_ALTED	|					 KEY_Y,				-1, TARGET_TAB,	"Target Last Ship to Send Transmission", CC_TYPE_TRIGGER, -1, -1, 0 },
	{ KEY_ALTED |               KEY_T,				-1, TARGET_TAB,	"Turn Off Auto-Targeting", CC_TYPE_TRIGGER, -1, -1, 0 },

	// targeting a ship's subsystem
	{                           KEY_V,				-1, TARGET_TAB,	"Target Subsystem in Reticle", CC_TYPE_TRIGGER, -1, -1, 0 },
	{                           KEY_S,				-1, TARGET_TAB,	"Target Next Subsystem", CC_TYPE_TRIGGER, -1, -1, 0 },
	{             KEY_SHIFTED | KEY_S,				-1, TARGET_TAB,	"Target Previous Subsystem", CC_TYPE_TRIGGER, -1, -1, 0 },
	{ KEY_ALTED |               KEY_S,				-1, TARGET_TAB,	"Turn Off Auto-Targeting of Subsystems", CC_TYPE_TRIGGER, -1, -1, 0 },

	// matching speed
	{                           KEY_M,				-1, COMPUTER_TAB,	"Match Target Speed", CC_TYPE_TRIGGER, -1, -1, 0 },
	{ KEY_ALTED |               KEY_M,				-1, COMPUTER_TAB,	"Toggle Auto Speed Matching", CC_TYPE_TRIGGER, -1, -1, 0 },

	// weapons
	#ifdef PANDORA
	{                           KEY_LCTRL,			0,	 WEAPON_TAB,	"Fire Primary Weapon", CC_TYPE_CONTINUOUS, KEY_RCTRL, -1, 0 },
	{                           KEY_SPACEBAR,	    1,  WEAPON_TAB,	"Fire Secondary Weapon", CC_TYPE_CONTINUOUS, KEY_SHIFTED | KEY_RCTRL, -1, 0 },
	{                           KEY_PERIOD,		-1, WEAPON_TAB,	"Cycle Forward Primary Weapon", CC_TYPE_TRIGGER, KEY_2, -1, 0 },
	{                           KEY_COMMA,			-1, WEAPON_TAB,	"Cycle Backward Primary Weapon", CC_TYPE_TRIGGER, KEY_3, -1, 0 },
	{                           KEY_DIVIDE,		-1, WEAPON_TAB,	"Cycle Secondary Weapon Bank", CC_TYPE_TRIGGER, KEY_W, -1, 0 },
	{             KEY_SHIFTED | KEY_DIVIDE,		-1, WEAPON_TAB,	"Cycle Secondary Weapon Firing Rate", CC_TYPE_TRIGGER, KEY_Q, -1, 0 },
	#else
	{                           KEY_LCTRL,			0,	 WEAPON_TAB,	"Fire Primary Weapon", CC_TYPE_CONTINUOUS, -1, -1, 0 },
	{                           KEY_SPACEBAR,		1,  WEAPON_TAB,	"Fire Secondary Weapon", CC_TYPE_CONTINUOUS, -1, -1, 0 },
	{                           KEY_PERIOD,		-1, WEAPON_TAB,	"Cycle Forward Primary Weapon", CC_TYPE_TRIGGER, -1, -1, 0 },
	{                           KEY_COMMA,			-1, WEAPON_TAB,	"Cycle Backward Primary Weapon", CC_TYPE_TRIGGER, -1, -1, 0 },
	{                           KEY_DIVIDE,		-1, WEAPON_TAB,	"Cycle Secondary Weapon Bank", CC_TYPE_TRIGGER, -1, -1, 0 },
	{             KEY_SHIFTED | KEY_DIVIDE,		-1, WEAPON_TAB,	"Cycle Secondary Weapon Firing Rate", CC_TYPE_TRIGGER, -1, -1, 0 },
	#endif
	{                           KEY_X,				3,	 WEAPON_TAB,	"Launch Countermeasure", CC_TYPE_TRIGGER, -1, -1, 0 },

	// controls
	#ifdef PANDORA
	{                           KEY_A,				-1, SHIP_TAB,		"Forward Thrust", CC_TYPE_CONTINUOUS, KEY_UP, -1, 0 },
	{                           KEY_Z,			-1, SHIP_TAB,		"Reverse Thrust", CC_TYPE_CONTINUOUS, KEY_DOWN, -1, 0 },
	{                           KEY_PAD7,			-1, SHIP_TAB,		"Bank Left", CC_TYPE_CONTINUOUS, KEY_LEFT, -1, 0 },
	{                           KEY_PAD9,			-1, SHIP_TAB,		"Bank Right", CC_TYPE_CONTINUOUS, KEY_RIGHT, -1, 0 },
	#else
	{                           KEY_A,				-1, SHIP_TAB,		"Forward Thrust", CC_TYPE_CONTINUOUS, -1, -1, 0 },
	{                           KEY_Z,				-1, SHIP_TAB,		"Reverse Thrust", CC_TYPE_CONTINUOUS, -1, -1, 0 },
	{                           KEY_PAD7,			-1, SHIP_TAB,		"Bank Left", CC_TYPE_CONTINUOUS, -1, -1, 0 },
	{                           KEY_PAD9,			-1, SHIP_TAB,		"Bank Right", CC_TYPE_CONTINUOUS, -1, -1, 0 },
	#endif
	{                           KEY_PAD8,			-1, SHIP_TAB,		"Pitch Forward", CC_TYPE_CONTINUOUS, -1, -1, 0 },
	{                           KEY_PAD2,			-1, SHIP_TAB,		"Pitch Backward", CC_TYPE_CONTINUOUS, -1, -1, 0 },
	{                           KEY_PAD4,			-1, SHIP_TAB,		"Turn Left", CC_TYPE_CONTINUOUS, -1, -1, 0 },
	{                           KEY_PAD6,			-1, SHIP_TAB,		"Turn Right", CC_TYPE_CONTINUOUS, -1, -1, 0 },

	// throttle controls
	{                           KEY_BACKSP,		-1, SHIP_TAB,		"Set Throttle to Zero", CC_TYPE_TRIGGER, -1, -1, 0 },
	#ifdef PANDORA
	{                           KEY_SLASH,			-1, SHIP_TAB,		"Set Throttle to Max", CC_TYPE_TRIGGER, KEY_0, -1, 0 },
	#else
	{                           KEY_SLASH,			-1, SHIP_TAB,		"Set Throttle to Max", CC_TYPE_TRIGGER, -1, -1, 0 },
	#endif
	{                           KEY_LBRACKET,		-1, SHIP_TAB,		"Set Throttle to One-Third", CC_TYPE_TRIGGER, -1, -1, 0 },
	{                           KEY_RBRACKET,		-1, SHIP_TAB,		"Set Throttle to Two-Thirds", CC_TYPE_TRIGGER, -1, -1, 0 },
	#ifdef PANDORA
	{                           KEY_EQUAL,			-1, SHIP_TAB,		"Increase Throttle 5 Percent", CC_TYPE_TRIGGER, KEY_PERIOD, -1, 0 },
	{                           KEY_MINUS,			-1, SHIP_TAB,		"Decrease Throttle 5 Percent", CC_TYPE_TRIGGER, KEY_COMMA, -1, 0 },
	#else
	{                           KEY_EQUAL,			-1, SHIP_TAB,		"Increase Throttle 5 Percent", CC_TYPE_TRIGGER, -1, -1, 0 },
	{                           KEY_MINUS,			-1, SHIP_TAB,		"Decrease Throttle 5 Percent", CC_TYPE_TRIGGER, -1, -1, 0 },
	#endif

	// squadmate messaging
	{             KEY_SHIFTED | KEY_A,				-1, COMPUTER_TAB,	"Attack My Target", CC_TYPE_TRIGGER, -1, -1, 0 },
	{             KEY_SHIFTED | KEY_Z,				-1, COMPUTER_TAB,	"Disarm My Target", CC_TYPE_TRIGGER, -1, -1, 0 },
	{             KEY_SHIFTED | KEY_D,				-1, COMPUTER_TAB,	"Disable My Target", CC_TYPE_TRIGGER, -1, -1, 0 },
	{             KEY_SHIFTED | KEY_V,				-1, COMPUTER_TAB,	"Attack my Subsystem", CC_TYPE_TRIGGER, -1, -1, 0 },
	{             KEY_SHIFTED | KEY_X,				-1, COMPUTER_TAB,	"Capture My Target", CC_TYPE_TRIGGER, -1, -1, 0 },
	{             KEY_SHIFTED | KEY_E,				-1, COMPUTER_TAB,	"Engage Enemy", CC_TYPE_TRIGGER, -1, -1, 0 },
	{             KEY_SHIFTED | KEY_W,				-1, COMPUTER_TAB,	"Form on my Wing", CC_TYPE_TRIGGER, -1, -1, 0 },
	{             KEY_SHIFTED | KEY_I,				-1, COMPUTER_TAB,	"Ignore my Target", CC_TYPE_TRIGGER, -1, -1, 0 },
	{             KEY_SHIFTED | KEY_P,				-1, COMPUTER_TAB,	"Protect my Target", CC_TYPE_TRIGGER, -1, -1, 0 },
	{             KEY_SHIFTED | KEY_C,				-1, COMPUTER_TAB,	"Cover me", CC_TYPE_TRIGGER, -1, -1, 0 },
	{             KEY_SHIFTED | KEY_J,				-1, COMPUTER_TAB,	"Return to base", CC_TYPE_TRIGGER, -1, -1, 0 },
	{				  KEY_SHIFTED | KEY_R,				-1, COMPUTER_TAB, "Rearm me", CC_TYPE_TRIGGER, -1, -1, 0 },

	{									 KEY_R,				6,  TARGET_TAB,	"Target Closest Attacking Ship", CC_TYPE_TRIGGER, -1, -1, 0 },

	// Views
	{                           KEY_PADMULTIPLY,	-1, COMPUTER_TAB,	"Chase View", CC_TYPE_TRIGGER, -1, -1, 0 },
	{                           KEY_PADPERIOD,	-1, COMPUTER_TAB,	"External View", CC_TYPE_TRIGGER, -1, -1, 0 },
	{                           KEY_PADENTER,		-1, COMPUTER_TAB,	"Toggle External Camera Lock", CC_TYPE_TRIGGER, -1, -1, 0 },
	{                           KEY_PAD0,			-1, COMPUTER_TAB,	"Free Look View", CC_TYPE_CONTINUOUS, -1, -1, 0 },
	{                           KEY_PADDIVIDE,	-1, COMPUTER_TAB,	"Current Target View", CC_TYPE_TRIGGER, -1, -1, 0 },
	{                           KEY_PADPLUS,		-1, COMPUTER_TAB,	"Increase View Distance", CC_TYPE_CONTINUOUS, -1, -1, 0 },
	{                           KEY_PADMINUS,		-1, COMPUTER_TAB,	"Decrease View Distance", CC_TYPE_CONTINUOUS, -1, -1, 0 },
	{                           KEY_PAD5,			-1, COMPUTER_TAB,	"Center View", CC_TYPE_CONTINUOUS, -1, -1, 0 },
	{									 -1,					33, COMPUTER_TAB, "View Up", CC_TYPE_CONTINUOUS, -1, -1, 0 },
	{									 -1,					32, COMPUTER_TAB, "View Rear", CC_TYPE_CONTINUOUS, -1, -1, 0 },
	{									 -1,					34, COMPUTER_TAB, "View Left", CC_TYPE_CONTINUOUS, -1, -1, 0 },
	{									 -1,					35, COMPUTER_TAB, "View Right", CC_TYPE_CONTINUOUS, -1, -1, 0 },

	{                           KEY_RAPOSTRO,		-1, COMPUTER_TAB,	"Cycle Radar Range", CC_TYPE_TRIGGER, -1, -1, 0 },
	{                           KEY_C,				-1, COMPUTER_TAB, "Communications Menu", CC_TYPE_TRIGGER, -1, -1, 0 },
	{                           -1,					-1, -1,				"Show Objectives", CC_TYPE_TRIGGER, -1, -1, 0 },
	{ KEY_ALTED |               KEY_J,				-1, COMPUTER_TAB,	"Enter Subspace (End Mission)", CC_TYPE_TRIGGER, -1, -1, 0 },
	{                           KEY_J,				-1, TARGET_TAB,	"Target Target's Target", CC_TYPE_TRIGGER, -1, -1, 0 },
	#ifdef PANDORA
	{                           KEY_TAB,	     		5,  SHIP_TAB,		"Afterburner", CC_TYPE_CONTINUOUS, KEY_1, -1, 0 },
	#else
	{                           KEY_TAB,			5,  SHIP_TAB,		"Afterburner", CC_TYPE_CONTINUOUS, -1, -1, 0 },
	#endif
	
	{                           KEY_INSERT,		-1, COMPUTER_TAB,	"Increase Weapon Energy", CC_TYPE_TRIGGER, -1, -1, 0 },
	{                           KEY_DELETE,		-1, COMPUTER_TAB,	"Decrease Weapon Energy", CC_TYPE_TRIGGER, -1, -1, 0 },
	{                           KEY_HOME,			-1, COMPUTER_TAB,	"Increase Shield Energy", CC_TYPE_TRIGGER, -1, -1, 0 },
	{                           KEY_END,			-1, COMPUTER_TAB,	"Decrease Shield Energy", CC_TYPE_TRIGGER, -1, -1, 0 },
	{                           KEY_PAGEUP,		-1, COMPUTER_TAB,	"Increase Engine Energy", CC_TYPE_TRIGGER, -1, -1, 0 },
	{                           KEY_PAGEDOWN,		-1, COMPUTER_TAB,	"Decrease Engine Energy", CC_TYPE_TRIGGER, -1, -1, 0 },
	{ KEY_ALTED |               KEY_D,				-1, COMPUTER_TAB, "Equalize Energy Settings", CC_TYPE_TRIGGER, -1, -1, 0 },

	#ifdef PANDORA
	{             				KEY_Q,				7,  COMPUTER_TAB,	"Equalize Shield", CC_TYPE_TRIGGER, KEY_SHIFTED | KEY_Q, -1, 0 },
	{             				KEY_UP,				-1, COMPUTER_TAB,	"Augment Forward Shield", CC_TYPE_TRIGGER, KEY_SHIFTED | KEY_UP, -1, 0 },
	{            				KEY_DOWN,			-1, COMPUTER_TAB,	"Augment Rear Shield", CC_TYPE_TRIGGER, KEY_SHIFTED | KEY_DOWN, -1, 0 },
	{             				KEY_LEFT,			-1, COMPUTER_TAB,	"Augment Left Shield", CC_TYPE_TRIGGER, KEY_SHIFTED | KEY_LEFT, -1, 0 },
	{             				KEY_RIGHT,			-1, COMPUTER_TAB,	"Augment Right Shield", CC_TYPE_TRIGGER, KEY_SHIFTED | KEY_RIGHT, -1, 0 },
	{                           KEY_SCROLLOCK,		-1, COMPUTER_TAB,	"Transfer Energy Laser->Shield", CC_TYPE_TRIGGER, KEY_8, -1, 0 },
	{             KEY_SHIFTED | KEY_SCROLLOCK,		-1, COMPUTER_TAB,	"Transfer Energy Shield->Laser", CC_TYPE_TRIGGER, KEY_SHIFTED | KEY_8, -1, 0 },
	#else
	{                           KEY_Q,				7,  COMPUTER_TAB,	"Equalize Shield", CC_TYPE_TRIGGER, -1, -1, 0 },
	{                           KEY_UP,				-1, COMPUTER_TAB,	"Augment Forward Shield", CC_TYPE_TRIGGER, -1, -1, 0 },
	{                           KEY_DOWN,			-1, COMPUTER_TAB,	"Augment Rear Shield", CC_TYPE_TRIGGER, -1, -1, 0 },
	{                           KEY_LEFT,			-1, COMPUTER_TAB,	"Augment Left Shield", CC_TYPE_TRIGGER, -1, -1, 0 },
	{                           KEY_RIGHT,			-1, COMPUTER_TAB,	"Augment Right Shield", CC_TYPE_TRIGGER, -1, -1, 0 },
	{                           KEY_SCROLLOCK,	-1, COMPUTER_TAB,	"Transfer Energy Laser->Shield", CC_TYPE_TRIGGER, -1, -1, 0 },
	{             KEY_SHIFTED | KEY_SCROLLOCK,	-1, COMPUTER_TAB,	"Transfer Energy Shield->Laser", CC_TYPE_TRIGGER, -1, -1, 0 },
	#endif
	{                           -1,					-1, -1,				"Show Damage Popup Window", CC_TYPE_TRIGGER, -1, -1, 0 },	

	{                           -1,					-1, SHIP_TAB,		"Bank When Pressed", CC_TYPE_CONTINUOUS, -1, -1, 0 },
	{									 -1,					-1, -1,				"Show NavMap", CC_TYPE_TRIGGER, -1, -1, 0 },
	{ KEY_ALTED |	             KEY_E,				-1, COMPUTER_TAB,	"Add or Remove Escort", CC_TYPE_TRIGGER, -1, -1, 0 },
	{ KEY_ALTED | KEY_SHIFTED | KEY_E,				-1, COMPUTER_TAB,	"Clear Escort List", CC_TYPE_TRIGGER, -1, -1, 0 },
	{					             KEY_E,				-1, TARGET_TAB,	"Target Next Escort Ship", CC_TYPE_TRIGGER, -1, -1, 0 },
	{ KEY_ALTED	|					 KEY_R,				-1, TARGET_TAB,	"Target Closest Repair Ship", CC_TYPE_TRIGGER, -1, -1, 0 },

	{                           KEY_U,				-1, TARGET_TAB,	"Target Next Uninspected Cargo", CC_TYPE_TRIGGER, -1, -1, 0 },
	{             KEY_SHIFTED | KEY_U,				-1, TARGET_TAB,	"Target Previous Uninspected Cargo", CC_TYPE_TRIGGER, -1, -1, 0 },
	{									 KEY_N,				-1, TARGET_TAB,	"Target Newest Ship In Area", CC_TYPE_TRIGGER, -1, -1, 0 },
	{                           KEY_K,				-1, TARGET_TAB,	"Target Next Live Turret", CC_TYPE_TRIGGER, -1, -1, 0 },
	{             KEY_SHIFTED | KEY_K,				-1, TARGET_TAB,	"Target Previous Live Turret", CC_TYPE_TRIGGER, -1, -1, 0 },

	{									 KEY_B,				-1, TARGET_TAB,	"Target Next Hostile Bomb or Bomber", CC_TYPE_TRIGGER, -1, -1, 0 },
	{             KEY_SHIFTED | KEY_B,				-1, TARGET_TAB,	"Target Previous Hostile Bomb or Bomber", CC_TYPE_TRIGGER, -1, -1, 0 },

	// multiplayer messaging keys
	#ifdef PANDORA
	{ 									 KEY_1,				-1, COMPUTER_TAB,	"(Multiplayer) Message All", CC_TYPE_CONTINUOUS, KEY_ALTED | KEY_1, -1, 0 },
	{									 KEY_2,				-1, COMPUTER_TAB, "(Multiplayer) Message Friendly", CC_TYPE_CONTINUOUS, KEY_ALTED | KEY_2, -1, 0 },
	{ 									 KEY_3,				-1, COMPUTER_TAB, "(Multiplayer) Message Hostile", CC_TYPE_CONTINUOUS, KEY_ALTED | KEY_3, -1, 0 },
	{ 									 KEY_4,				-1, COMPUTER_TAB, "(Multiplayer) Message Target", CC_TYPE_CONTINUOUS, KEY_ALTED	| KEY_4, -1, 0 },
	#else
	{									 KEY_1,				-1, COMPUTER_TAB,	"(Multiplayer) Message All", CC_TYPE_CONTINUOUS, -1, -1, 0 },
	{									 KEY_2,				-1, COMPUTER_TAB, "(Multiplayer) Message Friendly", CC_TYPE_CONTINUOUS, -1, -1, 0 },
	{									 KEY_3,				-1, COMPUTER_TAB, "(Multiplayer) Message Hostile", CC_TYPE_CONTINUOUS, -1, -1, 0 },
	{									 KEY_4,				-1, COMPUTER_TAB, "(Multiplayer) Message Target", CC_TYPE_CONTINUOUS, -1, -1, 0 },
	#endif
	{ KEY_ALTED	|					 KEY_X,				-1, COMPUTER_TAB, "(Multiplayer) Observer zoom to target", CC_TYPE_TRIGGER, -1, -1, 0 },	

	{             KEY_SHIFTED | KEY_PERIOD,		-1, COMPUTER_TAB,	"Increase time compression", CC_TYPE_TRIGGER, -1, -1, 0 },
	{             KEY_SHIFTED | KEY_COMMA,			-1, COMPUTER_TAB,	"Decrease time compression", CC_TYPE_TRIGGER, -1, -1, 0 },

	{									 KEY_L,				-1, COMPUTER_TAB, "Toggle high HUD contrast", CC_TYPE_TRIGGER, -1, -1, 0 },	

	{				  KEY_SHIFTED | KEY_N,				-1, COMPUTER_TAB, "(Multiplayer) Toggle network info", CC_TYPE_TRIGGER, -1, -1, 0 },
	{				  KEY_SHIFTED | KEY_END,			-1, COMPUTER_TAB, "(Multiplayer) Self destruct", CC_TYPE_TRIGGER, -1, -1, 0 },

	{                           -1,					-1, -1,				"", CC_TYPE_TRIGGER, -1, -1, 0 }
};

const char *Scan_code_text_german[] = {
	"",				"Esc",				"1",				"2",				"3",				"4",				"5",				"6",
#ifndef MAKE_FS1
	"7",				"8",				"9",				"0",				"Akzent '",				"\xE1",				"R\x81""cktaste",		"Tab",
#else
	"7",				"8",				"9",				"0",				"?",				"Akzent '",				"R\x81""cktaste",		"Tab",
#endif
	"Q",				"W",				"E",				"R",				"T",				"Z",				"U",				"I",
	"O",				"P",				"\x9A",				"+",				"Eingabe",			"Strg Links",			"A",				"S",

	"D",				"F",				"G",				"H",				"J",				"K",				"L",				"\x99",
#ifndef MAKE_FS1
	"\xAE",				"`",				"Shift",			"#",				"Y",				"X",				"C",				"V",
#else
	"\x8E",				"`",				"Shift",			"#",				"Y",				"X",				"C",				"V",
#endif
	"B",				"N",				"M",				",",				".",				"-",				"Shift",			"Num *",
	"Alt",				"Leertaste",			"Hochstell",			"F1",				"F2",				"F3",				"F4",				"F5",

	"F6",				"F7",				"F8",				"F9",				"F10",				"Pause",			"Rollen",			"Num 7",
	"Num 8",			"Num 9",			"Num -",			"Num 4",			"Num 5",			"Num 6",			"Num +",			"Num 1",
	"Num 2",			"Num 3",			"Num 0",			"Num ,",			"",				"",				"",				"F11",
	"F12",				"",				"",				"",				"",				"",				"",				"",

	"",				"",				"",				"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",				"",				"",				"",

	"",				"",				"",				"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",				"",				"",				"",
#ifdef PANDORA
	"",				"",				"",				"",				"Num Eingabe",			"[R]",			"",				"",
#else
	"",				"",				"",				"",				"Num Eingabe",			"[R]",			"",				"",
#endif

	"",				"",				"",				"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",				"Num /",			"",				"Druck",
	"Alt",				"",				"",				"",				"",				"",				"",				"",

#ifdef PANDORA
	"",				"",				"",				"",				"",				"Num Lock",			"",				"[A]",
	"Pfeil Hoch",	"[Y]",			"",				"Pfeil Links",	"",				"Pfeil Rechts",		"",				"[B]",
	"Pfeil Runter", "[X]",			"Einfg",		"Entf",			"",				"",					"",				"",
#else
	"",				"",				"",				"",				"",				"Num Lock",			"",				"Pos 1",
	"Pfeil Hoch",			"Bild Hoch",			"",				"Pfeil Links",			"",				"Pfeil Rechts",			"",				"Ende",
	"Pfeil Runter", 			"Bild Runter",			"Einfg",			"Entf",				"",				"",				"",				"",
#endif
	"",				"",				"",				"",				"",				"",				"",				"",

	"",				"",				"",				"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",				"",				"",				"",
};

const char *Joy_button_text_german[] = {
	"Knopf 1",		"Knopf 2",		"Knopf 3",		"Knopf 4",		"Knopf 5",		"Knopf 6",
	"Knopf 7",		"Knopf 8",		"Knopf 9",		"Knopf 10",		"Knopf 11",		"Knopf 12",
	"Knopf 13",		"Knopf 14",		"Knopf 15",		"Knopf 16",		"Knopf 17",		"Knopf 18",
	"Knopf 19",		"Knopf 20",		"Knopf 21",		"Knopf 22",		"Knopf 23",		"Knopf 24",
	"Knopf 25",		"Knopf 26",		"Knopf 27",		"Knopf 28",		"Knopf 29",		"Knopf 30",
	"Knopf 31",		"Knopf 32",		"Hut Hinten",	"Hut Vorne",	"Hut Links",	"Hut Rechts"
};

const char *Scan_code_text_french[] = {
	"",				"\x90""chap",			"1",				"2",				"3",				"4",				"5",				"6",
	"7",				"8",				"9",				"0",				"-",				"=",				"Fl\x82""che Ret.",			"Tab",
	"Q",				"W",				"E",				"R",				"T",				"Y",				"U",				"I",
	"O",				"P",				"[",				"]",				"Entr\x82""e",			"Ctrl Gauche",			"A",				"S",

	"D",				"F",				"G",				"H",				"J",				"K",				"L",				";",
	"'",				"`",				"Maj.",			"\\",				"Z",				"X",				"C",				"V",
	"B",				"N",				"M",				",",				".",				"/",				"Maj.",			"Pav\x82 *",
	"Alt",				"Espace",			"Verr. Maj.",			"F1",				"F2",				"F3",				"F4",				"F5",

	"F6",				"F7",				"F8",				"F9",				"F10",				"Pause",			"Arret defil",		"Pav\x82 7",
	"Pav\x82 8",			"Pav\x82 9",			"Pav\x82 -",			"Pav\x82 4",			"Pav\x82 5",			"Pav\x82 6",			"Pav\x82 +",			"Pav\x82 1",
	"Pav\x82 2",			"Pav\x82 3",			"Pav\x82 0",			"Pav\x82 .",			"",				"",				"",				"F11",
	"F12",				"",				"",				"",				"",				"",				"",				"",

	"",				"",				"",				"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",				"",				"",				"",

	"",				"",				"",				"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",				"",				"",				"",
#ifdef PANDORA
	"",				"",				"",				"",				"Pav\x82 Entr",	"[R]",			"",				"",
#else
	"",				"",				"",				"",				"Pav\x82 Entr",	"Ctrl Droite",	"",				"",
#endif

	"",				"",				"",				"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",				"Pav\x82 /",			"",				"Impr \x82""cran",
	"Alt",				"",				"",				"",				"",				"",				"",				"",

#ifdef PANDORA
	"",				"",				"",				"",				"",				"Verr num",			"",				"[A]",
	"Fl\x82""che Haut",			"[Y]",			"",				"Fl\x82""che Gauche",			"",				"Fl\x82""che Droite",			"",			"[B]",
	"Fl\x82""che Bas", 			"[X]",			"Inser",			"Suppr",			"",				"",				"",				"",
#else
	"",				"",				"",				"",				"",				"Verr num",			"",				"Orig.",
	"Fl\x82""che Haut",			"Page Haut",		"",				"Fl\x82""che Gauche","",			"Fl\x82""che Droite",		"",			"Fin",
	"Fl\x82""che Bas", 			"Page Bas",			"Inser",		"Suppr",		"",				"",				"",				"",
#endif
	"",				"",				"",				"",				"",				"",				"",				"",

	"",				"",				"",				"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",				"",				"",				"",
};

const char *Joy_button_text_french[] = {
	"Bouton 1",		"Bouton 2",		"Bouton 3",		"Bouton 4",		"Bouton 5",		"Bouton 6",
	"Bouton 7",		"Bouton 8",		"Bouton 9",		"Bouton 10",		"Bouton 11",		"Bouton 12",
	"Bouton 13",		"Bouton 14",		"Bouton 15",		"Bouton 16",		"Bouton 17",		"Bouton 18",
	"Bouton 19",		"Bouton 20",		"Bouton 21",		"Bouton 22",		"Bouton 23",		"Bouton 24",
	"Bouton 25",		"Bouton 26",		"Bouton 27",		"Bouton 28",		"Bouton 29",		"Bouton 30",
	"Bouton 31",		"Bouton 32",		"Chapeau Arrière",		"Chapeau Avant",		"Chapeau Gauche",		"Chapeau Droite"
};

//	This is the text that is displayed on the screen for the keys a player selects
const char *Scan_code_text_english[] = {
	"",				"Esc",			"1",				"2",				"3",				"4",				"5",				"6",
	"7",				"8",				"9",				"0",				"-",				"=",				"Backspace",	"Tab",
	"Q",				"W",				"E",				"R",				"T",				"Y",				"U",				"I",
	"O",				"P",				"[",				"]",				"Enter",			"Left Ctrl",	"A",				"S",

	"D",				"F",				"G",				"H",				"J",				"K",				"L",				";",
	"'",				"`",				"Shift",			"\\",				"Z",				"X",				"C",				"V",
	"B",				"N",				"M",				",",				".",				"/",				"Shift",			"Pad *",
	"Alt",			"Spacebar",		"Caps Lock",	"F1",				"F2",				"F3",				"F4",				"F5",

	"F6",				"F7",				"F8",				"F9",				"F10",			"Pause",			"Scroll Lock",	"Pad 7",
	"Pad 8",			"Pad 9",			"Pad -",			"Pad 4",			"Pad 5",			"Pad 6",			"Pad +",			"Pad 1",
	"Pad 2",			"Pad 3",			"Pad 0",			"Pad .",			"",				"",				"",				"F11",
	"F12",			"",				"",				"",				"",				"",				"",				"",

	"",				"",				"",				"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",				"",				"",				"",

	"",				"",				"",				"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",				"",				"",				"",
#ifdef PANDORA
	"",				"",				"",				"",				"Pad Enter",	"[R]",			"",				"",
#else
	"",				"",				"",				"",				"Pad Enter",	"Right Ctrl",	"",				"",
#endif

	"",				"",				"",				"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",				"Pad /",		"",				"Print Scrn",
	"Alt",			"",				"",				"",				"",				"",				"",				"",

#ifdef PANDORA
	"",				"",				"",				"",				"",				"Num Lock",		"",				"[A]",
	"Up Arrow",		"[Y]",			"",				"Left Arrow",	"",				"Right Arrow",	"",				"[B]",
	"Down Arrow",  	"[X]",			"Insert",		"Delete",		"",				"",				"",				"",
#else
	"",				"",				"",				"",				"",				"Num Lock",		"",				"Home",
	"Up Arrow",		"Page Up",		"",				"Left Arrow",	"",				"Right Arrow",	"",				"End",
	"Down Arrow",  "Page Down",		"Insert",		"Delete",		"",				"",				"",				"",
#endif
	"",				"",				"",				"",				"",				"",				"",				"",

	"",				"",				"",				"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",				"",				"",				"",
};

const char *Joy_button_text_english[] = {
	"Button 1",		"Button 2",		"Button 3",		"Button 4",		"Button 5",		"Button 6",
	"Button 7",		"Button 8",		"Button 9",		"Button 10",	"Button 11",	"Button 12",
	"Button 13",	"Button 14",	"Button 15",	"Button 16",	"Button 17",	"Button 18",
	"Button 19",	"Button 20",	"Button 21",	"Button 22",	"Button 23",	"Button 24",
	"Button 25",	"Button 26",	"Button 27",	"Button 28",	"Button 29",	"Button 30",
	"Button 31",	"Button 32",	"Hat Back",		"Hat Forward",	"Hat Left",		"Hat Right"
};

const char *Scan_code_text_polish[] = {
	"",				"Esc",			"1",				"2",				"3",				"4",				"5",				"6",
	"7",				"8",				"9",				"0",				"-",				"=",				"Backspace",	"Tab",
	"Q",				"W",				"E",				"R",				"T",				"Y",				"U",				"I",
	"O",				"P",				"[",				"]",				"Enter",			"Lewy Ctrl",	"A",				"S",

	"D",				"F",				"G",				"H",				"J",				"K",				"L",				";",
	"'",				"`",				"LShift",			"\\",				"Z",				"X",				"C",				"V",
	"B",				"N",				"M",				",",				".",				"/",				"PShift",			"Num *",
	"Alt",			"Spacja",		"CapsLock",	"F1",				"F2",				"F3",				"F4",				"F5",

	"F6",				"F7",				"F8",				"F9",				"F10",			"Pause",			"Scrlock",	"Num 7",
	"Num 8",			"Num 9",			"Num -",			"Num 4",			"Num 5",			"Num 6",			"Num +",			"Num 1",
	"Num 2",			"Num 3",			"Num 0",			"Num .",			"",				"",				"",				"F11",
	"F12",			"",				"",				"",				"",				"",				"",				"",

	"",				"",				"",				"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",				"",				"",				"",

	"",				"",				"",				"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",				"",				"",				"",
#ifdef PANDORA
	"",				"",				"",				"",				"Num Enter",	"[R]",			"",				"",
#else
	"",				"",				"",				"",				"Num Enter",	"Prawy Ctrl",	"",				"",
#endif

	"",				"",				"",				"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",				"Num /",			"",				"PrntScr",
	"Alt",			"",				"",				"",				"",				"",				"",				"",

#ifdef PANDORA
	"",				"",				"",				"",				"",				"Num Lock",		"",				"[A]",
	"Kursor G\xF3ra",		"[Y]",	"",				"Kursor Lewo",	"",				"Kursor Prawo",	"",				"[B]",
	"Kursor D\xF3\xB3",  "[B]",		"Insert",		"Delete",		"",				"",				"",				"",
#else
	"",				"",				"",				"",				"",				"Num Lock",		"",				"Home",
	"Kursor G\xF3ra",	"Page Up",	"",				"Kursor Lewo",	"",				"Kursor Prawo",	"",				"End",
	"Kursor D\xF3\xB3",  "Page Down","Insert",		"Delete",		"",				"",				"",				"",
#endif
	"",				"",				"",				"",				"",				"",				"",				"",

	"",				"",				"",				"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",				"",				"",				"",
};

const char *Joy_button_text_polish[] = {
	"Przyc.1",		"Przyc.2",		"Przyc.3",		"Przyc.4",		"Przyc.5",		"Przyc.6",
	"Przyc.7",		"Przyc.8",		"Przyc.9",		"Przyc.10",	"Przyc.11",	"Przyc.12",
	"Przyc.13",	"Przyc.14",	"Przyc.15",	"Przyc.16",	"Przyc.17",	"Przyc.18",
	"Przyc.19",	"Przyc.20",	"Przyc.21",	"Przyc.22",	"Przyc.23",	"Przyc.24",
	"Przyc.25",	"Przyc.26",	"Przyc.27",	"Przyc.28",	"Przyc.29",	"Przyc.30",
	"Przyc.31",	"Przyc.32",	"Hat Ty\xB3",		"Hat Prz\xF3\x64",	"Hat Lewo",		"Hat Prawo"
};

const char **Scan_code_text = Scan_code_text_english;
const char **Joy_button_text = Joy_button_text_english;

void set_modifier_status()
{
	int i;

	Alt_is_modifier = 0;
	Shift_is_modifier = 0;
	Ctrl_is_modifier = 0;

	for (i=0; i<CCFG_MAX; i++) {
		if (Control_config[i].key_id < 0)
			continue;

		if (Control_config[i].key_id & KEY_ALTED)
			Alt_is_modifier = 1;

		if (Control_config[i].key_id & KEY_SHIFTED)
			Shift_is_modifier = 1;

		if (Control_config[i].key_id & KEY_CTRLED) {
			Assert(0);  // get Alan
			Ctrl_is_modifier = 1;
		}
	}
}

int translate_key_to_index(const char *key)
{
	int i, index = -1, alt = 0, shift = 0, max_scan_codes;

	if (Lcl_gr) {
		max_scan_codes = sizeof(Scan_code_text_german) / sizeof(char *);
	} else if (Lcl_fr) {
		max_scan_codes = sizeof(Scan_code_text_french) / sizeof(char *);
	} else if (Lcl_pl) {
		max_scan_codes = sizeof(Scan_code_text_polish) / sizeof(char *);
	} else {
		max_scan_codes = sizeof(Scan_code_text_english) / sizeof(char *);
	}

	// look for modifiers
	Assert(key);
	if (!strnicmp(key, "Alt", 3)) {
		alt = 1;
		key += 3;
		if (*key)
			key++;
	}

	const char *translated_shift;
	
	if(Lcl_gr){
		translated_shift = "Shift";
	} else if(Lcl_fr){	
		translated_shift = "Maj.";
	} else {	
		translated_shift = "Shift";
	}

	if (!strnicmp(key, translated_shift, 5)) {
		shift = 1;
		key += 5;
		if (*key)
			key++;
	}

#ifdef MAKE_FS1
	// fix for German training mission key check
	if (Lcl_gr && !strcmp(key, "Links Ctrl")) {
		key = "Left Ctrl";
	}
#endif

	// look up index for default key
	if (*key) {
		for (i=0; i<max_scan_codes; i++)
			if (!stricmp(key, Scan_code_text_english[i])) {
				index = i;
				break;
			}

		if (i == max_scan_codes)
			return -1;

		if (shift)
			index |= KEY_SHIFTED;
		if (alt)
			index |= KEY_ALTED;

		// convert scancode to Control_config index
		for (i=0; i<CCFG_MAX; i++) {
			if (Control_config[i].key_default == index) {
				index = i;
				break;
			}
		}

		if (i == CCFG_MAX)
			return -1;

		return index;
	}

	return -1;
}

// Given the system default key 'key', return the current key that is bound to the function
// Both are 'key' and the return value are descriptive strings that can be displayed
// directly to the user.  If 'key' isn't a real key or not normally bound to anything,
// or there is no key current bound to the function, NULL is returned.
const char *translate_key(const char *key)
{
	int index = -1, code = -1;

	index = translate_key_to_index(key);
	if (index < 0)
		return NULL;

	code = Control_config[index].key_id;
	Failed_key_index = index;
	if (code < 0) {
		code = Control_config[index].joy_id;
		if (code >= 0)
			return Joy_button_text[code];
	}

	return textify_scancode(code);
}

const char *textify_scancode(int code)
{
	static char text[40];

	if (code < 0)
		return "None";

	*text = 0;
	if (code & KEY_ALTED) {
		if(Lcl_gr){		
			strcat(text, "Alt-");
		} else if(Lcl_fr){		
			strcat(text, "Alt-");
		} else {		
			strcat(text, "Alt-");
		}		
	}

	if (code & KEY_SHIFTED) {		
		if(Lcl_gr){
			strcat(text, "Shift-");
		} else if(Lcl_fr){		
			strcat(text, "Maj.-");
		} else {		
			strcat(text, "Shift-");
		}
	}

	strcat(text, Scan_code_text[code & KEY_MASK]);
	return text;
}
//XSTR:ON

// initialize common control config stuff - call at game startup after localization has been initialized
void control_config_common_init()
{
	if(Lcl_gr){
		Scan_code_text = Scan_code_text_german;
		Joy_button_text = Joy_button_text_german;
		
		// swap init bindings for y and z keys
		/*
		Control_config[TARGET_SHIP_IN_RETICLE].key_default = KEY_Z;
		Control_config[TARGET_LAST_TRANMISSION_SENDER].key_default = KEY_ALTED | KEY_Z;
		Control_config[REVERSE_THRUST].key_default = KEY_Y;
		Control_config[DISARM_MESSAGE].key_default = KEY_SHIFTED | KEY_Y;		
		*/
	} else if(Lcl_fr){
		Scan_code_text = Scan_code_text_french;
		Joy_button_text = Joy_button_text_french;
	} else if(Lcl_pl){
		Scan_code_text = Scan_code_text_polish;
		Joy_button_text = Joy_button_text_polish;
	} else {
		Scan_code_text = Scan_code_text_english;
		Joy_button_text = Joy_button_text_english;
	}
}
