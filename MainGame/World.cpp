#define _USE_MATH_DEFINES

#include "World.h"
#include "ObjectInitializer.h"

using namespace sf;

World::World(int width, int height) : focusedObject(nullptr)
{
	timeAfterSave = 60000000;
	timeForNewSave = 0;
	blockSize = Vector2i(1000, 1000); //conditionalSizeUnits
	microblockSize = Vector2i(20, 20);
	WorldObject::microBlockSize = microblockSize;
	initSpriteMap();
	scaleFactor = getScaleFactor();

	this->width = width;
	this->height = height;

	initShaders();
	inventorySystem.init();
	buildSystem.Init(inventorySystem.getSpriteList());

	for (int i = 0; i < 100; i++)
		for (int j = 0; j < 100; j++)		
			biomeMatrix[i][j].biomeCell = highGrass;		
}

World::~World()
{
	/*staticGrid.~GridList();
	dynamicGrid.~GridList();
	inventorySystem.~InventoryMaker();
	buildSystem.~BuildSystemMaker();
	focusedObject->~DynamicObject();
	for (auto&item : staticItems)
		item->~StaticObject();
	for (auto&item : visibleItems)
		item->~WorldObject();*/
}

void World::initShaders()
{
	if (!sf::Shader::isAvailable())
	{
		std::cerr << "Shader are not available" << std::endl;
	}

	// Load shaders

	if (!spiritWorldShader.loadFromFile("Game/shaders/water.frag", sf::Shader::Fragment))
	{
		std::cerr << "Error while shaders" << std::endl;
	}

	if (!distortionMap.loadFromFile("Game/shaders/noiseWater.png"))
	{
		sf::err() << "Error while loading distortion map" << std::endl;
	}
	// It is important to set repeated to true to enable scrolling upwards
	distortionMap.setRepeated(true);
	// Setting smooth to true lets us use small maps even on larger images
	distortionMap.setSmooth(true);

	// Set texture to the shader
	spiritWorldShader.setUniform("currentTexture", sf::Shader::CurrentTexture);
	spiritWorldShader.setUniform("distortionMapTexture", distortionMap);

	// Shader parameters
	float level = 0;
}

void World::setScaleFactor(int delta)
{
	if (delta == -1 && scaleFactor >= 0.998 * mainScale)
	{
		scaleFactor -= 0.01;
		scaleDecrease = -0.03;
	}
	else
		if (delta == 1 && scaleFactor <= 1.7 * mainScale)
		{
			scaleFactor += 0.01;
			scaleDecrease = 0.03;
		}

	if (scaleDecrease < 0 && scaleFactor <= 0.998 * mainScale)
		scaleFactor = 0.998 * mainScale;
	if (scaleDecrease > 0 && scaleFactor >= 1.7 * mainScale)
		scaleFactor = 1.7 * mainScale;
}

void World::scaleSmoothing()
{
	if (abs(scaleDecrease) >= 0.02 && timeForScaleDecrease >= 30000)
	{
		if (scaleFactor != 0.998 * mainScale && scaleFactor != 1.7 * mainScale)
			scaleFactor += scaleDecrease;
		if (scaleDecrease < 0 && scaleFactor <= 0.998 * mainScale)
			scaleFactor = 0.998 * mainScale;
		if (scaleDecrease > 0 && scaleFactor >= 1.7 * mainScale)
			scaleFactor = 1.7 * mainScale;

		if (scaleDecrease < 0)
		{
			scaleDecrease += 0.001;
		}
		else
			if (scaleDecrease > 0)
			{
				scaleDecrease -= 0.001;
			}


		timeForScaleDecrease = 0;
	}
}

float World::getScaleFactor()
{
	auto heroHeight = Vector2f(spriteMap[heroTextureName].texture.getSize()).y;
	auto screenHeight = Helper::GetScreenSize().y;
	auto ratio = heroHeight / float(screenHeight);

	auto mainObject = Deerchant("loadInit", Vector2f(0, 0));
	mainObject.calculateTextureOffset();
	mainScale = screenHeight / (5 * mainObject.getConditionalSizeUnits().y);
	mainScale = round(mainScale * 100) / 100;
	return mainScale;
	//return 1;
}

bool cmpImgDraw(spriteChainElement first, spriteChainElement second)
{
	if (first.zCoord == second.zCoord)
	{
		if (first.position.y == second.position.y)
		{
			if (first.position.x == second.position.x)
				return first.size.x * first.size.y < second.size.x * second.size.y;
			return first.position.x < second.position.x;
		}
		return first.position.y < second.position.y;
	}

	return first.zCoord < second.zCoord;
}

bool World::searchFiles(LPCTSTR lpszFileName, LPSEARCHFUNC lpSearchFunc, bool bInnerFolders)
{
	LPTSTR part;
	char tmp[MAX_PATH]; // временный массив
	char name[MAX_PATH];

	HANDLE hSearch = NULL;
	WIN32_FIND_DATA wfd;
	memset(&wfd, 0, sizeof(WIN32_FIND_DATA));

	// сначала поиск внутри вложенных папок ...
	if (bInnerFolders)
	{
		if (GetFullPathName(lpszFileName, MAX_PATH, tmp, &part) == 0) return FALSE;
		strcpy(name, part);
		strcpy(part, "*.*");

		// если папки существуют, делаем поиск
		wfd.dwFileAttributes = FILE_ATTRIBUTE_DIRECTORY;
		if (!((hSearch = FindFirstFile(tmp, &wfd)) == INVALID_HANDLE_VALUE))
			do
			{
				// в каждой папке есть две папки с именами "." и ".."
				// и эти папки мы не трогаем

				// пропускаем папки "." и ".."
				if (!strncmp(wfd.cFileName, ".", 1) || !strncmp(wfd.cFileName, "..", 2))
					continue;

				if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) // если мы нашли папку
				{
					char next[MAX_PATH];
					if (GetFullPathName(lpszFileName, MAX_PATH, next, &part) == 0) return FALSE;
					strcpy(part, wfd.cFileName);
					strcat(next, "\\");
					strcat(next, name);

					searchFiles(next, lpSearchFunc, 1);
				}
			} while (FindNextFile(hSearch, &wfd)); // ищем следующий файл

			FindClose(hSearch); // заканчиваем поиск
	}

	if ((hSearch = FindFirstFile(lpszFileName, &wfd)) == INVALID_HANDLE_VALUE)
		return TRUE; // в противном случае выходим
	do
		if (!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) // если мы нашли файл
		{
			char file[MAX_PATH];
			if (GetFullPathName(lpszFileName, MAX_PATH, file, &part) == 0) return FALSE;
			strcpy(part, wfd.cFileName);

			lpSearchFunc(file, spriteMap);
		}
	while (FindNextFile(hSearch, &wfd)); // ищем следующий файл
	FindClose(hSearch); // заканчиваем поиск

	return TRUE;
}

