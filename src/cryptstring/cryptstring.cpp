/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Cryptstring/cryptstring.cpp $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 *
 * Applet for crypting strings.
 *
 * $Log$
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
 * 2     10/23/98 6:21p Dave
 *
 * $NoKeywords: $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crypt.h"

int main(int argc, char **argv)
{
	int i;
	char *crypt_string;

	if ( argc == 1 ) {
		printf("Usage: cryptstring <string1> <string2> ...\n");
		printf("Output will be <crypt1> <crypt2>\n");
		exit(1);
	}

	for ( i = 1; i < argc; i++ ) {
		char *s;

		s = argv[i];
		// if the length of the string is greater than the number of crypted symbols we
		// return, then pass only the maximum length
		if ( strlen(s) > CRYPT_STRING_LENGTH )
			s += (strlen(s) - CRYPT_STRING_LENGTH);

		crypt_string = jcrypt(s);
		printf("%s\n", crypt_string);
	}

	return 0;
}

char *jcrypt (char *plainstring)
{
	int i,t,len;
	static char cryptstring[CRYPT_STRING_LENGTH + 1];

	len=strlen (plainstring);
	if (len > CRYPT_STRING_LENGTH)
		len = CRYPT_STRING_LENGTH;
   
	for (i = 0;i < len; i++) {
		cryptstring[i]=0; 

		for (t = 0; t < len; t++) {
			cryptstring[i]^=(plainstring[t] ^ plainstring[i%(t+1)]);
			cryptstring[i]%=90;
			cryptstring[i]+=33;
		}
	}

	cryptstring[i]=0;
	return ((char *)cryptstring);
}

