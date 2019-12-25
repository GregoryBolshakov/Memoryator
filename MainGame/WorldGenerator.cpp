#include "WorldGenerator.h"
#include "DynamicObject.h"
#include "EmptyObject.h"
#include "Brazier.h"

WorldGenerator::WorldGenerator()
{
}

void WorldGenerator::init(int width, int height, Vector2i blockSize, Vector2i microBlockSize, GridList* staticGrid, GridList* dynamicGrid, std::unordered_map<std::string, BoardSprite>* spriteMap, float* scaleFactor)
{
	this->width = width;
	this->height = height;
	this->blockSize = blockSize;
	this->microBlockSize = microBlockSize;
	this->staticGrid = staticGrid;
	this->dynamicGrid = dynamicGrid;
	this->spriteMap = spriteMap;
	this->scaleFactor = scaleFactor;
	stepSize = { blockSize.x / 10, blockSize.y / 10 };
}

WorldGenerator::~WorldGenerator()
{
}

void WorldGenerator::initializeStaticItem(Tag itemClass, Vector2f itemPosition, int itemType, std::string itemName, int count, Biomes biome, bool mirrored, std::vector<std::pair<Tag, int>> inventory)
{
	StaticObject* item = ObjectInitializer::initializeStaticItem(itemClass, itemPosition, itemType, itemName, count, biome, spriteMap, mirrored, inventory);

	// locked place check
	const auto terrain = dynamic_cast<TerrainObject*>(item);
	if (terrain)
	{
		std::map<std::pair<int, int>, bool> checkBlocks = {};
		for (int i = (item->getPosition().x - item->getMicroBlockCheckAreaBounds().x) / microBlockSize.x; i < (item->getPosition().x + item->getMicroBlockCheckAreaBounds().x) / microBlockSize.x; i++)
			for (int j = (item->getPosition().y - item->getMicroBlockCheckAreaBounds().y) / microBlockSize.y; j < (item->getPosition().y + item->getMicroBlockCheckAreaBounds().y) / microBlockSize.y; j++)
				if (!(i < 0 || i > width / microBlockSize.x || j < 0 || j > height / microBlockSize.y) && !staticGrid->microBlockMatrix[i][j])
					checkBlocks[{i, j}] = true;

		if (item->isLockedPlace(checkBlocks))
		{
			item->~StaticObject();
			delete item;
			return;
		}
	}
	//-------------------

	staticGrid->addItem(item, item->getName(), int(itemPosition.x), int(itemPosition.y));
}

void WorldGenerator::initializeDynamicItem(Tag itemClass, Vector2f itemPosition, std::string itemName, WorldObject* owner)
{
	DynamicObject* item = ObjectInitializer::initializeDynamicItem(itemClass, itemPosition, itemName, spriteMap, owner);
	if (itemClass == Tag::hero1)	
		focusedObject = item;			

	dynamicGrid->addItem(item, item->getName(), int(itemPosition.x), int(itemPosition.y));
}

void WorldGenerator::generate()
{	
	staticGrid->boundDynamicMatrix(&dynamicGrid->microBlockMatrix);	
	const int blocksCount = ceil(width / blockSize.x) * ceil(height / blockSize.y);

	initializeDynamicItem(Tag::hero1, Vector2f(15000, 15000), "hero1");
	initializeStaticItem(Tag::brazier, Vector2f(16300, 16300), 1, "brazier");
	initializeStaticItem(Tag::stump, Vector2f(15500, 15000), 10, "stump", 1, DarkWoods, false);

	// world generation
	initBiomesGenerationInfo();
	const Vector2f upperLeft(int(focusedObject->getPosition().x - Helper::GetScreenSize().x / 2 / (*scaleFactor) - blockSize.x / (*scaleFactor)), int(focusedObject->getPosition().y - Helper::GetScreenSize().y / 2 / (*scaleFactor) - blockSize.x / (*scaleFactor)));
	const Vector2f bottomRight(int(focusedObject->getPosition().x + Helper::GetScreenSize().x / 2 / (*scaleFactor) + blockSize.x / (*scaleFactor)), int(focusedObject->getPosition().y + Helper::GetScreenSize().y / 2 / (*scaleFactor) + blockSize.x / (*scaleFactor)));
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

	if (biomeMatrix[groundIndX][groundIndY].biomeCell == DarkWoods)
	{
		if (blockTypeProbablilty <= 10) // block with roofs		
			roomedBlocksContent = { {Tag::roof, 10}, {Tag::rock, 2}, {Tag::stump, 2}, {Tag::tree, 7} };
		else
			if (blockTypeProbablilty <= 30) // block with yarrow
				roomedBlocksContent = { {Tag::yarrow, 20}, {Tag::rock, 2}, {Tag::stump, 2}, {Tag::tree, 7} };
			else
				if (blockTypeProbablilty <= 99) // common block				
					roomedBlocksContent = { {Tag::rock, 2}, {Tag::stump, 2}, {Tag::tree, 7} };
		otherBlocksContent = { {Tag::grass, 6} , {Tag::mushroom, 4} };
	}
	else
		if (biomeMatrix[groundIndX][groundIndY].biomeCell == BirchGrove)
		{
			if (blockTypeProbablilty <= 30) // block with chamomile
				roomedBlocksContent = { {Tag::chamomile, 20}, {Tag::rock, 2}, {Tag::stump, 2}, {Tag::log, 2}, {Tag::bush, 5}, {Tag::tree, 7} };
			else
				if (blockTypeProbablilty <= 99) // common block
					roomedBlocksContent = { {Tag::rock, 5}, {Tag::stump, 2}, {Tag::log, 2}, {Tag::bush, 5}, {Tag::tree, 7} };
			otherBlocksContent = { {Tag::grass, 6} , {Tag::mushroom, 3} };
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
						initializeStaticItem(item.first, Vector2f(x, y), -1, "", 1, biomeMatrix[groundIndX][groundIndY].biomeCell);
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
						initializeStaticItem(item.first, Vector2f(x, y), -1, "", 1, biomeMatrix[groundIndX][groundIndY].biomeCell);
						break;
					}
				}
		}
	}	
}

