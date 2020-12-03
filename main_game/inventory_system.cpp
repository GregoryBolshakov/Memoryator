#include "inventory_system.h"
#include "text_chain_element.h"
#include "world_object.h"
#include "direction_system.h"
#include "world_metrics.h"
#include "sprite_pack.h"

#include<fstream>

inventory_system::inventory_system()
{
	held_item_speed_ = 0.00005F;
	drop_zone_radius_ = world_metrics::window_size.y * 2 / 7;
	held_item_.content = { entity_tag::empty_cell, 0 };
	bag_pos_dot.setRadius(world_metrics::window_size.y / 288);
	bag_pos_dot.setFillColor(sf::Color(53, 53, 53, 200));
	init_max_counts();
	success_init_ = true;
}

inventory_system::~inventory_system()
= default;

void inventory_system::init_max_counts(const std::string& file_path)
{
	std::ifstream file(file_path);

	auto tag_number = 508;
	auto max_count = 1;
	while (file >> tag_number >> max_count)
	{
		hero_bag::items_max_count[entity_tag(tag_number)] = max_count;
	}

	file.close();
}

void inventory_system::move_other_bags(const int cur) const
{
	const auto laps_count = 20;
	//if (boundBags->at(cur).currentState != bagClosed)
	//{
	for (auto& another_bag : *bound_bags_)
	{
		if (another_bag.get_position() == bound_bags_->at(cur).get_position() || another_bag.was_moved)
		{
			continue;
		}
		const auto new_pos = sf::Vector2f(bound_bags_->at(cur).get_position().x + bound_bags_->at(cur).shift_vector.x, bound_bags_->at(cur).get_position().y + bound_bags_->at(cur).shift_vector.y);

		if (world_metrics::get_dist(new_pos, another_bag.get_position()) < bound_bags_->at(cur).get_radius() + another_bag.get_radius())
		{
			const auto k = 0.05F * laps_count * (bound_bags_->at(cur).get_radius() + another_bag.get_radius() - world_metrics::get_dist(new_pos, another_bag.get_position())) / world_metrics::get_dist(
				new_pos,
				another_bag.get_position());
			another_bag.shift_vector = sf::Vector2f((another_bag.get_position().x - new_pos.x) * k, (another_bag.get_position().y - new_pos.y) * k);

			const auto temp_new_pos = sf::Vector2f(another_bag.get_position().x + another_bag.shift_vector.x, another_bag.get_position().y + another_bag.shift_vector.y);
			another_bag.move_position = temp_new_pos;
			another_bag.shift_vector.x = 0;
			another_bag.shift_vector.y = 0;
			//break;
		}
	}
	//}
}

void inventory_system::interact(const long long elapsed_time)
{
	const auto mouse_pos = sf::Vector2f(sf::Mouse::getPosition());

	auto cnt = -1;
	min_dist_to_closed_ = 10e4;
	min_dist_to_open_ = 10e4;

	for (auto& bag : *bound_bags_)
	{
		cnt++;

		if (cursor_text == "throw away" && world_metrics::get_dist(bag.get_position(), sf::Vector2f(world_metrics::window_size.x / 2, world_metrics::window_size.y / 2)) <= drop_zone_radius_ ||
			cursor_text.empty())
		{
			if (cnt == current_moving_bag_ || current_moving_bag_ == -1)
			{
				bag.mouse_move();
			}
		}

		//move other bags while opening
		move_other_bags(cnt);
		//-----------------------------

		// bag auto-moving
		bag.fix_pos();
		auto new_pos = bag.get_position();
		const sf::Vector2f shift = {bag.move_position.x - bag.get_position().x, bag.move_position.y - bag.get_position().y};
		const auto time = float(elapsed_time);

		if (bag.move_position.x != -1 && bag.move_position.y != -1)
		{
			const auto k = bag.speed * time / world_metrics::get_dist(bag.get_position(), bag.move_position);
			new_pos.x = bag.get_position().x + shift.x * k;
			new_pos.y = bag.get_position().y + shift.y * k;
		}
		else
		{
			if (bag.move_position.x == -1 && bag.move_position.y != -1)
			{
				new_pos.y += bag.speed * time / shift.y * abs(shift.y) / 2;
				bag.move_position.x = bag.get_position().x;
			}
			if (bag.move_position.y == -1 && bag.move_position.x != -1)
			{
				new_pos.x += bag.speed * time / shift.x * abs(shift.x) / 2;
				bag.move_position.y = bag.get_position().y;
			}
		}
		if (world_metrics::get_dist(bag.get_position(), bag.move_position) <= bag.speed * time)
		{
			new_pos = bag.move_position;
		}
		bag.set_position(new_pos);
		bag.fix_cells();
		//----------------

		// bag selection
		if (bag.current_state == bag_closed)
		{
			const auto selection_pos = sf::Vector2f(bag.get_position().x + bag.selection_zone_closed_offset.x, bag.get_position().y + bag.selection_zone_closed_offset.y);
			bag.ready_to_change_state = world_metrics::get_dist(mouse_pos, selection_pos) <= bag.closed_radius;
		}
		else if (bag.current_state == bag_open)
		{
			const auto selection_pos = sf::Vector2f(bag.get_position().x + bag.selection_zone_opened_offset.x, bag.get_position().y + bag.selection_zone_opened_offset.y);
			bag.ready_to_change_state = world_metrics::get_dist(mouse_pos, selection_pos) <= bag.opened_radius;
		}
		//--------------
	}

	// nearest bag
	if (current_moving_bag_ == -1)
	{
		for (auto& bound_bag : *bound_bags_)
		{
			if (world_metrics::get_dist(mouse_pos, bound_bag.get_position()) <= min_dist_to_closed_ && bound_bag.current_state == bag_closed)
			{
				min_dist_to_closed_ = world_metrics::get_dist(mouse_pos, bound_bag.get_position());
			}
			if (world_metrics::get_dist(mouse_pos, bound_bag.get_position()) <= min_dist_to_open_ && bound_bag.current_state == bag_open)
			{
				min_dist_to_open_ = world_metrics::get_dist(mouse_pos, bound_bag.get_position());
			}
		}
	}

	if (held_item_.content.first != entity_tag::empty_cell)
	{
		const auto shift_vector = sf::Vector2f(sf::Mouse::getPosition().x - held_item_.position.x, sf::Mouse::getPosition().y - held_item_.position.y);
		held_item_.position.x += shift_vector.x;
		held_item_.position.y += shift_vector.y;
	}

	effects_system_.interact(elapsed_time);
}

