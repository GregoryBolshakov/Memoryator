#include "WorldGenerator.h"
#include "DynamicObject.h"
#include "EmptyObject.h"
#include "Brazier.h"

WorldGenerator::WorldGenerator()
{
}

void WorldGenerator::initMainScale()
{
	auto mainObject = Deerchant("loadInit", Vector2f(0, 0));
	mainObject.calculateTextureOffset();
	mainScale = Helper::GetScreenSize().y / (5 * mainObject.getConditionalSizeUnits().y);
	mainScale = round(mainScale * 100) / 100;
}

void WorldGenerator::init(int width, int height, Vector2f blockSize, Vector2f microBlockSize, GridList* staticGrid, GridList* dynamicGrid, std::map<PackTag, SpritePack>* packsMap)
{
	this->width = width;
	this->height = height;
	this->blockSize = blockSize;
	this->microBlockSize = microBlockSize;
	this->staticGrid = staticGrid;
	this->dynamicGrid = dynamicGrid;
	this->packsMap = packsMap;
	this->scaleFactor = scaleFactor;
	stepSize = { blockSize.x / 10, blockSize.y / 10 };
}

WorldGenerator::~WorldGenerator()
{
}

void WorldGenerator::initializeStaticItem(Tag itemClass, Vector2f itemPosition, int itemType, std::string itemName, int count, Biomes biome, bool mirrored, std::vector<std::pair<Tag, int>> inventory)
{
	StaticObject* item = ObjectInitializer::initializeStaticItem(itemClass, itemPosition, itemType, itemName, count, biome, packsMap, mirrored, inventory);
	
	if (item == nullptr)
		return;

	// locked place check
	const auto terrain = dynamic_cast<TerrainObject*>(item);
	if (terrain)
	{
		std::map<std::pair<int, int>, bool> checkBlocks = {};
		for (int i = (item->getPosition().x - item->getMicroBlockCheckAreaBounds().x) / microBlockSize.x; i <= (item->getPosition().x + item->getMicroBlockCheckAreaBounds().x) / microBlockSize.x; i++)
			for (int j = (item->getPosition().y - item->getMicroBlockCheckAreaBounds().y) / microBlockSize.y; j <= (item->getPosition().y + item->getMicroBlockCheckAreaBounds().y) / microBlockSize.y; j++)
				if (!(i < 0 || i > width / microBlockSize.x || j < 0 || j > height / microBlockSize.y) && !staticGrid->microBlockMatrix[i][j])
					checkBlocks[{i, j}] = true;

		if (item->isLockedPlace(checkBlocks))
		{
			delete item;
			return;
		}
	}
	//-------------------
	if (int(item->tag) == 0)
		auto test = 123;
	staticGrid->addItem(item, item->getName(), int(itemPosition.x), int(itemPosition.y));
}

void WorldGenerator::initializeDynamicItem(Tag itemClass, Vector2f itemPosition, std::string itemName, WorldObject* owner)
{
	DynamicObject* item = ObjectInitializer::initializeDynamicItem(itemClass, itemPosition, itemName, packsMap, owner);
	
	if (item == nullptr)
		return;

	if (itemClass == Tag::hero)	
		focusedObject = item;			

	dynamicGrid->addItem(item, item->getName(), int(itemPosition.x), int(itemPosition.y));
}

void WorldGenerator::generate()
{	
	staticGrid->boundDynamicMatrix(&dynamicGrid->microBlockMatrix);	
	const int blocksCount = ceil(width / blockSize.x) * ceil(height / blockSize.y);

	initializeDynamicItem(Tag::hero, Vector2f(15800, 15800), "hero");	
	initializeStaticItem(Tag::brazier, Vector2f(16300, 16300), 1, "brazier");
	//initializeDynamicItem(Tag::nightmare1, Vector2f(15500, 15500), "enemy");

	// world generation
	initBiomesGenerationInfo();
	const Vector2f upperLeft(int(focusedObject->getPosition().x - (Helper::GetScreenSize().x / 2 + blockSize.x) / (FARTHEST_SCALE * mainScale)), int(focusedObject->getPosition().y - (Helper::GetScreenSize().y / 2 + blockSize.x) / (FARTHEST_SCALE * mainScale)));
	const Vector2f bottomRight(int(focusedObject->getPosition().x + (Helper::GetScreenSize().x / 2 + blockSize.y) / (FARTHEST_SCALE * mainScale)), int(focusedObject->getPosition().y + (Helper::GetScreenSize().y / 2 + blockSize.y) / (FARTHEST_SCALE * mainScale)));

	for (auto& block : staticGrid->getBlocksInSight(upperLeft.x, upperLeft.y, bottomRight.x, bottomRight.y))	
		inBlockGenerate(block);	
	//---------------
}

