/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Cfilearchiver/CfileArchiver.cpp $
 * $Revision: 247 $
 * $Date: 2004-12-15 05:10:45 +0100 (Wed, 15 Dec 2004) $
 * $Author: taylor $
 *
 * Program to create an archive file for use with cfile stuff
 *
 * $Log$
 * Revision 1.6  2004/12/15 04:10:45  taylor
 * outwnd_unix.cpp from fs2_open for logging to file in debug mode
 * fixes for default function values
 * always use vm_* functions for sanity sake
 * make cfilearchiver 64-bit compatible
 * fix crash on exit from double free()
 * fix crash on startup from extra long GL extension string in debug
 *
 * Revision 1.5  2003/05/04 04:49:48  taylor
 * improve error handling, instructions
 *
 * Revision 1.4  2003/01/30 20:01:46  relnev
 * ported (Taylor Richards)
 *
 * Revision 1.3  2002/06/09 04:41:15  relnev
 * added copyright header
 *
 * Revision 1.2  2002/05/07 03:16:43  theoddone33
 * The Great Newline Fix
 *
 * Revision 1.1.1.1  2002/05/03 03:28:08  root
 * Initial import.
 *
 * 
 * 2     10/23/98 6:15p Dave
 *
 * $NoKeywords: $
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifndef PLAT_UNIX
#include <direct.h>
#include <io.h>
#include <conio.h>
#else
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#endif

#include "pstypes.h"

static int data_error;
static int no_dir;

unsigned int Total_size=16; // Start with size of header
unsigned int Num_files =0;
FILE * fp_out = NULL;
FILE * fp_out_hdr = NULL;

typedef struct vp_header {
	char id[4];
	int version;
	int index_offset;
	int num_files;
} vp_header;

//vp_header Vp_header;

char archive_dat[1024];
char archive_hdr[1024];

#define BLOCK_SIZE (1024*1024)
#define VERSION_NUMBER 2;

char tmp_data[BLOCK_SIZE];		// 1 MB

void write_header()
{
	fseek(fp_out, 0, SEEK_SET);
	fwrite("VPVP", 1, 4, fp_out);
	int ver = VERSION_NUMBER;
	fwrite(&ver, 1, 4, fp_out);
	fwrite(&Total_size, 1, 4, fp_out);
	fwrite(&Num_files, 1, 4, fp_out);
}

int write_index(char *hf, char *df)
{
	FILE *h = fopen(hf, "rb");
	if (!h) return 0;
	FILE *d = fopen(df, "a+b");
	if (!d) return 0;
	for (unsigned int i=0;i<Num_files;i++) {
		fread(tmp_data, 32+4+4+4, 1, h);
		fwrite(tmp_data, 32+4+4+4, 1, d);
	}
	fclose(h);
	fclose(d);
	return 1;
}

void pack_file( char *filespec, char *filename, int filesize, fs_time_t time_write )
{
	char path[1024];

	if ( strstr( filename, ".vp" ))	{
		// Don't pack yourself!!
		return;
	}

	if ( strstr( filename, ".hdr" ))	{
		// Don't pack yourself!!
		return;
	}

	if ( filesize == 0 ) {
		// Don't pack 0 length files, screws up directory structure!
		return;
	}

	memset( path, 0, sizeof(path));
	strcpy( path, filename );
	if ( strlen(filename)>31 )	{
		printf( "Filename '%s' too long\n", filename );
		exit(1);
	}
	fwrite( &Total_size, 1, 4, fp_out_hdr );
	fwrite( &filesize, 1, 4, fp_out_hdr );
	fwrite( &path, 1, 32, fp_out_hdr );
	fwrite( &time_write, 1, sizeof(fs_time_t), fp_out_hdr);

	Total_size += filesize;
	Num_files++;
#ifndef PLAT_UNIX
	printf( "Packing %s\\%s...", filespec, filename );

	
	sprintf( path, "%s\\%s", filespec, filename );
#else
	printf( "Packing %s/%s...", filespec, filename );


	sprintf( path, "%s/%s", filespec, filename );
#endif

	FILE *fp = fopen( path, "rb" );
	
	if ( fp == NULL )	{
		printf( "Error opening '%s'\n", path );
		exit(1);
	}

	int nbytes, nbytes_read=0;

	do	{
		nbytes = fread( tmp_data, 1, BLOCK_SIZE, fp );
		if ( nbytes > 0 )	{
			fwrite( tmp_data, 1, nbytes, fp_out );
			nbytes_read += nbytes;

		}
	} while( nbytes > 0 );

	fclose(fp);

	printf( " %d bytes\n", nbytes_read );
}

