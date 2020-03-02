#include "WorldGenerator.h"

#include "DynamicObject.h"
#include "EmptyObject.h"

WorldGenerator::WorldGenerator()
= default;

void WorldGenerator::initMainScale()
{
	auto mainObject = deerchant("loadInit", Vector2f(0, 0));
	mainObject.calculateTextureOffset();
	mainScale = Helper::GetScreenSize().y / (5 * mainObject.getConditionalSizeUnits().y);
	mainScale = round(mainScale * 100) / 100;
}

void WorldGenerator::init(
	const int width,
	const int height,
	const Vector2f blockSize,
	const Vector2f microBlockSize,
	GridList* staticGrid,
	GridList* dynamicGrid,
	std::map<PackTag, sprite_pack>* packsMap)
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

WorldGenerator::~WorldGenerator()
= default;

void WorldGenerator::initializeStaticItem(
	Tag itemClass,
	Vector2f itemPosition,
	int itemType,
	const std::string& itemName,
	int count,
	Biomes biome,
	bool mirrored,
	const std::vector<std::pair<Tag, int>>& inventory) const
{
	auto item = ObjectInitializer::initializeStaticItem(itemClass, itemPosition, itemType, itemName, count, biome, packsMap, mirrored, inventory);
	
	if (item == nullptr)
		return;

	// locked place check
	const auto terrain = dynamic_cast<TerrainObject*>(item);
	if (terrain)
	{
		std::map<std::pair<int, int>, bool> checkBlocks = {};

		const auto end_i = int((item->getPosition().x + item->getMicroBlockCheckAreaBounds().x) / microBlockSize.x);
		const auto end_j = int((item->getPosition().y + item->getMicroBlockCheckAreaBounds().y) / microBlockSize.y);

		const auto width_to_micro_x = width / int(microBlockSize.x);
		const auto height_to_micro_y = height / int(microBlockSize.y);

		auto i = int((item->getPosition().x - item->getMicroBlockCheckAreaBounds().x) / microBlockSize.x);
		while (i <= end_i)
		{
			auto j = int((item->getPosition().y - item->getMicroBlockCheckAreaBounds().y) / microBlockSize.y);
			while (j <= end_j)
			{
				if (!(i < 0 || i >width_to_micro_x || j < 0 || j >height_to_micro_y) && !staticGrid->microBlockMatrix[int(round(i))][int(round(j))])
					checkBlocks[{i, j}] = true;
				j++;
			}
			i++;
		}

		if (item->isLockedPlace(checkBlocks))
		{
			delete item;
			return;
		}
	}
	
	staticGrid->addItem(item, item->getName(), itemPosition.x, itemPosition.y);
}

void WorldGenerator::initializeDynamicItem(const Tag itemClass, const Vector2f itemPosition, const std::string& itemName, WorldObject* owner)
{
	const auto item = ObjectInitializer::initializeDynamicItem(itemClass, itemPosition, itemName, packsMap, owner);
	
	if (item == nullptr)
		return;

	if (itemClass == Tag::hero)	
		focusedObject = item;			

	dynamicGrid->addItem(item, item->getName(), itemPosition.x, itemPosition.y);
}

void WorldGenerator::generate()
{	
	staticGrid->boundDynamicMatrix(&dynamicGrid->microBlockMatrix);	

	initializeDynamicItem(Tag::hero, Vector2f(15800, 15800), "hero");	
	initializeStaticItem(Tag::brazier, Vector2f(16300, 16300), 1, "brazier");
	//initializeDynamicItem(Tag::nightmare1, Vector2f(15500, 15500), "enemy");

	// world generation
	initBiomesGenerationInfo();
	const Vector2f upperLeft(
		floor(focusedObject->getPosition().x - (Helper::GetScreenSize().x / 2.0f + blockSize.x) / (FARTHEST_SCALE * mainScale)),
		floor(focusedObject->getPosition().y - (Helper::GetScreenSize().y / 2.0f + blockSize.x) / (FARTHEST_SCALE * mainScale)));
	const Vector2f bottomRight(
		floor(focusedObject->getPosition().x + (Helper::GetScreenSize().x / 2.0f + blockSize.y) / (FARTHEST_SCALE * mainScale)),
		floor(focusedObject->getPosition().y + (Helper::GetScreenSize().y / 2.0f + blockSize.y) / (FARTHEST_SCALE * mainScale)));

	for (auto& block : staticGrid->getBlocksInSight(upperLeft.x, upperLeft.y, bottomRight.x, bottomRight.y))	
		inBlockGenerate(block);	
}

