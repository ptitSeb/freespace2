/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/MissionUI/MissionCmdBrief.h $
 * $Revision: 165 $
 * $Date: 2003-01-22 19:23:02 +0100 (Wed, 22 Jan 2003) $
 * $Author: relnev $
 *
 * Mission Command Briefing Screen
 *
 * $Log$
 * Revision 1.3  2003/01/22 18:23:02  relnev
 * added missing include guard
 *
 * Revision 1.2  2002/06/09 04:41:13  relnev
 * added copyright header
 *
 * Revision 1.1.1.1  2002/05/03 03:28:12  root
 * Initial import.
 *
 * 
 * 2     10/07/98 10:53a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:49a Dave
 * 
 * 7     4/06/98 8:37p Hoffoss
 * Fixed a few bugs with command brief screen.  Now the voice starts after
 * the text has printed, and options screen doesn't reset cmd brief.
 * 
 * 6     3/26/98 5:24p Hoffoss
 * Changed Command Brief to use memory mapped ani files instead, so we
 * avoid the huge pauses for huge anis that play!
 * 
 * 5     3/19/98 4:25p Hoffoss
 * Added remaining support for command brief screen (ANI and WAVE file
 * playing).
 * 
 * 4     3/17/98 6:26p Hoffoss
 * Added wave filename to command brief structure.
 * 
 * 3     3/05/98 9:38p Hoffoss
 * Finished up command brief screen.
 * 
 * 2     3/05/98 3:59p Hoffoss
 * Added a bunch of new command brief stuff, and asteroid initialization
 * to Fred.
 * 
 * 1     3/02/98 6:14p Hoffoss
 *
 * $NoKeywords: $
 */

#ifndef MISSIONCMDBRIEF_H
#define MISSIONCMDBRIEF_H

#define CMD_BRIEF_TEXT_MAX		16384
#define CMD_BRIEF_STAGES_MAX	10

typedef struct {
	char *text;  // text to display
	char ani_filename[MAX_FILENAME_LEN];  // associated ani file to play
	anim_t *anim;
	anim_instance_t *anim_instance;
	int anim_ref;  // potential reference to another index (use it's anim instead of this's)
	char wave_filename[MAX_FILENAME_LEN];
	int wave;  // instance number of above
} cmd_brief_stage;

typedef struct {
	int num_stages;
	cmd_brief_stage stage[CMD_BRIEF_STAGES_MAX];
} cmd_brief;

extern cmd_brief Cmd_briefs[MAX_TEAMS];
extern cmd_brief *Cur_cmd_brief;  // pointer to one of the Cmd_briefs elements (the active one)

void cmd_brief_init(int stages);
void cmd_brief_close();
void cmd_brief_do_frame(float frametime);
void cmd_brief_hold();
void cmd_brief_unhold();

#endif
