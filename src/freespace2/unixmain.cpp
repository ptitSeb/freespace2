#include <sys/stat.h>

#include "pstypes.h"
#include "osregistry.h"
#include "osapi.h"

#undef malloc
#undef free

int PASCAL WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR szCmdLine, int nCmdShow);

#if defined(__APPLE__) && !defined(MACOSX)
char full_path[1024];
#endif

void vm_dump();

int main(int argc, char **argv)
{
	char userdir[MAX_PATH] = { 0 };

#if defined(__APPLE__) && !defined(MACOSX)
	strcpy( full_path, *argv );
#endif

	// create user game directory
	snprintf(userdir, MAX_PATH, "%s/%s/", detect_home(), Osreg_user_dir);
	_mkdir(userdir);	
	
	char *argptr = NULL;
	int i;
	int len = 0;

	for (i = 1; i < argc; i++) {
		len += strlen(argv[i]) + 1;
	}

	argptr = (char *)malloc(len+5);

	if (argptr == NULL) {
		fprintf(stderr, "ERROR: out of memory in main!\n");
		exit(1);
	}

	memset(argptr, 0, len+5);

	for (i = 1; i < argc; i++) {
		strcat(argptr, argv[i]);
		strcat(argptr, " ");
	}

	int retr = WinMain(1, 0, argptr, 0);

	free(argptr);
		
	vm_dump();
	
	return retr;	
}
