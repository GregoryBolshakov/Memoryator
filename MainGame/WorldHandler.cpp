#define _USE_MATH_DEFINES

#include "WorldHandler.h"
#include "ObjectInitializer.h"

using namespace sf;

WorldHandler::WorldHandler(int width, int height) : focusedObject(nullptr)
{
	WorldObject::microBlockSize = microblockSize;
	initSpriteMap();
	scaleFactor = getScaleFactor();
	this->width = width;
	this->height = height;
	staticGrid = GridList(this->width, this->height, blockSize, microblockSize);
	dynamicGrid = GridList(this->width, this->height, blockSize, microblockSize);

	initShaders();
	worldGenerator.init(width, height, blockSize, microblockSize, &staticGrid, &dynamicGrid, &spriteMap, &scaleFactor);
	inventorySystem.init();
	buildSystem.Init(inventorySystem.getSpriteList());
}

WorldHandler::~WorldHandler()
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

void WorldHandler::runWorldGenerator()
{
	staticGrid = GridList(this->width, this->height, blockSize, microblockSize);
	dynamicGrid = GridList(this->width, this->height, blockSize, microblockSize);
	for (auto& i : worldGenerator.biomeMatrix)
		for (auto& j : i)
			j.biomeCell = SwampyTrees;
	worldGenerator.generate();
	this->focusedObject = worldGenerator.focusedObject;
	brazier = dynamic_cast<Brazier*>(staticGrid.getItemByName("brazier"));
	brazier->linkWithBuildSystem(&buildSystem);
	worldGenerator.rememberedBlocks = { { staticGrid.getIndexByPoint(brazier->getPosition().x, brazier->getPosition().y), true } };
	cameraPosition = Vector2f(focusedObject->getPosition().x + Helper::GetScreenSize().x * camOffset.x, focusedObject->getPosition().y + Helper::GetScreenSize().y * camOffset.y);

	const auto hero = dynamic_cast<Deerchant*>(focusedObject);
	inventorySystem.inventoryBounding(&(hero->bags));
	buildSystem.inventoryBounding(&(hero->bags));
	buildSystem.succesInit = true;
}

void WorldHandler::initShaders()
{
	// Load shaders
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

void WorldHandler::setScaleFactor(int delta)
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

void WorldHandler::scaleSmoothing()
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

float WorldHandler::getScaleFactor()
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

bool WorldHandler::searchFiles(LPCTSTR lpszFileName, LPSEARCHFUNC lpSearchFunc, bool bInnerFolders)
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

void WorldHandler::initSpriteMap()
{
	//std::ifstream i("Game/worldSprites/hero/hero_absorb.json");
	//json j;
	//i >> j;
	//auto sp = j.get<sprite_pack::pack>();

	int objectsNumber;
	std::string name;

	searchFiles("Game/*.png", putImageToMap, 1);	
	effectSystem.init(&spriteMap);
}

void WorldHandler::initLightSystem(RenderWindow &window)
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
	light1->_emissionSprite.setScale(Vector2f(Helper::GetScreenSize().x * 1.5f / light1->_emissionSprite.getTexture()->getSize().x, Helper::GetScreenSize().y * 1.5f / light1->_emissionSprite.getTexture()->getSize().y));
	light1->_emissionSprite.setColor(commonWorldColorOutfill);
	light1->_sourceRadius = 6;
	light1->_emissionSprite.setPosition(Vector2f(Helper::GetScreenSize().x / 2, Helper::GetScreenSize().y / 2));
	ls.addLight(light1);

	/*brightner = std::make_shared<ltbl::LightPointEmission>();
	brightner->_emissionSprite.setOrigin(Vector2f(pointLightTexture.getSize().x * 0.5f, pointLightTexture.getSize().y * 0.5f));
	brightner->_emissionSprite.setTexture(pointLightTexture);
	brightner->_emissionSprite.setScale(Vector2f(float(Helper::GetScreenSize().x * 0.5f / brightner->_emissionSprite.getTexture()->getSize().x), float(Helper::GetScreenSize().y * 0.5f / brightner->_emissionSprite.getTexture()->getSize().y)));
	brightner->_emissionSprite.setColor(Color(240, 220, 220));
	brightner->_sourceRadius = 6;
	brightner->_emissionSprite.setPosition(Vector2f(Helper::GetScreenSize().x / 2, Helper::GetScreenSize().y / 2));
	ls.addLight(brightner);*/

	ls._ambientColor = commonWorldColor;
	
}

