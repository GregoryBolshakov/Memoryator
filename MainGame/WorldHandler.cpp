#define _USE_MATH_DEFINES

#include "WorldHandler.h"

#include <fstream>

#include "ObjectInitializer.h"

using namespace sf;

WorldHandler::WorldHandler(const int width, const int height, std::map<PackTag, sprite_pack>* packsMap)
{
    this->packsMap = packsMap;
	WorldObject::microBlockSize = microBlockSize;
    worldGenerator.initMainScale();
	worldGenerator.scaleFactor = worldGenerator.mainScale;
	this->width = width;
	this->height = height;
	staticGrid = GridList(this->width, this->height, blockSize, microBlockSize);
	dynamicGrid = GridList(this->width, this->height, blockSize, microBlockSize);

	// initShaders();
	worldGenerator.init(width, height, blockSize, microBlockSize, &staticGrid, &dynamicGrid, packsMap);
	buildSystem.init();
	inventorySystem.init();
	timeSystem.init(1, 0);
	lightSystem.init({0, 0, Helper::GetScreenSize().x, Helper::GetScreenSize().y});
}

WorldHandler::~WorldHandler()
{
	/*staticGrid.~GridList();
	dynamicGrid.~GridList();
	inventorySystem.~InventorySystem();
	buildSystem.~BuildSystem();
	focusedObject->~DynamicObject();
	for (auto&item : staticItems)
		item->~StaticObject();
	for (auto&item : visibleItems)
		item->~WorldObject();*/
}

void WorldHandler::runWorldGenerator()
{
	staticGrid = GridList(this->width, this->height, blockSize, microBlockSize);
	dynamicGrid = GridList(this->width, this->height, blockSize, microBlockSize);
	for (auto& i : worldGenerator.biomeMatrix)
		for (auto& j : i)
			j = SwampyTrees;
	worldGenerator.generate();
	this->focusedObject = worldGenerator.focusedObject;
	brazier = dynamic_cast<Brazier*>(staticGrid.getItemByName("brazier"));
	//brazier->linkWithBuildSystem(&buildSystem);
	worldGenerator.rememberedBlocks = { { staticGrid.getIndexByPoint(brazier->getPosition().x, brazier->getPosition().y), true } };
	cameraSystem.position = Vector2f(focusedObject->getPosition().x + Helper::GetScreenSize().x * camera_system::camOffset.x, focusedObject->getPosition().y + Helper::GetScreenSize().y * camera_system::camOffset.y);

	const auto hero = dynamic_cast<deerchant*>(focusedObject);
	inventorySystem.inventoryBounding(&hero->bags);
}

//void WorldHandler::initShaders()
//{
//	// Load shaders
//	if (!distortionMap.loadFromFile("Game/shaders/noiseWater.png"))
//	{
//		sf::err() << "Error while loading distortion map" << std::endl;
//	}
//	// It is important to set repeated to true to enable scrolling upwards
//	distortionMap.setRepeated(true);
//	// Setting smooth to true lets us use small maps even on larger images
//	distortionMap.setSmooth(true);
//
//	// Set texture to the shader
//	spiritWorldShader.setUniform("currentTexture", sf::Shader::CurrentTexture);
//	spiritWorldShader.setUniform("distortionMapTexture", distortionMap);
//
//	// Shader parameters
//	float level = 0;
//}

void WorldHandler::setScaleFactor(const int delta)
{
	if (delta == -1 && worldGenerator.scaleFactor > worldGenerator.FARTHEST_SCALE * worldGenerator.mainScale)
	{
		worldGenerator.scaleFactor -= 0.01f;
		scaleDecrease = -0.03f;
	}
	else
		if (delta == 1 && worldGenerator.scaleFactor < worldGenerator.CLOSEST_SCALE * worldGenerator.mainScale)
		{
			worldGenerator.scaleFactor += 0.01f;
			scaleDecrease = 0.03f;
		}

	if (scaleDecrease < 0 && worldGenerator.scaleFactor < worldGenerator.FARTHEST_SCALE * worldGenerator.mainScale)
		worldGenerator.scaleFactor = worldGenerator.FARTHEST_SCALE * worldGenerator.mainScale;
	if (scaleDecrease > 0 && worldGenerator.scaleFactor > worldGenerator.CLOSEST_SCALE * worldGenerator.mainScale)
		worldGenerator.scaleFactor = worldGenerator.CLOSEST_SCALE * worldGenerator.mainScale;
}

