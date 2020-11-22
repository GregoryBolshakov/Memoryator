#define _USE_MATH_DEFINES

#include "world_handler.h"

#include <fstream>

#include "object_initializer.h"

using namespace sf;

world_handler::world_handler(
	const int width,
	const int height,
	camera_system& camera_system,
	std::map<pack_tag, sprite_pack>* packsMap) : cameraSystem{ camera_system }, scale_system_{ camera_system.get_scale_system() }
{
	this->packsMap = packsMap;
	world_object::micro_block_size = microBlockSize;
	this->width = width;
	this->height = height;
	staticGrid = grid_list(this->width, this->height, blockSize, microBlockSize);
	dynamicGrid = grid_list(this->width, this->height, blockSize, microBlockSize);
	worldGenerator.init(width, height, blockSize, microBlockSize, &staticGrid, &dynamicGrid, &scale_system_, packsMap);
	//buildSystem.init();
	inventorySystem.init();
	lightSystem.init({ 0, 0, helper::GetScreenSize().x, helper::GetScreenSize().y });
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
	
    for (auto& i : worldGenerator.biome_matrix)		for (auto& j : i)
			j = swampy_trees;

	worldGenerator.generate();
	focusedObject = worldGenerator.focused_object;
	cameraSystem.set_focused_object(focusedObject);
	brazier = dynamic_cast<::brazier*>(staticGrid.get_item_by_name("brazier"));
	//brazier->linkWithBuildSystem(&buildSystem);
	worldGenerator.remembered_blocks = { { staticGrid.get_index_by_point(brazier->get_position().x, brazier->get_position().y), true } };
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
	scale_system_.set_scale_factor(delta);}

void world_handler::birthObjects()
{
	for (auto& object : localTerrain)
	{
		auto birthStaticStack = object->get_birth_objects().first;
		auto birthDynamicStack = object->get_birth_objects().second;

		while (!birthStaticStack.empty())
		{
			const auto biome = worldGenerator.biome_matrix[int(birthStaticStack.top().position.x / blockSize.x)][int(birthStaticStack.top().position.y / blockSize.y)];
			worldGenerator.initialize_static_item(birthStaticStack.top().tag, birthStaticStack.top().position, birthStaticStack.top().type_of_object, "", birthStaticStack.top().count, biome, true, birthStaticStack.top().inventory);
			birthStaticStack.pop();
		}
		while (!birthDynamicStack.empty())
		{
			worldGenerator.initialize_dynamic_item(birthDynamicStack.top().tag, birthDynamicStack.top().position, "", birthDynamicStack.top().owner);
			birthDynamicStack.pop();
		}
		object->clear_birth_stack();
	}
}