void putImageToMap(LPCTSTR lpszFileName, std::unordered_map<std::string, BoardSprite> &spriteMap)
{
	//std::string filePath = std::to_string(*lpszFileName);
	std::string filePath = lpszFileName;
	filePath.erase(0, filePath.find("\\Game") + 1);
	std::replace(filePath.begin(), filePath.end(), '\\', '/');
	spriteMap.insert({ filePath, BoardSprite{} });
	auto sprite = &spriteMap[filePath];
	sprite->texture.loadFromFile(filePath);
	sprite->texture.setSmooth(true);
	sprite->sprite.setTexture(sprite->texture);
}

void World::initSpriteMap()
{
	int objectsNumber;
	std::string name;

	searchFiles("Game/*.png", putImageToMap, 1);	
	effectSystem.init(&spriteMap);
}

void World::initLightSystem(RenderWindow &window)
{
	view = window.getDefaultView();

	contextSettings.antialiasingLevel = 8;// ¬ключить сглаживание.

	penumbraTexture.loadFromFile("data/penumbraTexture.png");
	penumbraTexture.setSmooth(true);

	pointLightTexture.loadFromFile("data/pointLightTexture.png");
	pointLightTexture.setSmooth(true);

	directionLightTexture.loadFromFile("data/directionLightTexture.jpg");
	directionLightTexture.setSmooth(true);

	unshadowShader.loadFromFile("data/unshadowShader.vert", "data/unshadowShader.frag");
	lightOverShapeShader.loadFromFile("data/lightOverShapeShader.vert", "data/lightOverShapeShader.frag");
	ls.create(ltbl::rectFromBounds(Vector2f(-1000.0f, -1000.0f), Vector2f(1000.0f, 1000.0f)), window.getSize(), penumbraTexture, unshadowShader, lightOverShapeShader);

	std::shared_ptr<ltbl::LightPointEmission> light1 = std::make_shared<ltbl::LightPointEmission>();
	light1->_emissionSprite.setOrigin(Vector2f(directionLightTexture.getSize().x * 0.5f, directionLightTexture.getSize().y * 0.5f));
	light1->_emissionSprite.setTexture(directionLightTexture);
	light1->_emissionSprite.setScale(Vector2f(Helper::GetScreenSize().x * 1.125f / light1->_emissionSprite.getTexture()->getSize().x, Helper::GetScreenSize().y * 1.125f / light1->_emissionSprite.getTexture()->getSize().y));
	light1->_emissionSprite.setColor(commonWorldColorOutfill);
	light1->_sourceRadius = 12;
	ls.addLight(light1);

	brightner = std::make_shared<ltbl::LightPointEmission>();
	brightner->_emissionSprite.setOrigin(Vector2f(pointLightTexture.getSize().x * 0.5f, pointLightTexture.getSize().y * 0.5f));
	brightner->_emissionSprite.setTexture(pointLightTexture);
	//brightner->_emissionSprite.setScale(spriteMap[heroTextureName].sprite.getGlobalBounds().width / brightner->_emissionSprite.getTexture()->getSize().x, spriteMap[heroTextureName].sprite.getGlobalBounds().height / brightner->_emissionSprite.getTexture()->getSize().y);
	brightner->_emissionSprite.setScale(Vector2f(float(Helper::GetScreenSize().x * 0.5f / brightner->_emissionSprite.getTexture()->getSize().x), float(Helper::GetScreenSize().y * 0.5f / brightner->_emissionSprite.getTexture()->getSize().y)));
	//brightner->_emissionSprite.setScale(Vector2f (Helper::GetScreenSize().x / light1->_emissionSprite.getTexture()->getSize().x, Helper::GetScreenSize().y / light1->_emissionSprite.getTexture()->getSize().y));
	brightner->_emissionSprite.setColor(Color(240, 220, 220));
	brightner->_sourceRadius = 12;
	//ls.addLight(brightner);

	ls._ambientColor = commonWorldColor;
	light1->_emissionSprite.setPosition(Vector2f(Helper::GetScreenSize().x / 2, Helper::GetScreenSize().y / 2));
}

void World::renderLightSystem(View view, RenderWindow &window)
{
	ls.render(view, unshadowShader, lightOverShapeShader);
	Lsprite.setTexture(ls.getLightingTexture());
	lightRenderStates.blendMode = sf::BlendMultiply;
	window.draw(Lsprite, lightRenderStates);
}

void World::initializeStaticItem(Tag itemClass, Vector2f itemPosition, int itemType, std::string itemName, int count, std::vector<std::pair<Tag, int>> inventory)
{
	StaticObject* item = ObjectInitializer::initializeStaticItem(itemClass, itemPosition, itemType, itemName, count, &spriteMap, inventory);

	// locked place check
	std::map<std::pair<int, int>, bool> checkBlocks = {};
	for (int i = (item->getPosition().x - item->getMicroBlockCheckAreaBounds().x) / microblockSize.x; i < (item->getPosition().x + item->getMicroBlockCheckAreaBounds().x) / microblockSize.x; i++)
		for (int j = (item->getPosition().y - item->getMicroBlockCheckAreaBounds().y) / microblockSize.y; j < (item->getPosition().y + item->getMicroBlockCheckAreaBounds().y) / microblockSize.y; j++)
			if (!(i < 0 || i > width / microblockSize.x || j < 0 || j > height / microblockSize.y) && !staticGrid.microBlockMatrix[i][j])
				checkBlocks[{i, j}] = true;
	
	if (item->isLockedPlace(checkBlocks))
	{
		item->~StaticObject();
		delete item;
		return;
	}
	//-------------------

	staticGrid.addItem(item, item->getName(), int(itemPosition.x), int(itemPosition.y));
}

void World::initializeDynamicItem(Tag itemClass, Vector2f itemPosition, std::string itemName, WorldObject* owner)
{
	DynamicObject* item = ObjectInitializer::initializeDynamicItem(itemClass, itemPosition, itemName, &spriteMap, owner);
	if (itemClass == Tag::hero1)
	{
		focusedObject = item;
		cameraPosition = Vector2f(itemPosition.x + Helper::GetScreenSize().x * camOffset.x, itemPosition.y + Helper::GetScreenSize().y * camOffset.y);
	}

	dynamicGrid.addItem(item, item->getName(), int(itemPosition.x), int(itemPosition.y));
}

void World::birthObjects()
{
	for (auto& object : localTerrain)
	{
		auto birthStaticStack = object->getBirthObjects().first;
		auto birthDynamicStack = object->getBirthObjects().second;

		while (!birthStaticStack.empty())
		{
			initializeStaticItem(birthStaticStack.top().tag, birthStaticStack.top().position, birthStaticStack.top().typeOfObject, "", birthStaticStack.top().count, birthStaticStack.top().inventory);
			birthStaticStack.pop();
		}
		while (!birthDynamicStack.empty())
		{
			initializeDynamicItem(birthDynamicStack.top().tag, birthDynamicStack.top().position, "", birthDynamicStack.top().owner);
			birthDynamicStack.pop();
		}
		object->clearBirthStack();
	}
}