bool cmpByChance(std::pair<Tag, int> a, std::pair<Tag, int> b)
{
	return a.second < b.second;
}

void WorldGenerator::inBlockGenerate(int blockIndex)
{
	const int blockTypeProbablilty = rand() % 100;
	const int groundIndX = staticGrid->getPointByIndex(blockIndex).x / blockSize.x;
	const int groundIndY = staticGrid->getPointByIndex(blockIndex).y / blockSize.y;
	std::vector<std::pair<Tag, int>> roomedBlocksContent = {}, otherBlocksContent = {};

	if (biomeMatrix[groundIndX][groundIndY] == DarkWoods)
	{
		if (blockTypeProbablilty <= 10) // block with roofs		
			roomedBlocksContent = { {Tag::roof, 10}, {Tag::rock, 2}, {Tag::stump, 2}, {Tag::tree, 7} };
		else
			if (blockTypeProbablilty <= 30) // block with yarrow
				roomedBlocksContent = { {Tag::yarrow, 0}, {Tag::rock, 2}, {Tag::stump, 2}, {Tag::tree, 7} };
			else
				if (blockTypeProbablilty <= 99) // common block				
					roomedBlocksContent = { {Tag::rock, 2}, {Tag::stump, 2}, {Tag::tree, 7} };
		otherBlocksContent = { {Tag::grass, 6} , {Tag::mushroom, 4} };
	}
	else
		if (biomeMatrix[groundIndX][groundIndY] == BirchGrove)
		{
			if (blockTypeProbablilty <= 30) // block with chamomile
				roomedBlocksContent = { {Tag::chamomile, 0}, {Tag::rock, 2}, {Tag::stump, 2}, {Tag::log, 2}, {Tag::bush, 5}, {Tag::tree, 7} };
			else
				if (blockTypeProbablilty <= 99) // common block
					roomedBlocksContent = { {Tag::rock, 5}, {Tag::stump, 2}, {Tag::log, 2}, {Tag::bush, 5}, {Tag::tree, 7} };
			otherBlocksContent = { {Tag::grass, 6} , {Tag::mushroom, 3} };
		}
		else
			if (biomeMatrix[groundIndX][groundIndY] == SwampyTrees)
			{
				if (blockTypeProbablilty <= 30) // block with chamomile
					roomedBlocksContent = { {Tag::rock, 2}, {Tag::lake, 2}, {Tag::stump, 2}, {Tag::root, 2}, {Tag::bush, 5}, {Tag::tree, 7} };
				else
					if (blockTypeProbablilty <= 99) // common block
						roomedBlocksContent = { {Tag::rock, 2}, {Tag::lake, 2}, {Tag::stump, 2}, {Tag::root, 2}, {Tag::bush, 5}, {Tag::tree, 7} };
				otherBlocksContent = { {Tag::grass, 6} , {Tag::mushroom, 2} };
			}

	std::sort(roomedBlocksContent.begin(), roomedBlocksContent.end(), cmpByChance);
	
	IntRect blockTransform = IntRect(staticGrid->getPointByIndex(blockIndex).x, staticGrid->getPointByIndex(blockIndex).y, Vector2f(blockSize).x, Vector2f(blockSize).y);
	generateGround(blockIndex);

	//block filling
	//return;

	for (int x = blockTransform.left; x < blockTransform.left + blockTransform.width; x += 100)
	{
		for (int y = blockTransform.top; y < blockTransform.top + blockTransform.height; y += 100)
		{
			if (isRoomyStepBlock(x, y))
			{
				for (auto& item : roomedBlocksContent)
				{
					const int placedObjectDeterminant = rand() % 100;
					if (item.second > placedObjectDeterminant)
					{
						initializeStaticItem(item.first, Vector2f(x, y), -1, "", 1, biomeMatrix[groundIndX][groundIndY]);
						break;
					}
				}
			}
			else
				for (auto& item : otherBlocksContent)
				{
					const int placedObjectDeterminant = rand() % 100;
					if (item.second > placedObjectDeterminant)
					{
						initializeStaticItem(item.first, Vector2f(x, y), -1, "", 1, biomeMatrix[groundIndX][groundIndY]);
						break;
					}
				}
		}
	}	
}

