/*
 * $Logfile: /Freespace2/code/movie/mveplayer.cpp $
 * $Revision: 279 $
 * $Date: 2005-10-01 23:48:01 +0200 (Sat, 01 Oct 2005) $
 * $Author: taylor $
 *
 * MVE movie playing routines
 *
 * $Log$
 * Revision 1.7  2005/10/01 21:48:01  taylor
 * various cleanups
 * fix decoder to swap opcode 0xb since it screws up on PPC
 * the previous opcode 0xc change was wrong since we had already determined that it messes up FS1 movies
 *
 * Revision 1.6  2005/08/12 08:47:24  taylor
 * use new audiostr code rather than old windows/unix version
 * update all OpenAL commands with new error checking macros
 * fix play_position to properly account for real position, fixes the talking heads and message text cutting out early
 * movies will now use better filtering when scaled
 *
 * Revision 1.5  2005/03/31 21:26:02  taylor
 * s/alGetSourceiv/alGetSourcei/
 *
 * Revision 1.4  2005/03/31 00:06:20  taylor
 * go back to more accurate timer and allow video scaling for movies
 *
 * Revision 1.3  2005/03/29 07:50:34  taylor
 * Update to newest movie code with much better video support and audio support from
 *   Pierre Willenbrock.  Movies are enabled always now (no longer a build option)
 *   and but can be skipped with the "--nomovies" or "-n" cmdline options.
 *
 *
 * $NoKeywords: $
 */
 
#ifdef PLAT_UNIX
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#endif

#include "pstypes.h"
#include "mvelib.h"
#include "movie.h"
#include "2d.h"
#include "key.h"
#include "osapi.h"
#include "timer.h"
#include "sound.h"
#include "bmpman.h"
#include "osregistry.h"
#include "ds.h"
#include "gropengl.h"


static int mve_playing;

// timer variables
static int g_spdFactorNum = 0;
static int g_spdFactorDenom = 10;
static int micro_frame_delay = 0;
static int timer_started = 0;
#ifdef PLAT_UNIX
static struct timeval timer_expire = {0, 0};
#else
static fix timer_expire;
#endif

// audio variables
#define MVE_AUDIO_BUFFERS 64  // total buffers to interact with stream
static int mve_audio_curbuf_curpos = 0;
static int mve_audio_bufhead = 0;
static int mve_audio_buftail = 0;
static int mve_audio_playing = 0;
static int mve_audio_canplay = 0;
static int mve_audio_compressed = 0;
static int audiobuf_created;

#ifdef PLAT_UNIX

// struct for the audio stream information
typedef struct MVE_AUDIO_T {
	ALenum format;
	int sample_rate;
	int bytes_per_sec;
	int channels;
	int bitsize;
	ALuint audio_data[MVE_AUDIO_BUFFERS];
	ALuint source_id;
	ALuint audio_buffer[MVE_AUDIO_BUFFERS];
} mve_audio_t;

mve_audio_t *mas;  // mve_audio_stream

#endif	// PLAT_UNIX


// video variables
int g_width, g_height;
void *g_vBuffers = NULL;
void *g_vBackBuf1, *g_vBackBuf2;
ushort *pixelbuf = NULL;
static int g_screenWidth, g_screenHeight;
static ubyte g_palette[768];
static ubyte *g_pCurMap=NULL;
static int g_nMapLength=0;
static int videobuf_created, video_inited;
static int hp2, wp2;
static uint mve_video_skiptimer = 0;
static int mve_scale_video = 0;
#ifdef PLAT_UNIX
static GLuint tex = 0;
#endif

// the decoder
void decodeFrame16(ubyte *pFrame, ubyte *pMap, int mapRemain, ubyte *pData, int dataRemain);

/*************************
 * general handlers
 *************************/
void mve_end_movie()
{
	mve_playing = 0;
}

/*************************
 * timer handlers
 *************************/