// This function adds a directory marker to the header file
void add_directory( char * dirname)
{
	char path[256];
	char *pathptr = path;
	char *tmpptr;

	strcpy(path, dirname);
	fwrite(&Total_size, 1, 4, fp_out_hdr);
	int i = 0;
	fwrite(&i, 1, 4, fp_out_hdr);
	// strip out any directories that this dir is a subdir of
#ifndef PLAT_UNIX
	while ((tmpptr = strchr(pathptr, '\\')) != NULL) {
#else
	while ((tmpptr = strchr(pathptr, '/')) != NULL) {
#endif
		pathptr = tmpptr+1;
	}
	fwrite(pathptr, 1, 32, fp_out_hdr);
	fwrite(&i, 1, 4, fp_out_hdr); // timestamp = 0
	Num_files++;
}

void pack_directory( char * filespec)
{
#ifndef PLAT_UNIX
	int find_handle;
	_finddata_t find;
#endif
	char tmp[512];
	char tmp1[512];

/*
	char dir_name[512];
	char *last_slash;

	last_slash = strrchr(filespec, '\\');
	if ( last_slash ) {
		strcpy(dir_name, last_slash+1);
	} else {
		strcpy(dir_name, filespec);
	}

	if ( !stricmp(dir_name, "voice") ) {
		return;
	}
*/

#ifdef PLAT_UNIX
	char *ts;

	// strip trailing '/'
	ts = filespec+(strlen(filespec)-1);
	while(*ts == '/' && ts > filespec)
		*ts = '\0';

	strcpy( tmp1, filespec );

	add_directory(filespec);
	strcat( tmp1, "/*.*" );
#else
	strcpy( tmp1, filespec );

	add_directory(filespec);
	strcat( tmp1, "\\*.*" );
#endif
	
	printf( "In dir '%s'\n", tmp1 );

#ifndef PLAT_UNIX
	find_handle = _findfirst( tmp1, &find );
	if( find_handle != -1 )	{
		if ( find.attrib & _A_SUBDIR )	{
			if (strcmp( "..", find.name) && strcmp( ".", find.name))	{
				strcpy( tmp, filespec );
				strcat( tmp, "\\" );
				strcat( tmp, find.name );
				pack_directory(tmp);
			}
		} else {
			pack_file( filespec, find.name, find.size, (fs_time_t)find.time_write );
		}

		while( !_findnext( find_handle, &find ) )	{
			if ( find.attrib & _A_SUBDIR )	{
				if (strcmp( "..", find.name) && strcmp( ".", find.name))	{
					strcpy( tmp, filespec );
					strcat( tmp, "\\" );
					strcat( tmp, find.name );
					pack_directory(tmp);

				}
			} else {
				pack_file( filespec, find.name, find.size, find.time_write );
			}
		}
	}
#else
	DIR *dirp;
	struct dirent *dir;

	dirp = opendir (filespec);
	if ( dirp ) {
		while ((dir = readdir(dirp)) != NULL) {

			char fn[MAX_PATH];
			snprintf(fn, MAX_PATH-1, "%s/%s", filespec, dir->d_name);
			fn[MAX_PATH-1] = 0;
			
			struct stat buf;
			if (stat(fn, &buf) == -1) {
				continue;
			}

			if ( (strcmp(dir->d_name, ".") == 0) || (strcmp(dir->d_name, "..") == 0) ) {
				continue;
			}

			if (S_ISDIR(buf.st_mode)) {
				strcpy( tmp, filespec );
				strcat( tmp, "/" );
				strcat( tmp, dir->d_name );
				pack_directory(tmp);
			} else {
				pack_file( filespec, dir->d_name, buf.st_size, buf.st_mtime );
			}
		}
		closedir(dirp);
	} else {
		printf("Error: Source directory does not exist!\n");
		no_dir = 1;
	}
#endif
	add_directory("..");
}

int verify_directory( char *filespec )
{
	char *ts;
	char *dd;

	// strip trailing '/'
	ts = filespec+(strlen(filespec)-1);
	while(*ts == '/' && ts > filespec)
		*ts = '\0';

	// make sure last directory is named "data", ignoring case
	dd = filespec+(strlen(filespec)-4);
	if ( stricmp( dd, "data" ) )
		data_error = 1;
	
	return data_error;
}

void print_instructions()
{
	printf( "Creates a vp archive out of a FreeSpace data tree.\n\n" );
	printf( "Usage:		cfilearchiver archive_name src_dir\n");
	printf( "Example:	cfilearchiver freespace /tmp/freespace/data\n\n");
	printf( "Directory structure options:\n" );
	printf( "   Effects                   (.ani .pcx .neb .tga)\n" );
	printf( "   Fonts                     (.vf)\n" );
	printf( "   Hud                       (.ani .pcx .tga\n" );
	printf( "   Interface                 (.pcx .ani .tga)\n" );
	printf( "   Maps                      (.pcx .ani .tga)\n" );
	printf( "   Missions                  (.ntl .ssv), FS1(.fsm .fsc), FS2(.fs2 .fc2)\n" );
	printf( "   Models                    (.pof)\n" );
	printf( "   Music                     (.wav)\n" );
	printf( "   Sounds/8b22k              (.wav)\n" );
	printf( "   Sounds/16b11k             (.wav)\n" );
	printf( "   Tables                    (.tbl)\n" );
	printf( "   Voice/Briefing            (.wav)\n" );
	printf( "   Voice/Command briefings   (.wav)\n" );
	printf( "   Voice/Debriefing          (.wav)\n" );
	printf( "   Voice/Personas            (.wav)\n" );
	printf( "   Voice/Special             (.wav)\n" );
	printf( "   Voice/Training            (.wav)\n" );

	exit(0);
}

int main(int argc, char *argv[] )
{
	char archive[1024];
	char *p;

	if ( argc < 3 )	{
#ifndef PLAT_UNIX
		printf( "Usage: %s archive_name src_dir\n", argv[0] );
		printf( "Example: %s freespace c:\\freespace\\data\n", argv[0] );
		printf( "Creates an archive named freespace out of the\nfreespace data tree\n" );
		printf( "Press any key to exit...\n" );
		getch();
		return 1;
#else
		print_instructions();
#endif
	}

	strcpy( archive, argv[1] );
	p = strchr( archive, '.' );
	if (p) *p = 0;		// remove extension	

	strcpy( archive_dat, archive );
	strcat( archive_dat, ".vp" );

	strcpy( archive_hdr, archive );
	strcat( archive_hdr, ".hdr" );

	fp_out = fopen( archive_dat, "wb" );
	if ( !fp_out )	{
		printf( "Couldn't open '%s'!\n", archive_dat );
#ifndef PLAT_UNIX
		printf( "Press any key to exit...\n" );
		getch();
		return 1;
#else
		exit(1);
#endif
	}

	fp_out_hdr = fopen( archive_hdr, "wb" );
	if ( !fp_out_hdr )	{
		printf( "Couldn't open '%s'!\n", archive_hdr );
#ifndef PLAT_UNIX
		printf( "Press any key to exit...\n" );
		getch();
		return 1;
#else
		exit(2);
#endif
	}

	if ( verify_directory( argv[2] ) != 0 ) {
		printf("Warning! Last directory must be named \"data\" (not case sensitive)\n");
		exit(3);
	}

	write_header();

	pack_directory( argv[2] );

	// in case the directory doesn't exist
	if ( no_dir )
		exit(4);

	write_header();

	fclose(fp_out);
	fclose(fp_out_hdr);

	printf( "Data files written, appending index...\n" );

	if (!write_index(archive_hdr, archive_dat)) {
		printf("Error appending index!\n");
#ifndef PLAT_UNIX
		printf("Press any key to exit...\n");
		getch();
#endif
		return 1;
	}
	
	printf( "%d total KB.\n", Total_size/1024 );
	return 0;
}