void World::Load()
{
	staticGrid = GridList(this->width, this->height, blockSize, microblockSize);
	dynamicGrid = GridList(this->width, this->height, blockSize, microblockSize);

	std::ifstream fin("save.txt");
	std::string saveName;
	float posX, posY;
	int num, typeOfObject;
	fin >> num;
	for (int i = 0; i < num; i++)
	{
		fin >> saveName >> posX >> posY;

		if (saveName == Nightmare1("loadInit", Vector2f(0, 0)).getToSaveName())
			initializeDynamicItem(Tag::monster, Vector2f(posX, posY), "");
		else
			if (saveName == Deerchant("loadInit", Vector2f(0, 0)).getToSaveName())
				initializeDynamicItem(Tag::hero1, Vector2f(posX, posY), "");
			else
				if (saveName == Wolf("loadInit", Vector2f(0, 0)).getToSaveName())
					initializeDynamicItem(Tag::wolf, Vector2f(posX, posY), "");
	}

	fin >> num;
	for (int i = 0; i < num; i++)
	{
		fin >> saveName >> typeOfObject >> posX >> posY;

		if (saveName == GroundConnection("loadInit", Vector2f(0, 0), typeOfObject).getToSaveName())
			initializeStaticItem(Tag::groundConnection, Vector2f(posX, posY), typeOfObject, "", 1);
		else
			if (saveName == Ground("loadInit", Vector2f(0, 0), typeOfObject).getToSaveName())
				initializeStaticItem(Tag::ground, Vector2f(posX, posY), typeOfObject, "", 1);
			else
				if (saveName == ForestTree("loadInit", Vector2f(0, 0), typeOfObject).getToSaveName())
					initializeStaticItem(Tag::tree, Vector2f(posX, posY), typeOfObject, "", 1);
				else
					if (saveName == Grass("loadInit", Vector2f(0, 0), typeOfObject).getToSaveName())
						initializeStaticItem(Tag::grass, Vector2f(posX, posY), typeOfObject, "", 1);
					else
						if (saveName == BonefireOfInsight("loadInit", Vector2f(0, 0), typeOfObject).getToSaveName())
							initializeStaticItem(Tag::bonefireOfInsight, Vector2f(posX, posY), typeOfObject, "", 1);
						else
							if (saveName == HomeCosiness("loadInit", Vector2f(0, 0), typeOfObject).getToSaveName())
								initializeStaticItem(Tag::homeCosiness, Vector2f(posX, posY), typeOfObject, "", 1);
	}

	fin.close();

	//preparations for the inventory system 
	auto hero = dynamic_cast<Deerchant*>(dynamicGrid.getItemByName(focusedObject->getName()));

	inventorySystem.inventoryBounding(&hero->bags);
	//------------------------------------
	buildSystem.inventoryBounding(&hero->bags);
	buildSystem.succesInit = true;
	buildSystem.succesInit = true;

	Save();
}

void World::Save()
{
	if (staticGrid.getSize() == 0)
		return;
	std::ofstream fout("save.txt");
	fout.clear();
	std::vector<StaticObject*> staticItems = ObjectInitializer::vectorCastToStatic(staticGrid.getItems(0, 0, width, height));
	std::vector<DynamicObject*> dynamicItems = ObjectInitializer::vectorCastToDynamic(dynamicGrid.getItems(0, 0, width, height));

	fout << dynamicItems.size() << std::endl;
	for (auto& dynamicItem : dynamicItems)
	{
		fout << dynamicItem->getToSaveName() << " " << dynamicItem->getPosition().x << " " << dynamicItem->getPosition().y << std::endl;
	}
	fout << staticItems.size() << std::endl;
	for (auto& staticItem : staticItems)
	{
		fout << staticItem->getToSaveName() << " " << staticItem->getType() << " " << staticItem->getPosition().x << " " << staticItem->getPosition().y << std::endl;
	}
	fout.close();
}

void World::generate(int objCount)
{
	staticGrid = GridList(this->width, this->height, blockSize, microblockSize);
	dynamicGrid = GridList(this->width, this->height, blockSize, microblockSize);

	const int blocksCount = ceil(width / blockSize.x) * ceil(height / blockSize.y);	

	initializeDynamicItem(Tag::hero1, Vector2f(15000, 15000), "hero1");
	//initializeStaticItem(Tag::tree, Vector2f(15500, 15000), 1, "testStructure");
	initializeStaticItem(Tag::brazier, Vector2f(16300, 16300), 1, "brazier");
	//initializeStaticItem(Tag::wreathTable, Vector2f(18800, 18500), 1, "table");
	//initializeDynamicItem(Tag::nightmare1, Vector2f(15000, 15800), "testEnemy1");
	//initializeDynamicItem(Tag::nightmare1, Vector2f(15400, 15800), "testEnemy2");
	//initializeDynamicItem(Tag::nightmare1, Vector2f(14000, 15800), "testEnemy3");
	Save();

	//preparations for the build system
	brazier = dynamic_cast<Brazier*>(staticGrid.getItemByName("brazier"));
	brazier->linkWithBuildSystem(&buildSystem);
	rememberedBlocks[staticGrid.getIndexByPoint(brazier->getPosition().x, brazier->getPosition().y)] = true;
	//---------------------------------
	//preparations for the inventory system 
	auto hero = dynamic_cast<Deerchant*>(dynamicGrid.getItemByName(focusedObject->getName()));

	inventorySystem.inventoryBounding(&(hero->bags));
	//-------------------------------------
	buildSystem.inventoryBounding(&(hero->bags));
	buildSystem.succesInit = true;

	// world generation
	for (auto i = 0; i < blocksCount; i++)	
		inBlockGenerate(i);
	//---------------
}