void inventory_system::crash_into_other_bags(const int cnt) const
{
	auto laps_count = 20.0F;
	while (true)
	{
		auto is_break = true;
		for (auto& another_bag : *bound_bags_)
		{
			if (another_bag.get_position() == bound_bags_->at(cnt).get_position())
			{
				continue;
			}
			const auto new_pos = sf::Vector2f(bound_bags_->at(cnt).get_position().x + bound_bags_->at(cnt).shift_vector.x, bound_bags_->at(cnt).get_position().y + bound_bags_->at(cnt).shift_vector.y);

			if (world_metrics::get_dist(new_pos, another_bag.get_position()) < bound_bags_->at(cnt).get_radius() + another_bag.get_radius())
			{
				if (bound_bags_->at(cnt).current_state == bag_closed)
				{
					const auto k = 0.05F * laps_count * (bound_bags_->at(cnt).get_radius() + another_bag.get_radius() - world_metrics::get_dist(new_pos, another_bag.get_position())) / world_metrics::get_dist(
						new_pos,
						another_bag.get_position());
					bound_bags_->at(cnt).shift_vector = sf::Vector2f((new_pos.x - another_bag.get_position().x) * k, (new_pos.y - another_bag.get_position().y) * k);
					is_break = false;
					break;
				}
			}
		}
		if (is_break)
		{
			break;
		}
		laps_count++;
	}

	const auto new_pos = sf::Vector2f(bound_bags_->at(cnt).get_position().x + bound_bags_->at(cnt).shift_vector.x, bound_bags_->at(cnt).get_position().y + bound_bags_->at(cnt).shift_vector.y);
	bound_bags_->at(cnt).move_position = new_pos;
	bound_bags_->at(cnt).shift_vector.x = 0;
	bound_bags_->at(cnt).shift_vector.y = 0;
}

