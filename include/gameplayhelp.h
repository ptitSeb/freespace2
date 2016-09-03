/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/GameHelp/GameplayHelp.h $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 *
 * Header for displaying in-game help
 *
 * $Log$
 * Revision 1.2  2002/06/09 04:41:13  relnev
 * added copyright header
 *
 * Revision 1.1.1.1  2002/05/03 03:28:12  root
 * Initial import.
 *
 * 
 * 2     10/07/98 10:52a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:48a Dave
 * 
 * 2     3/09/98 9:54p Lawrance
 * integrate new art for gameplay help
 * 
 * 1     3/09/98 5:05p Lawrance
 *
 * $NoKeywords: $
 */

#ifndef __GAMEPLAY_HELP_H__
#define __GAMEPLAY_HELP_H__

void gameplay_help_init();
void gameplay_help_close();
void gameplay_help_do_frame(float frametime);

#endif

