/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Network/multi_ping.h $
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
 * 2     10/07/98 10:53a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:50a Dave
 * 
 * 3     6/30/98 2:17p Dave
 * Revised object update system. Removed updates for all weapons. Put
 * button info back into control info packet.
 * 
 * 2     6/12/98 2:49p Dave
 * Patch 1.02 changes.
 * 
 * 1     3/03/98 5:09p Dave
 *  
 * $NoKeywords: $
 */

#ifndef _MULTIPLAYER_PING_HEADER_FILE
#define _MULTIPLAYER_PING_HEADER_FILE

// ------------------------------------------------------------------------------------
// MULTIPLAYER PING DEFINES/VARS
//

struct header;
struct net_player;

// the max ping we'll store to calculate the average
#define MAX_PINGS					10

typedef struct ping_struct {
	float ping_start;										// time the current ping was sent out, or -1 if none
	float ping_times[MAX_PINGS];						// ping times for calculating the average
	int num_pings;											// # of pings in the ping_times array
	int ping_add;											// where to add the next ping

	int ping_avg;											// in ms, this is the only thing we should be concerned with
} ping_struct;


// ------------------------------------------------------------------------------------
// MULTIPLAYER PING FUNCTIONS
//

// initialize all player ping times
void multi_ping_reset_players();

// initialize the given ping struct
void multi_ping_reset(ping_struct *ps);

// start a ping - call this when sending a ping packet
void multi_ping_start(ping_struct *ps);

// evaluate a pong return on the given struct
void multi_ping_eval_pong(ping_struct *ps);

// send a ping to a specific player
void multi_ping_send(net_player *p);

// send a ping to the specified address
void multi_ping_send(net_addr_t *addr,ping_struct *ps);

// send a ping to all players
void multi_ping_send_all();

// get the lowest existing ping in the ping struct, returning -1 if no pings
int multi_ping_get_lowest(ping_struct *ps);

// (average ping + lowest ping)/2
int multi_ping_lowest_avg(ping_struct *ps);

#endif

