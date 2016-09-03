/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Network/multi_log.h $
 * $Revision: 307 $
 * $Date: 2010-02-08 09:09:13 +0100 (Mon, 08 Feb 2010) $
 * $Author: taylor $
 *
 * Header file to support multiplayer logging functions
 *
 * $Log$
 * Revision 1.2  2002/06/09 04:41:14  relnev
 * added copyright header
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
 * 2     8/20/98 5:31p Dave
 * Put in handy multiplayer logfile system. Now need to put in useful
 * applications of it all over the code.
 * 
 * 1     8/20/98 2:00p Dave
 *  
 *
 * $NoKeywords: $
 */

#ifndef _FREESPACE_MULTIPLAYER_LOGFILE_HEADER_FILE
#define _FREESPACE_MULTIPLAYER_LOGFILE_HEADER_FILE

// ----------------------------------------------------------------------------------------------------
// MULTI LOGFILE DEFINES/VARS
//


// ----------------------------------------------------------------------------------------------------
// MULTI LOGFILE FUNCTIONS
//

// initialize the multi logfile
void multi_log_init();

// close down the multi logfile
void multi_log_close();

// give some processing time to the logfile system so it can check up on stuff
void multi_log_process();

// printf function itself called by the ml_printf macro
void ml_printf(const char *format, ...);

// string print function
void ml_string(const char *string, int add_time = 1);

#endif

