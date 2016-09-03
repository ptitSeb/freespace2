/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Cutscene/Cutscenes.h $
 * $Revision: 307 $
 * $Date: 2010-02-08 09:09:13 +0100 (Mon, 08 Feb 2010) $
 * $Author: taylor $
 *
 * Code for the cutscenes viewer screen
 *
 * $Log$
 * Revision 1.3  2003/06/11 18:30:32  taylor
 * plug memory leaks
 *
 * Revision 1.2  2002/06/09 04:41:12  relnev
 * added copyright header
 *
 * Revision 1.1.1.1  2002/05/03 03:28:11  root
 * Initial import.
 *
 * 
 * 2     10/07/98 10:52a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:48a Dave
 * 
 * 6     5/21/98 12:35a Lawrance
 * Tweak how CD is checked for
 * 
 * 5     5/10/98 10:05p Allender
 * only show cutscenes which have been seen before.  Made Fred able to
 * write missions anywhere, defaulting to player misison folder, not data
 * mission folder.  Fix FreeSpace code to properly read missions from
 * correct locations
 * 
 * 4     5/08/98 5:30p Lawrance
 * add cutscenes_validate_cd()
 * 
 * 3     5/08/98 4:07p Allender
 * more cutscene stuff
 * 
 * 2     4/17/98 6:33p Hoffoss
 * Made changes to the tech room group of screens.  Cutscenes screen is
 * now in a new file.
 *
 * $NoKeywords: $
 */

#ifndef _FREESPACE_CUTSCENES_SCREEN_HEADER_FILE
#define _FREESPACE_CUTSCENES_SCREEN_HEADER_FILE

#include "parselo.h"

#define MAX_CUTSCENES	10

// this cutscene is always available.
#define INTRO_CUTSCENE_FLAG		(1<<0)

typedef struct cutscene_info
{
	char		filename[MAX_FILENAME_LEN];
	char		name[NAME_LENGTH];
	char		*description;
	int		cd;
} cutscene_info;

extern cutscene_info Cutscenes[MAX_CUTSCENES];
extern int Num_cutscenes;
extern int Cutscenes_viewable;

// initializa table data
void cutscene_init();
void cutscene_tbl_close();
int cutscene_get_cd_num(char *filename);


void cutscenes_screen_init();
void cutscenes_screen_close();
void cutscenes_screen_do_frame();

int cutscenes_validate_cd(const char *mve_name, int prompt_for_cd = 1);
void cutscene_mark_viewable(const char *filename);

#endif

