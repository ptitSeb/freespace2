/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Hud/HudArtillery.h $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 * 
 *
 * $Log$
 * Revision 1.3  2002/06/09 04:41:13  relnev
 * added copyright header
 *
 * Revision 1.2  2002/05/03 13:34:33  theoddone33
 * More stuff compiles
 *
 * Revision 1.1.1.1  2002/05/03 03:28:12  root
 * Initial import.
 *  
 * 
 * 2     4/20/99 6:39p Dave
 * Almost done with artillery targeting. Added support for downloading
 * images on the PXO screen.
 * 
 * 1     4/20/99 12:00a Dave
 * 
 * 
 * $NoKeywords: $
 */

#ifndef _FS2_HUD_ARTILLERY_HEADER_FILE
#define _FS2_HUD_ARTILLERY_HEADER_FILE

// -----------------------------------------------------------------------------------------------------------------------
// ARTILLERY DEFINES/VARS
//


// -----------------------------------------------------------------------------------------------------------------------
// ARTILLERY FUNCTIONS
//

#include "parselo.h"

#define MAX_SSM_TYPES			10
#define MAX_SSM_STRIKES			10
#define MAX_SSM_COUNT			10

// global ssm types
typedef struct ssm_info {
	char			name[NAME_LENGTH+1];				// strike name
	int			count;								// # of missiles in this type of strike
	int			weapon_info_index;				// missile type
	float			warp_radius;						// radius of associated warp effect	
	float			warp_time;							// how long the warp effect lasts
	float			radius;								// radius around the shooting ship	
	float			offset;								// offset in front of the shooting ship
} ssm_info;

// creation info for the strike (useful for multiplayer)
typedef struct ssm_firing_info {
	int			delay_stamp[MAX_SSM_COUNT];	// timestamps
	vector		start_pos[MAX_SSM_COUNT];		// start positions
	
	int			ssm_index;							// index info ssm_info array
	vector		target;								// target for the strike	
} ssm_firing_info;

// the strike itself
typedef struct ssm_strike {
	int			fireballs[MAX_SSM_COUNT];		// warpin effect fireballs
	int			done_flags[MAX_SSM_COUNT];		// when we've fired off the individual missiles
	
	// this is the info that controls how the strike behaves (just like for beam weapons)
	ssm_firing_info		sinfo;

	ssm_strike	*next, *prev;						// for list
} ssm_strike;


// level init
void hud_init_artillery();

// update all hud artillery related stuff
void hud_artillery_update();

// render all hud artillery related stuff
void hud_artillery_render();

#endif