void world_handler::Load()
{
	/*staticGrid = grid_list(this->width, this->height, blockSize, microBlockSize);
	dynamicGrid = grid_list(this->width, this->height, blockSize, microBlockSize);

	std::ifstream fin("save.txt");
	std::string saveName;
	float posX, posY;
	int num, typeOfObject;
	fin >> num;
	for (auto i = 0; i < num; i++)
	{
		fin >> saveName >> posX >> posY;

		if (saveName == nightmare_first("loadInit", Vector2f(0, 0)).get_to_save_name())
			worldGenerator.initialize_dynamic_item(entity_tag::monster, Vector2f(posX, posY), "");
		else
			if (saveName == deerchant("loadInit", Vector2f(0, 0)).get_to_save_name())
				worldGenerator.initialize_dynamic_item(entity_tag::hero, Vector2f(posX, posY), "");
			else
				if (saveName == wolf("loadInit", Vector2f(0, 0)).get_to_save_name())
					worldGenerator.initialize_dynamic_item(entity_tag::wolf, Vector2f(posX, posY), "");
	}

	fin >> num;
	for (auto i = 0; i < num; i++)
	{
		fin >> saveName >> typeOfObject >> posX >> posY;

		if (saveName == ground_connection("loadInit", Vector2f(0, 0), typeOfObject).get_to_save_name())
			worldGenerator.initialize_static_item(entity_tag::groundConnection, Vector2f(posX, posY), typeOfObject, "", 1);
		else
			if (saveName == ground("loadInit", Vector2f(0, 0), typeOfObject).get_to_save_name())
				worldGenerator.initialize_static_item(entity_tag::ground, Vector2f(posX, posY), typeOfObject, "", 1);
			else
				if (saveName == forest_tree("loadInit", Vector2f(0, 0), typeOfObject).get_to_save_name())
					worldGenerator.initialize_static_item(entity_tag::tree, Vector2f(posX, posY), typeOfObject, "", 1);
				else
					if (saveName == grass("loadInit", Vector2f(0, 0), typeOfObject).get_to_save_name())
						worldGenerator.initialize_static_item(entity_tag::grass, Vector2f(posX, posY), typeOfObject, "", 1);
	}

	fin.close();

	//preparations for the inventory system 
	auto hero = dynamic_cast<deerchant*>(dynamicGrid.get_item_by_name(focusedObject->get_name()));

	inventorySystem.inventory_bounding(&hero->bags);
	//------------------------------------
	//buildSystem.inventoryBounding(&hero->bags);
	//buildSystem.succesInit = true;
	//buildSystem.succesInit = true;

	//Save();*/
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

Vector2f  world_handler::mouse_position() const
{
	const auto scale = scale_system_.get_scale_factor();
	const auto mouse = Vector2f(Mouse::getPosition());
	const auto position = (mouse - helper::GetScreenSize() / 2.0f + cameraSystem.position * scale) / scale;

	return position;
}

void world_handler::setTransparent(std::vector<world_object*>& visibleItems)
{
	mouseDisplayName = "";
	const auto mousePos = mouse_position();
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

bool world_handler::fixedClimbingBeyond(Vector2f& pos) const
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
	/*if (!(buildSystem.instant_build || focusedObject->get_current_action() == builds))
		return;

	if (buildSystem.selected_object != entity_tag::emptyCell && buildSystem.building_position != Vector2f(-1, -1))
	{
		if (buildSystem.dropped_loot_id_list.count(buildSystem.selected_object) > 0)
			worldGenerator.initialize_static_item(entity_tag::droppedLoot, buildSystem.building_position, int(buildSystem.selected_object), "", 1);
		else
			worldGenerator.initialize_static_item(buildSystem.selected_object, buildSystem.building_position, buildSystem.build_type, "", 1);

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
	}*/
}

void world_handler::onMouseUp(const int currentMouseButton)
{
	/*if (mouseDisplayName == "Set pedestal")
	{
		const auto terrain = dynamic_cast<terrain_object*>(selectedObject);
		pedestalController.start(terrain);
	}
	if (pedestalController.is_running())
		return;
	const auto mouseWorldPos = mouse_position();
	inventorySystem.on_mouse_up();

	if (buildSystem.get_success_init())
		buildSystem.on_mouse_up();

	if (mouseDisplayName.empty())
		selectedObject = nullptr;

	auto hero = dynamic_cast<deerchant*>(dynamicGrid.get_item_by_name(focusedObject->get_name()));
	hero->on_mouse_up(currentMouseButton, selectedObject, mouseWorldPos, (buildSystem.building_position != Vector2f(-1, -1) && !buildSystem.instant_build));*/
}

void world_handler::handleEvents(Event& event)
{
	pedestalController.handle_events(event);
}

void world_handler::interact(Vector2f render_target_size, long long elapsedTime, Event event)
{
	birthObjects();

	const auto extra = staticGrid.get_block_size();
	const auto characterPosition = focusedObject->get_position();

	const Vector2f worldUpperLeft(characterPosition.x - render_target_size.x / 2, characterPosition.y - render_target_size.y / 2);
	const Vector2f worldBottomRight(characterPosition.x + render_target_size.x / 2, characterPosition.y + render_target_size.y / 2);

	worldGenerator.beyond_screen_generation();

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
	hero->held_item = &inventorySystem.get_held_item();

	for (auto& dynamicItem : localDynamicItems)
	{
		dynamicItem->move_system.is_route_needed(staticGrid.micro_block_matrix, microBlockSize);
		dynamicItem->move_system.make_route(elapsedTime, &staticGrid, dynamicItem->sight_range / (scale_system_.get_scale_factor() * scale_system_.get_main_scale()));
		dynamicItem->move_system.pass_route_beginning(microBlockSize);
		/* crutch */ if (dynamicItem->tag == entity_tag::hero)
			dynamicItem->move_system.move_position = dynamicItem->move_system.lax_move_position;

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
		//------------------------------						

		if (dynamicItem->shake_speed != -1)
			cameraSystem.make_shake(4, dynamicItem->shake_speed);

		auto newPosition = dynamicItem->move_system.do_move(elapsedTime);

		fixedClimbingBeyond(newPosition);

		newPosition = dynamicItem->move_system.do_slip(newPosition, localStaticItems, float(height), elapsedTime);

		if (!fixedClimbingBeyond(newPosition))
			newPosition = dynamicItem->get_position();
		dynamicItem->set_position(newPosition);
		dynamicGrid.update_item_position(dynamicItem->get_name(), newPosition.x, newPosition.y);
	}

	setItemFromBuildSystem();

	//buildSystem.setHeldItem(inventorySystem.getHeldItem()->lootInfo);
	//buildSystem.interact(cameraSystem.position, scale_system_.get_scale_factor());
	inventorySystem.interact(elapsedTime);
	pedestalController.interact(elapsedTime, event);

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
	//------------
}

bool cmp_img_draw(const unique_ptr<drawable_chain_element>& first, const unique_ptr<drawable_chain_element>& second)
{
	auto* const first_sprite = dynamic_cast<sprite_chain_element*>(first.get());
	auto* const second_sprite = dynamic_cast<sprite_chain_element*>(second.get());

	if (!first_sprite || !second_sprite)
		return true;

	if (first_sprite->is_background && !second_sprite->is_background)
		return true;
	if (!first_sprite->is_background && second_sprite->is_background)
		return false;

	if (first_sprite->z_coordinate == second_sprite->z_coordinate)
	{
		if (first_sprite->position.y == second_sprite->position.y)
		{
			if (first_sprite->position.x == second_sprite->position.x)
				return first_sprite->size.x * first_sprite->size.y < second_sprite->size.x * second_sprite->size.y;
			return first_sprite->position.x < second_sprite->position.x;
		}
		return first_sprite->position.y < second_sprite->position.y;
	}

	return first_sprite->z_coordinate < second_sprite->z_coordinate;
}

std::vector<std::unique_ptr<drawable_chain_element>> world_handler::prepare_sprites(const long long elapsed_time, const bool only_background)
{
	std::vector<std::unique_ptr<drawable_chain_element>> result{};

	const auto extra = staticGrid.get_block_size();

	const auto screenSize = helper::GetScreenSize();
	const auto screenCenter = Vector2f(screenSize.x / 2, screenSize.y / 2);

	cameraSystem.position.x += (focusedObject->get_position().x + helper::GetScreenSize().x * camera_system::cam_offset.x - cameraSystem.position.x) *
		(focusedObject->move_system.speed * float(elapsed_time)) / camera_system::max_camera_distance.x;

	cameraSystem.position.y += (focusedObject->get_position().y + helper::GetScreenSize().y * camera_system::cam_offset.y - cameraSystem.position.y) *
		(focusedObject->move_system.speed * float(elapsed_time)) / camera_system::max_camera_distance.y;

	cameraSystem.shake_interact(elapsed_time);

	const auto scale = scale_system_.get_scale_factor();
	worldUpperLeft = Vector2f(
		ceil(cameraSystem.position.x - (screenCenter.x + extra.x) / scale),
		ceil(cameraSystem.position.y - (screenCenter.y + extra.y) / scale));
	worldBottomRight = Vector2f(
		ceil(cameraSystem.position.x + (screenCenter.x + extra.x) / scale),
		ceil(cameraSystem.position.y + (screenCenter.y + extra.y) / scale));

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
		if ((only_background && item->is_background) || (!only_background && !item->is_background))
		{
			auto sprites = item->prepare_sprites(elapsed_time);
			result.insert(result.end(), std::make_move_iterator(sprites.begin()), std::make_move_iterator(sprites.end()));
		}
	}
	for (auto& item : localDynamicItems)
	{
		if (!only_background)
		{
			auto sprites = item->prepare_sprites(elapsed_time);
			result.insert(result.end(), std::make_move_iterator(sprites.begin()), std::make_move_iterator(sprites.end()));
		}
	}

	sort(result.begin(), result.end(), cmp_img_draw);

	/*auto deer = dynamic_cast<::dynamic_object*>(dynamicGrid.get_item_by_name("deer"));

	for (const auto block : staticGrid.get_item_by_name("brazier")->get_locked_micro_blocks())
	{
		const Vector2f shape_pos = { (block.x * microBlockSize.x - cameraSystem.position.x) * worldGenerator.scaleFactor + helper::GetScreenSize().x / 2,
			(block.y * microBlockSize.y - cameraSystem.position.y) * worldGenerator.scaleFactor + helper::GetScreenSize().y / 2 };

		result.push_back(new shape_chain_element(shape_pos, 7.5, { 3.75f, 3.75f }, sf::Color::Red));
	}

	for (const auto block : deer->route)
	{
		const Vector2f shape_pos = { (block.first * microBlockSize.x - cameraSystem.position.x) * worldGenerator.scaleFactor + helper::GetScreenSize().x / 2,
			(block.second * microBlockSize.y - cameraSystem.position.y) * worldGenerator.scaleFactor + helper::GetScreenSize().y / 2 };

		result.push_back(new shape_chain_element(shape_pos, 7, { 3.5f, 3.5f }));
	}

	const Vector2f shape_pos = { (long(deer->get_position().x) / long(microBlockSize.x) * long(microBlockSize.x) - cameraSystem.position.x) * worldGenerator.scaleFactor + helper::GetScreenSize().x / 2,
			(long(deer->get_position().y) / long(microBlockSize.y) * long(microBlockSize.y) - cameraSystem.position.y) * worldGenerator.scaleFactor + helper::GetScreenSize().y / 2 };

	result.push_back(new shape_chain_element(shape_pos, 7.5, { 3.75f, 3.75f }, sf::Color::Blue));*/

	return result;
}
