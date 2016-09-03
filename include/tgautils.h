/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/TgaUtils/TgaUtils.h $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 *
 *
 * $Log$
 * Revision 1.4  2002/06/09 04:41:15  relnev
 * added copyright header
 *
 * Revision 1.3  2002/05/26 20:22:48  theoddone33
 * Most of network/ works
 *
 * Revision 1.2  2002/05/04 04:36:56  theoddone33
 * More changes, took out a lot of the sound stuff which will bite later but
 * I don't care.
 *
 * Revision 1.1.1.1  2002/05/03 03:28:12  root
 * Initial import.
 *
 * 
 * 3     3/20/99 3:46p Dave
 * Added support for model-based background nebulae. Added 3 new
 * sexpressions.
 * 
 * 2     12/01/98 4:46p Dave
 * Put in targa bitmap support (16 bit).
 *  
 * $NoKeywords: $
 */


#ifndef __TARGA_H
#define __TARGA_H

#include "pstypes.h"

// --------------------
//
// Defines
//
// --------------------

#define TARGA_ERROR_NONE			0
#define TARGA_ERROR_READING		1
#define TARGA_ERROR_WRITING		2

// --------------------
//
// Prototypes
//
// --------------------

int targa_read_header(char *filename, int *w, int *h, int *bpp, ubyte *palette=NULL );
int targa_read_bitmap(char *filename, ubyte *data, ubyte *palette, int dest_size );
int targa_write_bitmap(char *filename, ubyte *data, ubyte *palette, int w, int h, int bpp);

// The following are used by the tools\vani code.
int targa_compress(char *out, char *in, int outsize, int pixsize, int bytecount);
int targa_uncompress( ubyte *dst, ubyte *src, int bitmap_width, int bytes_per_pixel );

#endif // __TARGA_H