int mve_timer_create(ubyte *data)
{
	longlong temp;

	micro_frame_delay = mve_get_int(data) * (int)mve_get_short(data+4);

	if (g_spdFactorNum != 0) {
		temp = micro_frame_delay;
		temp *= g_spdFactorNum;
		temp /= g_spdFactorDenom;
		micro_frame_delay = (int)temp;
	}

	return 1;
}

static void mve_timer_start(void)
{
#ifdef PLAT_UNIX
	int nsec = 0;

	gettimeofday(&timer_expire, NULL);

	timer_expire.tv_usec += micro_frame_delay;

	if (timer_expire.tv_usec > 1000000) {
		nsec = timer_expire.tv_usec / 1000000;
		timer_expire.tv_sec += nsec;
		timer_expire.tv_usec -= nsec * 1000000;
	}
#else
	timer_expire = timer_get_microseconds();
	timer_expire += micro_frame_delay;
#endif

	timer_started = 1;
}

static int mve_do_timer_wait(void)
{
	if (!timer_started)
		return 0;

#ifdef PLAT_UNIX
	int nsec = 0;
	struct timespec ts, tsRem;
	struct timeval tv;

	gettimeofday(&tv, NULL);

	if (tv.tv_sec > timer_expire.tv_sec)
		goto end;

	if ( (tv.tv_sec == timer_expire.tv_sec) && (tv.tv_usec >= timer_expire.tv_usec) )
		goto end;

	ts.tv_sec = timer_expire.tv_sec - tv.tv_sec;
	ts.tv_nsec = 1000 * (timer_expire.tv_usec - tv.tv_usec);

	if (ts.tv_nsec < 0) {
		ts.tv_nsec += 1000000000UL;
		--ts.tv_sec;
	}

	if ( (nanosleep(&ts, &tsRem) == -1) && (errno == EINTR) ) {
		mprintf(("MVE: Timer error! Aborting movie playback!\n"));
		return 1;
	}

end:
    timer_expire.tv_usec += micro_frame_delay;

    if (timer_expire.tv_usec > 1000000) {
        nsec = timer_expire.tv_usec / 1000000;
        timer_expire.tv_sec += nsec;
        timer_expire.tv_usec -= nsec * 1000000;
    }
#else
	fix tv, ts, ts2;

	tv = timer_get_microseconds();

	if (tv > timer_expire)
		goto end;

	ts = timer_expire - tv;

	ts2 = ts/1000;

	Sleep(ts2);

end:
	timer_expire += micro_frame_delay;
#endif

	return 0;
}

static void mve_timer_stop()
{
	timer_expire.tv_sec = 0;
	timer_expire.tv_usec = 0;
	timer_started = 0;
}

/*************************
 * audio handlers
 *************************/

