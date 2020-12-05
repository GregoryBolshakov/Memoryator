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
#include "drawable_chain_element.h"
#include "sprite_chain_element.h"
#include "text_chain_element.h"
#include "sprite_pack.h"

#include <utility>

world_handler::world_handler(const shared_ptr<camera_system>& camera_system) :
	  camera_system_{ camera_system }
	, effect_system_{std::make_shared<effects_system>()}
	, inventory_system_{ std::make_shared<inventory_system>() }
	, light_system_{ std::make_shared<light_system>(sf::FloatRect(0, 0, world_metrics::window_size.x, world_metrics::window_size.y)) }
	, grid_map_{ make_unique<grid_map>() }
	, world_generator_{ make_shared<world_generator>(
		  grid_map_
		  , camera_system_.lock()->get_scale_system()) }
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
	world_metrics::set_world_metrics(world_metrics::window.lock());
	world_generator_->primordial_generation();
	player_ = world_generator_->all_dynamic_objects().at("hero");
	mouse_selected_object_ = world_generator_->player();
	camera_system_.lock()->set_focus(player_);
	main_object_ = dynamic_pointer_cast<brazier>(world_generator_->main_object().lock());

	world_metrics::update_scale(camera_system_.lock()->get_scale_system()->calculate_scale());
	player_.lock()->set_position(world_metrics::center);
	// here the size of all matrices will be calculated
	grid_map_->init_matrices();
	world_generator_->fill_inner_zone();
	grid_map_->add_constant_block(grid_map_->get_block_by_point(main_object_.lock()->get_position()));

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
		fout << staticItem->get_to_save_name() << " " << staticItem->get_kind() << " " << staticItem->get_position().x << " " << staticItem->get_position().y << std::endl;
	}
	fout.close();*/
}