void WorldHandler::renderLightSystem(View view, RenderWindow &window)
{
	ls.render(view, unshadowShader, lightOverShapeShader);
	Lsprite.setTexture(ls.getLightingTexture());
	lightRenderStates.blendMode = sf::BlendMultiply;
	window.draw(Lsprite, lightRenderStates);
}

void WorldHandler::birthObjects()
{
	for (auto& object : localTerrain)
	{
		auto birthStaticStack = object->getBirthObjects().first;
		auto birthDynamicStack = object->getBirthObjects().second;

		while (!birthStaticStack.empty())
		{
			Biomes biome = worldGenerator.biomeMatrix[int(birthStaticStack.top().position.x / blockSize.x)][int(birthStaticStack.top().position.y / blockSize.y)].biomeCell;
			worldGenerator.initializeStaticItem(birthStaticStack.top().tag, birthStaticStack.top().position, birthStaticStack.top().typeOfObject, "", birthStaticStack.top().count, biome, true, birthStaticStack.top().inventory);
			birthStaticStack.pop();
		}
		while (!birthDynamicStack.empty())
		{
			worldGenerator.initializeDynamicItem(birthDynamicStack.top().tag, birthDynamicStack.top().position, "", birthDynamicStack.top().owner);
			birthDynamicStack.pop();
		}
		object->clearBirthStack();
	}
}