// setup the audio information from the data stream
void mve_audio_createbuf(ubyte minor, ubyte *data)
{
	if (audiobuf_created)
		return;

	// if game sound disabled don't try and play movie audio
	if (!Sound_enabled) {
		mve_audio_canplay = 0;
		return;
	}

#ifdef PLAT_UNIX
    int flags, desired_buffer, sample_rate;

    mas = (mve_audio_t *) malloc ( sizeof(mve_audio_t) );
	memset(mas, 0, sizeof(mve_audio_t));

	mas->format = AL_INVALID;

    flags = mve_get_ushort(data + 2);
    sample_rate = mve_get_ushort(data + 4);
    desired_buffer = mve_get_int(data + 6);

    mas->channels = (flags & 0x0001) ? 2 : 1;
	mas->bitsize = (flags & 0x0002) ? 16 : 8;

	mas->sample_rate = sample_rate;

    if (minor > 0) {
    	mve_audio_compressed = flags & 0x0004 ? 1 : 0;
    } else {
		mve_audio_compressed = 0;
    }

    if (mas->bitsize == 16) {
		if (mas->channels == 2) {
			mas->format = AL_FORMAT_STEREO16;
		} else if (mas->channels == 1) {
			mas->format = AL_FORMAT_MONO16;
		}
	} else if (mas->bitsize == 8) {
		if (mas->channels == 2) {
			mas->format = AL_FORMAT_STEREO8;
		} else if (mas->channels == 1) {
			mas->format = AL_FORMAT_MONO8;
		}
	}

	// somethings wrong, bail now
	if (mas->format == AL_INVALID) {
		mve_audio_canplay = 0;
		audiobuf_created = 1;
		return;
	}

	OpenAL_ErrorCheck( alGenSources(1, &mas->source_id), { mve_audio_canplay = 0; return; } );

	mve_audio_canplay = 1;

	OpenAL_ErrorPrint( alSourcef(mas->source_id, AL_GAIN, 1.0f) );
	OpenAL_ErrorPrint( alSource3f(mas->source_id, AL_POSITION, 0.0f, 0.0f, 0.0f) );
	OpenAL_ErrorPrint( alSource3f(mas->source_id, AL_VELOCITY, 0.0f, 0.0f, 0.0f) );
	OpenAL_ErrorPrint( alSource3f(mas->source_id, AL_DIRECTION, 0.0f, 0.0f, 0.0f) );
	OpenAL_ErrorPrint( alSourcef(mas->source_id, AL_ROLLOFF_FACTOR, 0.0f ) );
	OpenAL_ErrorPrint( alSourcei(mas->source_id, AL_SOURCE_RELATIVE, AL_TRUE ) );

	memset(mas->audio_buffer, 0, MVE_AUDIO_BUFFERS * sizeof(ALuint));

    mve_audio_bufhead = 0;
    mve_audio_buftail = 0;

	audiobuf_created = 1;
#endif
}

// play and stream the audio
void mve_audio_play()
{
#ifdef PLAT_UNIX
	if (mve_audio_canplay) {
		ALint status, bqueued;

		OpenAL_ErrorCheck( alGetSourcei(mas->source_id, AL_SOURCE_STATE, &status), return );
	
		OpenAL_ErrorCheck( alGetSourcei(mas->source_id, AL_BUFFERS_QUEUED, &bqueued), return );
	
		mve_audio_playing = 1;

		if (status != AL_PLAYING && bqueued > 0) {
			OpenAL_ErrorPrint( alSourcePlay(mas->source_id) );
		}
	}
#endif
}

// call this in shutdown to stop and close audio
static void mve_audio_stop()
{
	if (!audiobuf_created)
		return;

#ifdef PLAT_UNIX
	ALint p = 0;

	mve_audio_playing = 0;

	OpenAL_ErrorPrint( alSourceStop(mas->source_id) );
	OpenAL_ErrorPrint( alGetSourcei(mas->source_id, AL_BUFFERS_PROCESSED, &p) );
	OpenAL_ErrorPrint( alSourceUnqueueBuffers(mas->source_id, p, mas->audio_buffer) );
	OpenAL_ErrorPrint( alDeleteBuffers(MVE_AUDIO_BUFFERS, mas->audio_buffer) );
	OpenAL_ErrorPrint( alDeleteSources(1, &mas->source_id) );

	if (mas != NULL) {
		free(mas);
		mas = NULL;
	}
#endif
}

