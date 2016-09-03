/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Network/multi_observer.h $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 *
 * $Log$
 * Revision 1.3  2002/06/09 04:41:14  relnev
 * added copyright header
 *
 * Revision 1.2  2002/05/27 00:40:47  theoddone33
 * Fix net_addr vs net_addr_t
 *
 * Revision 1.1.1.1  2002/05/03 03:28:12  root
 * Initial import.
 *  
 * 
 * 3     11/05/98 5:55p Dave
 * Big pass at reducing #includes
 * 
 * 2     10/07/98 10:53a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:50a Dave
 * 
 * 3     4/18/98 5:00p Dave
 * Put in observer zoom key. Made mission sync screen more informative.
 * 
 * 2     3/13/98 2:51p Dave
 * Put in support for observers to join ingame.
 * 
 * 1     3/12/98 5:44p Dave
 *  
 * $NoKeywords: $
 */

#ifndef _MULTI_OBSERVER_HEADER_FILE
#define _MULTI_OBSERVER_HEADER_FILE

// ---------------------------------------------------------------------------------------
// MULTI OBSERVER DEFINES/VARS
//

struct player;
struct net_player; 

// ---------------------------------------------------------------------------------------
// MULTI OBSERVER FUNCTIONS
//

// create a _permanent_ observer player 
int multi_obs_create_player(int player_num,char *name,net_addr_t *addr,player *pl);

// create an explicit observer object and assign it to the passed player
void multi_obs_create_observer(net_player *pl);

// create observer object locally, and additionally, setup some other information
// ( client-side equivalent of multi_obs_create_observer() )
void multi_obs_create_observer_client();

// create objects for all known observers in the game at level start
// call this before entering a mission
// this implies for the local player in the case of a client or for _all_ players in the case of a server
void multi_obs_level_init();

// if i'm an observer, zoom to near my targted object (if any)
void multi_obs_zoom_to_target();

#endif

