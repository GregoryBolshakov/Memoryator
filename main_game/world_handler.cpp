#define _USE_MATH_DEFINES

#include "world_handler.h"
#include "object_initializer.h"
#include "world_metrics.h"
#include "world_generator.h"
#include "camera_system.h"
#include "inventory_system.h"
#include "light_system.h"
#include "scale_system.h"
#include "grid_map.h"
#include "world_object.h"
#include "dynamic_object.h"
#include "brazier.h"
#include "deerchant.h"
#include "helper.h"
#include "drawable_chain_element.h"
#include "sprite_chain_element.h"
#include "sprite_pack.h"

#include <utility>

world_handler::world_handler(
	const shared_ptr<camera_system>& camera_system,
	shared_ptr<std::map<pack_tag, sprite_pack>> packs_map) :
	  camera_system_{ camera_system }
	, effect_system_{std::make_shared<effects_system>()}
	, inventory_system_{ std::make_shared<inventory_system>() }
	, light_system_{ std::make_shared<light_system>(sf::FloatRect(0, 0, helper::GetScreenSize().x, helper::GetScreenSize().y)) }
	, packs_map_ {std::move(packs_map)}
	, grid_map_{ make_unique<grid_map>() }
	, world_generator_{ make_shared<world_generator>(
		  grid_map_
		  , camera_system_.lock()->get_scale_system()
		  , this->packs_map_) }
{
	//buildSystem.init();
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

void world_handler::run_world_generator()
{
	world_generator_->primordial_generation();
	player_ = world_generator_->player();
	selected_object_ = world_generator_->player();
	camera_system_.lock()->set_focus(player_);
	main_object_ = dynamic_pointer_cast<brazier>(world_generator_->main_object().lock());
	camera_system_.lock()->position = world_metrics::center;
	world_generator_->fill_zone();

	const auto hero = dynamic_pointer_cast<deerchant>(player_.lock());
	inventory_system_->inventory_bounding(&hero->bags);
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

void world_handler::birth_objects()
{
	/*for (auto& object : local_terrain_)
	{
		auto birthStaticStack = object->get_birth_objects().first;
		auto birthDynamicStack = object->get_birth_objects().second;

		while (!birthStaticStack.empty())
		{
			const auto biome = world_generator_->biome_matrix[int(birthStaticStack.top().position.x / block_size_.x)][int(birthStaticStack.top().position.y / block_size_.y)];
			world_generator_->initialize_static_item(birthStaticStack.top().tag, birthStaticStack.top().position, birthStaticStack.top().type_of_object, "", birthStaticStack.top().count, biome, true, birthStaticStack.top().inventory);
			birthStaticStack.pop();
		}
		while (!birthDynamicStack.empty())
		{
			world_generator_->initialize_dynamic_item(birthDynamicStack.top().tag, birthDynamicStack.top().position, "", birthDynamicStack.top().owner);
			birthDynamicStack.pop();
		}
		object->clear_birth_stack();
	}*/
}

void world_handler::load()
{
	/*staticGrid = grid_map(this->width, this->height, blockSize, microBlockSize);
	dynamicGrid = grid_map(this->width, this->height, blockSize, microBlockSize);

	std::ifstream fin("save.txt");
	std::string saveName;
	float posX, posY;
	int num, typeOfObject;
	fin >> num;
	for (auto i = 0; i < num; i++)
	{
		fin >> saveName >> posX >> posY;

		if (saveName == nightmare_first("loadInit", sf::Vector2f(0, 0)).get_to_save_name())
			worldGenerator.initialize_dynamic_item(entity_tag::monster, sf::Vector2f(posX, posY), "");
		else
			if (saveName == deerchant("loadInit", sf::Vector2f(0, 0)).get_to_save_name())
				worldGenerator.initialize_dynamic_item(entity_tag::hero, sf::Vector2f(posX, posY), "");
			else
				if (saveName == wolf("loadInit", sf::Vector2f(0, 0)).get_to_save_name())
					worldGenerator.initialize_dynamic_item(entity_tag::wolf, sf::Vector2f(posX, posY), "");
	}

	fin >> num;
	for (auto i = 0; i < num; i++)
	{
		fin >> saveName >> typeOfObject >> posX >> posY;

		if (saveName == ground_connection("loadInit", sf::Vector2f(0, 0), typeOfObject).get_to_save_name())
			worldGenerator.initialize_static_item(entity_tag::groundConnection, sf::Vector2f(posX, posY), typeOfObject, "", 1);
		else
			if (saveName == ground("loadInit", sf::Vector2f(0, 0), typeOfObject).get_to_save_name())
				worldGenerator.initialize_static_item(entity_tag::ground, sf::Vector2f(posX, posY), typeOfObject, "", 1);
			else
				if (saveName == forest_tree("loadInit", sf::Vector2f(0, 0), typeOfObject).get_to_save_name())
					worldGenerator.initialize_static_item(entity_tag::tree, sf::Vector2f(posX, posY), typeOfObject, "", 1);
				else
					if (saveName == grass("loadInit", sf::Vector2f(0, 0), typeOfObject).get_to_save_name())
						worldGenerator.initialize_static_item(entity_tag::grass, sf::Vector2f(posX, posY), typeOfObject, "", 1);
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

void world_handler::save()
{
	/*if (static_grid_->get_size() == 0)
		return;

	std::ofstream fout("save.txt");
	fout.clear();
	auto static_items = object_initializer::vector_cast_to_static(static_grid_->get_items(0, 0, float(width_), float(height_)));
	auto dynamic_items = object_initializer::vector_cast_to_dynamic(dynamic_grid_->get_items(0, 0, float(width_), float(height_)));

	fout << dynamic_items.size() << std::endl;
	for (auto& dynamicItem : dynamic_items)
	{
		fout << dynamicItem->get_to_save_name() << " " << dynamicItem->get_position().x << " " << dynamicItem->get_position().y << std::endl;
	}
	fout << static_items.size() << std::endl;
	for (auto& staticItem : static_items)
	{
		fout << staticItem->get_to_save_name() << " " << staticItem->get_type() << " " << staticItem->get_position().x << " " << staticItem->get_position().y << std::endl;
	}
	fout.close();*/
}

Vector2f  world_handler::mouse_position() const
{
	const auto scale = camera_system_.lock()->get_scale_system()->calculate_scale();
	const auto mouse = sf::Vector2f(sf::Mouse::getPosition());
	const auto position = (mouse - helper::GetScreenSize() / 2.0f + camera_system_.lock()->position * scale) / scale;

	return position;
}

void world_handler::set_transparent(std::vector<shared_ptr<world_object>>& visible_items)
{
	mouse_display_name_ = "";
	const auto mousePos = mouse_position();
	auto minCapacity = 1e6f, minDistance = 1e6f;

	for (auto& visibleItem : visible_items)
	{
		if (!visibleItem || visibleItem->get_name() == player_.lock()->get_name() || visibleItem->intangible)
			continue;

		visibleItem->is_transparent = false;
		const auto itemPos = sf::Vector2f(visibleItem->get_position().x - visibleItem->get_texture_offset().x, visibleItem->get_position().y - visibleItem->get_texture_offset().y);

		if (mousePos.x >= itemPos.x && mousePos.x <= itemPos.x + visibleItem->get_conditional_size_units().x &&
			mousePos.y >= itemPos.y && mousePos.y <= itemPos.y + visibleItem->get_conditional_size_units().y)
		{
			visibleItem->is_selected = true;
		}
		else
			visibleItem->is_selected = false;

		if (!visibleItem->is_background && helper::isIntersects(mousePos, sf::FloatRect(itemPos.x, itemPos.y, visibleItem->get_conditional_size_units().x, visibleItem->get_conditional_size_units().y)) ||
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

				const auto terrain = dynamic_pointer_cast<terrain_object>(visibleItem);
				if (terrain && pedestal_controller.ready_to_start)
					mouse_display_name_ = "Set pedestal";
				else
					switch (visibleItem->tag)
					{
					case entity_tag::tree:
					{
						mouse_display_name_ = "Absorb";
						break;
					}
					case entity_tag::brazier:
					{
						if (inventory_system_->get_held_item().content.first != entity_tag::empty_cell &&
							helper::getDist(main_object_.lock()->getPlatePosition(), mousePos) <= main_object_.lock()->getPlateRadius())
						{
							if (helper::getDist(main_object_.lock()->getPlatePosition(), player_.lock()->get_position()) <= main_object_.lock()->getPlateRadius() + player_.lock()->get_radius())
								mouse_display_name_ = "Toss";
							else
								mouse_display_name_ = "Come to toss";
						}
						break;
					}
					case entity_tag::hare:
					case entity_tag::owl:
					{
						if (inventory_system_->get_held_item().content.first == entity_tag::inky_black_pen)
							mouse_display_name_ = "Sketch";
						else
							mouse_display_name_ = "Catch up";
						break;
					}
					case entity_tag::fern:
					{
						if (!visibleItem->inventory.empty())
							mouse_display_name_ = "Open";
						else
							mouse_display_name_ = "Pick up";
						break;
					}
					case entity_tag::yarrow:
					case entity_tag::chamomile:
					case entity_tag::mugwort:
					case entity_tag::noose:
					case entity_tag::hare_trap:
					case entity_tag::dropped_loot:
					{
						mouse_display_name_ = "Pick up";
						break;
					}
					default:
					{
						mouse_display_name_ = object_initializer::mapped_tags.at(visibleItem->tag);
						break;
					}
					}

				selected_object_ = visibleItem;
			}
		}

		if (player_.lock()->get_position().x >= itemPos.x && player_.lock()->get_position().x <= itemPos.x + visibleItem->get_conditional_size_units().x && player_.lock()->get_position().y >= itemPos.y && player_.lock()->get_position().y <= visibleItem->get_position().y && !visibleItem->is_background)
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

bool world_handler::fixed_climbing_beyond(sf::Vector2f& pos)
{
	if (pos.x < world_metrics::visible_zone.left)
	{
		pos.x = world_metrics::visible_zone.left;
		return false;
	}
	if (pos.x > world_metrics::visible_zone.left + world_metrics::visible_zone.width)
	{
		pos.x = world_metrics::visible_zone.left + world_metrics::visible_zone.width;
		return false;
	}
	if (pos.y < world_metrics::visible_zone.top)
	{
		pos.y = world_metrics::visible_zone.top;
		return false;
	}
	if (pos.y > world_metrics::visible_zone.top + world_metrics::visible_zone.height)
	{
		pos.y = world_metrics::visible_zone.top + world_metrics::visible_zone.height;
		return false;
	}
	return true;
}

void world_handler::set_item_from_build_system()
{
	/*if (!(buildSystem.instant_build || focusedObject->get_current_action() == builds))
		return;

	if (buildSystem.selected_object != entity_tag::emptyCell && buildSystem.building_position != sf::Vector2f(-1, -1))
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

		buildSystem.building_position = sf::Vector2f(-1, -1);
		buildSystem.build_type = 1;
		main_object->clearCurrentCraft();
	}*/
}

void world_handler::on_mouse_up(const int current_mouse_button)
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
	hero->on_mouse_up(currentMouseButton, selectedObject, mouseWorldPos, (buildSystem.building_position != sf::Vector2f(-1, -1) && !buildSystem.instant_build));*/
}

void world_handler::handle_events(sf::Event& event)
{
	pedestal_controller.handle_events(event);
}

Vector2f world_handler::interact_movement(const shared_ptr<dynamic_object>& dynamic_item, const long long elapsed_time) const
{
	auto new_position = dynamic_item->move_system.do_move(elapsed_time);
	fixed_climbing_beyond(new_position);

	new_position = dynamic_item->move_system.do_slip(new_position, world_generator_->all_static_objects(), world_metrics::constant_zone.height, elapsed_time);

	if (!fixed_climbing_beyond(new_position))
		new_position = dynamic_item->get_position();

	return new_position;
}

void world_handler::interact(const long long elapsed_time)
{
	birth_objects();

	const auto hero = world_generator_->all_dynamic_objects().at("hero");
	whole_world_offset_ = hero->get_position() - interact_movement(hero, elapsed_time);

	for (const auto& static_item : world_generator_->all_static_objects())
	{
		if (whole_world_offset_ != sf::Vector2f(0, 0))
			auto t = 1;
		const auto& mapped_item = world_generator_->all_static_objects().at(static_item.second->get_name());
		mapped_item->set_position(mapped_item->get_position() + whole_world_offset_);
	}
	
	for (const auto& dynamic_item : world_generator_->all_dynamic_objects())
	{
		if (dynamic_item.second->get_name() == "hero")
			continue;

		const auto& mapped_item = world_generator_->all_dynamic_objects().at(dynamic_item.second->get_name());
		mapped_item->set_position(interact_movement(dynamic_item.second, elapsed_time) + whole_world_offset_);
	}

	//for (auto& dynamicItem : localDynamicItems)
	//{
	//	dynamicItem->move_system.is_route_needed(static_grid_->micro_block_matrix, micro_block_size_);
	//	dynamicItem->move_system.make_route(elapsed_time, *static_grid_, dynamicItem->sight_range / (camera_system_->get_scale_system()->get_scale_factor() * camera_system_->get_scale_system()->get_main_scale()));
	//	dynamicItem->move_system.pass_route_beginning(micro_block_size_);
	//	/* crutch */ if (dynamicItem->tag == entity_tag::hero)
	//		dynamicItem->move_system.move_position = dynamicItem->move_system.lax_move_position;

	//	dynamicItem->behavior(elapsed_time);

	//	//interaction with other objects
	//	for (auto& otherDynamicItem : localDynamicItems)
	//	{
	//		if (dynamicItem == otherDynamicItem)
	//			continue;
	//		dynamicItem->set_target(*otherDynamicItem);
	//	}
	//	for (auto& otherDynamicItem : localDynamicItems)
	//	{
	//		if (dynamicItem == otherDynamicItem)
	//			continue;
	//		dynamicItem->behavior_with_dynamic(otherDynamicItem, elapsed_time);
	//	}
	//	for (auto& otherStaticItem : localStaticItems)
	//		dynamicItem->behavior_with_static(otherStaticItem, elapsed_time);
	//	//------------------------------

	//	if (dynamicItem->shake_speed != -1)
	//		camera_system_->make_shake(4, dynamicItem->shake_speed);

	//	auto newPosition = dynamicItem->move_system.do_move(elapsed_time);

	//	fixed_climbing_beyond(newPosition);

	//	newPosition = dynamicItem->move_system.do_slip(newPosition, localStaticItems, float(height_), elapsed_time);

	//	if (!fixed_climbing_beyond(newPosition))
	//		newPosition = dynamicItem->get_position();
	//	dynamicItem->set_position(newPosition);
	//	dynamic_grid_->update_item_position(dynamicItem->get_name(), newPosition.x, newPosition.y);
	//}

	//set_item_from_build_system();
	//buildSystem.setHeldItem(inventorySystem.getHeldItem()->lootInfo);
	//buildSystem.interact(cameraSystem.position, scale_system_->get_scale_factor());
	//inventory_system_->interact(elapsed_time);
	//pedestal_controller.interact(elapsed_time, event);
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

std::vector<std::unique_ptr<drawable_chain_element>> world_handler::prepare_sprites(const long long elapsed_time, const bool only_background) const
{
	std::vector<std::unique_ptr<drawable_chain_element>> result;

	camera_system_.lock()->shake_interact(elapsed_time);

	const auto scale = camera_system_.lock()->get_scale_system()->calculate_scale();

	for (const auto& item : world_generator_->all_static_objects())
	{
		if ((only_background && item.second->is_background) || (!only_background && !item.second->is_background))
		{
			auto sprites = item.second->prepare_sprites(elapsed_time);
			result.insert(result.end(), std::make_move_iterator(sprites.begin()), std::make_move_iterator(sprites.end()));
		}
	}
	for (const auto& item : world_generator_->all_dynamic_objects())
	{
		if (!only_background)
		{
			auto sprites = item.second->prepare_sprites(elapsed_time);
			result.insert(result.end(), std::make_move_iterator(sprites.begin()), std::make_move_iterator(sprites.end()));
		}
	}

	sort(result.begin(), result.end(), cmp_img_draw);

	/*auto deer = dynamic_cast<::dynamic_object*>(dynamicGrid.get_item_by_name("deer"));

	for (const auto block : staticGrid.get_item_by_name("brazier")->get_locked_micro_blocks())
	{
		const sf::Vector2f shape_pos = { (block.x * microBlockSize.x - cameraSystem.position.x) * worldGenerator.scaleFactor + helper::GetScreenSize().x / 2,
			(block.y * microBlockSize.y - cameraSystem.position.y) * worldGenerator.scaleFactor + helper::GetScreenSize().y / 2 };

		result.push_back(new shape_chain_element(shape_pos, 7.5, { 3.75f, 3.75f }, sf::Color::Red));
	}

	for (const auto block : deer->route)
	{
		const sf::Vector2f shape_pos = { (block.first * microBlockSize.x - cameraSystem.position.x) * worldGenerator.scaleFactor + helper::GetScreenSize().x / 2,
			(block.second * microBlockSize.y - cameraSystem.position.y) * worldGenerator.scaleFactor + helper::GetScreenSize().y / 2 };

		result.push_back(new shape_chain_element(shape_pos, 7, { 3.5f, 3.5f }));
	}

	const sf::Vector2f shape_pos = { (long(deer->get_position().x) / long(microBlockSize.x) * long(microBlockSize.x) - cameraSystem.position.x) * worldGenerator.scaleFactor + helper::GetScreenSize().x / 2,
			(long(deer->get_position().y) / long(microBlockSize.y) * long(microBlockSize.y) - cameraSystem.position.y) * worldGenerator.scaleFactor + helper::GetScreenSize().y / 2 };

	result.push_back(new shape_chain_element(shape_pos, 7.5, { 3.75f, 3.75f }, sf::Color::Blue));*/

	return result;
}