void world_handler::mouse_selection_logic(const shared_ptr<world_object>& visible_item)
{
	const auto mouse_pos = world_metrics::screen_to_world_position(Vector2f(Mouse::getPosition()));

	if (!visible_item || visible_item->get_name() == player_.lock()->get_name() || visible_item->intangible)
		return;

	visible_item->is_transparent = false;
	const FloatRect item_rect(
		visible_item->get_position().x - visible_item->get_offset().x,
		visible_item->get_position().y - visible_item->get_offset().y,
		visible_item->get_size().x,
		visible_item->get_size().y);

	if (visible_item->is_background || !item_rect.contains(mouse_pos))
		visible_item->is_selected = false;
	else
	{
		visible_item->is_selected = true;

		const auto item_capacity = visible_item->get_size().x + visible_item->get_size().y;

		const auto distance_to_item_center = abs(mouse_pos.x - (item_rect.left + visible_item->get_size().x / 2)) +
			abs(mouse_pos.y - (item_rect.top + visible_item->get_size().y / 2));

		if (distance_to_item_center < min_selection_distance_ || distance_to_item_center == min_selection_distance_ && item_capacity < min_selection_capacity_)
		{
			min_selection_capacity_ = item_capacity;
			min_selection_distance_ = distance_to_item_center;

			const auto terrain = dynamic_pointer_cast<terrain_object>(visible_item);
			if (terrain && pedestal_controller.ready_to_start)
				mouse_display_name_ = "Set pedestal";
			else
				switch (visible_item->tag)
				{
				case entity_tag::tree:
				{
					mouse_display_name_ = "Absorb";
					break;
				}
				case entity_tag::brazier:
				{
					if (inventory_system_->get_held_item().content.first != entity_tag::empty_cell &&
						world_metrics::get_dist(main_object_.lock()->getPlatePosition(), mouse_pos) <= main_object_.lock()->getPlateRadius())
					{
						if (world_metrics::get_dist(main_object_.lock()->getPlatePosition(), player_.lock()->get_position()) <= main_object_.lock()->getPlateRadius() + player_.lock()->get_radius())
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
					if (!visible_item->inventory.empty())
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
					mouse_display_name_ = object_initializer::mapped_tags.at(visible_item->tag);
					break;
				}
				}

			mouse_selected_object_ = visible_item;
		}
	}

	if (player_.lock()->get_position().x >= item_rect.left && player_.lock()->get_position().x <= item_rect.left + visible_item->get_size().x && player_.lock()->get_position().y >= item_rect.top && player_.lock()->get_position().y <= visible_item->get_position().y && !visible_item->is_background)
	{
		visible_item->is_transparent = true;
		if (visible_item->color.a > 125)
			visible_item->color.a -= 1;
	}
	else
	{
		if (visible_item->color.a < 255)
			visible_item->color.a += 1;
	}
}

bool world_handler::fixed_climbing_beyond(sf::Vector2f& pos)
{
	if (pos.x < world_metrics::constant_zone.left)
	{
		pos.x = world_metrics::constant_zone.left;
		return false;
	}
	if (pos.x > world_metrics::constant_zone.left + world_metrics::constant_zone.width)
	{
		pos.x = world_metrics::constant_zone.left + world_metrics::constant_zone.width;
		return false;
	}
	if (pos.y < world_metrics::constant_zone.top)
	{
		pos.y = world_metrics::constant_zone.top;
		return false;
	}
	if (pos.y > world_metrics::constant_zone.top + world_metrics::constant_zone.height)
	{
		pos.y = world_metrics::constant_zone.top + world_metrics::constant_zone.height;
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
	if (mouse_display_name_ == "Set pedestal")
		if (const auto terrain = dynamic_pointer_cast<terrain_object>(mouse_selected_object_.lock()))
			pedestal_controller.start(terrain);
	if (pedestal_controller.is_running())
		return;
	const auto mouseWorldPos = world_metrics::screen_to_world_position(Vector2f(Mouse::getPosition()));
	//inventorySystem.on_mouse_up();

	//if (buildSystem.get_success_init())
		//buildSystem.on_mouse_up();

	//if (mouseDisplayName.empty())
		//selectedObject = nullptr;

	//auto hero = dynamic_cast<deerchant*>(dynamicGrid.get_item_by_name(focusedObject->get_name()));
	//hero->on_mouse_up(currentMouseButton, selectedObject, mouseWorldPos, (buildSystem.building_position != sf::Vector2f(-1, -1) && !buildSystem.instant_build));
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
	camera_system_.lock()->interact(elapsed_time);
	pedestal_controller.interact(elapsed_time);

	min_selection_capacity_ = 1e6f; min_selection_distance_ = 1e6f;
	mouse_display_name_ = ""; mouse_selected_object_.reset();

	whole_world_offset_ = world_metrics::center - interact_movement(player_.lock(), elapsed_time);
	world_metrics::constant_zone_no_scale.left += whole_world_offset_.x; world_metrics::constant_zone_no_scale.top += whole_world_offset_.y;
	player_.lock()->set_position(world_metrics::center);

	world_generator_->interact();

	for (const auto& mapped_static_item : world_generator_->all_static_objects())
	{
		mapped_static_item.second->set_position(mapped_static_item.second->get_position() + whole_world_offset_);
		mouse_selection_logic(mapped_static_item.second);
	}
	
	for (const auto& mapped_dynamic_item : world_generator_->all_dynamic_objects())
	{
		const auto& dynamic_item = mapped_dynamic_item.second;
		
		if (dynamic_item->get_name() != "hero")
			dynamic_item->set_position(interact_movement(dynamic_item, elapsed_time) + whole_world_offset_);

		dynamic_item->behavior(elapsed_time);

		for (const auto& mapped_other_dynamic_item : world_generator_->all_dynamic_objects())
		{
			const auto& other_dynamic_item = mapped_other_dynamic_item.second;
			if (dynamic_item == other_dynamic_item)
				continue;
			dynamic_item->behavior_with_dynamic(other_dynamic_item, elapsed_time);
		}

		for (const auto& mapped_static_item : world_generator_->all_static_objects())
			dynamic_item->behavior_with_static(mapped_static_item.second, elapsed_time);

		mouse_selection_logic(dynamic_item);
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

std::vector<std::unique_ptr<drawable_chain_element>> world_handler::prepare_sprites(const long long elapsed_time, const bool only_background)
{
	std::vector<std::unique_ptr<drawable_chain_element>> result;

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

	// draw pedestal controller
	auto pedestal_controller_sprites = pedestal_controller.prepare_sprites();
	result.insert(result.end(), std::make_move_iterator(pedestal_controller_sprites.begin()), std::make_move_iterator(pedestal_controller_sprites.end()));
	// draw mouse selected string
	if (const auto object = mouse_selected_object_.lock())
		result.push_back(std::make_unique<text_chain_element>(Vector2f(sf::Mouse::getPosition()), Vector2f(), Color::White, mouse_display_name_));

	/*auto deer = dynamic_cast<::dynamic_object*>(dynamicGrid.get_item_by_name("deer"));

	for (const auto block : staticGrid.get_item_by_name("brazier")->get_locked_micro_blocks())
	{
		const sf::Vector2f shape_pos = { (block.x * microBlockSize.x - cameraSystem.position.x) * worldGenerator.scaleFactor + world_metrics::window_size.x / 2,
			(block.y * microBlockSize.y - cameraSystem.position.y) * worldGenerator.scaleFactor + world_metrics::window_size.y / 2 };

		result.push_back(new shape_chain_element(shape_pos, 7.5, { 3.75f, 3.75f }, sf::Color::Red));
	}

	for (const auto block : deer->route)
	{
		const sf::Vector2f shape_pos = { (block.first * microBlockSize.x - cameraSystem.position.x) * worldGenerator.scaleFactor + world_metrics::window_size.x / 2,
			(block.second * microBlockSize.y - cameraSystem.position.y) * worldGenerator.scaleFactor + world_metrics::window_size.y / 2 };

		result.push_back(new shape_chain_element(shape_pos, 7, { 3.5f, 3.5f }));
	}

	const sf::Vector2f shape_pos = { (long(deer->get_position().x) / long(microBlockSize.x) * long(microBlockSize.x) - cameraSystem.position.x) * worldGenerator.scaleFactor + world_metrics::window_size.x / 2,
			(long(deer->get_position().y) / long(microBlockSize.y) * long(microBlockSize.y) - cameraSystem.position.y) * worldGenerator.scaleFactor + world_metrics::window_size.y / 2 };

	result.push_back(new shape_chain_element(shape_pos, 7.5, { 3.75f, 3.75f }, sf::Color::Blue));*/

	return result;
}
