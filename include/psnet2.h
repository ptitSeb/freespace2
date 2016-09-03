/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Network/Psnet2.h $
 * $Revision: 274 $
 * $Date: 2005-10-01 23:38:32 +0200 (Sat, 01 Oct 2005) $
 * $Author: taylor $
 *
 * Header file for the application level network-interface.
 *
 * $Log$
 * Revision 1.7  2005/10/01 21:38:32  taylor
 * some reorg of includes to not requires the same headers in many different files
 * handle FS1 specific language changes
 * some OS X changes for paths and to handle socklen_t better
 *
 * Revision 1.6  2002/06/16 05:20:01  relnev
 * work around some gcc 3.1 weirdness
 *
 * Revision 1.5  2002/06/09 04:41:14  relnev
 * added copyright header
 *
 * Revision 1.4  2002/05/27 00:40:47  theoddone33
 * Fix net_addr vs net_addr_t
 *
 * Revision 1.3  2002/05/26 21:27:53  theoddone33
 * More progress (I hate psnet2)
 *
 * Revision 1.2  2002/05/26 20:22:48  theoddone33
 * Most of network/ works
 *
 * Revision 1.1.1.1  2002/05/03 03:28:12  root
 * Initial import.
 *
 * 
 * 9     8/16/99 4:06p Dave
 * Big honking checkin.
 * 
 * 8     7/28/99 11:46a Dave
 * Put in FS2_DEMO defines for port stuff.
 * 
 * 7     6/25/99 11:59a Dave
 * Multi options screen.
 * 
 * 6     6/07/99 9:51p Dave
 * Consolidated all multiplayer ports into one.
 * 
 * 5     3/09/99 6:24p Dave
 * More work on object update revamping. Identified several sources of
 * unnecessary bandwidth.
 * 
 * 4     11/19/98 4:19p Dave
 * Put IPX sockets back in psnet. Consolidated all multiplayer config
 * files into one.
 * 
 * 3     11/19/98 8:04a Dave
 * Full support for D3-style reliable sockets. Revamped packet lag/loss
 * system, made it receiver side and at the lowest possible level.
 * 
 * $NoKeywords: $
 */


#ifndef _PSNET2_H
#define _PSNET2_H

/* sigh */
#ifdef PLAT_UNIX
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

#include "pstypes.h"

#if defined(__APPLE__) && !defined(_SOCKLEN_T)
typedef int socklen_t;
#endif

// -------------------------------------------------------------------------------------------------------
// PSNET 2 DEFINES/VARS
//

#define NET_NONE		0		// if no protocol is active or none are selected
#define NET_TCP		1
#define NET_IPX		2
#define NET_VMT		3

#define MAX_PACKET_SIZE		512

#ifdef FS2_DEMO
	#define DEFAULT_GAME_PORT 7802
#else
	#define DEFAULT_GAME_PORT 7808
#endif

typedef struct net_addr {
	uint	type;			// See NET_ defines above
	ubyte	net_id[4];	// used for IPX only
	ubyte addr[6];		// address (first 4 used when IP, all 6 used when IPX)
	short port;			
} net_addr_t;

// define these in such a manner that a call to psnet_send_reliable is exactly the same and the new code in unobtrusive
typedef uint PSNET_SOCKET;
typedef uint PSNET_SOCKET_RELIABLE;
#undef INVALID_SOCKET
#define INVALID_SOCKET (PSNET_SOCKET)(~0)

// defines for protocol overheads
#define UDP_HEADER_SIZE						34
#define TCP_HEADER_SIZE						40
#define TCP_HEADER_SIZE_COMPRESSED		6

// define values for network errors when trying to enter the ready room
#define NETWORK_ERROR_NONE					0
#define NETWORK_ERROR_NO_TYPE				-1
#define NETWORK_ERROR_NO_WINSOCK			-2
#define NETWORK_ERROR_NO_PROTOCOL		-3
#define NETWORK_ERROR_RELIABLE			-4
#define NETWORK_ERROR_CONNECT_TO_ISP	-5
#define NETWORK_ERROR_LAN_AND_RAS		-6

// psnet packet types
#define PSNET_NUM_TYPES						5
#define PSNET_TYPE_UNRELIABLE				0
#define PSNET_TYPE_RELIABLE				1
#define PSNET_TYPE_USER_TRACKER			2
#define PSNET_TYPE_GAME_TRACKER			3
#define PSNET_TYPE_VALIDATION				4

