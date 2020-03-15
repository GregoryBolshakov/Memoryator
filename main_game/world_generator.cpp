#include "world_generator.h"

#include "dynamic_object.h"
#include "empty_object.h"

world_generator::world_generator()
= default;

void world_generator::initMainScale()
{
	auto mainObject = deerchant("loadInit", Vector2f(0, 0));
	mainObject.calculate_texture_offset();
	mainScale = helper::GetScreenSize().y / (5 * mainObject.get_conditional_size_units().y);
	mainScale = round(mainScale * 100) / 100;
}

void world_generator::init(
	const int width,
	const int height,
	const Vector2f blockSize,
	const Vector2f microBlockSize,
	grid_list* staticGrid,
	grid_list* dynamicGrid,
	std::map<pack_tag, sprite_pack>* packsMap)
{
	this->width = width;
	this->height = height;
	this->blockSize = blockSize;
	this->microBlockSize = microBlockSize;
	this->staticGrid = staticGrid;
	this->dynamicGrid = dynamicGrid;
	this->packsMap = packsMap;
	stepSize = { blockSize.x / 10, blockSize.y / 10 };
}

world_generator::~world_generator()
= default;

void world_generator::initializeStaticItem(
	entity_tag itemClass,
	Vector2f itemPosition,
	int itemType,
	const std::string& itemName,
	int count,
	biomes biome,
	bool mirrored,
	const std::vector<std::pair<entity_tag, int>>& inventory) const
{
	auto item = object_initializer::initialize_static_item(itemClass, itemPosition, itemType, itemName, count, biome, packsMap, mirrored, inventory);
	
	if (item == nullptr)
		return;

	// locked place check
	const auto terrain = dynamic_cast<terrain_object*>(item);
	if (terrain)
	{
		std::map<std::pair<int, int>, bool> checkBlocks = {};

		const auto end_i = int((item->get_position().x + item->get_micro_block_check_area_bounds().x) / microBlockSize.x);
		const auto end_j = int((item->get_position().y + item->get_micro_block_check_area_bounds().y) / microBlockSize.y);

		const auto width_to_micro_x = width / int(microBlockSize.x);
		const auto height_to_micro_y = height / int(microBlockSize.y);

		auto i = int((item->get_position().x - item->get_micro_block_check_area_bounds().x) / microBlockSize.x);
		while (i <= end_i)
		{
			auto j = int((item->get_position().y - item->get_micro_block_check_area_bounds().y) / microBlockSize.y);
			while (j <= end_j)
			{
				if (!(i < 0 || i >width_to_micro_x || j < 0 || j >height_to_micro_y) && !staticGrid->micro_block_matrix[int(round(i))][int(round(j))])
					checkBlocks[{i, j}] = true;
				j++;
			}
			i++;
		}

		if (item->is_locked_place(checkBlocks))
		{
			delete item;
			return;
		}
	}
	
	staticGrid->add_item(item, item->get_name(), itemPosition.x, itemPosition.y);
}

void world_generator::initializeDynamicItem(const entity_tag itemClass, const Vector2f itemPosition, const std::string& itemName, world_object* owner)
{
	const auto item = object_initializer::initialize_dynamic_item(itemClass, itemPosition, itemName, packsMap, owner);
	
	if (item == nullptr)
		return;

	if (itemClass == entity_tag::hero)	
		focusedObject = item;			

	dynamicGrid->add_item(item, item->get_name(), itemPosition.x, itemPosition.y);
}

void world_generator::generate()
{	
	initializeDynamicItem(entity_tag::hero, Vector2f(15800, 16300), "hero");
	initializeDynamicItem(entity_tag::hare, Vector2f(15000, 16350), "hare");
	initializeStaticItem(entity_tag::brazier, Vector2f(16300, 15800), 1, "brazier");

	// world generation
	initBiomesGenerationInfo();
	const Vector2f upperLeft(
		floor(focusedObject->get_position().x - (helper::GetScreenSize().x / 2.0f + blockSize.x) / (FARTHEST_SCALE * mainScale)),
		floor(focusedObject->get_position().y - (helper::GetScreenSize().y / 2.0f + blockSize.x) / (FARTHEST_SCALE * mainScale)));
	const Vector2f bottomRight(
		floor(focusedObject->get_position().x + (helper::GetScreenSize().x / 2.0f + blockSize.y) / (FARTHEST_SCALE * mainScale)),
		floor(focusedObject->get_position().y + (helper::GetScreenSize().y / 2.0f + blockSize.y) / (FARTHEST_SCALE * mainScale)));

	for (auto& block : staticGrid->get_blocks_in_sight(upperLeft.x, upperLeft.y, bottomRight.x, bottomRight.y))	
		inBlockGenerate(block);	
}

bool cmpByChance(const std::pair<entity_tag, int> a, const std::pair<entity_tag, int> b)
{
	return a.second < b.second;
}

