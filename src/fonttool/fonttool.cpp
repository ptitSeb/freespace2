/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Fonttool/FontTool.cpp $
 * $Revision: 173 $
 * $Date: 2003-01-30 21:03:48 +0100 (Thu, 30 Jan 2003) $
 * $Author: relnev $
 *
 * Tool for creating/kerning fonts
 *
 * $Log$
 * Revision 1.4  2003/01/30 20:03:48  relnev
 * various files ported needed for fonttool.  There is a bug where on exit it segfaults in SDL_GL_SwapBuffers, I'm probably missing something (don't know what) but it works fine otherwise (Taylor Richards)
 *
 * Revision 1.3  2002/06/09 04:41:16  relnev
 * added copyright header
 *
 * Revision 1.2  2002/05/07 03:16:43  theoddone33
 * The Great Newline Fix
 *
 * Revision 1.1.1.1  2002/05/03 03:28:08  root
 * Initial import.
 *
 * 
 * 2     10/24/98 5:15p Dave
 * 
 * 1     10/24/98 4:58p Dave
 * 
 * 4     10/30/97 4:56p John
 * Fixed up font stuff to build.  Fixed bug where it didn't show the last
 * 3 characters in kerning table.
 * 
 * 3     6/23/97 6:05p Hoffoss
 * Added stubbs to fix linking errors.
 * 
 * 2     6/05/97 4:53p John
 * First rev of new antialiased font stuff.
 * 
 * 1     6/02/97 4:04p John
 *
 * $NoKeywords: $
 */

#include <stdlib.h>
#include <stdio.h>
#ifndef PLAT_UNIX
#include <io.h>
#include <conio.h>
#endif

#include "pstypes.h"
#include "osapi.h"
#include "cfile.h"
#include "2d.h"
#include "key.h"
#include "mouse.h"
#include "palman.h"
#include "timer.h"

#include "fonttool.h"

char Usage[] = "Usage:\n"	\
"\nFontTool x.pcx [y.vf]\n" \
"\n  If you specify a PCX file, then a font will be\n" \
"  created with the same base name.   If you also\n" \
"  specify a font file, then it will use the kerning\n" \
"  data from that font file when it creates the new\n" \
"  font from the PCX file.\n" \
"\nFontTool x.vf\n" \
"\n  If you specify a font file by itself then it will\n" \
"  allow you to interactively kern that font.\n" \
"\nFontTool x.vf y.vf\n" \
"\n  If you specify two font files, then the kerning\n" \
"  data from the first font will be copied into the\n" \
"  second font.\n" \
"\n";

#define NONE 0
#define PCX 1
#define FONT 2

int Font1 = -1;

void demo_set_playback_filter() {}
float flFrametime = 0.0f;

void freespace_menu_background()
{
	gr_reset_clip();
	gr_clear();
}

int main(int argc, char *argv[] )
{
	int t1, t2;

	t1 = NONE;
	t2 = NONE;

	if ( (argc < 1) || (argc>3) )	{
		printf( Usage );
		return 1;
	}

	if ( argc > 1 )	{
		strlwr( argv[1] );

		if ( strstr( argv[1], ".pcx" ) )
			t1 = PCX;
		else if ( strstr( argv[1], ".vf" ) )
			t1 = FONT;
	}

	if ( argc > 2 )	{
		strlwr( argv[2] );

		if ( strstr( argv[2], ".pcx" ) )
			t2 = PCX;
		else if ( strstr( argv[2], ".vf" ) )
			t2 = FONT;
	}

	if ( (t1==PCX) && (t2==NONE) )
		fonttool_create_font( argv[1], NULL );
	else if ( (t1==PCX) && (t2==FONT) )
		fonttool_create_font( argv[1], argv[2] );
	else if ( (t1==FONT) && (t2==NONE) )
#ifdef PLAT_UNIX
		fonttool_edit_kerning( argv[1], NULL );
#else
		fonttool_edit_kerning( argv[1] );
#endif
	else if ( (t1==FONT) && (t2==FONT) )
		fonttool_kerning_copy( argv[1], argv[2] );
	else
		printf( Usage );

	return 0;
}