void World::inBlockGenerate(int blockIndex)
{
	const int blockType = rand() % 50;
	int genFactor, genFactorRange, fogFactor;

	const int groundIndX = staticGrid.getPointByIndex(blockIndex).x / blockSize.x;
	const int groundIndY = staticGrid.getPointByIndex(blockIndex).y / blockSize.y;

	switch (blockType)
	{
	case 1:
	{
		genFactor = 10;
		genFactorRange = 20;
		fogFactor = 0;
		break;
	}
	case 2:
	{
		genFactor = 10;
		genFactorRange = 20;
		fogFactor = 0;
		break;
	}
	case 3:
	{
		genFactor = 10;
		genFactorRange = 20;
		fogFactor = 0;
		break;
	}
	default:
	{
		genFactor = 20;
		genFactorRange = 10;
		if (biomeMatrix[groundIndX][groundIndY].biomeCell != 1 && biomeMatrix[groundIndX][groundIndY].biomeCell != 4)
			fogFactor = rand() % 2 + 1;
		else
			fogFactor = 0;
		break;
	}
	}	

	IntRect blockTransform = IntRect(staticGrid.getPointByIndex(blockIndex).x, staticGrid.getPointByIndex(blockIndex).y, Vector2f(blockSize).x, Vector2f(blockSize).y);

	const int goundType = (biomeMatrix[groundIndX][groundIndY].biomeCell);
	biomeMatrix[groundIndX][groundIndY].groundCell = new Ground("ground" + std::to_string(blockIndex), Vector2f(groundIndX * blockSize.x, groundIndY * blockSize.y), goundType);
	const auto textureBounds = spriteMap["Game/worldSprites/terrainObjects/ground/ground1.png"].sprite.getGlobalBounds();
	const auto textureSize = Vector2f(int(textureBounds.width), int(textureBounds.height));
	biomeMatrix[groundIndX][groundIndY].groundCell->setTextureSize(textureSize);
	staticGrid.addItem(biomeMatrix[groundIndX][groundIndY].groundCell, biomeMatrix[groundIndX][groundIndY].groundCell->getName(), biomeMatrix[groundIndX][groundIndY].groundCell->getPosition().x, biomeMatrix[groundIndX][groundIndY].groundCell->getPosition().y);

	const auto connectionPos = biomeMatrix[groundIndX][groundIndY].groundCell->getPosition();
	const auto currentType = biomeMatrix[groundIndX][groundIndY].groundCell->getType();	

	initializeStaticItem(Tag::groundConnection, Vector2f(connectionPos), (currentType - 1) * 4 + 1, "", 1);
	initializeStaticItem(Tag::groundConnection, Vector2f(connectionPos.x + blockSize.x - 1, connectionPos.y), (currentType - 1) * 4 + 2, "", 1);
	initializeStaticItem(Tag::groundConnection, Vector2f(connectionPos), (currentType - 1) * 4 + 3, "", 1);
	initializeStaticItem(Tag::groundConnection, Vector2f(connectionPos.x, connectionPos.y + blockSize.y - 1), (currentType - 1) * 4 + 4, "", 1);

	//block filling
	//if (rememberedBlocks.count(blockIndex) > 0)
		//return;	

	int saturation = rand() % 20 + 10;

	for (float i = blockTransform.left; i < blockTransform.left + blockTransform.width; i += 250)
	{
		for (float j = blockTransform.top; j < blockTransform.top + blockTransform.height; j += 250)
		{
			Vector2f randomOffset = Vector2f(rand() % 200 - 100, rand() % 200 - 100);
			auto position = Vector2f(i + randomOffset.x, j + randomOffset.y);
			if (position.x < 0)
				position.x = 0;
			if (position.y < 0)
				position.y = 0;

			int probability = rand() % 100, terrainVariety = rand() % 100;
			genFactor += rand() % genFactorRange;

			if (probability <= genFactor)
			{
				switch (blockType)
				{
				case 1:
				{
					initializeStaticItem(Tag::yarrow, position, -1, "", false);
					break;
				}
				case 2:
				{
					initializeStaticItem(Tag::chamomile, position, -1, "", false);
					break;
				}
				case 3:
				{
					initializeStaticItem(Tag::mugwort, position, -1, "", false);
					break;
				}
				default:
				{
					if (terrainVariety <= 10)
						initializeStaticItem(Tag::tree, position, -1, "", false);
					else
						if (terrainVariety <= 13)
							initializeStaticItem(Tag::rock, position, -1, "", false);
						else
							if (terrainVariety <= 23)
								initializeStaticItem(Tag::grass, position, -1, "", false);
							else
								if (terrainVariety <= 25)
									initializeStaticItem(Tag::stump, position, -1, "", false);
					break;
				}
				}
			}

		}
	}
}

void World::BiomesGenerate(int offset)
{
	//initialize generation border
	Vector2i upperLeftCell = Vector2i((worldUpperLeft.x - blockSize.x * offset) / blockSize.x, (worldUpperLeft.y - blockSize.y * offset) / blockSize.y),
		bottomRightCell = Vector2i((worldBottomRight.x + blockSize.x * offset) / blockSize.x, (worldBottomRight.y + blockSize.y * offset) / blockSize.y);

	Vector2i innerUpperLeftCell = Vector2i((worldUpperLeft.x - blockSize.x) / blockSize.x, (worldUpperLeft.y - blockSize.y) / blockSize.y),
		innerBottomRightCell = Vector2i((worldBottomRight.x + blockSize.x) / blockSize.x, (worldBottomRight.y + blockSize.y) / blockSize.y);

	int inLineCount = bottomRightCell.x - upperLeftCell.x, inRawCount = bottomRightCell.y - upperLeftCell.y;
	Vector2i roughBiomeSize = Vector2i(sqrt(inLineCount), sqrt(inRawCount));

	upperLeftCell.x = std::max(0, upperLeftCell.x);
	upperLeftCell.y = std::max(0, upperLeftCell.y);
	bottomRightCell.x = std::min(int(width / blockSize.x), bottomRightCell.x);
	bottomRightCell.y = std::min(int(height / blockSize.y), bottomRightCell.y);
	//----------------------------

	for (int i = upperLeftCell.x; i <= bottomRightCell.x; i += roughBiomeSize.x)
	{
		for (int j = upperLeftCell.y; j <= bottomRightCell.y; j += roughBiomeSize.y)
		{
			int currentBiome = rand() % 4 + 1;
			for (int x = i; x <= i + roughBiomeSize.x + rand() % 2; x++)
				for (int y = j; y <= j + roughBiomeSize.y + rand() % 2; y++)
				{
					if (x >= innerUpperLeftCell.x && x <= innerBottomRightCell.x && y >= innerUpperLeftCell.y && y <= innerBottomRightCell.y)
						continue;
					biomeMatrix[x][y].biomeCell = Biomes(currentBiome);
				}
		}
	}
}

void World::perimeterGeneration(int offset)
{
	auto screenSize = Helper::GetScreenSize();

	if (focusedObject->getDirection() != STAND)
	{
		for (auto& block : staticGrid.getBlocksAround(worldUpperLeft.x - blockSize.x * scaleFactor, worldUpperLeft.y - blockSize.y * scaleFactor, worldBottomRight.x + blockSize.x * scaleFactor, worldBottomRight.y + blockSize.y * scaleFactor, offset))
		{
			if (canBeRegenerated(block))
			{
				staticGrid.clearCell(block);
				dynamicGrid.clearCell(block);
				inBlockGenerate(block);
			}
		}
	}
}

void World::beyondScreenGeneration()
{
	if (focusedObjectBlock != Vector2i(focusedObject->getPosition().x / blockSize.x, focusedObject->getPosition().y / blockSize.y))
	{
		if (abs(BiomesChangeCenter.x - (focusedObject->getPosition().x / blockSize.x)) >= biomeGenerateDistance ||
			abs(BiomesChangeCenter.y - (focusedObject->getPosition().y / blockSize.y)) >= biomeGenerateDistance)
		{
			BiomesGenerate(4);
			BiomesChangeCenter = Vector2i((focusedObject->getPosition().x / blockSize.x), (focusedObject->getPosition().y / blockSize.y));
		}
		perimeterGeneration(0);
		focusedObjectBlock = Vector2i(focusedObject->getPosition().x / blockSize.x, focusedObject->getPosition().y / blockSize.y);
	}
}

