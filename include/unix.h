// unix.h - duplicates some MS defines

#ifndef _UNIX_H
#define _UNIX_H

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include "SDL.h"

#define TRUE 1
#define FALSE 0
#define _MAX_FNAME 255
#define _MAX_PATH 255
#define MAX_PATH 255
#define _MAX_DIR 256
#define MAX_FILENAME_LENGTH 64
#define _cdecl
#define __cdecl
#define __int64 long long
#define LARGE_INTEGER long long
#define stricmp strcasecmp
#define strnicmp strncasecmp
#define _strnicmp strncasecmp
#define _isnan isnan
#define HANDLE int
#define HINSTANCE int
#define _getcwd getcwd
#define _chdir chdir
#define _strlwr strlwr
#define _unlink unlink
#define _hypot hypot
#define _access access
#define byte unsigned char
#define __try try
#define __except catch
#define LPSTR char *
#define PASCAL
#define CALLBACK

#if ( defined(__x86_64__) || defined(_WIN64) )
typedef unsigned int DWORD;
#else
typedef unsigned long DWORD;
#endif
typedef unsigned short WORD;

extern void strlwr (char *str);
extern void strupr (char *str);
extern int filelength (int fd);
extern int MulDiv (int, int, int);

extern void Sleep (int miliseconds);
extern unsigned long _beginthread (void (*pfuncStart)(void *), unsigned unStackSize, void* pArgList);
extern void OutputDebugString (const char *);
extern int WSAGetLastError ();
extern int _mkdir(const char *path);
extern void _splitpath (const char *path, char *drive, char *dir, char *fname, char *ext);

typedef struct FILETIME_s {
	    DWORD dwLowDateTime;
	    DWORD dwHighDateTime;
} FILETIME, *PFILETIME;

#define WAVE_FORMAT_PCM		1
#define WAVE_FORMAT_ADPCM	2

#pragma pack(1)
typedef struct {
	WORD wFormatTag;
	WORD nChannels;
	DWORD nSamplesPerSec;
	DWORD nAvgBytesPerSec;
	WORD nBlockAlign;
} WAVEFORMAT;
                    
typedef struct {
	WAVEFORMAT wf;
	WORD wBitsPerSample;
} PCMWAVEFORMAT;

typedef struct {
	WORD  wFormatTag;
	WORD  nChannels;
	DWORD nSamplesPerSec;
	DWORD nAvgBytesPerSec;
	WORD  nBlockAlign;
	WORD  wBitsPerSample;
	WORD  cbSize;
} WAVEFORMATEX;
#pragma pack()

#define CRITICAL_SECTION SDL_mutex*

#define closesocket(A) close(A)
#define CopyMemory(A,B,C) memcpy(A,B,C)
#define UINT unsigned int
#define SOCKET int
#define SOCKADDR_IN struct sockaddr_in
#define SOCKADDR struct sockaddr
#define LPSOCKADDR struct sockaddr*
#define LPHOSTENT struct hostent*
#define HOSTENT struct hostent
#define LPINADDR struct in_addr*
#define LPIN_ADDR struct in_addr*
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1
#define TIMEVAL struct timeval
#define SERVENT struct servent
#define BOOL int

#define WSAEALREADY EALREADY
#define WSAEINVAL EINVAL
#define WSAEWOULDBLOCK EAGAIN
#define WSAEISCONN EISCONN
#define WSAECONNRESET ECONNRESET
#define WSAECONNABORTED ECONNABORTED
#define WSAESHUTDOWN ESHUTDOWN
#define WSAENOTSOCK ENOTSOCK

#endif