void inventory_system::on_mouse_up()
{
	used_mouse_ = false;
	const auto mouse_pos = sf::Vector2f(sf::Mouse::getPosition());

	if (picked_cell_ != nullptr)
	{
		hero_bag::put_item_in(picked_cell_, bound_bags_);
	}

	auto cnt = -1;
	for (auto& bag : *bound_bags_)
	{
		cnt++;
		// crash into other bags
		if (bag.current_state == bag_closed && bag.was_moved)
		{
			crash_into_other_bags(cnt);
		}
		//----------------------

		// bag state changing
		if (cnt == current_moving_bag_ || current_moving_bag_ == -1)
		{
			used_mouse_ = true;
			if (bag.ready_to_eject)
			{
				bag.current_state = ejected;
			}
			if (bag.current_state != bag_open)
			{
				if (bag.current_state == bag_closed && bag.ready_to_change_state && !bag.was_moved)
				{
					bag.current_state = bag_opening;
					bag.was_moved = false;
					continue;
				}
			}
			else
			{
				if (bag.current_state == bag_open && bag.ready_to_change_state)
				{
					bag.current_state = bag_closing;
					bag.was_moved = false;
					continue;
				}
			}
		}
		bag.was_moved = false;
		if (bag.current_state != bag_open)
		{
			continue;
		}
		//-------------------

		// put cursor item to bag
		if (held_item_.content.first != entity_tag::empty_cell)
		{
			const auto cur_index = bag.get_selected_cell(mouse_pos);
			if (cur_index == -1)
			{
				continue;
			}
			auto& item = bag.cells[cur_index];
			if (item.content.first == entity_tag::empty_cell || item.content.first == held_item_.content.first)
			{
				item.content.first = held_item_.content.first;
				item.content.second += held_item_.content.second;
				if (item.content.second > hero_bag::items_max_count.at(entity_tag(item.content.first)))
				{
					held_item_.content.second = item.content.second % hero_bag::items_max_count.at(entity_tag(item.content.first));
					item.content.second = hero_bag::items_max_count.at(entity_tag(item.content.first));
				}
				else
				{
					held_item_.content = {entity_tag::empty_cell, 0};
				}
				break;
			}
			const auto temp = held_item_.content;
			held_item_.content = item.content;
			item.content = temp;
		}
		else
		{
			const auto cur_index = bag.get_selected_cell(mouse_pos);
			if (cur_index != -1)
			{
				held_item_.content = bag.cells[cur_index].content;
				held_item_.position = bag.cells[cur_index].position;
				bag.cells[cur_index].content = {entity_tag::empty_cell, 0};
			}
		}
		//-----------------------
	}
	current_moving_bag_ = -1;
}

std::vector<std::unique_ptr<drawable_chain_element>> inventory_system::prepare_sprites(const long long elapsed_time)
{
	std::vector<std::unique_ptr<drawable_chain_element>> result;
	used_mouse_ = false;
	const auto mouse_pos = sf::Vector2f(sf::Mouse::getPosition());
	// draw bags
	auto cnt = -1;
	auto cursor_turned_on = false;
	//cursorText = "";
	bag_pos_dot.setPosition(0, 0);
	for (auto& bag : *bound_bags_)
	{
		cnt++;
		result.push_back(std::move(bag.prepare_sprite(elapsed_time)));
		bag.ready_to_eject = false;
		if (bag.was_moved)
		{
			current_moving_bag_ = cnt;
		}
		const auto cur_index = bag.get_selected_cell(mouse_pos);

		if (bag.ready_to_change_state || held_item_.content.first != entity_tag::empty_cell || cur_index != -1)
		{
			used_mouse_ = true;
		}

		// dropping bag
		if (world_metrics::get_dist(bag.get_position(), sf::Vector2f(world_metrics::window_size.x / 2, world_metrics::window_size.y / 2)) <= drop_zone_radius_ && bag.current_state == bag_closed)
		{
			cursor_turned_on = true;
			//if (cursor_text.empty())
				//effects_system_.add_effect(effects::transparency_removal, &drop_zone, "dropZone", long(3 * 10e4));			
			cursor_text = "throw away";
			cursor_text_pos_ = bag.get_position();
			bag.ready_to_eject = true;
			bag_pos_dot.setPosition(bag.get_position());
		}
		//-------------

		if (cnt == current_moving_bag_)
		{
			bag_pos_dot.setPosition(bag.get_position());
		}

		// drawing bag content
		if (bag.current_state != bag_open)
		{
			continue;
		}
		for (auto& cell : bag.cells)
		{
			const auto item = cell;

			//drawing cell background
			auto icon_background = sprite_pack::tag_to_icon(entity_tag::empty_object, false);
			icon_background->position = item.position;
			result.push_back(std::move(icon_background));
			//-----------------------

			if (cell.content.first == entity_tag::empty_cell)
			{
				continue;
			}

			auto icon = sprite_pack::tag_to_icon(item.content.first, world_metrics::get_dist(mouse_pos, cell.position) <= sprite_pack::icon_size.x / 2, 1);
			icon->position = item.position;
			result.push_back(std::move(icon));

			if (hero_bag::items_max_count.at(item.content.first) != 1)
			{
				result.emplace_back(std::make_unique<text_chain_element>(
					icon->position,
					/*{ -SpritePack::iconWithoutSpaceSize.x / 2, -SpritePack::iconWithoutSpaceSize.y / 2 },*/
					sf::Vector2f(0, 0),
					sf::Color(255, 255, 255, 180),
					std::to_string(item.content.second),
					text_chain_element::default_character_size * 1.5F));
			}
		}
		//--------------------
	}
	//----------

	//drawing held item
	if (held_item_.content.first != entity_tag::empty_cell)
	{
		auto held_item_icon = sprite_pack::tag_to_icon(held_item_.content.first, true, 1);
		held_item_icon->position = held_item_.position;
		result.push_back(std::move(held_item_icon));
		result.push_back(std::move(held_item_icon));
		if (hero_bag::items_max_count.at(held_item_.content.first) != 1)
		{
			result.push_back(std::make_unique<text_chain_element>(
				held_item_icon->position,
				/*{ -SpritePack::iconWithoutSpaceSize.x / 2, -SpritePack::iconWithoutSpaceSize.y / 2 },*/
				sf::Vector2f(0, 0),
				sf::Color(255, 255, 255, 180),
				std::to_string(held_item_.content.second),
				text_chain_element::default_character_size * 1.5F));
		}
	}
	//-----------------

	// draw cursor text
	if (!cursor_turned_on)
	{
		cursor_text = "";
		effects_system_.reset_effects({"dropZone"});
	}
	if (cursor_text == "throw away")
	{
		result.push_back(std::make_unique<text_chain_element>(
			cursor_text_pos_,
			sf::Vector2f(text_system::get_text_box_size(cursor_text, text_chain_element::default_character_size * 1.5F, font_name::normal_font).x / 2.0F, 0),
			sf::Color(0, 0, 0, 180),
			cursor_text,
			text_chain_element::default_character_size * 1.5F));
		result.push_back(std::make_unique<sprite_chain_element>(pack_tag::inventory, pack_part::areas, direction::DOWN, 2, sf::Vector2f(0, 0), world_metrics::window_size));
	}
	//if (bagPosDot.getPosition() != sf::Vector2f(0, 0))
	//window.draw(bagPosDot);
	return result;
}