int mve_audio_data(ubyte major, ubyte *data)
{
#ifdef PLAT_UNIX
	static const int selected_chan=1;
	int chan;
	int nsamp;

	if (mve_audio_canplay) {
		chan = mve_get_ushort(data + 2);
		nsamp = mve_get_ushort(data + 4);

		if (chan & selected_chan) {
			ALint bprocessed, bqueued, status;
			ALuint bid;

			OpenAL_ErrorCheck( alGetSourcei(mas->source_id, AL_BUFFERS_PROCESSED, &bprocessed), return 0 );

			while (bprocessed-- > 2) {
				OpenAL_ErrorPrint( alSourceUnqueueBuffers(mas->source_id, 1, &bid) );
			//	fprintf(stderr,"Unqueued buffer %d(%d)\n", mve_audio_bufhead, bid);
		
				if (++mve_audio_bufhead == MVE_AUDIO_BUFFERS)
					mve_audio_bufhead = 0;
			}

			OpenAL_ErrorCheck( alGetSourcei(mas->source_id, AL_BUFFERS_QUEUED, &bqueued), return 0 );
		    
			if (bqueued == 0) 
				mprintf(("MVE: Buffer underun (First is normal)\n"));

			OpenAL_ErrorCheck( alGetSourcei(mas->source_id, AL_SOURCE_STATE, &status), return 0 );

			if (mve_audio_playing && status != AL_PLAYING && bqueued > 0) {
				OpenAL_ErrorCheck( alSourcePlay(mas->source_id), return 0 );
			}

			if (bqueued < MVE_AUDIO_BUFFERS) {
				short *buf = NULL;

				/* HACK: +4 mveaudio_uncompress adds 4 more bytes */
				if (major == 8) {
				    if (mve_audio_compressed) {
						nsamp += 4;

						buf = (short *)malloc(nsamp);
						mveaudio_uncompress(buf, data, -1); /* XXX */
					} else {
						nsamp -= 8;
						data += 8;

						buf = (short *)malloc(nsamp);
						memcpy(buf, data, nsamp);
					}	              
				} else {
					buf = (short *)malloc(nsamp);

					memset(buf, 0, nsamp); /* XXX */
				}


				if (!mas->audio_buffer[mve_audio_buftail]) {
					OpenAL_ErrorCheck( alGenBuffers(1,&mas->audio_buffer[mve_audio_buftail]), { free(buf); return 0; } );
				}

				OpenAL_ErrorCheck( alBufferData(mas->audio_buffer[mve_audio_buftail], mas->format, buf, nsamp, mas->sample_rate), { free(buf); return 0; } );
	    
				OpenAL_ErrorCheck( alSourceQueueBuffers(mas->source_id, 1, &mas->audio_buffer[mve_audio_buftail]), { free(buf); return 0;} );

				//fprintf(stderr,"Queued buffer %d(%d)\n", mve_audio_buftail, mas->audio_buffer[mve_audio_buftail]);

				if (++mve_audio_buftail == MVE_AUDIO_BUFFERS)
					mve_audio_buftail = 0;

				bqueued++;
				free(buf);
			} else {
				mprintf(("MVE: Buffer overrun: Queue full\n"));
			}

			//fprintf(stderr,"Buffers queued: %d\n", bqueued);
		}
	}
#endif

	return 1;
}

/*************************
 * video handlers
 *************************/

int mve_video_createbuf(ubyte minor, ubyte *data)
{
	if (videobuf_created)
		return 1;

	short w, h;
	short count, truecolor;
	w = mve_get_short(data);
	h = mve_get_short(data+2);
	
	if (minor > 0) {
		count = mve_get_short(data+4);
	} else {
		count = 1;
	}
	
	if (minor > 1) {
		truecolor = mve_get_short(data+6);
	} else {
		truecolor = 0;
	}

	g_width = w << 3;
	g_height = h << 3;

	// with Pierre's decoder16 fix in opcode 0xc, 8 should no longer be needed
	g_vBackBuf1 = g_vBuffers = malloc(g_width * g_height * 4);

	if (g_vBackBuf1 == NULL) {
		mprintf(("MOVIE", "ERROR: Can't allocate video buffer"));
		videobuf_created = 1;
		return 0;
	}

	g_vBackBuf2 = (ushort *)g_vBackBuf1 + (g_width * g_height);
		
	memset(g_vBackBuf1, 0, g_width * g_height * 4);

	videobuf_created = 1;

	return 1;
}

static void mve_convert_and_draw()
{
	ushort *pDests;
	ushort *pSrcs;
	ushort *pixels = (ushort *)g_vBackBuf1;
	int x, y;

	pSrcs = pixels;

	pDests = pixelbuf;

	if (g_screenWidth > g_width) {
		pDests += ((g_screenWidth - g_width) / 2) / 2;
	}
	if (g_screenHeight > g_height) {
		pDests += ((g_screenHeight - g_height) / 2) * g_screenWidth;
	}

	for (y=0; y<g_height; y++) {
		for (x = 0; x < g_width; x++) {
			#ifdef HAVE_GLES
			pDests[x] = (1)|(*pSrcs&0x7fff)<<1;
//			pDests[x] = (1)|((*pSrcs)&0x1f)<<11|((*pSrcs)&0x3e0)<<1|((*pSrcs)&0x7c00)>>9;
			#else
			pDests[x] = (1<<15)|*pSrcs;
			#endif

			pSrcs++;
		}
		pDests += g_screenWidth;
	}
}