bool World::canBeRegenerated(int blockIndex)
{
	if (rememberedBlocks.count(blockIndex) > 0)
		return false;

	for (auto& item : dynamicGrid.getItems(blockIndex))
	{
		const auto dynamicItem = dynamic_cast<DynamicObject*>(item);
		if (dynamicItem)
			return false;		
	}

	return true;

	/*if (dynamicGrid.getItems(blockIndex).size() != 0)
		return false;

	return true;*/
}

void World::clearWorld()
{
	staticGrid.~GridList();
	dynamicGrid.~GridList();
}

void World::setTransparent(std::vector<WorldObject*> visibleItems, float elapsedTime)
{
	mouseDisplayName = "";
	Vector2f mousePos = Vector2f((Mouse::getPosition().x - Helper::GetScreenSize().x / 2 + cameraPosition.x*scaleFactor) / scaleFactor,
		(Mouse::getPosition().y - Helper::GetScreenSize().y / 2 + cameraPosition.y*scaleFactor) / scaleFactor);

	float minCapacity = 10e8f, minDistance = 10e8f;

	for (auto& visibleItem : visibleItems)
	{
		if (visibleItem->getName() == focusedObject->getName() || visibleItem->intangible)
			continue;

		visibleItem->isTransparent = false;
		Vector2f itemPos = Vector2f(visibleItem->getPosition().x - visibleItem->getTextureOffset().x, visibleItem->getPosition().y - visibleItem->getTextureOffset().y);

		if (mousePos.x >= itemPos.x && mousePos.x <= itemPos.x + visibleItem->getConditionalSizeUnits().x &&
			mousePos.y >= itemPos.y && mousePos.y <= itemPos.y + visibleItem->getConditionalSizeUnits().y)
		{
			visibleItem->isSelected = true;
		}
		else
			visibleItem->isSelected = false;

		if (visibleItem->isTerrain && Helper::isIntersects(mousePos, IntRect(itemPos.x, itemPos.y, visibleItem->getConditionalSizeUnits().x, visibleItem->getConditionalSizeUnits().y)) ||
			Helper::getDist(mousePos, visibleItem->getPosition()) <= visibleItem->getRadius())
		{
			visibleItem->isVisibleName = true;
			float itemCapacity = visibleItem->getConditionalSizeUnits().x + visibleItem->getConditionalSizeUnits().y;
			if (visibleItem->tag == Tag::brazier)
				itemCapacity /= 10;
			float distanceToItemCenter;
			if (visibleItem->isTerrain)
				distanceToItemCenter = abs(mousePos.x - (itemPos.x + visibleItem->getConditionalSizeUnits().x / 2)) +
				abs(mousePos.y - (itemPos.y + visibleItem->getConditionalSizeUnits().y / 2));
			else
				distanceToItemCenter = Helper::getDist(mousePos, visibleItem->getPosition());

			if (itemCapacity < minCapacity || (itemCapacity == minCapacity && distanceToItemCenter <= minDistance))
			{
				minCapacity = itemCapacity;
				minDistance = distanceToItemCenter;

				bool isIntersect = (sqrt(pow(focusedObject->getPosition().x - visibleItem->getPosition().x, 2) + pow(focusedObject->getPosition().y - visibleItem->getPosition().y, 2)) <= (focusedObject->getRadius() + visibleItem->getRadius()));

				switch (visibleItem->tag)
				{
				case Tag::tree:
					{
						mouseDisplayName = "Absorb";
						break;
					}
				case Tag::brazier:
				{					
					if (inventorySystem.getHeldItem().content.first != Tag::emptyCell &&
						Helper::getDist(brazier->getPlatePosition(), mousePos) <= brazier->getPlateRadius())
					{
						if (Helper::getDist(brazier->getPlatePosition(), focusedObject->getPosition()) <= brazier->getPlateRadius() + focusedObject->getRadius())
							mouseDisplayName = "Toss";
						else
							mouseDisplayName = "Come to toss";
					}
					break;
				}
				case Tag::hare:
				case Tag::owl:
					{
						if (inventorySystem.getHeldItem().content.first == Tag::inkyBlackPen)
							mouseDisplayName = "Sketch";
						else
							mouseDisplayName = "Catch up";
						break;
					}
				case Tag::fern:
					{
						if (!visibleItem->inventory.empty())
							mouseDisplayName = "Open";
						else
							mouseDisplayName = "Pick up";
						break;
					}
				case Tag::yarrow:
				case Tag::chamomile:				
				case Tag::mugwort:
				case Tag::noose:
				case Tag::hareTrap:
				case Tag::droppedLoot:
					{
						mouseDisplayName = "Pick up";
						break;
					}
					default:
					{
						mouseDisplayName = visibleItem->getToSaveName();
						break;
					}
				}

				selectedObject = visibleItem;
			}
		}

		if (focusedObject->getPosition().x >= itemPos.x && focusedObject->getPosition().x <= itemPos.x + visibleItem->getConditionalSizeUnits().x && focusedObject->getPosition().y >= itemPos.y && focusedObject->getPosition().y <= visibleItem->getPosition().y && !visibleItem->isBackground)
		{
			visibleItem->isTransparent = true;
			if (visibleItem->color.a > 125)
				visibleItem->color.a -= 1;
		}
		else
		{
			if (visibleItem->color.a < 255)
				visibleItem->color.a += 1;
		}
	}
}

bool World::fixedClimbingBeyond(Vector2f &pos)
{
	const auto screenSize = Helper::GetScreenSize();
	const auto extra = staticGrid.getBlockSize();

	if (pos.x < (screenSize.x / 2 + extra.x) * 1.5)
	{
		pos.x = (screenSize.x / 2 + extra.x) * 1.5;
		return false;
	}
	if (pos.x > width - (screenSize.x / 2 + extra.x) * 1.5)
	{
		pos.x = width - (screenSize.x / 2 + extra.x) * 1.5;
		return false;
	}
	if (pos.y < (screenSize.y / 2 + extra.y) * 1.5)
	{
		pos.y = (screenSize.y / 2 + extra.y) * 1.5;
		return false;
	}
	if (pos.y > height - (screenSize.y / 2 + extra.y) * 1.5)
	{
		pos.y = height - (screenSize.y / 2 + extra.y) * 1.5;
		return false;
	}
	return true;
}