void WorldGenerator::generateGround(int blockIndex)
{
	const auto position = staticGrid->getPointByIndex(blockIndex);
	const int groundIndX = position.x / blockSize.x;
	const int groundIndY = position.y / blockSize.y;
	const auto biome = biomeMatrix[groundIndX][groundIndY];
	
	initializeStaticItem(Tag::ground, position, int(biome), "", 1, biome);
	initializeStaticItem(Tag::groundConnection, Vector2f(position), (biome - 1) * 4 + 1, "", 1);
	initializeStaticItem(Tag::groundConnection, Vector2f(position.x, position.y + blockSize.y - 1), (biome - 1) * 4 + 2, "", 1);
	initializeStaticItem(Tag::groundConnection, Vector2f(position), (biome - 1) * 4 + 3, "", 1);
	initializeStaticItem(Tag::groundConnection, Vector2f(position.x + blockSize.x - 1, position.y), (biome - 1) * 4 + 4, "", 1);
}

bool WorldGenerator::isRoomyStepBlock(int x, int y) const
{
	return (roomyStepBlocks.count(std::make_pair((x % int(blockSize.x) / stepSize.x), (y % int(blockSize.y) / stepSize.y))) > 0);
}

bool WorldGenerator::isTriggerBlock(int x, int y)
{
	return abs(x - biomesChangeCenter.x) >= 4 || abs(y - biomesChangeCenter.y) >= 4 || abs(x - biomesChangeCenter.x) + abs(y - biomesChangeCenter.y) >= 5;
}

void WorldGenerator::initBiomesGenerationInfo()
{
	biomesBlocksOffsets.resize(8);
	for (int x = -5; x <= 5; x++)
		for (int y = -5; y <= 5; y++)		
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
	biomesChangeCenter = Vector2i(focusedObject->getPosition().x / blockSize.x, focusedObject->getPosition().y / blockSize.y);
	focusedObjectBlock = biomesChangeCenter;
	biomesGenerate();
}

void WorldGenerator::biomesGenerate()
{
	biomesChangeCenter = focusedObjectBlock;

	for (auto& biome : biomesBlocksOffsets)
	{
		const int biomeId = rand() % 3 + 1;
		for (auto offset : biome)
			biomeMatrix[biomesChangeCenter.x + offset.x][biomesChangeCenter.y + offset.y] = Biomes(biomeId);
	}
}

void WorldGenerator::perimeterGeneration(int offset)
{
	const auto screenSize = Helper::GetScreenSize();
	const auto characterPosition = focusedObject->getPosition();
	const Vector2f worldUpperLeft(int(characterPosition.x - (screenSize.x / 2 + blockSize.x) / (FARTHEST_SCALE * mainScale)), int(characterPosition.y - (screenSize.y / 2 + blockSize.y) / (FARTHEST_SCALE * mainScale)));
	const Vector2f worldBottomRight(int(characterPosition.x + (screenSize.x / 2 + blockSize.x) / (FARTHEST_SCALE * mainScale)), int(characterPosition.y + (screenSize.y / 2 + blockSize.y) / (FARTHEST_SCALE * mainScale)));

	if (focusedObject->getDirection() != Direction::STAND)
	{
		for (auto& block : staticGrid->getBlocksAround(worldUpperLeft.x, worldUpperLeft.y, worldBottomRight.x, worldBottomRight.y, offset))
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
	if (focusedObjectBlock != Vector2i(focusedObject->getPosition().x / blockSize.x, focusedObject->getPosition().y / blockSize.y))
	{		
		perimeterGeneration(0);
		focusedObjectBlock = Vector2i(focusedObject->getPosition().x / blockSize.x, focusedObject->getPosition().y / blockSize.y);
	}
	if (isTriggerBlock(focusedObjectBlock.x, focusedObjectBlock.y))
		biomesGenerate();
}

bool WorldGenerator::canBeRegenerated(int blockIndex)
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