void WorldGenerator::generateGround(int blockIndex)
{
	const int groundIndX = staticGrid->getPointByIndex(blockIndex).x / blockSize.x;
	const int groundIndY = staticGrid->getPointByIndex(blockIndex).y / blockSize.y;

	const int groundType = int(biomeMatrix[groundIndX][groundIndY].biomeCell);
	biomeMatrix[groundIndX][groundIndY].groundCell = new Ground("ground" + std::to_string(blockIndex), Vector2f(groundIndX * blockSize.x, groundIndY * blockSize.y), groundType);
	const auto textureBounds = spriteMap->at("Game/worldSprites/terrainObjects/ground/ground1.png").sprite.getGlobalBounds();
	const auto textureSize = Vector2f(int(textureBounds.width), int(textureBounds.height));
	biomeMatrix[groundIndX][groundIndY].groundCell->setTextureSize(textureSize);
	staticGrid->addItem(biomeMatrix[groundIndX][groundIndY].groundCell, biomeMatrix[groundIndX][groundIndY].groundCell->getName(), biomeMatrix[groundIndX][groundIndY].groundCell->getPosition().x, biomeMatrix[groundIndX][groundIndY].groundCell->getPosition().y);

	const auto connectionPos = biomeMatrix[groundIndX][groundIndY].groundCell->getPosition();
	const auto currentType = biomeMatrix[groundIndX][groundIndY].groundCell->getType();

	if (biomeMatrix[groundIndX][groundIndY].biomeCell == BirchGrove)
		auto test = 1;
	initializeStaticItem(Tag::groundConnection, Vector2f(connectionPos), (currentType - 1) * 4 + 1, "", 1);
	initializeStaticItem(Tag::groundConnection, Vector2f(connectionPos.x + blockSize.x - 1, connectionPos.y), (currentType - 1) * 4 + 2, "", 1);
	initializeStaticItem(Tag::groundConnection, Vector2f(connectionPos), (currentType - 1) * 4 + 3, "", 1);
	initializeStaticItem(Tag::groundConnection, Vector2f(connectionPos.x, connectionPos.y + blockSize.y - 1), (currentType - 1) * 4 + 4, "", 1);
}

bool WorldGenerator::isRoomyStepBlock(int x, int y) const
{
	return (roomyStepBlocks.count(std::make_pair(int((x % blockSize.x) / stepSize.x), int((y % blockSize.y) / stepSize.y))) > 0);
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
			biomeMatrix[biomesChangeCenter.x + offset.x][biomesChangeCenter.y + offset.y].biomeCell = Biomes(biomeId);
	}
}

void WorldGenerator::perimeterGeneration(int offset)
{
	const auto screenSize = Helper::GetScreenSize();
	const auto characterPosition = focusedObject->getPosition();
	const Vector2f worldUpperLeft(int(characterPosition.x - screenSize.x / 2 / (*scaleFactor)), int(characterPosition.y - screenSize.y / 2 / (*scaleFactor)));
	const Vector2f worldBottomRight(int(characterPosition.x + screenSize.x / 2 / (*scaleFactor)), int(characterPosition.y + screenSize.y / 2 / (*scaleFactor)));

	if (focusedObject->getDirection() != STAND)
	{
		for (auto& block : staticGrid->getBlocksAround(worldUpperLeft.x - blockSize.x / (*scaleFactor), worldUpperLeft.y - blockSize.y / (*scaleFactor), worldBottomRight.x + blockSize.x / (*scaleFactor), worldBottomRight.y + blockSize.y / (*scaleFactor), offset))
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