void world_generator::inBlockGenerate(const int blockIndex)
{
	const auto blockTypeProbability = rand() % 100;
	const auto groundIndX = int(ceil(staticGrid->get_point_by_index(blockIndex).x / blockSize.x));
	const auto groundIndY = int(ceil(staticGrid->get_point_by_index(blockIndex).y / blockSize.y));

	std::vector<std::pair<entity_tag, int>> roomedBlocksContent = {};
	std::vector<std::pair<entity_tag, int>> otherBlocksContent = {};

	if (biomeMatrix[groundIndX][groundIndY] == dark_woods)
	{
		if (blockTypeProbability <= 10) // block with roofs		
			roomedBlocksContent = { {entity_tag::roof, 10}, {entity_tag::rock, 2}, {entity_tag::stump, 2}, {entity_tag::tree, 7} };
		else
			if (blockTypeProbability <= 30) // block with yarrow
				roomedBlocksContent = { {entity_tag::yarrow, 0}, {entity_tag::rock, 2}, {entity_tag::stump, 2}, {entity_tag::tree, 7} };
			else
				if (blockTypeProbability <= 99) // common block				
					roomedBlocksContent = { {entity_tag::rock, 2}, {entity_tag::stump, 2}, {entity_tag::tree, 7} };
		otherBlocksContent = { {entity_tag::grass, 6} , {entity_tag::mushroom, 4} };
	}
	else
		if (biomeMatrix[groundIndX][groundIndY] == birch_grove)
		{
			if (blockTypeProbability <= 30) // block with chamomile
				roomedBlocksContent = { {entity_tag::chamomile, 0}, {entity_tag::rock, 2}, {entity_tag::stump, 2}, {entity_tag::log, 2}, {entity_tag::bush, 5}, {entity_tag::tree, 7} };
			else
				if (blockTypeProbability <= 99) // common block
					roomedBlocksContent = { {entity_tag::rock, 5}, {entity_tag::stump, 2}, {entity_tag::log, 2}, {entity_tag::bush, 5}, {entity_tag::tree, 7} };
			otherBlocksContent = { {entity_tag::grass, 6} , {entity_tag::mushroom, 3} };
		}
		else
			if (biomeMatrix[groundIndX][groundIndY] == swampy_trees)
			{
				if (blockTypeProbability <= 30) // block with chamomile
					roomedBlocksContent = { {entity_tag::rock, 2}, {entity_tag::lake, 2}, {entity_tag::stump, 2}, {entity_tag::root, 2}, {entity_tag::bush, 5}, {entity_tag::tree, 7} };
				else
					if (blockTypeProbability <= 99) // common block
						roomedBlocksContent = { {entity_tag::rock, 2}, {entity_tag::lake, 2}, {entity_tag::stump, 2}, {entity_tag::root, 2}, {entity_tag::bush, 5}, {entity_tag::tree, 7} };
				otherBlocksContent = { {entity_tag::grass, 6} , {entity_tag::mushroom, 2} };
			}

	std::sort(roomedBlocksContent.begin(), roomedBlocksContent.end(), cmpByChance);

	const auto blockTransform = IntRect(
		int(staticGrid->get_point_by_index(blockIndex).x),
		int(staticGrid->get_point_by_index(blockIndex).y),
		int(Vector2f(blockSize).x),
		int(Vector2f(blockSize).y));
	
	generateGround(blockIndex);

	//block filling
	return;

	for (auto x = blockTransform.left; x < blockTransform.left + blockTransform.width; x += 100)
	{
		for (auto y = blockTransform.top; y < blockTransform.top + blockTransform.height; y += 100)
		{
			if (isRoomyStepBlock(x, y))
			{
				for (auto& item : roomedBlocksContent)
				{
					const auto placedObjectDeterminant = rand() % 100;
					if (item.second > placedObjectDeterminant)
					{
						initializeStaticItem(item.first, Vector2f(float(x), float(y)), -1, "", 1, biomeMatrix[groundIndX][groundIndY]);
						break;
					}
				}
			}
			else
				for (auto& item : otherBlocksContent)
				{
					const auto placedObjectDeterminant = rand() % 100;
					if (item.second > placedObjectDeterminant)
					{
						initializeStaticItem(item.first, Vector2f(float(x), float(y)), -1, "", 1, biomeMatrix[groundIndX][groundIndY]);
						break;
					}
				}
		}
	}	
}

void world_generator::generateGround(const int blockIndex)
{
	const auto position = staticGrid->get_point_by_index(blockIndex);
	const auto groundIndX = int(ceil(position.x / blockSize.x));
	const auto groundIndY = int(ceil(position.y / blockSize.y));
	const auto biome = biomeMatrix[groundIndX][groundIndY];
	
	initializeStaticItem(entity_tag::ground, position, int(biome), "", 1, biome);
	initializeStaticItem(entity_tag::groundConnection, Vector2f(position), (biome - 1) * 4 + 1, "", 1);
	initializeStaticItem(entity_tag::groundConnection, Vector2f(position.x, position.y + blockSize.y - 1), (biome - 1) * 4 + 2, "", 1);
	initializeStaticItem(entity_tag::groundConnection, Vector2f(position), (biome - 1) * 4 + 3, "", 1);
	initializeStaticItem(entity_tag::groundConnection, Vector2f(position.x + blockSize.x - 1, position.y), (biome - 1) * 4 + 4, "", 1);
}

