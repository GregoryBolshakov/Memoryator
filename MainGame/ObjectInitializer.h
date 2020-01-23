#pragma once
#ifndef OBJECTINITIALIZER_H
#define OBJECTINITIALIZER_H

#include "StaticObject.h"
#include "SpriteStructures.h"
#include "Rock.h"
#include "Ground.h"
#include "GroundConnection.h"
#include "DroppedLoot.h"
#include "Grass.h"
#include "Ground.h"
#include "Spawn.h"
#include "Chamomile.h"
#include "Yarrow.h"
#include "Mugwort.h"
#include "Fern.h"
#include "Brazier.h"
#include "Totem.h"
#include "HareTrap.h"
#include "Fence.h"
#include "Stump.h"
#include "ForestTree.h"
#include "WreathTable.h"
#include "Mushroom.h"
#include "Log.h"
#include "Bush.h"
#include "Lake.h"
#include "Root.h"
#include  "Roof.h"

#include "Monster.h"
#include "Deerchant.h"
#include "Wolf.h"
#include "Hare.h"
#include "Deer.h"
#include "Bear.h"
#include "Owl.h"
#include "OwlBoss.h"
#include "Nightmare1.h"
#include "Nightmare2.h"
#include "Nightmare3.h"
#include "Noose.h"
#include "ClapWhirl.h"
#include <unordered_map>

enum Biomes { SwampyTrees = 1, DarkWoods = 2, BirchGrove = 3, FlowerValley = 4 };

class ObjectInitializer
{
public:
	ObjectInitializer();
	~ObjectInitializer();
	static StaticObject* initializeStaticItem(
		Tag itemClass,
		Vector2f itemPosition,
		int itemType,
		std::string itemName, 
		int count,
		Biomes biome,		
		std::unordered_map<std::string, BoardSprite>* spriteMap,
		bool mirrored = true,
		std::vector<std::pair<Tag, int>> inventory = {});
	static int newNameId;
	static int getRandomTypeByBiome(WorldObject* object, Biomes biome);
	static DynamicObject* initializeDynamicItem(Tag itemClass,
		Vector2f itemPosition,
		std::string itemName,
		std::unordered_map<std::string, BoardSprite>* spriteMap,
		WorldObject* owner = nullptr);
	static std::vector<StaticObject*> vectorCastToStatic(std::vector<WorldObject*> items);
	static std::vector<DynamicObject*> vectorCastToDynamic(std::vector<WorldObject*> items);

	static std::map<Tag, std::string> mappedTags;
	static std::map<std::string, Tag> mappedStrings;
};

#endif

