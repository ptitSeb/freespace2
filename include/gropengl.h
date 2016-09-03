/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Graphics/GrOpenGL.h $
 * $Revision: 274 $
 * $Date: 2005-10-01 23:38:32 +0200 (Sat, 01 Oct 2005) $
 * $Author: taylor $
 *
 * Include file for OpenGL renderer
 *
 * $Log$
 * Revision 1.3  2005/10/01 21:38:32  taylor
 * some reorg of includes to not requires the same headers in many different files
 * handle FS1 specific language changes
 * some OS X changes for paths and to handle socklen_t better
 *
 * Revision 1.2  2002/06/09 04:41:13  relnev
 * added copyright header
 *
 * Revision 1.1.1.1  2002/05/03 03:28:12  root
 * Initial import.
 *
 * 
 * 3     6/29/99 10:35a Dave
 * Interface polygon bitmaps! Whee!
 * 
 * 2     10/07/98 10:53a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:49a Dave
 * 
 * 1     5/12/97 12:14p John
 *
 * $NoKeywords: $
 */

#ifndef _GROPENGL_H
#define _GROPENGL_H

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#ifdef HAVE_GLES
#include <GLES/gl.h>
#include <GLES/glext.h>
#else
#include <GL/gl.h>
#endif
#endif

void gr_opengl_init();
void gr_opengl_cleanup();

void gr_opengl_bitmap(int x, int y);
void gr_opengl_bitmap_ex(int x, int y, int w, int h, int sx, int sy);

#endif