void WorldHandler::Load()
{
	staticGrid = GridList(this->width, this->height, blockSize, microblockSize);
	dynamicGrid = GridList(this->width, this->height, blockSize, microblockSize);
	staticGrid.boundDynamicMatrix(&dynamicGrid.microBlockMatrix);

	std::ifstream fin("save.txt");
	std::string saveName;
	float posX, posY;
	int num, typeOfObject;
	fin >> num;
	for (int i = 0; i < num; i++)
	{
		fin >> saveName >> posX >> posY;

		if (saveName == Nightmare1("loadInit", Vector2f(0, 0)).getToSaveName())
			worldGenerator.initializeDynamicItem(Tag::monster, Vector2f(posX, posY), "");
		else
			if (saveName == Deerchant("loadInit", Vector2f(0, 0)).getToSaveName())
				worldGenerator.initializeDynamicItem(Tag::hero1, Vector2f(posX, posY), "");
			else
				if (saveName == Wolf("loadInit", Vector2f(0, 0)).getToSaveName())
					worldGenerator.initializeDynamicItem(Tag::wolf, Vector2f(posX, posY), "");
	}

	fin >> num;
	for (int i = 0; i < num; i++)
	{
		fin >> saveName >> typeOfObject >> posX >> posY;

		if (saveName == GroundConnection("loadInit", Vector2f(0, 0), typeOfObject).getToSaveName())
			worldGenerator.initializeStaticItem(Tag::groundConnection, Vector2f(posX, posY), typeOfObject, "", 1);
		else
			if (saveName == Ground("loadInit", Vector2f(0, 0), typeOfObject).getToSaveName())
				worldGenerator.initializeStaticItem(Tag::ground, Vector2f(posX, posY), typeOfObject, "", 1);
			else
				if (saveName == ForestTree("loadInit", Vector2f(0, 0), typeOfObject).getToSaveName())
					worldGenerator.initializeStaticItem(Tag::tree, Vector2f(posX, posY), typeOfObject, "", 1);
				else
					if (saveName == Grass("loadInit", Vector2f(0, 0), typeOfObject).getToSaveName())
						worldGenerator.initializeStaticItem(Tag::grass, Vector2f(posX, posY), typeOfObject, "", 1);																
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

void WorldHandler::Save()
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

void WorldHandler::clearWorld()
{
	staticGrid.~GridList();
	dynamicGrid.~GridList();
}

void WorldHandler::setTransparent(std::vector<WorldObject*> visibleItems, float elapsedTime)
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

				auto terrain = dynamic_cast<TerrainObject*>(visibleItem);
				if (terrain && pedestalController.readyToStart)				
					mouseDisplayName = "Set pedestal";
				else
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

bool WorldHandler::fixedClimbingBeyond(Vector2f &pos)
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

void WorldHandler::setItemFromBuildSystem()
{
	if (!(buildSystem.instantBuild || focusedObject->getCurrentAction() == builds))
		return;

	if (buildSystem.selectedObject != Tag::emptyCell && buildSystem.buildingPosition != Vector2f(-1, -1))
	{
		if (buildSystem.droppedLootIdList.count(buildSystem.selectedObject) > 0)
			worldGenerator.initializeStaticItem(Tag::droppedLoot, buildSystem.buildingPosition, int(buildSystem.selectedObject), "", 1);
		else
			worldGenerator.initializeStaticItem(buildSystem.selectedObject, buildSystem.buildingPosition, buildSystem.buildType, "", 1);

		if (buildSystem.selectedObject == Tag::totem)
		{
			if (buildSystem.buildType == 2)
				buildSystem.clearHareBags(staticGrid.getIndexByPoint(buildSystem.buildingPosition.x, buildSystem.buildingPosition.y), staticGrid, &localTerrain);
		}
		buildSystem.wasPlaced();
		buildSystem.instantBuild = false;

		buildSystem.buildingPosition = Vector2f(-1, -1);
		buildSystem.buildType = 1;
		brazier->clearCurrentCraft();
	}
}

void WorldHandler::makeCameraShake(float power)
{
	cameraShakeVector = Vector2f(int(rand() % 60 - 30), int(rand() % 60 - 30));
	if (cameraShakeVector == Vector2f(0, 0))
		cameraShakeVector = { 1, 1 };
	cameraShakeDuration = 2e5;
}

void WorldHandler::cameraShakeInteract(float elapsedTime)
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

void WorldHandler::onMouseUp(int currentMouseButton)
{	
	if (mouseDisplayName == "Set pedestal")
	{
		auto terrain = dynamic_cast<TerrainObject*>(selectedObject);
		pedestalController.start(terrain);
	}
	if (pedestalController.isRunning())
		return;
	Vector2i mousePos = Mouse::getPosition();
	Vector2f mouseWorldPos = Vector2f((mousePos.x - Helper::GetScreenSize().x / 2 + cameraPosition.x*scaleFactor) / scaleFactor,
		(mousePos.y - Helper::GetScreenSize().y / 2 + cameraPosition.y*scaleFactor) / scaleFactor);
	
	inventorySystem.onMouseUp();

	if (buildSystem.succesInit /* && inventorySystem.getHeldItem()->first == -1*/)
		buildSystem.onMouseUp(focusedObject->getPosition(), scaleFactor);

	if (mouseDisplayName == "")
		selectedObject = nullptr;

	auto hero = dynamic_cast<Deerchant*>(dynamicGrid.getItemByName(focusedObject->getName()));
	hero->onMouseUp(currentMouseButton, selectedObject, mouseWorldPos, (buildSystem.buildingPosition != Vector2f(-1, -1) && !buildSystem.instantBuild));
	inventorySystem.inventoryBounding(&hero->bags);
}

void WorldHandler::handleEvents(Event& event)
{
	pedestalController.handleEvents(event);
}

void WorldHandler::interact(RenderWindow& window, long long elapsedTime, Event event)
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

	worldGenerator.beyondScreenGeneration();

	std::vector<StaticObject*> localStaticItems = ObjectInitializer::vectorCastToStatic(staticGrid.getItems(worldUpperLeft.x - extra.x, worldUpperLeft.y - extra.y, worldBottomRight.x + extra.x, worldBottomRight.y + extra.y));
	std::vector<DynamicObject*> localDynamicItems = ObjectInitializer::vectorCastToDynamic(dynamicGrid.getItems(worldUpperLeft.x - extra.x, worldUpperLeft.y - extra.y, worldBottomRight.x + extra.x, worldBottomRight.y + extra.y));

	const auto hero = dynamic_cast<Deerchant*>(dynamicGrid.getItemByName(focusedObject->getName()));
	hero->heldItem = &inventorySystem.getHeldItem();


	//making route to the desire position
	timeAfterNewRoute += elapsedTime;
	if (timeAfterNewRoute >= timeForNewRoute)
	{
		timeAfterNewRoute = 0;
		for (auto& dynamicItem : localDynamicItems)
		{
			//staticGrid.setLockedMicroBlocks(dynamicItem, true, true);
			//dynamicItem->initMicroBlocks();
			if (dynamicItem->getRouteGenerationAbility() && dynamicItem->laxMovePosition != Vector2f(-1, -1) && dynamicItem->getCurrentAction() != jerking && dynamicItem->tag != Tag::hero1)
			{
				int permissibleDistance = 0;
				if (dynamicItem->getBoundTarget())
					permissibleDistance = dynamicItem->getBoundTarget()->getPermissibleDistance();
				timeAfterNewRoute = 0;
				staticGrid.makeRoute(dynamicItem->getPosition(), dynamicItem->laxMovePosition,
					dynamicItem->getPosition().x - dynamicItem->sightRange / scaleFactor,
					dynamicItem->getPosition().y - dynamicItem->sightRange / scaleFactor,
					dynamicItem->getPosition().x + dynamicItem->sightRange / scaleFactor,
					dynamicItem->getPosition().y + dynamicItem->sightRange / scaleFactor, permissibleDistance);
				dynamicItem->setRoute(staticGrid.route);
				staticGrid.setLockedMicroBlocks(dynamicItem, false, true);
			}
			else			
				dynamicItem->changeMovePositionToRoute(dynamicItem->laxMovePosition);
			//staticGrid.setLockedMicroBlocks(dynamicItem, false, true);
		}
	}


	for (auto& dynamicItem : localDynamicItems)
	{		
		// passing the beginning  of the route
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
		//------------------------------------

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

		if (dynamicItem->doShake)
		{
			makeCameraShake();
			dynamicItem->doShake = false;
		}

		auto newPosition = dynamicItem->doMove(elapsedTime);

		fixedClimbingBeyond(newPosition);

		newPosition = dynamicItem->doSlip(newPosition, localStaticItems, height, elapsedTime);
		//newPosition = dynamicItem->doSlipOffDynamic(newPosition, localDynamicItems, height, elapsedTime);

		if (!fixedClimbingBeyond(newPosition))
			newPosition = dynamicItem->getPosition();				
		dynamicItem->setPosition(newPosition);
		dynamicGrid.updateItemPosition(dynamicItem->getName(), newPosition.x, newPosition.y);

		dynamicItem->behavior(elapsedTime); // the last because of position changes
	}

	//lightSystemInteract
	//brightner->_emissionSprite.setPosition(Vector2f(focusedObject->getPosition().x - cameraPosition.x + Helper::GetScreenSize().x / 2, focusedObject->getPosition().y - cameraPosition.y + Helper::GetScreenSize().y / 2));

	//if (focusedObject->getCurrentAction() == builds)
		setItemFromBuildSystem();

	//buildSystem.setHeldItem(inventorySystem.getHeldItem()->lootInfo);
	buildSystem.interact();
	inventorySystem.interact(elapsedTime);
	pedestalController.interact(elapsedTime, event);
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

void WorldHandler::draw(RenderWindow& window, long long elapsedTime)
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

	sort(visibleTerrain.begin(), visibleTerrain.end(), cmpImgDraw);
	setTransparent(localTerrain, elapsedTime);
	drawVisibleItems(window, elapsedTime, visibleTerrain);

	//renderLightSystem(view, window);

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
	pedestalController.draw(&window, cameraPosition, scaleFactor);
	/*for (int i = (focusedObject->getPosition().x - 500) / microblockSize.x; i <= (focusedObject->getPosition().x + 500) / microblockSize.x; i++)
		for (int j = (focusedObject->getPosition().y - 500) / microblockSize.y; j <= (focusedObject->getPosition().y + 500) / microblockSize.y; j++)
		{
			if (!staticGrid.microBlockMatrix[i][j])
			{
				rec.setPosition((i * microblockSize.x - cameraPosition.x - microblockSize.x / 2) * scaleFactor + Helper::GetScreenSize().x / 2, (j * microblockSize.y - cameraPosition.y - microblockSize.y / 2) * scaleFactor + Helper::GetScreenSize().y / 2);
				window.draw(rec);
			}
		}*/
	/*for (int i = (dynamicGrid.getItemByName("testEnemy1")->getPosition().x - 500) / microblockSize.x; i <= (dynamicGrid.getItemByName("testEnemy1")->getPosition().x + 500) / microblockSize.x; i++)
		for (int j = (dynamicGrid.getItemByName("testEnemy1")->getPosition().y - 500) / microblockSize.y; j <= (dynamicGrid.getItemByName("testEnemy1")->getPosition().y + 500) / microblockSize.y; j++)
		{
			if (!staticGrid.dynamicMicroBlockMatrix->at(i)[j])
			{
				rec2.setPosition((i * microblockSize.x - cameraPosition.x - microblockSize.x / 2) * scaleFactor + Helper::GetScreenSize().x / 2, (j * microblockSize.y - cameraPosition.y - microblockSize.y / 2) * scaleFactor + Helper::GetScreenSize().y / 2);
				window.draw(rec2);
			}
		}*/
	/*for (auto&cell : dynamic_cast<DynamicObject*>(dynamicGrid.getItemByName("testEnemy1"))->route)
	{		
		Vector2f recPos = Vector2f(cell.first * microblockSize.x, cell.second * microblockSize.y);
		rec2.setPosition((recPos.x - cameraPosition.x - microblockSize.x / 2) * scaleFactor + Helper::GetScreenSize().x / 2, (recPos.y - cameraPosition.y - microblockSize.y / 2) * scaleFactor + Helper::GetScreenSize().y / 2);
		window.draw(rec2);
	}*/
	/*const Vector2i rec3Pos = Vector2i(dynamicGrid.getItemByName("hero1")->getPosition().x / microblockSize.x, dynamicGrid.getItemByName("hero1")->getPosition().y / microblockSize.y);
	rec3.setPosition((rec3Pos.x * microblockSize.x - cameraPosition.x - microblockSize.x / 2) * scaleFactor + Helper::GetScreenSize().x / 2,
		(rec3Pos.y * microblockSize.y - cameraPosition.y - microblockSize.y / 2) * scaleFactor + Helper::GetScreenSize().y / 2);
	window.draw(rec3);*/
}

void WorldHandler::runBuildSystemDrawing(RenderWindow& window, float elapsedTime)
{
	bool showBuildedObj = true;
	if (focusedObject->getBoundTarget() != nullptr)
		showBuildedObj = focusedObject->getBoundTarget()->tag != Tag::buildObject;

	buildSystem.draw(window, elapsedTime, spriteMap, staticGrid, scaleFactor, cameraPosition, localTerrain, showBuildedObj);
}

void WorldHandler::runInventorySystemDrawing(RenderWindow& window, float elapsedTime)
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

void WorldHandler::drawVisibleItems(RenderWindow& window, long long elapsedTime, std::vector<spriteChainElement> sprites)
{
	const auto screenSize = window.getSize();
	const auto screenCenter = Vector2f(screenSize.x / 2, screenSize.y / 2);

	for (auto& spriteChainItem : sprites)
	{
		auto sprite = (&spriteMap[spriteChainItem.path])->sprite;
		const auto worldItemPosition = spriteChainItem.position;

		const auto spriteLeft = float((worldItemPosition.x - cameraPosition.x - spriteChainItem.offset.x) * scaleFactor + screenCenter.x);
		auto spriteTop = float((worldItemPosition.y - cameraPosition.y - spriteChainItem.offset.y) * scaleFactor + screenCenter.y);
		
		//sprite.setOrigin(sprite.getTextureRect().left, sprite.getTextureRect().top + sprite.getTextureRect().height);
		if (spriteChainItem.antiTransparent)
			spriteChainItem.color.a = 255;
		
		sprite.setColor(spriteChainItem.color);
		
		if (!spriteChainItem.isBackground && unscaledObjects.count(spriteChainItem.tag) == 0 && spriteChainItem.tag != Tag::noose)
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

