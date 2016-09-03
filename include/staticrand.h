/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/Math/StaticRand.h $
 * $Revision: 110 $
 * $Date: 2002-06-09 06:41:30 +0200 (Sun, 09 Jun 2002) $
 * $Author: relnev $
 *
 * header for Static Random functions
 *
 * $Log$
 * Revision 1.2  2002/06/09 04:41:15  relnev
 * added copyright header
 *
 * Revision 1.1.1.1  2002/05/03 03:28:12  root
 * Initial import.
 *
 * 
 * 2     10/07/98 10:53a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:49a Dave
 * 
 * 4     3/17/98 12:16a Allender
 * asteroids in multiplayer -- minor problems with position being correct
 * 
 * 3     12/30/97 5:46p Lawrance
 * Rename rnd() to rand_alt().
 * 
 * 2     12/30/97 4:27p Lawrance
 * Add new rnd() function that doesn't affect rand() sequence.
 * 
 * 1     8/08/97 3:38p Allender
 * 
*/

#ifndef _STATIC_RAND_H

#include "pstypes.h"

#define	SEMIRAND_MAX_LOG	4
#define	SEMIRAND_MAX		(2 << SEMIRAND_MAX_LOG)	//	Do not change this!  Change SEMIRAND_MAX_LOG!

extern int Semirand[SEMIRAND_MAX];			// this array is saved by the ai code on save/restore

extern void init_semirand();
extern int static_rand(int num);
extern float static_randf(int num);
extern void static_randvec(int num, vector *vp);
extern float static_randf_range(int num, float min, float max);

// Alternate random number generator that doesn't affect rand() sequence

void	srand_alt(int seed);	// Seed the random number generator
int	rand_alt();				// Get a random integer between 1 and RND_MAX
float	frand_alt();				// Get a random float between 0 and 1.0

#endif

