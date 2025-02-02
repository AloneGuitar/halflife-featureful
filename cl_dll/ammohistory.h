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
//
// ammohistory.h
//
#pragma once
#if !defined(AMMOHISTORY_H)
#define AMMOHISTORY_H

#include "ammo.h"

// this is the max number of items in each bucket
#define MAX_WEAPON_POSITIONS		8

struct BucketPreference
{
	char szName[MAX_WEAPON_NAME];
	int iPreferredSlot;
	int iPreferredSlotPos;
};

class WeaponsResource
{
private:
	// Information about weapons & ammo
	WEAPON		rgWeapons[MAX_WEAPONS];	// Weapons Array

	// counts of weapons * ammo
	WEAPON*		rgSlots[WEAPON_SLOTS_HARDLIMIT][MAX_WEAPON_POSITIONS + 1];	// The slots currently in use by weapons.  The value is a pointer to the weapon;  if it's NULL, no weapon is there
	int			riAmmo[MAX_AMMO_TYPES];					// count of each ammo type

	WEAPON*	weaponTable[WEAPON_SLOTS_HARDLIMIT][MAX_WEAPON_POSITIONS + 1]; // Unlike rgSlots this is always filled with registered weapons
	BucketPreference bucketPreferences[MAX_WEAPONS];
public:
	void Init( void );

	void Reset( void );

///// WEAPON /////
	int			iOldWeaponBits;

	WEAPON *GetWeapon( int iId ) { return &rgWeapons[iId]; }
	void AddWeapon( WEAPON *wp );

	void PickupWeapon( WEAPON *wp )
	{
		rgSlots[wp->iSlot][wp->iSlotPos] = wp;
	}

	void DropWeapon( WEAPON *wp )
	{
		rgSlots[wp->iSlot][wp->iSlotPos] = NULL;
	}

	void DropAllWeapons( void )
	{
		for( int i = 0; i < MAX_WEAPONS; i++ )
		{
			if( rgWeapons[i].iId )
				DropWeapon( &rgWeapons[i] );
		}
	}

	WEAPON* GetWeaponSlot( int slot, int pos ) { return rgSlots[slot][pos]; }

	void LoadWeaponSprites( WEAPON* wp );
	void LoadAllWeaponSprites( void );
	WEAPON* GetFirstPos( int iSlot );
	void SelectSlot( int iSlot, int fAdvance, int iDirection );
	WEAPON* GetNextActivePos( int iSlot, int iSlotPos );

	int HasAmmo( WEAPON *p );

///// AMMO /////
	AMMO GetAmmo( int iId ) { return iId; }

	void SetAmmo( int iId, int iCount ) { riAmmo[ iId ] = iCount;	}

	int CountAmmo( int iId );

	HSPRITE* GetAmmoPicFromWeapon( int iAmmoId, wrect_t& rect );

	int m_maxWeaponSlots;
};

extern WeaponsResource gWR;

#define MAX_HISTORY 12
enum {
	HISTSLOT_EMPTY,
	HISTSLOT_AMMO,
	HISTSLOT_WEAP,
	HISTSLOT_ITEM
};

class HistoryResource
{
private:
	struct HIST_ITEM {
		int type;
		float DisplayTime;  // the time at which this item should be removed from the history
		int iCount;
		int iId;
		int packedColor;
	};

	HIST_ITEM rgAmmoHistory[MAX_HISTORY];

	void ScaleColorsAccordingToDisplayTime(float displayTime, float flTime, int& r, int& g, int& b);

public:

	void Init( void )
	{
		Reset();
	}

	void Reset( void )
	{
		memset( rgAmmoHistory, 0, sizeof rgAmmoHistory );
		iCurrentHistorySlot = 0;
	}

	int iHistoryGap;
	int iCurrentHistorySlot;

	void AddToHistory( int iType, int iId, int iCount = 0 );
	void AddToHistory(int iType, const char *szName, int iCount = 0, int packedColor = 0);

	void CheckClearHistory( void );
	int DrawAmmoHistory( float flTime );
};

extern HistoryResource gHR;
#endif // AMMOHISTORY_H
