#pragma once
#ifndef WORLDGENERATOR_H
#define WORLDGENERATOR_H

#include "ObjectInitializer.h"
#include "GridList.h"

struct BiomesMapCell
{
	StaticObject* groundCell;
	Biomes biomeCell;
};

class WorldGenerator
{
public:
	void generate();
	void init(int width, int height, Vector2i blockSize, Vector2i microBlockSize, GridList* staticGrid, GridList* dynamicGrid, std::map<PackTag, SpritePack>* packsMap);
    void initMainScale();

	void initializeStaticItem(Tag itemClass, Vector2f itemPosition, int itemType, std::string itemName, int count = 1, Biomes biome = DarkWoods, bool mirrored = true, std::vector<std::pair<Tag, int>> inventory = {});
	void initializeDynamicItem(Tag itemClass, Vector2f itemPosition, std::string itemName, WorldObject* owner = nullptr);

	//active generation
	void inBlockGenerate(int blockIndex);
	bool canBeRegenerated(int blockIndex);
	void biomesGenerate();
	void perimeterGeneration(int offset);
	void beyondScreenGeneration();

	WorldGenerator();
	~WorldGenerator();

	BiomesMapCell biomeMatrix[100][100];
	std::map<int, bool> rememberedBlocks = { {0, true} };
	DynamicObject* focusedObject = nullptr;
	const float CLOSEST_SCALE = 1.7, FARTHEST_SCALE = 0.998;
	float scaleFactor = 1, mainScale = 1;
private:
	GridList* staticGrid = nullptr;
	GridList* dynamicGrid = nullptr;	
	std::map<PackTag, SpritePack>* packsMap;
	int biomeGenerateDistance = 4, width = 0, height = 0;
	Vector2i blockSize = { 0, 0 }, microBlockSize = { 0, 0 };
	Vector2i focusedObjectBlock = { 0, 0 };

	// block generation
	void generateGround(int blockIndex);
	bool isRoomyStepBlock(int x, int y) const;
	std::map<std::pair<int, int>, bool> roomyStepBlocks = {
		{std::make_pair(1, 1), true}, {std::make_pair(3, 1), true}, { std::make_pair(5, 1), true}, { std::make_pair(7, 1), true},
		{std::make_pair(2, 3), true}, { std::make_pair(4, 3), true}, { std::make_pair(6, 3), true}, { std::make_pair(8, 3), true},
		{ std::make_pair(1, 5), true}, { std::make_pair(3, 5), true}, { std::make_pair(5, 5), true}, { std::make_pair(7, 5), true},
		{ std::make_pair(2, 7), true}, { std::make_pair(4, 7), true}, { std::make_pair(6, 7), true}, { std::make_pair(8, 7), true} };
	Vector2i stepSize = { 0, 0 };

	// biomes generation
	void initBiomesGenerationInfo();
	bool isTriggerBlock(int x, int y);
	Vector2i biomesChangeCenter = { 0, 0 };
	std::vector<std::vector<Vector2i>> biomesBlocksOffsets = {};
};

#endif