void mve_video_display()
{
	fix t1 = timer_get_fixed_seconds();
	mve_convert_and_draw();

	int x, y;
	int h = g_screenHeight;
	int w = g_screenWidth;

#ifdef PLAT_UNIX
	if (mve_scale_video) {
		x = y = 0;
	} else {
		// centers on 1024x768, fills on 640x480
		x = ((gr_screen.max_w - g_screenWidth) / 2);
		y = ((gr_screen.max_h - g_screenHeight) / 2);
	}

	// micro_frame_delay is divided by 10 to match mve_video_skiptimer overflow catch
	if ( mve_video_skiptimer > (uint)(micro_frame_delay/10) ) {
		// we are running slow so subtract desired time from actual and skip this frame
		mve_video_skiptimer -= (micro_frame_delay/10);
		return;
	} else {
		// zero out so we can get a new count
		mve_video_skiptimer = 0;
	}

	glBindTexture(GL_TEXTURE_2D, tex);
	
	#ifdef HAVE_GLES
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, pixelbuf);
	#else
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_BGRA, GL_UNSIGNED_SHORT_1_5_5_5_REV, pixelbuf);
	#endif

	#ifdef HAVE_GLES
			GLfloat vtx1[] = {
			 x,y,
			 x,y+256,
			 x+256,y+256,
			 x+256,y,
			};
			GLfloat tex1[] = {
			 0,0,
			 0,i2fl(256)/i2fl(hp2),
			 i2fl(256)/i2fl(wp2),i2fl(256)/i2fl(hp2),
			 i2fl(256)/i2fl(wp2),0
			};
		 
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		 
			glVertexPointer(2, GL_FLOAT, 0, vtx1);
			glTexCoordPointer(2, GL_FLOAT, 0, tex1);
			glDrawArrays(GL_TRIANGLE_FAN,0,4);	// 0,0
			
			vtx1[0]=vtx1[2]=x; vtx1[1]=vtx1[7]=y+256; vtx1[3]=vtx1[5]=y+h; vtx1[4]=vtx1[6]=x+256;
			tex1[0]=tex1[2]=0; tex1[1]=tex1[7]=i2fl(256)/i2fl(hp2); tex1[3]=tex1[5]=i2fl(h)/i2fl(hp2); tex1[4]=tex1[6]=i2fl(256)/i2fl(wp2);
			glVertexPointer(2, GL_FLOAT, 0, vtx1);
			glTexCoordPointer(2, GL_FLOAT, 0, tex1);
			glDrawArrays(GL_TRIANGLE_FAN,0,4);	// 0, 256

			vtx1[0]+=256; vtx1[1]=y; vtx1[2]+=256.0f; vtx1[3]=y+256; vtx1[4]+=256; vtx1[5]=y+256; vtx1[6]+=256.0f; vtx1[7]=y;
			tex1[0]=tex1[2]=i2fl(256)/i2fl(wp2); tex1[1]=tex1[7]=0;  tex1[3]=tex1[5]=i2fl(256)/i2fl(hp2); tex1[4]=tex1[6]=i2fl(512)/i2fl(wp2);
			glVertexPointer(2, GL_FLOAT, 0, vtx1);
			glTexCoordPointer(2, GL_FLOAT, 0, tex1);
			glDrawArrays(GL_TRIANGLE_FAN,0,4);	// 256, 0
			
			vtx1[1]+=256; vtx1[3]=vtx1[5]=y+h; vtx1[7]+=256;
			tex1[1]=tex1[7]=i2fl(256)/i2fl(hp2); tex1[3]=tex1[5]=i2fl(h)/i2fl(hp2);
			glVertexPointer(2, GL_FLOAT, 0, vtx1);
			glTexCoordPointer(2, GL_FLOAT, 0, tex1);
			glDrawArrays(GL_TRIANGLE_FAN,0,4);	// 256, 256
			
			vtx1[0]=vtx1[2]=x+512; vtx1[1]=vtx1[7]=y; vtx1[3]=vtx1[5]=y+256; vtx1[4]=vtx1[6]=x+w;
			tex1[0]=tex1[2]=i2fl(512)/i2fl(wp2); tex1[1]=tex1[7]=0; tex1[3]=tex1[5]=i2fl(256)/i2fl(hp2); tex1[4]=tex1[6]=i2fl(w)/i2fl(wp2);
			glVertexPointer(2, GL_FLOAT, 0, vtx1);
			glTexCoordPointer(2, GL_FLOAT, 0, tex1);
			glDrawArrays(GL_TRIANGLE_FAN,0,4);	// 512, 0

			vtx1[0]=vtx1[2]=x+512; vtx1[1]=vtx1[7]=y+256; vtx1[3]=vtx1[5]=y+h; vtx1[4]=vtx1[6]=x+w;
			tex1[0]=tex1[2]=i2fl(512)/i2fl(wp2); tex1[1]=tex1[7]=i2fl(256)/i2fl(hp2); tex1[3]=tex1[5]=i2fl(h)/i2fl(hp2); tex1[4]=tex1[6]=i2fl(w)/i2fl(wp2);
			glVertexPointer(2, GL_FLOAT, 0, vtx1);
			glTexCoordPointer(2, GL_FLOAT, 0, tex1);
			glDrawArrays(GL_TRIANGLE_FAN,0,4);	// 512, 256

			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	#else
	// 0, 0
	glBegin(GL_QUADS);
		glTexCoord2f(0,0);										glVertex2i(x,y);
		glTexCoord2f(0,i2fl(256)/i2fl(hp2));					glVertex2i(x,y+256);
		glTexCoord2f(i2fl(256)/i2fl(wp2),i2fl(256)/i2fl(hp2));	glVertex2i(x+256,y+256);
		glTexCoord2f(i2fl(256)/i2fl(wp2),0);					glVertex2i(x+256,y);
	glEnd();

	// 0, 256
	glBegin(GL_QUADS);
		glTexCoord2f(0,i2fl(256)/i2fl(hp2));					glVertex2i(x,y+256);
		glTexCoord2f(0,i2fl(h)/i2fl(hp2));						glVertex2i(x,y+h);
		glTexCoord2f(i2fl(256)/i2fl(wp2),i2fl(h)/i2fl(hp2));	glVertex2i(x+256,y+h);
		glTexCoord2f(i2fl(256)/i2fl(wp2),i2fl(256)/i2fl(hp2));	glVertex2i(x+256,y+256);
	glEnd();

	// 256, 0
	glBegin(GL_QUADS);
		glTexCoord2f(i2fl(256)/i2fl(wp2),0);					glVertex2i(x+256,y);
		glTexCoord2f(i2fl(256)/i2fl(wp2),i2fl(256)/i2fl(hp2));	glVertex2i(x+256,y+256);
		glTexCoord2f(i2fl(512)/i2fl(wp2),i2fl(256)/i2fl(hp2));	glVertex2i(x+512,y+256);
		glTexCoord2f(i2fl(512)/i2fl(wp2),0);					glVertex2i(x+512,y);
	glEnd();

	// 256, 256
	glBegin(GL_QUADS);
		glTexCoord2f(i2fl(256)/i2fl(wp2),i2fl(256)/i2fl(hp2));	glVertex2i(x+256,y+256);
		glTexCoord2f(i2fl(256)/i2fl(wp2),i2fl(h)/i2fl(hp2));	glVertex2i(x+256,y+h);
		glTexCoord2f(i2fl(512)/i2fl(wp2),i2fl(h)/i2fl(hp2));	glVertex2i(x+512,y+h);
		glTexCoord2f(i2fl(512)/i2fl(wp2),i2fl(256)/i2fl(hp2));	glVertex2i(x+512,y+256);
	glEnd();

	// 512, 0
	glBegin(GL_QUADS);
		glTexCoord2f(i2fl(512)/i2fl(wp2),0);					glVertex2i(x+512,y);
		glTexCoord2f(i2fl(512)/i2fl(wp2),i2fl(256)/i2fl(hp2));	glVertex2i(x+512,y+256);
		glTexCoord2f(i2fl(w)/i2fl(wp2),i2fl(256)/i2fl(hp2));	glVertex2i(x+w,y+256);
		glTexCoord2f(i2fl(w)/i2fl(wp2),0);						glVertex2i(x+w,y);
	glEnd();

	// 512, 256
	glBegin(GL_QUADS);
		glTexCoord2f(i2fl(512)/i2fl(wp2),i2fl(256)/i2fl(hp2));	glVertex2i(x+512,y+256);
		glTexCoord2f(i2fl(512)/i2fl(wp2),i2fl(h)/i2fl(hp2));	glVertex2i(x+512,y+h);
		glTexCoord2f(i2fl(w)/i2fl(wp2),i2fl(h)/i2fl(hp2));		glVertex2i(x+w,y+h);
		glTexCoord2f(i2fl(w)/i2fl(wp2),i2fl(256)/i2fl(hp2));	glVertex2i(x+w,y+256);
	glEnd();
	#endif
