#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "pstypes.h"

// use system versions of this stuff in here rather than the vm_* versions
#undef malloc
#undef free
#undef strdup


#define MAX_LINE_WIDTH 128

void strlwr (char * str)
{
	while (*str) {*str = tolower (*str); str++; }
}

void strupr (char * str)
{
	while (*str) {*str = toupper (*str); str++; }
}

int filelength (int fd)
{
	struct stat buf;
	if (fstat (fd, &buf) == -1)
		return -1;
		
	return buf.st_size;
}

unsigned long _beginthread (void (*pfuncStart)(void *), unsigned unStackSize, void* pArgList)
{
	STUB_FUNCTION;
	
	return 0;
}

void Sleep (int mili)
{
#ifdef __APPLE__
	// ughh, SDL_Delay causes a slowdown on Tiger for some reason and though I hate
	// doing this, even the few Apple examples I've seen do this over what SDL_Delay does.
	uint then = SDL_GetTicks() + mili;

	while ( then > SDL_GetTicks() );
#else
	SDL_Delay( long(mili) );
#endif
}

void OutputDebugString (const char *str)
{
	fprintf(stderr, "OutputDebugString: %s\n", str);
}

int WSAGetLastError()
{
	return errno;
}

// make specified directory, recursively
// NOTE: since this is for use with CFILE this code assumes that there will be a trailing '/'
//       or a trailing filename.  any directory name not followed by a '/' will be considered a file
int _mkdir(const char *path)
{
	int status = 1;		// if we don't ever call mkdir() to update this then assume we are in error
	char *c, tmp_path[MAX_PATH] = { 0 };

	strncpy(tmp_path, path, MAX_PATH-1);

	c = &tmp_path[1];

	while (c++) {
		c = strchr(c, '/');

		if (c) {
			*c = '\0';

			status = mkdir(tmp_path, 0700);

#ifndef NDEBUG
			int m_error = errno;

			if (status && (m_error != EEXIST) ) {
				Warning(__FILE__, __LINE__, "Cannot mkdir %s: %s", tmp_path, strerror(m_error));
			}
#endif

			*c = '/';
		}
	}

	return status;
}

void _splitpath (const char *path, char *drive, char *dir, char *fname, char *ext)
{
	if (path == NULL)
		return;

	/* fs2 only uses fname */
	if (fname != NULL) {
		const char *ls = strrchr(path, '/');
		if (ls != NULL) {
			ls++;		// move past '/'
		} else {
			ls = path;
		}
	
		const char *lp = strrchr(path, '.');
		if (lp == NULL) {
			lp = ls + strlen(ls);	// move to the end
		}
	
		int dist = lp-ls;
		if (dist > (_MAX_FNAME-1))
			dist = _MAX_FNAME-1;
		
		strncpy(fname, ls, dist);
		fname[dist] = 0;	// add null, just in case
	}
}

int MulDiv(int a, int b, int c)
{
	/* slow long long version */
	__extension__ long long aa = a;
	__extension__ long long bb = b;
	__extension__ long long cc = c;
	
	__extension__ long long dd = aa * bb;
	__extension__ long long ee = dd / cc;
	
	int retr = (int) ee;
	
	return retr;
}

/* mem debug junk */
#ifndef NDEBUG
//#define WANT_DEBUG
#endif

int TotalRam = 0;

#ifdef WANT_DEBUG
typedef struct RAM {
	void *addr;
	int size;
	
	char *file;
	int line;
	
	RAM *next;
} RAM;

static RAM *RamTable;
#endif

