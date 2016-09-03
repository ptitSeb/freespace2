/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/MenuUI/MainHallTemp.h $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 *
 * Header file for main-hall menu code
 *
 * $Log$
 * Revision 1.2  2002/06/09 04:41:13  relnev
 * added copyright header
 *
 * Revision 1.1.1.1  2002/05/03 03:28:12  root
 * Initial import.
 *
 * 
 * 2     6/03/99 10:15p Dave
 * Put in temporary main hall screen.
 * 
 * $NoKeywords: $
 *
 */

#ifndef __TEMP_MAIN_HALL_MENU_HEADER_FILE
#define __TEMP_MAIN_HALL_MENU_HEADER_FILE

// ------------------------------------------------------------------------------------------------------------------------
// TEMP MAIN HALL DEFINES/VARS
//


// ------------------------------------------------------------------------------------------------------------------------
// TEMP MAIN HALL FUNCTIONS
//

// initialize the temporary main hall
void mht_init();

// do a frame for the main hall
void mht_do();

// close the temporary main hall
void mht_close();

#endif

