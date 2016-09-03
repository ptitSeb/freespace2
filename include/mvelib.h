/*
 * $Logfile: /Freespace2/include/mvelib.h $
 * $Revision: 307 $
 * $Date: 2010-02-08 09:09:13 +0100 (Mon, 08 Feb 2010) $
 * $Author: taylor $
 *
 * Lib functions for MVE player
 *
 * $Log$
 * Revision 1.2  2005/03/29 07:50:34  taylor
 * Update to newest movie code with much better video support and audio support from
 *   Pierre Willenbrock.  Movies are enabled always now (no longer a build option)
 *   and but can be skipped with the "--nomovies" or "-n" cmdline options.
 *
 *
 *
 * $NoKeywords: $
 *
 */
 
#ifndef INCLUDED_MVELIB_H
#define INCLUDED_MVELIB_H

#include "pstypes.h"

struct CFILE;

// structure for maintaining info on a MVEFILE stream
typedef struct MVEFILE
{
	CFILE	*stream;
	ubyte	*cur_chunk;
	int		buf_size;
	int		cur_fill;
	int		next_segment;
} MVEFILE;

// open a .MVE file
MVEFILE *mvefile_open(const char *filename);

// close a .MVE file
void mvefile_close(MVEFILE *movie);

// get size of next segment in chunk (-1 if no more segments in chunk)
int mvefile_get_next_segment_size(MVEFILE *movie);

// get type of next segment in chunk (0xff if no more segments in chunk)
ubyte mvefile_get_next_segment_major(MVEFILE *movie);

// get subtype (version) of next segment in chunk (0xff if no more segments in chunk)
ubyte mvefile_get_next_segment_minor(MVEFILE *movie);

// see next segment (return NULL if no next segment)
ubyte *mvefile_get_next_segment(MVEFILE *movie);

// advance to next segment
void mvefile_advance_segment(MVEFILE *movie);

// fetch the next chunk (return 0 if at end of stream)
int mvefile_fetch_next_chunk(MVEFILE *movie);

// structure for maintaining an MVE stream
typedef struct MVESTREAM
{
	MVEFILE		*movie;
} MVESTREAM;

// open an MVE stream
MVESTREAM *mve_open(const char *filename);

// close an MVE stream
void mve_close(MVESTREAM *movie);

// play next chunk
int mve_play_next_chunk(MVESTREAM *movie);

// basic movie playing functions
void mve_init(MVESTREAM *mve);
void mve_play(MVESTREAM *mve);
void mve_shutdown();

// utility functions
short mve_get_short(ubyte *data);
ushort mve_get_ushort(ubyte *data);
int mve_get_int(ubyte *data);

// callbacks for data handling
// audio
void mve_audio_createbuf(ubyte minor, ubyte *data);
int mve_audio_data(ubyte major, ubyte *data);
void mve_audio_play();
// video
int mve_video_createbuf(ubyte minor, ubyte *data);
int mve_video_init(ubyte *data);
void mve_video_palette(ubyte *data);
void mve_video_data(ubyte *data, int len);
void mve_video_codemap(ubyte *data, int len);
void mve_video_display();
// misc
void mve_end_movie();
void mve_end_chunk();
int mve_timer_create(ubyte *data);

// special audio functions
void mveaudio_uncompress(short *buffer, unsigned char *data, int length);
typedef short mves;

#endif /* INCLUDED_MVELIB_H */
