#ifndef LOCUS_H
#define LOCUS_H

#include "cbase.h"

bool		IsLikelyNumber(const char* szText);
bool		TryCalcLocus_Position	( CBaseEntity *pEntity, CBaseEntity *pLocus, const char *szText , Vector& result );
bool		TryCalcLocus_Velocity	( CBaseEntity *pEntity, CBaseEntity *pLocus, const char *szText, Vector& result );
bool		TryCalcLocus_Ratio		( CBaseEntity *pLocus, const char *szText, float& result );
bool		TryCalcLocus_Color		( CBaseEntity *pEntity, CBaseEntity *pLocus, const char *szText, Vector& result );

#endif
