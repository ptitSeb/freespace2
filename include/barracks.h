/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/MenuUI/Barracks.h $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 *
 * C source file for implementing barracks
 *
 * $Log$
 * Revision 1.2  2002/06/09 04:41:12  relnev
 * added copyright header
 *
 * Revision 1.1.1.1  2002/05/03 03:28:12  root
 * Initial import.
 *
 * 
 * 3     2/02/99 11:58a Neilk
 * added vss revision/log comments
 *
 * $NoKeywords: $
 */

#ifndef _BARRACKS_H
#define _BARRACKS_H

// initialize the barracks 
void barracks_init();

// do a frame for the barrracks
void barracks_do_frame(float frametime);

// close the barracks
void barracks_close();

#endif // _BARRACKS_H

