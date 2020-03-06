#define _USE_MATH_DEFINES

#include "world_handler.h"

#include <fstream>

#include "object_initializer.h"

using namespace sf;

world_handler::world_handler(const int width, const int height, std::map<pack_tag, sprite_pack>* packsMap)
{
    this->packsMap = packsMap;
	world_object::micro_block_size = microBlockSize;
    worldGenerator.initMainScale();
	worldGenerator.scaleFactor = worldGenerator.mainScale;
	this->width = width;
	this->height = height;
	staticGrid = grid_list(this->width, this->height, blockSize, microBlockSize);
	dynamicGrid = grid_list(this->width, this->height, blockSize, microBlockSize);

	worldGenerator.init(width, height, blockSize, microBlockSize, &staticGrid, &dynamicGrid, packsMap);
	buildSystem.init();
	inventorySystem.init();
	timeSystem.init(10.0 / (24 * 60));
	lightSystem.init({0, 0, helper::GetScreenSize().x, helper::GetScreenSize().y});
}

world_handler::~world_handler()
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

void world_handler::runWorldGenerator()
{
	staticGrid = grid_list(this->width, this->height, blockSize, microBlockSize);
	dynamicGrid = grid_list(this->width, this->height, blockSize, microBlockSize);
	for (auto& i : worldGenerator.biomeMatrix)
		for (auto& j : i)
			j = swampy_trees;
	worldGenerator.generate();
	this->focusedObject = worldGenerator.focusedObject;
	brazier = dynamic_cast<::brazier*>(staticGrid.get_item_by_name("brazier"));
	//brazier->linkWithBuildSystem(&buildSystem);
	worldGenerator.rememberedBlocks = { { staticGrid.get_index_by_point(brazier->get_position().x, brazier->get_position().y), true } };
	cameraSystem.position = Vector2f(focusedObject->get_position().x + helper::GetScreenSize().x * camera_system::cam_offset.x, focusedObject->get_position().y + helper::GetScreenSize().y * camera_system::cam_offset.y);

	const auto hero = dynamic_cast<deerchant*>(focusedObject);
	inventorySystem.inventory_bounding(&hero->bags);
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

void world_handler::setScaleFactor(const int delta)
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

void world_handler::scaleSmoothing()
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

void world_handler::birthObjects()
{
	for (auto& object : localTerrain)
	{
		auto birthStaticStack = object->get_birth_objects().first;
		auto birthDynamicStack = object->get_birth_objects().second;

		while (!birthStaticStack.empty())
		{
			const auto biome = worldGenerator.biomeMatrix[int(birthStaticStack.top().position.x / blockSize.x)][int(birthStaticStack.top().position.y / blockSize.y)];
			worldGenerator.initializeStaticItem(birthStaticStack.top().tag, birthStaticStack.top().position, birthStaticStack.top().type_of_object, "", birthStaticStack.top().count, biome, true, birthStaticStack.top().inventory);
			birthStaticStack.pop();
		}
		while (!birthDynamicStack.empty())
		{
			worldGenerator.initializeDynamicItem(birthDynamicStack.top().tag, birthDynamicStack.top().position, "", birthDynamicStack.top().owner);
			birthDynamicStack.pop();
		}
		object->clear_birth_stack();
	}
}

void world_handler::Load()
{
	staticGrid = grid_list(this->width, this->height, blockSize, microBlockSize);
	dynamicGrid = grid_list(this->width, this->height, blockSize, microBlockSize);
	staticGrid.bound_dynamic_matrix(&dynamicGrid.micro_block_matrix);

	std::ifstream fin("save.txt");
	std::string saveName;
	float posX, posY;
	int num, typeOfObject;
	fin >> num;
	for (auto i = 0; i < num; i++)
	{
		fin >> saveName >> posX >> posY;

		if (saveName == nightmare_first("loadInit", Vector2f(0, 0)).get_to_save_name())
			worldGenerator.initializeDynamicItem(entity_tag::monster, Vector2f(posX, posY), "");
		else
			if (saveName == deerchant("loadInit", Vector2f(0, 0)).get_to_save_name())
				worldGenerator.initializeDynamicItem(entity_tag::hero, Vector2f(posX, posY), "");
			else
				if (saveName == wolf("loadInit", Vector2f(0, 0)).get_to_save_name())
					worldGenerator.initializeDynamicItem(entity_tag::wolf, Vector2f(posX, posY), "");
	}

	fin >> num;
	for (auto i = 0; i < num; i++)
	{
		fin >> saveName >> typeOfObject >> posX >> posY;

		if (saveName == ground_connection("loadInit", Vector2f(0, 0), typeOfObject).get_to_save_name())
			worldGenerator.initializeStaticItem(entity_tag::groundConnection, Vector2f(posX, posY), typeOfObject, "", 1);
		else
			if (saveName == ground("loadInit", Vector2f(0, 0), typeOfObject).get_to_save_name())
				worldGenerator.initializeStaticItem(entity_tag::ground, Vector2f(posX, posY), typeOfObject, "", 1);
			else
				if (saveName == forest_tree("loadInit", Vector2f(0, 0), typeOfObject).get_to_save_name())
					worldGenerator.initializeStaticItem(entity_tag::tree, Vector2f(posX, posY), typeOfObject, "", 1);
				else
					if (saveName == grass("loadInit", Vector2f(0, 0), typeOfObject).get_to_save_name())
						worldGenerator.initializeStaticItem(entity_tag::grass, Vector2f(posX, posY), typeOfObject, "", 1);																
	}

	fin.close();

	//preparations for the inventory system 
	auto hero = dynamic_cast<deerchant*>(dynamicGrid.get_item_by_name(focusedObject->get_name()));

	inventorySystem.inventory_bounding(&hero->bags);
	//------------------------------------
	//buildSystem.inventoryBounding(&hero->bags);
	//buildSystem.succesInit = true;
	//buildSystem.succesInit = true;

	Save();
}

void world_handler::Save()
{
	if (staticGrid.get_size() == 0)
		return;
	
	std::ofstream fout("save.txt");
	fout.clear();
	auto staticItems = object_initializer::vector_cast_to_static(staticGrid.get_items(0, 0, float(width), float(height)));
	auto dynamicItems = object_initializer::vector_cast_to_dynamic(dynamicGrid.get_items(0, 0, float(width), float(height)));

	fout << dynamicItems.size() << std::endl;
	for (auto& dynamicItem : dynamicItems)
	{
		fout << dynamicItem->get_to_save_name() << " " << dynamicItem->get_position().x << " " << dynamicItem->get_position().y << std::endl;
	}
	fout << staticItems.size() << std::endl;
	for (auto& staticItem : staticItems)
	{
		fout << staticItem->get_to_save_name() << " " << staticItem->get_type() << " " << staticItem->get_position().x << " " << staticItem->get_position().y << std::endl;
	}
	fout.close();
}

void world_handler::clearWorld()
{
	staticGrid.~grid_list();
	dynamicGrid.~grid_list();
}

void world_handler::setTransparent(std::vector<world_object*>& visibleItems)
{
	mouseDisplayName = "";
	const auto mousePos = Vector2f((Mouse::getPosition().x - helper::GetScreenSize().x / 2 + cameraSystem.position.x*worldGenerator.scaleFactor) / worldGenerator.scaleFactor,
	                               (Mouse::getPosition().y - helper::GetScreenSize().y / 2 + cameraSystem.position.y*worldGenerator.scaleFactor) / worldGenerator.scaleFactor);

	auto minCapacity = 1e6f, minDistance = 1e6f;

	for (auto& visibleItem : visibleItems)
	{
		if (!visibleItem || visibleItem->get_name() == focusedObject->get_name() || visibleItem->intangible)
			continue;

		visibleItem->is_transparent = false;
		const auto itemPos = Vector2f(visibleItem->get_position().x - visibleItem->get_texture_offset().x, visibleItem->get_position().y - visibleItem->get_texture_offset().y);

		if (mousePos.x >= itemPos.x && mousePos.x <= itemPos.x + visibleItem->get_conditional_size_units().x &&
			mousePos.y >= itemPos.y && mousePos.y <= itemPos.y + visibleItem->get_conditional_size_units().y)
		{
			visibleItem->is_selected = true;
		}
		else
			visibleItem->is_selected = false;

		if (!visibleItem->is_background && helper::isIntersects(mousePos, FloatRect(itemPos.x, itemPos.y, visibleItem->get_conditional_size_units().x, visibleItem->get_conditional_size_units().y)) ||
			helper::getDist(mousePos, visibleItem->get_position()) <= visibleItem->get_radius())
		{
			auto itemCapacity = visibleItem->get_conditional_size_units().x + visibleItem->get_conditional_size_units().y;
			if (visibleItem->tag == entity_tag::brazier)
				itemCapacity /= 10;
			float distanceToItemCenter;
			if (!visibleItem->is_background)
				distanceToItemCenter = abs(mousePos.x - (itemPos.x + visibleItem->get_conditional_size_units().x / 2)) +
				abs(mousePos.y - (itemPos.y + visibleItem->get_conditional_size_units().y / 2));
			else
				distanceToItemCenter = helper::getDist(mousePos, visibleItem->get_position());

			if (itemCapacity < minCapacity || (itemCapacity == minCapacity && distanceToItemCenter <= minDistance))
			{
				minCapacity = itemCapacity;
				minDistance = distanceToItemCenter;

				const auto terrain = dynamic_cast<terrain_object*>(visibleItem);
				if (terrain && pedestalController.ready_to_start)				
					mouseDisplayName = "Set pedestal";
				else
					switch (visibleItem->tag)
					{
					case entity_tag::tree:
						{
							mouseDisplayName = "Absorb";
							break;
						}
					case entity_tag::brazier:
					{					
						if (inventorySystem.get_held_item().content.first != entity_tag::emptyCell &&
							helper::getDist(brazier->getPlatePosition(), mousePos) <= brazier->getPlateRadius())
						{
							if (helper::getDist(brazier->getPlatePosition(), focusedObject->get_position()) <= brazier->getPlateRadius() + focusedObject->get_radius())
								mouseDisplayName = "Toss";
							else
								mouseDisplayName = "Come to toss";
						}
						break;
					}
					case entity_tag::hare:
					case entity_tag::owl:
						{
							if (inventorySystem.get_held_item().content.first == entity_tag::inkyBlackPen)
								mouseDisplayName = "Sketch";
							else
								mouseDisplayName = "Catch up";
							break;
						}
					case entity_tag::fern:
						{
							if (!visibleItem->inventory.empty())
								mouseDisplayName = "Open";
							else
								mouseDisplayName = "Pick up";
							break;
						}
					case entity_tag::yarrow:
					case entity_tag::chamomile:				
					case entity_tag::mugwort:
					case entity_tag::noose:
					case entity_tag::hareTrap:
					case entity_tag::droppedLoot:
						{
							mouseDisplayName = "Pick up";
							break;
						}
						default:
						{
							mouseDisplayName = visibleItem->get_to_save_name();
							break;
						}
					}

				selectedObject = visibleItem;
			}
		}

		if (focusedObject->get_position().x >= itemPos.x && focusedObject->get_position().x <= itemPos.x + visibleItem->get_conditional_size_units().x && focusedObject->get_position().y >= itemPos.y && focusedObject->get_position().y <= visibleItem->get_position().y && !visibleItem->is_background)
		{
			visibleItem->is_transparent = true;
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

bool world_handler::fixedClimbingBeyond(Vector2f &pos) const
{
	const auto screenSize = helper::GetScreenSize();
	const auto extra = staticGrid.get_block_size();

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

void world_handler::setItemFromBuildSystem()
{
	if (!(buildSystem.instant_build || focusedObject->get_current_action() == builds))
		return;

	if (buildSystem.selected_object != entity_tag::emptyCell && buildSystem.building_position != Vector2f(-1, -1))
	{
		if (buildSystem.dropped_loot_id_list.count(buildSystem.selected_object) > 0)
			worldGenerator.initializeStaticItem(entity_tag::droppedLoot, buildSystem.building_position, int(buildSystem.selected_object), "", 1);
		else
			worldGenerator.initializeStaticItem(buildSystem.selected_object, buildSystem.building_position, buildSystem.build_type, "", 1);

		if (buildSystem.selected_object == entity_tag::totem)
		{
			if (buildSystem.build_type == 2)
				buildSystem.clear_hare_bags(staticGrid.get_index_by_point(buildSystem.building_position.x, buildSystem.building_position.y), staticGrid, &localTerrain);
		}
		buildSystem.was_placed();
		buildSystem.instant_build = false;

		buildSystem.building_position = Vector2f(-1, -1);
		buildSystem.build_type = 1;
		brazier->clearCurrentCraft();
	}
}

void world_handler::onMouseUp(const int currentMouseButton)
{	
	if (mouseDisplayName == "Set pedestal")
	{
		const auto terrain = dynamic_cast<terrain_object*>(selectedObject);
		pedestalController.start(terrain);
	}
	if (pedestalController.is_running())
		return;
	const auto mousePos = Mouse::getPosition();
	const auto mouseWorldPos = Vector2f((mousePos.x - helper::GetScreenSize().x / 2 + cameraSystem.position.x*worldGenerator.scaleFactor) / worldGenerator.scaleFactor,
	                                    (mousePos.y - helper::GetScreenSize().y / 2 + cameraSystem.position.y*worldGenerator.scaleFactor) / worldGenerator.scaleFactor);
	
	inventorySystem.on_mouse_up();

	if (buildSystem.get_success_init() /* && inventorySystem.getHeldItem()->first == -1*/)
		buildSystem.on_mouse_up();

	if (mouseDisplayName.empty())
		selectedObject = nullptr;

	auto hero = dynamic_cast<deerchant*>(dynamicGrid.get_item_by_name(focusedObject->get_name()));
	hero->onMouseUp(currentMouseButton, selectedObject, mouseWorldPos, (buildSystem.building_position != Vector2f(-1, -1) && !buildSystem.instant_build));
}

void world_handler::handleEvents(Event& event)
{
	pedestalController.handle_events(event);
}

void world_handler::interact(Vector2f render_target_size, long long elapsedTime, Event event)
{
	scaleSmoothing();
	birthObjects();
	timeSystem.interact(elapsedTime);

	timeForScaleDecrease += scaleDecreaseClock.getElapsedTime().asMicroseconds();
	scaleDecreaseClock.restart();

	const auto extra = staticGrid.get_block_size();
	const auto characterPosition = focusedObject->get_position();

	const Vector2f worldUpperLeft(characterPosition.x - render_target_size.x / 2, characterPosition.y - render_target_size.y / 2);
	const Vector2f worldBottomRight(characterPosition.x + render_target_size.x / 2, characterPosition.y + render_target_size.y / 2);

	worldGenerator.beyondScreenGeneration();

	auto localItems = staticGrid.get_items(worldUpperLeft.x - extra.x, worldUpperLeft.y - extra.y, worldBottomRight.x + extra.x, worldBottomRight.y + extra.y);
	auto localStaticItems = object_initializer::vector_cast_to_static(localItems);
	auto localDynamicItems = object_initializer::vector_cast_to_dynamic(dynamicGrid.get_items(worldUpperLeft.x - extra.x, worldUpperLeft.y - extra.y, worldBottomRight.x + extra.x, worldBottomRight.y + extra.y));

	localTerrain.clear();
	for (auto& item : localStaticItems)
		if (!item->is_background)
			localTerrain.push_back(item);
	for (auto& item : localDynamicItems)
		localTerrain.push_back(item);

	setTransparent(localTerrain);

	const auto hero = dynamic_cast<deerchant*>(dynamicGrid.get_item_by_name(focusedObject->get_name()));
	hero->heldItem = &inventorySystem.get_held_item();

	//making route to the desire position
	timeAfterNewRoute += elapsedTime;
	if (timeAfterNewRoute >= timeForNewRoute)
	{
		timeAfterNewRoute = 0;
		for (auto& dynamicItem : localDynamicItems)
		{
			//staticGrid.setLockedMicroBlocks(dynamicItem, true, true);
			//dynamicItem->initMicroBlocks();
			if (dynamicItem->get_route_generation_ability() && dynamicItem->lax_move_position != Vector2f(-1, -1) && dynamicItem->get_current_action() != jerking && dynamicItem->tag != entity_tag::hero)
			{
				const auto permissibleDistance = 10;			
				timeAfterNewRoute = 0;
				staticGrid.make_route(dynamicItem->get_position(), dynamicItem->lax_move_position,
				                     dynamicItem->get_position().x - dynamicItem->sight_range / (worldGenerator.scaleFactor * worldGenerator.mainScale),
				                     dynamicItem->get_position().y - dynamicItem->sight_range / (worldGenerator.scaleFactor * worldGenerator.mainScale),
				                     dynamicItem->get_position().x + dynamicItem->sight_range / (worldGenerator.scaleFactor * worldGenerator.mainScale),
				                     dynamicItem->get_position().y + dynamicItem->sight_range / (worldGenerator.scaleFactor * worldGenerator.mainScale), permissibleDistance);
				dynamicItem->set_route(staticGrid.route);
				staticGrid.set_locked_micro_blocks(dynamicItem, false, true);
			}
			else			
				dynamicItem->change_move_position_to_route(dynamicItem->lax_move_position);

			if (dynamicItem->route.empty())
				dynamicItem->change_move_position_to_route(dynamicItem->lax_move_position);
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
				if (helper::getDist(dynamicItem->lax_move_position, routePos) < helper::getDist(dynamicItem->lax_move_position, dynamicItem->get_position()) && dynamicItem->route.size() > 1)
					break;

				dynamicItem->route.erase(dynamicItem->route.begin());
			} while (!dynamicItem->route.empty());
			
			dynamicItem->change_move_position_to_route(Vector2f(routeMicroBlock.first * microBlockSize.x, routeMicroBlock.second * microBlockSize.y));
		}
		else
			dynamicItem->change_move_position_to_route(dynamicItem->lax_move_position);
		//------------------------------------

		dynamicItem->behavior(elapsedTime);
		//interaction with other objects		
		for (auto& otherDynamicItem : localDynamicItems)
		{
			if (dynamicItem == otherDynamicItem)
				continue;
			dynamicItem->set_target(*otherDynamicItem);
		}
		for (auto& otherDynamicItem : localDynamicItems)
		{
			if (dynamicItem == otherDynamicItem)
				continue;
			dynamicItem->behavior_with_dynamic(otherDynamicItem, elapsedTime);
		}
		for (auto& otherStaticItem : localStaticItems)
			dynamicItem->behavior_with_static(otherStaticItem, elapsedTime);
		//--------						

		if (dynamicItem->shake_speed != -1)
			cameraSystem.make_shake(4, dynamicItem->shake_speed);

		auto newPosition = dynamicItem->get_move_system().doMove(elapsedTime);

		fixedClimbingBeyond(newPosition);

		newPosition = dynamicItem->get_move_system().doSlip(newPosition, localStaticItems, float(height), elapsedTime);
		//newPosition = dynamicItem->doSlipOffDynamic(newPosition, localDynamicItems, height, elapsedTime);

		if (!fixedClimbingBeyond(newPosition))
			newPosition = dynamicItem->get_position();				
		dynamicItem->set_position(newPosition);
		dynamicGrid.update_item_position(dynamicItem->get_name(), newPosition.x, newPosition.y);		
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
		if (item->get_delete_promise())		
			staticGrid.delete_item(item->get_name());
	for (auto& item : localDynamicItems)
		if (item->get_delete_promise())
			dynamicGrid.delete_item(item->get_name());
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
	if (first->is_background && !second->is_background)
		return true;
	if (!first->is_background && second->is_background)
		return false;

	if (first->z_coordinate == second->z_coordinate)
	{
		if (first->position.y == second->position.y)
		{
			if (first->position.x == second->position.x)
				return first->size.x * first->size.y < second->size.x * second->size.y;
			return first->position.x < second->position.x;
		}
		return first->position.y < second->position.y;
	}

	return first->z_coordinate < second->z_coordinate;
}

std::vector<sprite_chain_element*> world_handler::prepareSprites(const long long elapsedTime, const bool onlyBackground)
{
    std::vector<sprite_chain_element*> result = {};

    const auto extra = staticGrid.get_block_size();

	const auto screenSize = helper::GetScreenSize();
	const auto screenCenter = Vector2f(screenSize.x / 2, screenSize.y / 2);

	cameraSystem.position.x += (focusedObject->get_position().x + helper::GetScreenSize().x * camera_system::cam_offset.x - cameraSystem.position.x) *
		(focusedObject->get_move_system().speed * float(elapsedTime)) / camera_system::max_camera_distance.x;
	
	cameraSystem.position.y += (focusedObject->get_position().y + helper::GetScreenSize().y * camera_system::cam_offset.y - cameraSystem.position.y) *
		(focusedObject->get_move_system().speed * float(elapsedTime)) / camera_system::max_camera_distance.y;
	
	cameraSystem.shake_interact(elapsedTime);

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

    auto localStaticItems = staticGrid.get_items(worldUpperLeft.x, worldUpperLeft.y, worldBottomRight.x, worldBottomRight.y);
	auto localDynamicItems = dynamicGrid.get_items(worldUpperLeft.x, worldUpperLeft.y, worldBottomRight.x, worldBottomRight.y);

	for (auto& item : localStaticItems)
	{
        if ((onlyBackground && item->is_background) || (!onlyBackground && !item->is_background))
        {
            auto sprites = item->prepare_sprites(elapsedTime);
            result.insert(result.end(), sprites.begin(), sprites.end());
        }
    }
    for (auto& item : localDynamicItems)
	{
        if (!onlyBackground)
        {
            auto sprites = item->prepare_sprites(elapsedTime);
            result.insert(result.end(), sprites.begin(), sprites.end());
        }
    }

    sort(result.begin(), result.end(), cmpImgDraw);

    return result;
}