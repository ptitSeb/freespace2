/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Localization/fhash.h $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 *
 *
 * $Log$
 * Revision 1.2  2002/06/09 04:41:13  relnev
 * added copyright header
 *
 * Revision 1.1.1.1  2002/05/03 03:28:12  root
 * Initial import.
 *
 * 
 * 4     12/01/98 4:46p Dave
 * Put in targa bitmap support (16 bit).
 *  
 * $NoKeywords: $
 */

#ifndef _FRED_XSTR_HASH_TABLE_HEADER_FILE
#define _FRED_XSTR_HASH_TABLE_HEADER_FILE

// -----------------------------------------------------------------------------------------------
// HASH DEFINES/VARS
//


// -----------------------------------------------------------------------------------------------
// HASH FUNCTIONS
//

// initialize the hash table
void fhash_init();

// set the hash table to be active for parsing
void fhash_activate();

// set the hash table to be inactive for parsing
void fhash_deactivate();

// if the hash table is active
int fhash_active();

// flush out the hash table, freeing up everything
void fhash_flush();

// add a string with the given id# to the has table
void fhash_add_str(char *str, int id);

// determine if the passed string exists in the table
// returns : -2 if the string doesn't exit, or >= -1 as the string id # otherwise
int fhash_string_exists(char *str);

#endif

