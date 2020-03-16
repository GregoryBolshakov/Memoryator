#pragma once

#include "object_initializer.h"
#include "grid_list.h"
#include "scale_system.h"

class world_generator
{
public:	
	void generate();
	void init(int width, int height, Vector2f blockSize, Vector2f microBlockSize, grid_list* staticGrid, grid_list* dynamicGrid, scale_system* scale_system, std::map<pack_tag, sprite_pack>* packsMap);

	void initializeStaticItem(
		entity_tag itemClass,
		Vector2f itemPosition,
		int itemType,
		const std::string& itemName,
		int count = 1,
		biomes biome = dark_woods,
		bool mirrored = true,
		const std::vector<std::pair<entity_tag, int>>& inventory = {}) const;
	
	void initializeDynamicItem(entity_tag itemClass, Vector2f itemPosition, const std::string& itemName, world_object* owner = nullptr);

	//active generation
	void inBlockGenerate(int blockIndex);
	bool canBeRegenerated(int blockIndex) const;
	void biomesGenerate();
	void perimeterGeneration();
	void beyondScreenGeneration();

	world_generator();
	~world_generator();

	biomes biomeMatrix[100][100]{};
	std::map<int, bool> rememberedBlocks = { {0, true} };
	dynamic_object* focusedObject = nullptr;
	
private:
	grid_list* staticGrid = nullptr;
	grid_list* dynamicGrid = nullptr;	
	std::map<pack_tag, sprite_pack>* packsMap{};
	int width = 0;
	int height = 0;
	Vector2f blockSize = { 0, 0 };
	Vector2f microBlockSize = { 0, 0 };
	Vector2i focusedObjectBlock = { 0, 0 };

	scale_system* scale_system_ ;

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