#else
	// centers on 1024x768, fills on 640x480
	x = ((gr_screen.max_w - g_screenWidth) / 2);
	y = ((gr_screen.max_h - g_screenHeight) / 2);

	// DDOI - This is probably really fricking slow
	int bitmap = bm_create (16, w, h, pixelbuf, 0);
	gr_set_bitmap (bitmap);
	gr_bitmap (x, y);
	bm_release (bitmap);
#endif

	gr_flip ();
#ifdef PLAT_UNIX
	os_poll ();	/* DDOI - run event loop(s) */
#endif

	fix t2 = timer_get_fixed_seconds();

	// only get a new count if we are definitely through with old count
	if ( mve_video_skiptimer == 0 ) {
		// for a more accurate count convert the frame rate to a float and multiply
		// by one-hundred-thousand before converting to an uint.
		mve_video_skiptimer = (uint)(f2fl(t2-t1) * 100000);
	}

	int k = key_inkey();
	if ( k == KEY_ESC ) {
		mve_playing = 0;
	}

//	fprintf(stderr, "mve frame took this long: %.6f\n", f2fl(t2-t1));
}

int mve_video_init(ubyte *data)
{
	if (video_inited)
		return 1;

	short width, height;

	width = mve_get_short(data);
	height = mve_get_short(data+2);

	// DDOI - Allocate RGB565 pixel buffer
	pixelbuf = (ushort *)malloc (width * height * 2);

	if (pixelbuf == NULL) {
		mprintf(("MOVIE", "ERROR: Can't allocate memory for pixelbuf"));
		video_inited = 1;
		return 0;
	}

	memset(pixelbuf, 0, width * height * 2);

	g_screenWidth = width;
	g_screenHeight = height;

#ifdef PLAT_UNIX
	int i, tex_w, tex_h;

	tex_w = g_screenWidth;
	tex_h = g_screenHeight;

	// set height and width to a power of 2
	for (i=0; i<16; i++ )	{
		if ( (tex_w > (1<<i)) && (tex_w <= (1<<(i+1))) )	{
			tex_w = 1 << (i+1);
			break;
		}
	}

	for (i=0; i<16; i++ )	{
		if ( (tex_h > (1<<i)) && (tex_h <= (1<<(i+1))) )	{
			tex_h = 1 << (i+1);
			break;
		}
	}

	// try to keep an 8:1 size ratio
	if (tex_w/tex_h > 8)
		tex_h = tex_w/8;
	if (tex_h/tex_w > 8)
		tex_w = tex_h/8;

	wp2 = tex_w;
	hp2 = tex_h;

	glGenTextures(1, &tex);

	Assert(tex != 0);

	if ( tex == 0 ) {
		mprintf(("MOVIE", "ERROR: Can't create a GL texture"));
		video_inited = 1;
		return 0;
	}

	glBindTexture(GL_TEXTURE_2D, tex);
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_ALWAYS);
	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if ( os_config_read_uint(NULL, NOX("ScaleMovies"), 0) == 1 ) {
		float scale_by = (float)gr_screen.max_w / (float)g_screenWidth;

		// don't bother setting anything if we aren't going to need it
		if (scale_by != 1.0f) {
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();

			glScalef( scale_by, scale_by, 1.0f );
			mve_scale_video = 1;
		}
	}

	// NOTE: using NULL instead of pixelbuf crashes some drivers, but then so does pixelbuf so less of two evils...
	#ifdef HAVE_GLES
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, wp2, hp2, 0, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, NULL);
	#else
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB5_A1, wp2, hp2, 0, GL_BGRA, GL_UNSIGNED_SHORT_1_5_5_5_REV, NULL);
	#endif