bool cmpByChance(const std::pair<Tag, int> a, const std::pair<Tag, int> b)
{
	return a.second < b.second;
}

void WorldGenerator::inBlockGenerate(const int blockIndex)
{
	const auto blockTypeProbability = rand() % 100;
	const auto groundIndX = int(ceil(staticGrid->getPointByIndex(blockIndex).x / blockSize.x));
	const auto groundIndY = int(ceil(staticGrid->getPointByIndex(blockIndex).y / blockSize.y));

	std::vector<std::pair<Tag, int>> roomedBlocksContent = {};
	std::vector<std::pair<Tag, int>> otherBlocksContent = {};

	if (biomeMatrix[groundIndX][groundIndY] == DarkWoods)
	{
		if (blockTypeProbability <= 10) // block with roofs		
			roomedBlocksContent = { {Tag::roof, 10}, {Tag::rock, 2}, {Tag::stump, 2}, {Tag::tree, 7} };
		else
			if (blockTypeProbability <= 30) // block with yarrow
				roomedBlocksContent = { {Tag::yarrow, 0}, {Tag::rock, 2}, {Tag::stump, 2}, {Tag::tree, 7} };
			else
				if (blockTypeProbability <= 99) // common block				
					roomedBlocksContent = { {Tag::rock, 2}, {Tag::stump, 2}, {Tag::tree, 7} };
		otherBlocksContent = { {Tag::grass, 6} , {Tag::mushroom, 4} };
	}
	else
		if (biomeMatrix[groundIndX][groundIndY] == BirchGrove)
		{
			if (blockTypeProbability <= 30) // block with chamomile
				roomedBlocksContent = { {Tag::chamomile, 0}, {Tag::rock, 2}, {Tag::stump, 2}, {Tag::log, 2}, {Tag::bush, 5}, {Tag::tree, 7} };
			else
				if (blockTypeProbability <= 99) // common block
					roomedBlocksContent = { {Tag::rock, 5}, {Tag::stump, 2}, {Tag::log, 2}, {Tag::bush, 5}, {Tag::tree, 7} };
			otherBlocksContent = { {Tag::grass, 6} , {Tag::mushroom, 3} };
		}
		else
			if (biomeMatrix[groundIndX][groundIndY] == SwampyTrees)
			{
				if (blockTypeProbability <= 30) // block with chamomile
					roomedBlocksContent = { {Tag::rock, 2}, {Tag::lake, 2}, {Tag::stump, 2}, {Tag::root, 2}, {Tag::bush, 5}, {Tag::tree, 7} };
				else
					if (blockTypeProbability <= 99) // common block
						roomedBlocksContent = { {Tag::rock, 2}, {Tag::lake, 2}, {Tag::stump, 2}, {Tag::root, 2}, {Tag::bush, 5}, {Tag::tree, 7} };
				otherBlocksContent = { {Tag::grass, 6} , {Tag::mushroom, 2} };
			}

	std::sort(roomedBlocksContent.begin(), roomedBlocksContent.end(), cmpByChance);

	const auto blockTransform = IntRect(
		int(staticGrid->getPointByIndex(blockIndex).x),
		int(staticGrid->getPointByIndex(blockIndex).y),
		int(Vector2f(blockSize).x),
		int(Vector2f(blockSize).y));
	
	generateGround(blockIndex);

	//block filling
	//return;

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

void WorldGenerator::generateGround(const int blockIndex)
{
	const auto position = staticGrid->getPointByIndex(blockIndex);
	const auto groundIndX = int(ceil(position.x / blockSize.x));
	const auto groundIndY = int(ceil(position.y / blockSize.y));
	const auto biome = biomeMatrix[groundIndX][groundIndY];
	
	initializeStaticItem(Tag::ground, position, int(biome), "", 1, biome);
	initializeStaticItem(Tag::groundConnection, Vector2f(position), (biome - 1) * 4 + 1, "", 1);
	initializeStaticItem(Tag::groundConnection, Vector2f(position.x, position.y + blockSize.y - 1), (biome - 1) * 4 + 2, "", 1);
	initializeStaticItem(Tag::groundConnection, Vector2f(position), (biome - 1) * 4 + 3, "", 1);
	initializeStaticItem(Tag::groundConnection, Vector2f(position.x + blockSize.x - 1, position.y), (biome - 1) * 4 + 4, "", 1);
}

bool WorldGenerator::isRoomyStepBlock(const int x, const int y) const
{
	return roomyStepBlocks.count(std::make_pair(x % int(blockSize.x) / int(stepSize.x), y % int(blockSize.y) / int(stepSize.y))) > 0;
}

bool WorldGenerator::isTriggerBlock(const int x, const int y) const
{
	return abs(x - biomesChangeCenter.x) >= 4 || abs(y - biomesChangeCenter.y) >= 4 || abs(x - biomesChangeCenter.x) + abs(y - biomesChangeCenter.y) >= 5;
}

void WorldGenerator::initBiomesGenerationInfo()
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
		int(focusedObject->getPosition().x / blockSize.x),
		int(focusedObject->getPosition().y / blockSize.y));
	focusedObjectBlock = biomesChangeCenter;
	biomesGenerate();
}