void World::setItemFromBuildSystem()
{
	if (buildSystem.selectedObject != Tag::emptyCell && buildSystem.buildingPosition != Vector2f(-1, -1))
	{
		if (buildSystem.droppedLootIdList.count(buildSystem.selectedObject) > 0)
			initializeStaticItem(Tag::droppedLoot, buildSystem.buildingPosition, int(buildSystem.selectedObject), "", 1);
		else
			initializeStaticItem(buildSystem.selectedObject, buildSystem.buildingPosition, buildSystem.getBuildType(), "", 1);

		if (buildSystem.selectedObject == Tag::totem)
		{
			if (buildSystem.getBuildType() == 2)
				buildSystem.clearHareBags(staticGrid.getIndexByPoint(buildSystem.buildingPosition.x, buildSystem.buildingPosition.y), staticGrid, &localTerrain);
		}
		buildSystem.wasPlaced();

		buildSystem.buildingPosition = Vector2f(-1, -1);
		brazier->clearCurrentCraft();		
	}
}

void World::makeCameraShake(float power)
{
	cameraShakeVector = Vector2f(int(rand() % 60 - 30), int(rand() % 60 - 30));
	if (cameraShakeVector == Vector2f(0, 0))
		cameraShakeVector = { 1, 1 };
	cameraShakeDuration = 2e5;
}

void World::cameraShakeInteract(float elapsedTime)
{
	if (cameraShakeDuration > 0)
	{
		const float k = cameraShakePower * elapsedTime / sqrt(pow(cameraShakeVector.x, 2) + pow(cameraShakeVector.y, 2));
		cameraPosition.x += cameraShakeVector.x * k; cameraPosition.y += cameraShakeVector.y * k;
		cameraShakeDuration -= elapsedTime;
	}
	else
		cameraShakeDuration = 0;
}

void World::onMouseUp(int currentMouseButton)
{	
	Vector2i mousePos = Mouse::getPosition();
	Vector2f mouseWorldPos = Vector2f((mousePos.x - Helper::GetScreenSize().x / 2 + cameraPosition.x*scaleFactor) / scaleFactor,
		(mousePos.y - Helper::GetScreenSize().y / 2 + cameraPosition.y*scaleFactor) / scaleFactor);
	
	inventorySystem.onMouseUp();

	if (buildSystem.succesInit /* && inventorySystem.getHeldItem()->first == -1*/)
		buildSystem.onMouseUp(focusedObject->getPosition(), scaleFactor);

	if (mouseDisplayName == "")
		selectedObject = nullptr;

	auto hero = dynamic_cast<Deerchant*>(dynamicGrid.getItemByName(focusedObject->getName()));
	hero->onMouseUp(currentMouseButton, selectedObject, mouseWorldPos, (buildSystem.buildingPosition != Vector2f(-1, -1)));
	inventorySystem.inventoryBounding(&hero->bags);
	/*Helper::drawText(std::to_string(inventorySystem.canAfford({ std::make_pair(Tag::noose, 1), std::make_pair(Tag::chamomile, 6) })), 30, 200, 200, &window);	
	if (inventorySystem.canAfford({ std::make_pair(Tag::noose, 1), std::make_pair(Tag::chamomile, 4) }))*/
	//auto hero = dynamic_cast<Deerchant*>(focusedObject);
	//HeroBag::putItemsIn({ std::make_pair(Tag::noose, 1), std::make_pair(Tag::chamomile, 4) }, &(hero->bags));
}

void World::interact(RenderWindow& window, long long elapsedTime)
{
	scaleSmoothing();
	birthObjects();
	timeForScaleDecrease += scaleDecreaseClock.getElapsedTime().asMicroseconds();
	scaleDecreaseClock.restart();

	const auto extra = staticGrid.getBlockSize();
	auto screenSize = window.getSize();
	auto characterPosition = focusedObject->getPosition();

	Vector2f worldUpperLeft(int(characterPosition.x - screenSize.x / 2), int(characterPosition.y - screenSize.y / 2));
	Vector2f worldBottomRight(int(characterPosition.x + screenSize.x / 2), int(characterPosition.y + screenSize.y / 2));

	beyondScreenGeneration();

	std::vector<StaticObject*> localStaticItems = ObjectInitializer::vectorCastToStatic(staticGrid.getItems(worldUpperLeft.x - extra.x, worldUpperLeft.y - extra.y, worldBottomRight.x + extra.x, worldBottomRight.y + extra.y));
	std::vector<DynamicObject*> localDynamicItems = ObjectInitializer::vectorCastToDynamic(dynamicGrid.getItems(worldUpperLeft.x - extra.x, worldUpperLeft.y - extra.y, worldBottomRight.x + extra.x, worldBottomRight.y + extra.y));

	const auto hero = dynamic_cast<Deerchant*>(dynamicGrid.getItemByName(focusedObject->getName()));
	hero->heldItem = &inventorySystem.getHeldItem();

	for (auto& dynamicItem : localDynamicItems)
	{		
		//interaction with other objects		
		for (auto& otherDynamicItem : localDynamicItems)
		{
			if (dynamicItem == otherDynamicItem)
				continue;
			dynamicItem->setTarget(*otherDynamicItem);
		}
		for (auto& otherDynamicItem : localDynamicItems)
		{
			if (dynamicItem == otherDynamicItem)
				continue;
			dynamicItem->behaviorWithDynamic(otherDynamicItem, elapsedTime);
		}
		for (auto& otherStaticItem : localStaticItems)
			dynamicItem->behaviorWithStatic(otherStaticItem, elapsedTime);
		//--------		
		
		//making route to the desire position
		//debugInfo = std::to_string(dynamicGrid.getItemByName("testEnemy1")->getMovePosition().x) + " " + std::to_string(dynamicGrid.getItemByName("testEnemy1")->getMovePosition().y);
		//if (dynamicItem->getMovePosition() != Vector2f(-1, -1) && dynamicItem->getRouteGenerationAbility() && dynamicItem->getCurrentAction() != jerking)
		dynamicItem->increaseTimeAfterNewRoute(elapsedTime);
		if (dynamicItem->tag != Tag::hero1 && dynamicItem->laxMovePosition != Vector2f(-1, -1))
		{
			//if (!staticGrid.microBlockMatrix[dynamicItem->getPosition().x / microblockSize.x][dynamicItem->getPosition().y / microblockSize.y])
				//auto test = 1;
			if (dynamicItem->getTimeAfterNewRoute() >= dynamicItem->getTimeForNewRoute())
			{
				dynamicItem->resetTimeAfterNewRoute();
				/*int permissibleDistance = 1;
				if (dynamicItem->tag == Tag::hero1)
					permissibleDistance = (ceil(hero->getBoundTarget()->getMicroBlockCheckAreaBounds().x / microblockSize.x / 2) + ceil(hero->getBoundTarget()->getMicroBlockCheckAreaBounds().y / microblockSize.x / 2));*/
				staticGrid.makeRoute(dynamicItem->getPosition(), dynamicItem->laxMovePosition,
					dynamicItem->getPosition().x - dynamicItem->sightRange / scaleFactor,
					dynamicItem->getPosition().y - dynamicItem->sightRange / scaleFactor,
					dynamicItem->getPosition().x + dynamicItem->sightRange / scaleFactor,
					dynamicItem->getPosition().y + dynamicItem->sightRange / scaleFactor, 0);
				dynamicItem->setRoute(staticGrid.route);
			}

			if (!dynamicItem->route.empty())
			{
				std::pair<int, int> routeMicroBlock = dynamicItem->route[0];
				while (!dynamicItem->route.empty())
				{
					const Vector2f routePos = Vector2f(routeMicroBlock.first * microblockSize.x, routeMicroBlock.second * microblockSize.y);
					if (Helper::getDist(dynamicItem->laxMovePosition, routePos) < Helper::getDist(dynamicItem->laxMovePosition, dynamicItem->getPosition()) && dynamicItem->route.size() > 1)
						break;

					dynamicItem->route.erase(dynamicItem->route.begin());
					routeMicroBlock = dynamicItem->route[0];
				}
				dynamicItem->changeMovePositionToRoute(Vector2f(routeMicroBlock.first * microblockSize.x, routeMicroBlock.second * microblockSize.y));
			}
			else
				dynamicItem->changeMovePositionToRoute(dynamicItem->laxMovePosition);
			//dynamicItem->changeMovePositionToRoute(dynamicItem->laxMovePosition);
		}
		else
			dynamicItem->changeMovePositionToRoute(dynamicItem->laxMovePosition);

		if (dynamicItem->doShake)
		{
			makeCameraShake();
			dynamicItem->doShake = false;
		}

		auto intersects = false;
		auto newPosition = dynamicItem->doMove(elapsedTime);

		fixedClimbingBeyond(newPosition);

		newPosition = dynamicItem->doSlip(newPosition, localStaticItems, height, elapsedTime);
		newPosition = dynamicItem->doSlipOffDynamic(newPosition, localDynamicItems, height, elapsedTime);

		if (!fixedClimbingBeyond(newPosition))
			newPosition = dynamicItem->getPosition();				

		if (!intersects)
			dynamicItem->setPosition(newPosition);
		else
			newPosition = dynamicItem->getPosition();
		dynamicGrid.updateItemPosition(dynamicItem->getName(), newPosition.x, newPosition.y);

		dynamicItem->behavior(elapsedTime); // the last because of position changes
	}

	//lightSystemInteract
	brightner->_emissionSprite.setPosition(Vector2f(focusedObject->getPosition().x - cameraPosition.x + Helper::GetScreenSize().x / 2, focusedObject->getPosition().y - cameraPosition.y + Helper::GetScreenSize().y / 2));

	if (focusedObject->getCurrentAction() == builds)
		setItemFromBuildSystem();
	//buildSystem.setHeldItem(inventorySystem.getHeldItem()->lootInfo);
	buildSystem.interact();
	inventorySystem.interact(elapsedTime);
	//-------------------

	//deleting items
	for (auto& item : localStaticItems)	
		if (item->getDeletePromise())		
			staticGrid.deleteItem(item->getName());
	for (auto& item : localDynamicItems)
		if (item->getDeletePromise())
			dynamicGrid.deleteItem(item->getName());
	//--------------

	//saving world
	timeForNewSave += elapsedTime;
	if (timeForNewSave >= timeAfterSave)
	{
		timeForNewSave = 0;
		Save();
	}
}

