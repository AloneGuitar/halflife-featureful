#pragma once
#ifndef HUD_INVENTORY_H
#define HUD_INVENTORY_H

#include "cl_dll.h"
#include "cdll_int.h"

#include <vector>

#define INVENTORY_PLACE_HIDE -1
#define INVENTORY_PLACE_DEFAULT 0
#define INVENTORY_PLACE_TOP_LEFT 1
#define INVENTORY_PLACE_TOP_RIGHT 2
#define INVENTORY_PLACE_BOTTOM_CENTER 3

struct InventoryItemHudSpec
{
	InventoryItemHudSpec();
	char itemName[24];
	char spriteName[24];
	int packedColor;
	int alpha;
	int position;
	bool colorDefined;
};

class InventoryHudSpec
{
public:
	InventoryHudSpec();
	bool ReadFromFile(const char* fileName);
	const InventoryItemHudSpec* GetInventoryItemSpec(const char* itemName);

	int DefaultSpriteAlpha() const { return defaultSpriteAlpha; }
	int TextAlpha() const { return textAlpha; }
private:
	std::vector<InventoryItemHudSpec> inventory;
	int defaultSpriteAlpha;
	int textAlpha;
};

#endif