bool world_generator::isRoomyStepBlock(const int x, const int y) const
{
	return roomyStepBlocks.count(std::make_pair(x % int(blockSize.x) / int(stepSize.x), y % int(blockSize.y) / int(stepSize.y))) > 0;
}

bool world_generator::isTriggerBlock(const int x, const int y) const
{
	return abs(x - biomesChangeCenter.x) >= 4 || abs(y - biomesChangeCenter.y) >= 4 || abs(x - biomesChangeCenter.x) + abs(y - biomesChangeCenter.y) >= 5;
}

void world_generator::initBiomesGenerationInfo()
{
	biomesBlocksOffsets.resize(8);
	for (auto x = -5; x <= 5; x++)
		for (auto y = -5; y <= 5; y++)		
			if (abs(x) + abs(y) >= 6)
			{
				if (x < 0 && y < 0)
					biomesBlocksOffsets[0].emplace_back(x, y);
				if (x > 0 && y < 0)
					biomesBlocksOffsets[2].emplace_back(x, y);
				if (x > 0 && y > 0)
					biomesBlocksOffsets[4].emplace_back(x, y);
				if (x < 0 && y > 0)
					biomesBlocksOffsets[6].emplace_back(x, y);
			}
			else
				if (abs(x) >= 3 || abs(y) >= 3)
				{
					if (y < 0 && abs(x) < abs(y))
						biomesBlocksOffsets[1].emplace_back(x, y);
					if (x > 0 && abs(x) > abs(y))
						biomesBlocksOffsets[3].emplace_back(x, y);
					if (y > 0 && abs(x) < abs(y))
						biomesBlocksOffsets[5].emplace_back(x, y);
					if (x < 0 && abs(x) > abs(y))
						biomesBlocksOffsets[7].emplace_back(x, y);
				}
	biomesChangeCenter = Vector2i(
		int(focusedObject->get_position().x / blockSize.x),
		int(focusedObject->get_position().y / blockSize.y));
	focusedObjectBlock = biomesChangeCenter;
	biomesGenerate();
}

void world_generator::biomesGenerate()
{
	biomesChangeCenter = focusedObjectBlock;

	for (auto& biome : biomesBlocksOffsets)
	{
		const auto biomeId = rand() % 3 + 1;
		for (const auto offset : biome)
			biomeMatrix[biomesChangeCenter.x + offset.x][biomesChangeCenter.y + offset.y] = biomes(biomeId);
	}
}

void world_generator::perimeterGeneration()
{
	const auto screenSize = helper::GetScreenSize();
	const auto characterPosition = focusedObject->get_position();
	const Vector2f worldUpperLeft(
		ceil(characterPosition.x - (screenSize.x / 2 + blockSize.x) / (FARTHEST_SCALE * mainScale)),
		ceil(characterPosition.y - (screenSize.y / 2 + blockSize.y) / (FARTHEST_SCALE * mainScale)));
	const Vector2f worldBottomRight(
		ceil(characterPosition.x + (screenSize.x / 2 + blockSize.x) / (FARTHEST_SCALE * mainScale)),
		ceil(characterPosition.y + (screenSize.y / 2 + blockSize.y) / (FARTHEST_SCALE * mainScale)));

	if (focusedObject->direction_system.direction != direction::STAND)
	{
		for (auto& block : staticGrid->get_blocks_around(worldUpperLeft.x, worldUpperLeft.y, worldBottomRight.x, worldBottomRight.y))
		{
			if (canBeRegenerated(block))
			{
				staticGrid->clear_cell(block);
				dynamicGrid->clear_cell(block);
				inBlockGenerate(block);
			}
		}
	}
}

void world_generator::beyondScreenGeneration()
{
	const auto block = Vector2i(
		int(focusedObject->get_position().x / blockSize.x),
		int(focusedObject->get_position().y / blockSize.y));
	
	if (focusedObjectBlock != block)
	{		
		perimeterGeneration();
		focusedObjectBlock = block;
	}
	if (isTriggerBlock(focusedObjectBlock.x, focusedObjectBlock.y))
		biomesGenerate();
}

bool world_generator::canBeRegenerated(const int blockIndex) const
{
	if (rememberedBlocks.count(blockIndex) > 0)
		return false;

	for (auto& item : dynamicGrid->get_items(blockIndex))
	{
		const auto dynamicItem = dynamic_cast<dynamic_object*>(item);
		if (dynamicItem)
			return false;
	}

	return true;

	/*if (dynamicGrid->getItems(blockIndex).size() != 0)
		return false;

	return true;*/
}

