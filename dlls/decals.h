/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
#pragma once
#if !defined(DECALS_H)
#define DECALS_H
#include "mod_features.h"
//
// Dynamic Decals
//
enum decal_e 
{	
	DECAL_GUNSHOT1 = 0, 
	DECAL_GUNSHOT2,
	DECAL_GUNSHOT3,
	DECAL_GUNSHOT4,
	DECAL_GUNSHOT5,
	DECAL_LAMBDA1,
	DECAL_LAMBDA2,
	DECAL_LAMBDA3,
	DECAL_LAMBDA4,
	DECAL_LAMBDA5,
	DECAL_LAMBDA6,
	DECAL_SCORCH1,
	DECAL_SCORCH2,
	DECAL_BLOOD1, 
	DECAL_BLOOD2, 
	DECAL_BLOOD3, 
	DECAL_BLOOD4, 
	DECAL_BLOOD5, 
	DECAL_BLOOD6, 
	DECAL_YBLOOD1, 
	DECAL_YBLOOD2, 
	DECAL_YBLOOD3, 
	DECAL_YBLOOD4, 
	DECAL_YBLOOD5, 
	DECAL_YBLOOD6, 
	DECAL_GLASSBREAK1,
	DECAL_GLASSBREAK2,
	DECAL_GLASSBREAK3,
	DECAL_BIGSHOT1,
	DECAL_BIGSHOT2,
	DECAL_BIGSHOT3,
	DECAL_BIGSHOT4,
	DECAL_BIGSHOT5,
	DECAL_SPIT1,
	DECAL_SPIT2,
	DECAL_BPROOF1,		// Bulletproof glass decal
	DECAL_GARGSTOMP1,	// Gargantua stomp crack
	DECAL_SMALLSCORCH1,	// Small scorch mark
	DECAL_SMALLSCORCH2,	// Small scorch mark
	DECAL_SMALLSCORCH3,	// Small scorch mark
	DECAL_MOMMABIRTH,	// Big momma birth splatter
	DECAL_MOMMASPLAT,
	DECAL_BASE_COUNT,
	DECAL_SPR_SPLT1 = DECAL_BASE_COUNT,
	DECAL_SPR_SPLT2,
	DECAL_SPR_SPLT3,
	DECAL_OPFOR_SCORCH1,
	DECAL_OPFOR_SCORCH2,
	DECAL_OPFOR_SCORCH3,
	DECAL_OPFOR_SMALLSCORCH1,
	DECAL_OPFOR_SMALLSCORCH2,
	DECAL_OPFOR_SMALLSCORCH3,
};

typedef struct 
{
	const char *name;
	int index;
} DLL_DECALLIST;

extern DLL_DECALLIST gDecals[];
#endif //DECALS_H