void World::draw(RenderWindow& window, long long elapsedTime)
{
	inventorySystem.wasDrawing = false;

	//shaders logic
	spiritWorldShader.setUniform("time", timer.getElapsedTime().asSeconds() / 5);
	spiritWorldShader.setUniform("level", 0);

	const auto extra = staticGrid.getBlockSize();

	const auto screenSize = window.getSize();
	const auto screenCenter = Vector2f(screenSize.x / 2, screenSize.y / 2);
	cameraPosition.x += (focusedObject->getPosition().x + Helper::GetScreenSize().x * camOffset.x - cameraPosition.x) * (focusedObject->getSpeed() * elapsedTime) / maxCameraDistance.x;
	cameraPosition.y += (focusedObject->getPosition().y + Helper::GetScreenSize().y * camOffset.y - cameraPosition.y) * (focusedObject->getSpeed() * elapsedTime) / maxCameraDistance.y;
	cameraShakeInteract(elapsedTime);

	worldUpperLeft = Vector2f(int(cameraPosition.x - (screenCenter.x + extra.x) / scaleFactor), int(cameraPosition.y - (screenCenter.y + extra.y) / scaleFactor));
	worldBottomRight = Vector2f(int(cameraPosition.x + (screenCenter.x + extra.x) / scaleFactor), int(cameraPosition.y + (screenCenter.y + extra.y) / scaleFactor));

	if (worldUpperLeft.x < 0)
		worldUpperLeft.x = 0;
	if (worldUpperLeft.y < 0)
		worldUpperLeft.y = 0;
	if (worldBottomRight.x > width)
		worldBottomRight.x = width;
	if (worldBottomRight.y > height)
		worldBottomRight.y = height;

	auto localStaticItems = staticGrid.getItems(worldUpperLeft.x, worldUpperLeft.y, worldBottomRight.x, worldBottomRight.y);
	auto localDynamicItems = dynamicGrid.getItems(worldUpperLeft.x, worldUpperLeft.y, worldBottomRight.x, worldBottomRight.y);

	visibleBackground.clear();
	visibleTerrain.clear();
    localTerrain.clear();

	for (auto& item : localStaticItems)
	{
        item->prepareSpriteNames(elapsedTime, scaleFactor);
        for (auto& sprite : item->additionalSprites)
        {
			if (sprite.position == Vector2f(0, 0))
				sprite.position = item->getPosition();
            sprite.zCoord = item->getZCoords();
            sprite.isBackground = item->isBackground;
            if (sprite.scaleRatio == Vector2f(0, 0))
                sprite.scaleRatio = item->getScaleRatio();
			sprite.tag = item->tag;
			sprite.mirrored = item->getMirroredState();
			sprite.color = item->color;
        }
		if (item->isBackground)
			visibleBackground.insert(visibleBackground.end(), item->additionalSprites.begin(), item->additionalSprites.end());
		else
        {
			localTerrain.push_back(item);
            visibleTerrain.insert(visibleTerrain.end(), item->additionalSprites.begin(), item->additionalSprites.end());
        }
	}
    for (auto& item : localDynamicItems)
    {
        item->prepareSpriteNames(elapsedTime, scaleFactor);
        for (auto& sprite : item->additionalSprites)
        {
			if (sprite.position == Vector2f(0, 0))
				sprite.position = item->getPosition();
            sprite.zCoord = item->getZCoords();
            sprite.isBackground = item->isBackground;
            if (sprite.scaleRatio == Vector2f(0, 0))
                sprite.scaleRatio = item->getScaleRatio();
			sprite.tag = item->tag;
			sprite.color = item->color;
        }
        visibleTerrain.insert(visibleTerrain.end(), item->additionalSprites.begin(), item->additionalSprites.end());
        localTerrain.push_back(item);
    }
	sort(visibleBackground.begin(), visibleBackground.end(), cmpImgDraw);
	drawVisibleItems(window, elapsedTime, visibleBackground);

	renderLightSystem(view, window);	

	sort(visibleTerrain.begin(), visibleTerrain.end(), cmpImgDraw);
	setTransparent(localTerrain, elapsedTime);
	drawVisibleItems(window, elapsedTime, visibleTerrain);

	RectangleShape rec, rec2, rec3;
	rec.setSize(Vector2f(microblockSize));
	rec.setFillColor(Color(240, 128, 128, 200));
	rec.setSize(Vector2f((microblockSize.x - 1) * scaleFactor, (microblockSize.y - 1) * scaleFactor));
	rec2.setSize(Vector2f(microblockSize));
	rec2.setFillColor(Color(65, 105, 225, 200));
	rec2.setSize(Vector2f((microblockSize.x - 1) * scaleFactor, (microblockSize.y - 1) * scaleFactor));
	rec3.setSize(Vector2f(microblockSize));
	rec3.setFillColor(Color::Green);
	rec3.setSize(Vector2f((microblockSize.x - 1) * scaleFactor, (microblockSize.y - 1) * scaleFactor));

	for (int i = (focusedObject->getPosition().x - 500) / microblockSize.x; i <= (focusedObject->getPosition().x + 500) / microblockSize.x; i++)
		for (int j = (focusedObject->getPosition().y - 500) / microblockSize.y; j <= (focusedObject->getPosition().y + 500) / microblockSize.y; j++)
		{
			if (!staticGrid.microBlockMatrix[i][j])
			{
				rec.setPosition((i * microblockSize.x - cameraPosition.x - microblockSize.x / 2) * scaleFactor + Helper::GetScreenSize().x / 2, (j * microblockSize.y - cameraPosition.y - microblockSize.y / 2) * scaleFactor + Helper::GetScreenSize().y / 2);
				window.draw(rec);
			}
		}
	/*for (auto&cell : dynamic_cast<DynamicObject*>(dynamicGrid.getItemByName("testEnemy1"))->route)
	{		
		Vector2f recPos = Vector2f(cell.first * microblockSize.x, cell.second * microblockSize.y);
		rec2.setPosition((recPos.x - cameraPosition.x - microblockSize.x / 2) * scaleFactor + Helper::GetScreenSize().x / 2, (recPos.y - cameraPosition.y - microblockSize.y / 2) * scaleFactor + Helper::GetScreenSize().y / 2);
		window.draw(rec2);
	}
	const Vector2i rec3Pos = Vector2i(dynamicGrid.getItemByName("hero1")->getPosition().x / microblockSize.x, dynamicGrid.getItemByName("hero1")->getPosition().y / microblockSize.y);
	rec3.setPosition((rec3Pos.x * microblockSize.x - cameraPosition.x - microblockSize.x / 2) * scaleFactor + Helper::GetScreenSize().x / 2,
		(rec3Pos.y * microblockSize.y - cameraPosition.y - microblockSize.y / 2) * scaleFactor + Helper::GetScreenSize().y / 2);
	window.draw(rec3);*/
}