#endif

	memset(g_palette, 0, 768);
	
	video_inited = 1;
	
	return 1;
}

void mve_video_palette(ubyte *data)
{
	short start, count;
	start = mve_get_short(data);
	count = mve_get_short(data+2);
	memcpy(g_palette + 3*start, data+4, 3*count);
}

void mve_video_codemap(ubyte *data, int len)
{
	g_pCurMap = data;
	g_nMapLength = len;
}

void mve_video_data(ubyte *data, int len)
{
	short nFrameHot, nFrameCold;
	short nXoffset, nYoffset;
	short nXsize, nYsize;
	ushort nFlags;
	ubyte *temp;

	nFrameHot = mve_get_short(data);
	nFrameCold = mve_get_short(data+2);
	nXoffset = mve_get_short(data+4);
	nYoffset = mve_get_short(data+6);
	nXsize = mve_get_short(data+8);
	nYsize = mve_get_short(data+10);
	nFlags = mve_get_ushort(data+12);

	if (nFlags & 1) {
		temp = (ubyte *)g_vBackBuf1;
		g_vBackBuf1 = g_vBackBuf2;
		g_vBackBuf2 = temp;
	}

	decodeFrame16((ubyte *)g_vBackBuf1, g_pCurMap, g_nMapLength, data+14, len-14);
}