void WorldGenerator::biomesGenerate()
{
	biomesChangeCenter = focusedObjectBlock;

	for (auto& biome : biomesBlocksOffsets)
	{
		const auto biomeId = rand() % 3 + 1;
		for (const auto offset : biome)
			biomeMatrix[biomesChangeCenter.x + offset.x][biomesChangeCenter.y + offset.y] = Biomes(biomeId);
	}
}

void WorldGenerator::perimeterGeneration()
{
	const auto screenSize = Helper::GetScreenSize();
	const auto characterPosition = focusedObject->getPosition();
	const Vector2f worldUpperLeft(
		ceil(characterPosition.x - (screenSize.x / 2 + blockSize.x) / (FARTHEST_SCALE * mainScale)),
		ceil(characterPosition.y - (screenSize.y / 2 + blockSize.y) / (FARTHEST_SCALE * mainScale)));
	const Vector2f worldBottomRight(
		ceil(characterPosition.x + (screenSize.x / 2 + blockSize.x) / (FARTHEST_SCALE * mainScale)),
		ceil(characterPosition.y + (screenSize.y / 2 + blockSize.y) / (FARTHEST_SCALE * mainScale)));

	if (focusedObject->getDirectionSystem().direction != Direction::STAND)
	{
		for (auto& block : staticGrid->getBlocksAround(worldUpperLeft.x, worldUpperLeft.y, worldBottomRight.x, worldBottomRight.y))
		{
			if (canBeRegenerated(block))
			{
				staticGrid->clearCell(block);
				dynamicGrid->clearCell(block);
				inBlockGenerate(block);
			}
		}
	}
}

void WorldGenerator::beyondScreenGeneration()
{
	const auto block = Vector2i(
		int(focusedObject->getPosition().x / blockSize.x),
		int(focusedObject->getPosition().y / blockSize.y));
	
	if (focusedObjectBlock != block)
	{		
		perimeterGeneration();
		focusedObjectBlock = block;
	}
	if (isTriggerBlock(focusedObjectBlock.x, focusedObjectBlock.y))
		biomesGenerate();
}

bool WorldGenerator::canBeRegenerated(const int blockIndex) const
{
	if (rememberedBlocks.count(blockIndex) > 0)
		return false;

	for (auto& item : dynamicGrid->getItems(blockIndex))
	{
		const auto dynamicItem = dynamic_cast<DynamicObject*>(item);
		if (dynamicItem)
			return false;
	}

	return true;

	/*if (dynamicGrid->getItems(blockIndex).size() != 0)
		return false;

	return true;*/
}