void World::runBuildSystemDrawing(RenderWindow& window, float elapsedTime)
{
	bool showBuildedObj = true;
	if (focusedObject->getBoundTarget() != nullptr)
		showBuildedObj = focusedObject->getBoundTarget()->tag != Tag::buildObject;

	buildSystem.draw(window, elapsedTime, spriteMap, staticGrid, scaleFactor, cameraPosition, localTerrain, showBuildedObj);
}

void World::runInventorySystemDrawing(RenderWindow& window, float elapsedTime)
{
	const auto screenSize = window.getSize();
	const auto screenCenter = Vector2f(screenSize.x / 2, screenSize.y / 2);

	const Vector2i mousePos = Mouse::getPosition();
	const Vector2f mouseWorldPos = Vector2f((mousePos.x - Helper::GetScreenSize().x / 2 + cameraPosition.x*scaleFactor) / scaleFactor,
		(mousePos.y - Helper::GetScreenSize().y / 2 + cameraPosition.y*scaleFactor) / scaleFactor);

	inventorySystem.drawHeroInventory(elapsedTime, window);

	inventorySystem.resetPickedCell();
	for (auto& object : localTerrain)
		if (object->isVisibleInventory)		
			inventorySystem.drawInventory(&(object->inventory), screenCenter, window);		

	if (mouseDisplayName != "")
		Helper::drawText(mouseDisplayName, 30, Mouse::getPosition().x, Mouse::getPosition().y, &window);
}

void World::drawVisibleItems(RenderWindow& window, long long elapsedTime, std::vector<spriteChainElement> sprites)
{
	const auto screenSize = window.getSize();
	const auto screenCenter = Vector2f(screenSize.x / 2, screenSize.y / 2);

	for (auto& spriteChainItem : sprites)
	{
		if (spriteChainItem.tag == Tag::fog)
			int a = 12345;
		auto sprite = (&spriteMap[spriteChainItem.path])->sprite;
		const auto worldItemPosition = spriteChainItem.position;

		const auto spriteLeft = float((worldItemPosition.x - cameraPosition.x - spriteChainItem.offset.x) * scaleFactor + screenCenter.x);
		auto spriteTop = float((worldItemPosition.y - cameraPosition.y - spriteChainItem.offset.y) * scaleFactor + screenCenter.y);
		
		//sprite.setOrigin(sprite.getTextureRect().left, sprite.getTextureRect().top + sprite.getTextureRect().height);
		if (spriteChainItem.antiTransparent)
			spriteChainItem.color.a = 255;
		
		sprite.setColor(spriteChainItem.color);
		
		if (!spriteChainItem.isBackground && unscaledObjects[spriteChainItem.tag] && spriteChainItem.tag != Tag::noose)
		{
			sprite.setScale(spriteChainItem.size.x / sprite.getGlobalBounds().width * scaleFactor, spriteChainItem.size.y / sprite.getGlobalBounds().height * scaleFactor * pow(scaleFactor, double(1) / 6));
			spriteTop -= (pow(scaleFactor, double(1) / 6) - 1) * sprite.getGlobalBounds().height;
		}
		else
			sprite.setScale(spriteChainItem.size.x / sprite.getGlobalBounds().width * scaleFactor, spriteChainItem.size.y / sprite.getGlobalBounds().height * scaleFactor);
		sprite.setRotation(spriteChainItem.rotation);
		sprite.setPosition(Vector2f(spriteLeft, spriteTop));

		if (spriteChainItem.mirrored)
			sprite.setTextureRect(IntRect(sprite.getTextureRect().left + sprite.getTextureRect().width, sprite.getTextureRect().top, -sprite.getTextureRect().width, sprite.getTextureRect().height));		

		window.draw(sprite);
	}

}