#ifndef NDEBUG
void vm_free(void* ptr, const char *file, int line)
#else
void vm_free(void* ptr)
#endif
{
#ifdef WANT_DEBUG
	fprintf(stderr, "FREE: %s:%d addr = %p\n", file, line, ptr);
	
	RAM *item = RamTable;
	RAM **mark = &RamTable;
	
	while (item != NULL) {
		if (item->addr == ptr) {
			RAM *tmp = item;
			
			*mark = item->next;
			
			free(tmp->addr);
			free(tmp);
			
			return;
		}
		
		mark = &(item->next);
		
		item = item->next;
	}
	
	fprintf(stderr, "ERROR: vm_free caught invalid free: addr = %p, file = %s/%d\n", ptr, file, line);
#else	
	free(ptr);
#endif
}

#ifndef NDEBUG
void *vm_malloc(int size, const char *file, int line)
#else
void *vm_malloc(int size)
#endif
{
#ifdef WANT_DEBUG
	fprintf(stderr, "MALLOC: %s:%d %d bytes\n", file, line, size);
	
	RAM *next = (RAM *)malloc(sizeof(RAM));
	
	next->addr = malloc(size);
	next->size = size;
	next->file = file;
	next->line = line;
	
	next->next = RamTable;
	RamTable = next;
	
	return next->addr;
#else
	return malloc(size);
#endif	
}

#ifndef NDEBUG
char *vm_strdup(char const* str, const char *file, int line)
#else
char *vm_strdup(char const* str)
#endif
{
#ifdef WANT_DEBUG
	fprintf(stderr, "STRDUP: %s:%d\n", file, line);
	
	RAM *next = (RAM *)malloc(sizeof(RAM));
	
	next->addr = strdup(str);
	next->size = strlen(str)+1;
	next->file = file;
	next->line = line;
	
	next->next = RamTable;
	RamTable = next;
	
	return (char *)next->addr;
#else
	return strdup(str);
#endif
}

void vm_dump()
{
#ifdef WANT_DEBUG
	int i = 0;
	int mem = 0;
	fprintf(stderr, "\nDumping allocated memory:\n");
	
	RAM *ptr = RamTable;
	while (ptr) {
		fprintf(stderr, "%d: file: %s:%d: addr:%p size:%d\n", i, ptr->file, ptr->line, ptr->addr, ptr->size);
		mem += ptr->size;
		ptr = ptr->next;
		i++;
	}
	
	fprintf(stderr, "\nTotal of %d left-over bytes from %d allocations\n", mem, i);
#endif	
}

void windebug_memwatch_init()
{
	TotalRam = 0;
}

/* error message debugging junk */
/*
int Log_debug_output_to_file = 0;

void load_filter_info(void)
{
//	STUB_FUNCTION;
}

void outwnd_printf(char* id, char* format, ...)
{
	char tmp[MAX_LINE_WIDTH*4];
	va_list args;

	va_start (args, format);
	vsprintf (tmp, format, args);
	va_end(args);
	fprintf (stderr, "%s: %s\n", id, tmp);
}

void outwnd_printf2(char* format, ...)
{
	char tmp[MAX_LINE_WIDTH*4];
	va_list args;

	va_start (args, format);
	vsprintf (tmp, format, args);
	va_end(args);
	fprintf (stderr, "General: %s", tmp);
}

void outwnd_close()
{
//	STUB_FUNCTION;
}
*/
void Warning( const char * filename, int line, const char * format, ... )
{
	char tmp[MAX_LINE_WIDTH*4] = { 0 };
	va_list args;

	va_start (args, format);
	vsnprintf (tmp, sizeof(tmp), format, args);
	va_end(args);
	fprintf (stderr, "Warning: (%s:%d): %s\n", filename, line, tmp);
}

void Error( const char * filename, int line, const char * format, ... )
{
	char tmp[MAX_LINE_WIDTH*4] = { 0 };
	va_list args;

	va_start (args, format);
	vsnprintf (tmp, sizeof(tmp), format, args);
	va_end(args);
	fprintf (stderr, "Error: (%s:%d): %s\n", filename, line, tmp);
	exit (1);
}

void WinAssert(const char * text, const char *filename, int line)
{
	fprintf (stderr, "Assertion: (%s:%d) %s\n", filename, line, text);
//	exit(1);
}
