/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

#ifndef _MODELSINC_H
#define _MODELSINC_H

#include "model.h"

#ifndef MODEL_LIB 
#error This should only be used internally by the model library.  See John if you think you need to include this elsewhere.
#endif

#define OP_EOF 			0
#define OP_DEFPOINTS 	1
#define OP_FLATPOLY		2
#define OP_TMAPPOLY		3
#define OP_SORTNORM		4
#define OP_BOUNDBOX		5

// change header for freespace2
#ifdef MAKE_FS1
	#define FREESPACE1_FORMAT
#else
	#define FREESPACE2_FORMAT
#endif
#if defined( FREESPACE1_FORMAT )
#elif defined ( FREESPACE2_FORMAT )
#else
	#error Neither FREESPACE1_FORMAT or FREESPACE2_FORMAT defined
#endif

// FREESPACE1 FORMAT
#if defined( FREESPACE1_FORMAT )
	#define ID_OHDR 0x5244484f	// RDHO, POF file header
	#define ID_SOBJ 0x4a424f53	// JBOS, Subobject header
#else
	#define ID_OHDR 0x32524448	// 2RDH, POF file header
	#define ID_SOBJ 0x324a424f	// 2JBO, Subobject header
#endif
#define ID_PSPO 0x4f505350	// OPSP, POF header ID
#define ID_TXTR 0x52545854	// RTXT, Texture filename list
#define ID_INFO 0x464e4950	// FNIP, POF file information, like command line, etc
#define ID_GRID 0x44495247	// DIRG, Grid information
#define ID_SPCL 0x4c435053	// LCPS, Special object -- like a gun, missile, docking point, etc.
#define ID_PATH 0x48544150	// HTAP, A spline based path
#define ID_GPNT 0x544e5047	// TNPG, gun points
#define ID_MPNT 0x544e504d  // TNPM, missile points
#define ID_DOCK 0x4b434f44	// KCOD, docking points
#define ID_TGUN 0x4e554754  // NUGT, turret gun points
#define ID_TMIS 0x53494d54	// SIMT, turret missile points
#define ID_FUEL 0x4c455546	// LEUF, thruster points
#define ID_SHLD 0x444c4853	// DLHS, shield definition
#define ID_EYE  0x20455945	// ' EYE', eye information
#define ID_INSG 0x47534e49	// GSNI, insignia information
#define ID_ACEN 0x4e454341	// NECA, autocentering information

#define uw(p)	(*((uint *) (p)))
#define w(p)	(*((int *) (p)))
#define wp(p)	((int *) (p))
#define vp(p)	((vector *) (p))
#define fl(p)	(*((float *) (p)))

extern int model_interp(matrix * orient, ubyte * data, polymodel * pm );

// Creates the octants for a given polygon model
void model_octant_create( polymodel * pm );

// frees the memory the octants use for a given polygon model
void model_octant_free( polymodel * pm );

void model_calc_bound_box( vector *box, vector *big_mn, vector *big_mx);

void interp_clear_instance();

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
// Endian swapping stuff  tigital
void swap_bsp_data( polymodel * pm, void *model_ptr );
#endif

#define MAX_POLYGON_VECS	1100		//6500 (7x)
#define MAX_POLYGON_NORMS	2800		//6500 (3x)

extern vector *Interp_verts[MAX_POLYGON_VECS];

#endif

