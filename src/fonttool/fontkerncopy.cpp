/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Fonttool/FontKernCopy.cpp $
 * $Revision: 173 $
 * $Date: 2003-01-30 21:03:48 +0100 (Thu, 30 Jan 2003) $
 * $Author: relnev $
 *
 * Tool for copying font kerning info
 *
 * $Log$
 * Revision 1.3  2003/01/30 20:03:48  relnev
 * various files ported needed for fonttool.  There is a bug where on exit it segfaults in SDL_GL_SwapBuffers, I'm probably missing something (don't know what) but it works fine otherwise (Taylor Richards)
 *
 * Revision 1.2  2002/06/09 04:41:16  relnev
 * added copyright header
 *
 * Revision 1.1.1.1  2002/05/03 03:28:08  root
 * Initial import.
 *
 * 
 * 2     10/24/98 5:15p Dave
 * 
 * 1     10/24/98 4:58p Dave
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

font fnt1, fnt2;

void fonttool_kerning_copy( char *fname1, char *fname2 )
{
	printf( "Copying kerning data from %s to %s\n", fname1, fname2 );

	fonttool_read( fname1, &fnt1 );
	fonttool_read( fname2, &fnt2 );
	fonttool_copy_kern( &fnt1, &fnt2 );

	fonttool_dump( fname2, &fnt2 );

	exit(0);
}