void WorldHandler::scaleSmoothing()
{
	if (abs(scaleDecrease) >= 0.02 && timeForScaleDecrease >= 30000)
	{
		if (worldGenerator.scaleFactor != worldGenerator.FARTHEST_SCALE * worldGenerator.mainScale && worldGenerator.scaleFactor != worldGenerator.CLOSEST_SCALE * worldGenerator.mainScale)
			worldGenerator.scaleFactor += scaleDecrease;
		if (scaleDecrease < 0 && worldGenerator.scaleFactor <= worldGenerator.FARTHEST_SCALE * worldGenerator.mainScale)
			worldGenerator.scaleFactor = worldGenerator.FARTHEST_SCALE * worldGenerator.mainScale;
		if (scaleDecrease > 0 && worldGenerator.scaleFactor >= worldGenerator.CLOSEST_SCALE * worldGenerator.mainScale)
			worldGenerator.scaleFactor = worldGenerator.CLOSEST_SCALE * worldGenerator.mainScale;

		if (scaleDecrease < 0)
		{
			scaleDecrease += 0.001f;
		}
		else
			if (scaleDecrease > 0)
			{
				scaleDecrease -= 0.001f;
			}


		timeForScaleDecrease = 0;
	}
}

void WorldHandler::birthObjects()
{
	for (auto& object : localTerrain)
	{
		auto birthStaticStack = object->getBirthObjects().first;
		auto birthDynamicStack = object->getBirthObjects().second;

		while (!birthStaticStack.empty())
		{
			const auto biome = worldGenerator.biomeMatrix[int(birthStaticStack.top().position.x / blockSize.x)][int(birthStaticStack.top().position.y / blockSize.y)];
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
	staticGrid = GridList(this->width, this->height, blockSize, microBlockSize);
	dynamicGrid = GridList(this->width, this->height, blockSize, microBlockSize);
	staticGrid.boundDynamicMatrix(&dynamicGrid.microBlockMatrix);

	std::ifstream fin("save.txt");
	std::string saveName;
	float posX, posY;
	int num, typeOfObject;
	fin >> num;
	for (auto i = 0; i < num; i++)
	{
		fin >> saveName >> posX >> posY;

		if (saveName == nightmare_first("loadInit", Vector2f(0, 0)).getToSaveName())
			worldGenerator.initializeDynamicItem(Tag::monster, Vector2f(posX, posY), "");
		else
			if (saveName == deerchant("loadInit", Vector2f(0, 0)).getToSaveName())
				worldGenerator.initializeDynamicItem(Tag::hero, Vector2f(posX, posY), "");
			else
				if (saveName == wolf("loadInit", Vector2f(0, 0)).getToSaveName())
					worldGenerator.initializeDynamicItem(Tag::wolf, Vector2f(posX, posY), "");
	}

	fin >> num;
	for (auto i = 0; i < num; i++)
	{
		fin >> saveName >> typeOfObject >> posX >> posY;

		if (saveName == ground_connection("loadInit", Vector2f(0, 0), typeOfObject).getToSaveName())
			worldGenerator.initializeStaticItem(Tag::groundConnection, Vector2f(posX, posY), typeOfObject, "", 1);
		else
			if (saveName == ground("loadInit", Vector2f(0, 0), typeOfObject).getToSaveName())
				worldGenerator.initializeStaticItem(Tag::ground, Vector2f(posX, posY), typeOfObject, "", 1);
			else
				if (saveName == ForestTree("loadInit", Vector2f(0, 0), typeOfObject).getToSaveName())
					worldGenerator.initializeStaticItem(Tag::tree, Vector2f(posX, posY), typeOfObject, "", 1);
				else
					if (saveName == grass("loadInit", Vector2f(0, 0), typeOfObject).getToSaveName())
						worldGenerator.initializeStaticItem(Tag::grass, Vector2f(posX, posY), typeOfObject, "", 1);																
	}

	fin.close();

	//preparations for the inventory system 
	auto hero = dynamic_cast<deerchant*>(dynamicGrid.getItemByName(focusedObject->getName()));

	inventorySystem.inventoryBounding(&hero->bags);
	//------------------------------------
	//buildSystem.inventoryBounding(&hero->bags);
	//buildSystem.succesInit = true;
	//buildSystem.succesInit = true;

	Save();
}

void WorldHandler::Save()
{
	if (staticGrid.getSize() == 0)
		return;
	
	std::ofstream fout("save.txt");
	fout.clear();
	auto staticItems = ObjectInitializer::vectorCastToStatic(staticGrid.getItems(0, 0, float(width), float(height)));
	auto dynamicItems = ObjectInitializer::vectorCastToDynamic(dynamicGrid.getItems(0, 0, float(width), float(height)));

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

void WorldHandler::setTransparent(std::vector<WorldObject*>& visibleItems)
{
	mouseDisplayName = "";
	const auto mousePos = Vector2f((Mouse::getPosition().x - Helper::GetScreenSize().x / 2 + cameraSystem.position.x*worldGenerator.scaleFactor) / worldGenerator.scaleFactor,
	                               (Mouse::getPosition().y - Helper::GetScreenSize().y / 2 + cameraSystem.position.y*worldGenerator.scaleFactor) / worldGenerator.scaleFactor);

	auto minCapacity = 1e6f, minDistance = 1e6f;

	for (auto& visibleItem : visibleItems)
	{
		if (!visibleItem || visibleItem->getName() == focusedObject->getName() || visibleItem->intangible)
			continue;

		visibleItem->isTransparent = false;
		const auto itemPos = Vector2f(visibleItem->getPosition().x - visibleItem->getTextureOffset().x, visibleItem->getPosition().y - visibleItem->getTextureOffset().y);

		if (mousePos.x >= itemPos.x && mousePos.x <= itemPos.x + visibleItem->getConditionalSizeUnits().x &&
			mousePos.y >= itemPos.y && mousePos.y <= itemPos.y + visibleItem->getConditionalSizeUnits().y)
		{
			visibleItem->isSelected = true;
		}
		else
			visibleItem->isSelected = false;

		if (!visibleItem->isBackground && Helper::isIntersects(mousePos, FloatRect(itemPos.x, itemPos.y, visibleItem->getConditionalSizeUnits().x, visibleItem->getConditionalSizeUnits().y)) ||
			Helper::getDist(mousePos, visibleItem->getPosition()) <= visibleItem->getRadius())
		{
			auto itemCapacity = visibleItem->getConditionalSizeUnits().x + visibleItem->getConditionalSizeUnits().y;
			if (visibleItem->tag == Tag::brazier)
				itemCapacity /= 10;
			float distanceToItemCenter;
			if (!visibleItem->isBackground)
				distanceToItemCenter = abs(mousePos.x - (itemPos.x + visibleItem->getConditionalSizeUnits().x / 2)) +
				abs(mousePos.y - (itemPos.y + visibleItem->getConditionalSizeUnits().y / 2));
			else
				distanceToItemCenter = Helper::getDist(mousePos, visibleItem->getPosition());

			if (itemCapacity < minCapacity || (itemCapacity == minCapacity && distanceToItemCenter <= minDistance))
			{
				minCapacity = itemCapacity;
				minDistance = distanceToItemCenter;

				const auto terrain = dynamic_cast<TerrainObject*>(visibleItem);
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

bool WorldHandler::fixedClimbingBeyond(Vector2f &pos) const
{
	const auto screenSize = Helper::GetScreenSize();
	const auto extra = staticGrid.getBlockSize();

	const auto limit_x = (screenSize.x / 2.0f + extra.x) * 1.5f;
	const auto limit_y = (screenSize.y / 2.0f + extra.y) * 1.5f;

	if (pos.x < limit_x)
	{
		pos.x = limit_x;
		return false;
	}
	if (pos.x > float(width) - limit_x)
	{
		pos.x = float(width) - limit_x;
		return false;
	}
	if (pos.y < limit_y)
	{
		pos.y = limit_y;
		return false;
	}
	if (pos.y > float(height) - limit_y)
	{
		pos.y = float(height) - limit_y;
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

void WorldHandler::onMouseUp(const int currentMouseButton)
{	
	if (mouseDisplayName == "Set pedestal")
	{
		const auto terrain = dynamic_cast<TerrainObject*>(selectedObject);
		pedestalController.start(terrain);
	}
	if (pedestalController.isRunning())
		return;
	const auto mousePos = Mouse::getPosition();
	const auto mouseWorldPos = Vector2f((mousePos.x - Helper::GetScreenSize().x / 2 + cameraSystem.position.x*worldGenerator.scaleFactor) / worldGenerator.scaleFactor,
	                                    (mousePos.y - Helper::GetScreenSize().y / 2 + cameraSystem.position.y*worldGenerator.scaleFactor) / worldGenerator.scaleFactor);
	
	inventorySystem.onMouseUp();

	if (buildSystem.getSuccessInit() /* && inventorySystem.getHeldItem()->first == -1*/)
		buildSystem.onMouseUp();

	if (mouseDisplayName.empty())
		selectedObject = nullptr;

	auto hero = dynamic_cast<deerchant*>(dynamicGrid.getItemByName(focusedObject->getName()));
	hero->onMouseUp(currentMouseButton, selectedObject, mouseWorldPos, (buildSystem.buildingPosition != Vector2f(-1, -1) && !buildSystem.instantBuild));
}

void WorldHandler::handleEvents(Event& event)
{
	pedestalController.handleEvents(event);
}

void WorldHandler::interact(Vector2f render_target_size, long long elapsedTime, Event event)
{
	scaleSmoothing();
	birthObjects();
	timeSystem.interact(elapsedTime);

	timeForScaleDecrease += scaleDecreaseClock.getElapsedTime().asMicroseconds();
	scaleDecreaseClock.restart();

	const auto extra = staticGrid.getBlockSize();
	const auto characterPosition = focusedObject->getPosition();

	const Vector2f worldUpperLeft(characterPosition.x - render_target_size.x / 2, characterPosition.y - render_target_size.y / 2);
	const Vector2f worldBottomRight(characterPosition.x + render_target_size.x / 2, characterPosition.y + render_target_size.y / 2);

	worldGenerator.beyondScreenGeneration();

	auto localItems = staticGrid.getItems(worldUpperLeft.x - extra.x, worldUpperLeft.y - extra.y, worldBottomRight.x + extra.x, worldBottomRight.y + extra.y);
	auto localStaticItems = ObjectInitializer::vectorCastToStatic(localItems);
	auto localDynamicItems = ObjectInitializer::vectorCastToDynamic(dynamicGrid.getItems(worldUpperLeft.x - extra.x, worldUpperLeft.y - extra.y, worldBottomRight.x + extra.x, worldBottomRight.y + extra.y));

	localTerrain.clear();
	for (auto& item : localStaticItems)
		if (!item->isBackground)
			localTerrain.push_back(item);
	for (auto& item : localDynamicItems)
		localTerrain.push_back(item);

	setTransparent(localTerrain);

	const auto hero = dynamic_cast<deerchant*>(dynamicGrid.getItemByName(focusedObject->getName()));
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
			if (dynamicItem->getRouteGenerationAbility() && dynamicItem->laxMovePosition != Vector2f(-1, -1) && dynamicItem->getCurrentAction() != jerking && dynamicItem->tag != Tag::hero)
			{
				const auto permissibleDistance = 10;			
				timeAfterNewRoute = 0;
				staticGrid.makeRoute(dynamicItem->getPosition(), dynamicItem->laxMovePosition,
				                     dynamicItem->getPosition().x - dynamicItem->sightRange / (worldGenerator.scaleFactor * worldGenerator.mainScale),
				                     dynamicItem->getPosition().y - dynamicItem->sightRange / (worldGenerator.scaleFactor * worldGenerator.mainScale),
				                     dynamicItem->getPosition().x + dynamicItem->sightRange / (worldGenerator.scaleFactor * worldGenerator.mainScale),
				                     dynamicItem->getPosition().y + dynamicItem->sightRange / (worldGenerator.scaleFactor * worldGenerator.mainScale), permissibleDistance);
				dynamicItem->setRoute(staticGrid.route);
				staticGrid.setLockedMicroBlocks(dynamicItem, false, true);
			}
			else			
				dynamicItem->changeMovePositionToRoute(dynamicItem->laxMovePosition);

			if (dynamicItem->route.empty())
				dynamicItem->changeMovePositionToRoute(dynamicItem->laxMovePosition);
			//staticGrid.setLockedMicroBlocks(dynamicItem, false, true);
		}
	}
	//-----------------------------------

	for (auto& dynamicItem : localDynamicItems)
	{		
		// passing the beginning  of the route
		if (!dynamicItem->route.empty())
		{
			std::pair<int, int> routeMicroBlock;
			do
			{
				routeMicroBlock = dynamicItem->route[0];
				const auto routePos = Vector2f(routeMicroBlock.first * microBlockSize.x, routeMicroBlock.second * microBlockSize.y);
				if (Helper::getDist(dynamicItem->laxMovePosition, routePos) < Helper::getDist(dynamicItem->laxMovePosition, dynamicItem->getPosition()) && dynamicItem->route.size() > 1)
					break;

				dynamicItem->route.erase(dynamicItem->route.begin());
			} while (!dynamicItem->route.empty());
			
			dynamicItem->changeMovePositionToRoute(Vector2f(routeMicroBlock.first * microBlockSize.x, routeMicroBlock.second * microBlockSize.y));
		}
		else
			dynamicItem->changeMovePositionToRoute(dynamicItem->laxMovePosition);
		//------------------------------------

		dynamicItem->behavior(elapsedTime);
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

		if (dynamicItem->shakeSpeed != -1)
			cameraSystem.makeShake(4, dynamicItem->shakeSpeed);

		auto newPosition = dynamicItem->doMove(elapsedTime);

		fixedClimbingBeyond(newPosition);

		newPosition = dynamicItem->doSlip(newPosition, localStaticItems, float(height), elapsedTime);
		//newPosition = dynamicItem->doSlipOffDynamic(newPosition, localDynamicItems, height, elapsedTime);

		if (!fixedClimbingBeyond(newPosition))
			newPosition = dynamicItem->getPosition();				
		dynamicItem->setPosition(newPosition);
		dynamicGrid.updateItemPosition(dynamicItem->getName(), newPosition.x, newPosition.y);		
	}

	//if (focusedObject->getCurrentAction() == builds)
		setItemFromBuildSystem();

	//buildSystem.setHeldItem(inventorySystem.getHeldItem()->lootInfo);
	buildSystem.interact(cameraSystem.position, worldGenerator.scaleFactor);
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

bool cmpImgDraw(sprite_chain_element* first, sprite_chain_element* second)
{
	if (first->isBackground && !second->isBackground)
		return true;
	if (!first->isBackground && second->isBackground)
		return false;

	if (first->zCoord == second->zCoord)
	{
		if (first->position.y == second->position.y)
		{
			if (first->position.x == second->position.x)
				return first->size.x * first->size.y < second->size.x * second->size.y;
			return first->position.x < second->position.x;
		}
		return first->position.y < second->position.y;
	}

	return first->zCoord < second->zCoord;
}

std::vector<sprite_chain_element*> WorldHandler::prepareSprites(const long long elapsedTime, const bool onlyBackground)
{
    std::vector<sprite_chain_element*> result = {};

    const auto extra = staticGrid.getBlockSize();

	const auto screenSize = Helper::GetScreenSize();
	const auto screenCenter = Vector2f(screenSize.x / 2, screenSize.y / 2);

	cameraSystem.position.x += (focusedObject->getPosition().x + Helper::GetScreenSize().x * camera_system::camOffset.x - cameraSystem.position.x) *
		(focusedObject->getMoveSystem().speed * float(elapsedTime)) / camera_system::maxCameraDistance.x;
	
	cameraSystem.position.y += (focusedObject->getPosition().y + Helper::GetScreenSize().y * camera_system::camOffset.y - cameraSystem.position.y) *
		(focusedObject->getMoveSystem().speed * float(elapsedTime)) / camera_system::maxCameraDistance.y;
	
	cameraSystem.shakeInteract(elapsedTime);

    worldUpperLeft = Vector2f(
		ceil(cameraSystem.position.x - (screenCenter.x + extra.x) / worldGenerator.scaleFactor),
		ceil(cameraSystem.position.y - (screenCenter.y + extra.y) / worldGenerator.scaleFactor));
	worldBottomRight = Vector2f(
		ceil(cameraSystem.position.x + (screenCenter.x + extra.x) / worldGenerator.scaleFactor),
		ceil(cameraSystem.position.y + (screenCenter.y + extra.y) / worldGenerator.scaleFactor));
	
	if (worldUpperLeft.x < 0)
		worldUpperLeft.x = 0;
	if (worldUpperLeft.y < 0)
		worldUpperLeft.y = 0;
	if (worldBottomRight.x > float(width))
		worldBottomRight.x = float(width);
	if (worldBottomRight.y > float(height))
		worldBottomRight.y = float(height);

    auto localStaticItems = staticGrid.getItems(worldUpperLeft.x, worldUpperLeft.y, worldBottomRight.x, worldBottomRight.y);
	auto localDynamicItems = dynamicGrid.getItems(worldUpperLeft.x, worldUpperLeft.y, worldBottomRight.x, worldBottomRight.y);

	for (auto& item : localStaticItems)
	{
        if ((onlyBackground && item->isBackground) || (!onlyBackground && !item->isBackground))
        {
            auto sprites = item->prepareSprites(elapsedTime);
            result.insert(result.end(), sprites.begin(), sprites.end());
        }
    }
    for (auto& item : localDynamicItems)
	{
        if (!onlyBackground)
        {
            auto sprites = item->prepareSprites(elapsedTime);
            result.insert(result.end(), sprites.begin(), sprites.end());
        }
    }

    sort(result.begin(), result.end(), cmpImgDraw);

    return result;
}