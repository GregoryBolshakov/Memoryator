#pragma once

#include "ObjectInitializer.h"
#include "GridList.h"

class WorldGenerator
{
public:	
	const float CLOSEST_SCALE = 1.7f;
	const float FARTHEST_SCALE = 0.998f;
	
	void generate();
	void init(int width, int height, Vector2f blockSize, Vector2f microBlockSize, GridList* staticGrid, GridList* dynamicGrid, std::map<PackTag, SpritePack>* packsMap);
    void initMainScale();

	void initializeStaticItem(
		Tag itemClass,
		Vector2f itemPosition,
		int itemType,
		const std::string& itemName,
		int count = 1,
		Biomes biome = DarkWoods,
		bool mirrored = true,
		const std::vector<std::pair<Tag, int>>& inventory = {}) const;
	
	void initializeDynamicItem(Tag itemClass, Vector2f itemPosition, const std::string& itemName, WorldObject* owner = nullptr);

	//active generation
	void inBlockGenerate(int blockIndex);
	bool canBeRegenerated(int blockIndex) const;
	void biomesGenerate();
	void perimeterGeneration();
	void beyondScreenGeneration();

	WorldGenerator();
	~WorldGenerator();

	Biomes biomeMatrix[100][100]{};
	std::map<int, bool> rememberedBlocks = { {0, true} };
	DynamicObject* focusedObject = nullptr;
	float scaleFactor = 1;
	float mainScale = 1;
private:
	GridList* staticGrid = nullptr;
	GridList* dynamicGrid = nullptr;	
	std::map<PackTag, SpritePack>* packsMap{};
	int width = 0;
	int height = 0;
	Vector2f blockSize = { 0, 0 };
	Vector2f microBlockSize = { 0, 0 };
	Vector2i focusedObjectBlock = { 0, 0 };

	// block generation
	void generateGround(int blockIndex);
	[[nodiscard]] bool isRoomyStepBlock(int x, int y) const;
	std::map<std::pair<int, int>, bool> roomyStepBlocks = {
		{std::make_pair(1, 1), true}, {std::make_pair(3, 1), true}, { std::make_pair(5, 1), true}, { std::make_pair(7, 1), true},
		{std::make_pair(2, 3), true}, { std::make_pair(4, 3), true}, { std::make_pair(6, 3), true}, { std::make_pair(8, 3), true},
		{ std::make_pair(1, 5), true}, { std::make_pair(3, 5), true}, { std::make_pair(5, 5), true}, { std::make_pair(7, 5), true},
		{ std::make_pair(2, 7), true}, { std::make_pair(4, 7), true}, { std::make_pair(6, 7), true}, { std::make_pair(8, 7), true} };
	Vector2f stepSize = { 0, 0 };

	// biomes generation
	void initBiomesGenerationInfo();
	bool isTriggerBlock(int x, int y) const;
	Vector2i biomesChangeCenter = { 0, 0 };
	std::vector<std::vector<Vector2i>> biomesBlocksOffsets = {};
};