extern net_addr_t Psnet_my_addr;							// address information of this machine
extern uint Psnet_my_ip;
extern int Psnet_my_addr_valid;

extern int Network_status;
extern int Tcp_failure_code;
extern int Ipx_failure_code;

extern int Tcp_active;
extern int Ipx_active;

extern int Socket_type;										// protocol type in use (see NET_* defines above)

// specified their internet connnection type
#define NETWORK_CONNECTION_NONE			1
#define NETWORK_CONNECTION_DIALUP		2
#define NETWORK_CONNECTION_LAN			3

extern int Psnet_connection;

extern ushort Psnet_default_port;

// Reliable socket states
#define RNF_UNUSED			0		// Completely clean socket..
#define RNF_CONNECTED		1		// Connected and running fine
#define RNF_BROKEN			2		// Broken - disconnected abnormally
#define RNF_DISCONNECTED	3		// Disconnected cleanly
#define RNF_CONNECTING		4		// We received the connecting message, but haven't told the game yet.
#define RNF_LIMBO				5		// between connecting and connected

extern SOCKET Unreliable_socket;	// all PXO API modules should use this to send and receive on

// -------------------------------------------------------------------------------------------------------
// PSNET 2 TOP LAYER FUNCTIONS - these functions simply buffer and store packets based upon type (see PSNET_TYPE_* defines)
//

#ifndef PLAT_UNIX
struct sockaddr;
struct fd_set;
struct timeval;
#endif

// wrappers around select() and recvfrom() for lagging/losing data, and for sorting through different packet types
int RECVFROM(uint s, char * buf, int len, int flags, sockaddr *from, int *fromlen, int psnet_type);
int SELECT(int nfds, fd_set *readfds, fd_set *writefds, fd_set*exceptfds, const timeval* timeout, int psnet_type);

// wrappers around sendto to sorting through different packet types
int SENDTO(uint s, char * buf, int len, int flags, sockaddr * to, int tolen, int psnet_type);

// call this once per frame to read everything off of our socket
void PSNET_TOP_LAYER_PROCESS();


// -------------------------------------------------------------------------------------------------------
// PSNET 2 FUNCTIONS
//

// initialize psnet to use the specified port
void psnet_init(int protocol, int default_port);

// shutdown psnet
void psnet_close();

// set the protocol to use
int psnet_use_protocol(int type);

// get the status of the network
int psnet_get_network_status();

// convert a net_addr to a string
char *psnet_addr_to_string( char * text, net_addr_t * address );

// convert a string to a net addr
void psnet_string_to_addr( net_addr_t * address, char * text );

// compare 2 addresses
int psnet_same( net_addr_t * a1, net_addr_t * a2 );

// send data unreliably
int psnet_send( net_addr_t * who_to, void * data, int len, int np_index = -1 );

// get data from the unreliable socket
int psnet_get( void * data, net_addr_t * from_addr );

// broadcast data on unreliable socket
int psnet_broadcast( net_addr_t * who_to, void * data, int len );

// flush all sockets
void psnet_flush();

// if the passed string is a valid IP string
int psnet_is_valid_ip_string( char *ip_string, int allow_port=1 );

// mark a socket as having received data
void psnet_mark_received(PSNET_SOCKET_RELIABLE socket);


// -------------------------------------------------------------------------------------------------------
// PSNET 2 RELIABLE SOCKET FUNCTIONS
//

// shutdown a reliable socket
void psnet_rel_close_socket(PSNET_SOCKET_RELIABLE *sockp);

// obsolete function - left in for compatibility sake
int psnet_rel_check();

// send data on the reliable socket
int psnet_rel_send(PSNET_SOCKET_RELIABLE socket, ubyte *data, int length, int np_index = -1);

// Return codes:
// -1 socket not connected
// 0 No packet ready to receive
// >0 Buffer filled with the number of bytes recieved
int psnet_rel_get(PSNET_SOCKET_RELIABLE socket, ubyte *buffer, int max_length);

// process all active reliable sockets
void psnet_rel_work();

// get the status of a reliable socket, see RNF_* defines above
int psnet_rel_get_status(PSNET_SOCKET_RELIABLE sock);

// check the listen socket for pending reliable connections
int psnet_rel_check_for_listen(net_addr_t *addr);

// perform a reliable socket connect to the specified server
void psnet_rel_connect_to_server(PSNET_SOCKET_RELIABLE *s, net_addr_t *server_addr);

#endif

