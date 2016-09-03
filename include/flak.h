/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Weapon/Flak.h $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 *
 * flak functions
 *
 * $Log$
 * Revision 1.4  2002/06/09 04:41:13  relnev
 * added copyright header
 *
 * Revision 1.3  2002/05/26 20:22:48  theoddone33
 * Most of network/ works
 *
 * Revision 1.2  2002/05/04 04:36:56  theoddone33
 * More changes, took out a lot of the sound stuff which will bite later but
 * I don't care.
 *
 * Revision 1.1.1.1  2002/05/03 03:28:12  root
 * Initial import.
 * 
 * 
 * 6     7/31/99 2:57p Dave
 * Scaled flak aim and jitter by weapon subsystem strength.
 * 
 * 5     5/24/99 5:45p Dave
 * Added detail levels to the nebula, with a decent speedup. Split nebula
 * lightning into its own section.
 * 
 * $NoKeywords: $
 */

#ifndef _FLAK_WEAPONS_HEADER_FILE
#define _FLAK_WEAPONS_HEADER_FILE

// --------------------------------------------------------------------------------------------------------------------------------------
// FLAK DEFINES/VARS
//
#include "pstypes.h"

struct weapon;
struct object;


// --------------------------------------------------------------------------------------------------------------------------------------
// FLAK FUNCTIONS
//

// initialize flak stuff for the level
void flak_level_init();

// close down flak stuff
void flak_level_close();

// given a newly created weapon, turn it into a flak weapon
void flak_create(weapon *wp);

// free up a flak object
void flak_delete(int flak_index);

// given a just fired flak shell, pick a detonating distance for it
void flak_pick_range(object *objp, vector *predicted_target_pos, float weapon_subsys_strength);

// add some jitter to a flak gun's aiming direction, take into account range to target so that we're never _too_ far off
// assumes dir is normalized
void flak_jitter_aim(vector *dir, float dist_to_target, float weapon_subsys_strength);

// create a muzzle flash from a flak gun based upon firing position and weapon type
void flak_muzzle_flash(vector *pos, vector *dir, int turret_weapon_class);

// maybe detonate a flak shell early/late (call from weapon_process_pre(...))
void flak_maybe_detonate(object *obj);

// set the range on a flak object
void flak_set_range(object *objp, vector *start_pos, float range);

// get the current range for the flak object
float flak_get_range(object *objp);

#endif