void mve_end_chunk()
{
	g_pCurMap = NULL;
}

void mve_init(MVESTREAM *mve)
{
	// reset to default values
	mve_audio_curbuf_curpos = 0;
	mve_audio_bufhead = 0;
	mve_audio_buftail = 0;
	mve_audio_playing = 0;
	mve_audio_canplay = 0;
	mve_audio_compressed = 0;
	audiobuf_created = 0;

	videobuf_created = 0;
	video_inited = 0;
	mve_scale_video = 0;

	mve_playing = 1;
}

void mve_play(MVESTREAM *mve)
{
	int init_timer = 0, timer_error = 0;
	int cont = 1;

	if (!timer_started)
		mve_timer_start();

	while (cont && mve_playing && !timer_error) {
		cont = mve_play_next_chunk(mve);

		if (micro_frame_delay && !init_timer) {
			mve_timer_start();
			init_timer = 1;
		}

		timer_error = mve_do_timer_wait();
	}
}

void mve_shutdown()
{
	mve_audio_stop();

	mve_timer_stop();

	if (pixelbuf != NULL) {
		free(pixelbuf);
		pixelbuf = NULL;
	}

	if (g_vBuffers != NULL) {
		free(g_vBuffers);
		g_vBuffers = NULL;
	}

#ifdef PLAT_UNIX
	if (mve_scale_video) {
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		glLoadIdentity();
	}

	glDeleteTextures(1, &tex);
	tex = 0;

	glEnable(GL_DEPTH_TEST);
#endif
}