/*void InventorySystem::drawInventory(std::vector<std::pair<Tag, int>>* inventory, sf::Vector2f position, RenderWindow& window)
{
	for (int i = 0; i < inventory->size(); i++)
		if (inventory->at(i).first == Tag::emptyCell || inventory->at(i).second == 0)
			inventory->erase(inventory->begin() + i);

	const sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition());
	//const sf::FloatRect cellRect = cellsSpriteList.at(Tag::chamomile).sprite.getGlobalBounds();	
	int maxInRaw;
	if (inventory->size() > 3)
		maxInRaw = sqrt(inventory->size());
	else
		maxInRaw = inventory->size();
	int raw = 0, column = 0;
	while (raw * maxInRaw + column < inventory->size())
	{		
		//drawing cell background		
		if (!(mousePos.x < position.x || mousePos.y < position.y))
			if (int(mousePos.x - position.x) / int(cellRect.width) == column && 
				int(mousePos.y - position.y) / int(cellRect.height) == raw)
			{
				pickedCell = &(inventory->at(raw * maxInRaw + column));
				const sf::Vector2f backgroundOffset = cellsSpriteList.at(Tag::emptyCell).offset;
				selectedCellBackground->setPosition(position.x + column * cellRect.width - backgroundOffset.x, position.y + raw * cellRect.height - backgroundOffset.y);
				window.draw(*selectedCellBackground);
			}
		//-----------------------

		if (inventory->at(raw * maxInRaw + column).first != Tag::emptyCell)
		{
			if (cellsSpriteList.count(Tag(inventory->at(raw * maxInRaw + column).first)) > 0)
			{
				auto sprite = cellsSpriteList.at(Tag(inventory->at(raw * maxInRaw + column).first)).sprite;
				sprite.setScale(HeroBag::itemCommonRadius * 2.6 / sprite.getGlobalBounds().width, HeroBag::itemCommonRadius * 2.6 / sprite.getGlobalBounds().height);
				const sf::Vector2f offset = cellsSpriteList.at(Tag(inventory->at(raw * maxInRaw + column).first)).offset;
				sprite.setPosition(sf::Vector2f(position.x + column * cellRect.width - offset.x, position.y + raw * cellRect.width - offset.y));
				sprite.setColor(Color(sprite.getColor().r, sprite.getColor().g, sprite.getColor().b, 255));
				window.draw(sprite);

				if (HeroBag::itemsMaxCount.at(inventory->at(raw * maxInRaw + column).first) != 1)
					textWriter.drawNumberOfItems(sprite.getPosition(), inventory->at(raw * maxInRaw + column).second, window);
			}
		}
		column++;
		if (column >= maxInRaw)
		{
			raw++;
			column = 0;
		}
	}
}*/

void inventory_system::reset_animation_values()
{
	animation_counter_ = 0;
}
