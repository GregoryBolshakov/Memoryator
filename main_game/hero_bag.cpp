#include "hero_bag.h"
#include "helper.h"

hero_bag::hero_bag()
{
	state_change_time = 100000;
}

hero_bag::~hero_bag()
= default;

std::unordered_map<entity_tag, int> hero_bag::items_max_count = std::unordered_map<entity_tag, int>();

std::vector<std::pair<entity_tag, int>> hero_bag::test_inventory =
{ {entity_tag::noose, 1}, {entity_tag::yarrow, 3}, {entity_tag::emptyCell, 0}, {entity_tag::inkyBlackPen, 1}, {entity_tag::hare, 1}, {entity_tag::chamomile, 2}, {entity_tag::emptyCell, 0} };
std::vector<std::pair<entity_tag, int>> hero_bag::empty_inventory =
{ {entity_tag::emptyCell, 0}, {entity_tag::emptyCell, 0}, {entity_tag::emptyCell, 0}, {entity_tag::emptyCell, 0}, {entity_tag::emptyCell, 0}, {entity_tag::emptyCell, 0}, {entity_tag::emptyCell, 0} };

void hero_bag::initialize(const Vector2f position, const bool is_selectable, std::vector<std::pair<entity_tag, int>> inventory)
{
	size_closed_ = Vector2f(helper::GetScreenSize().x / 12.0f, helper::GetScreenSize().y / 6.0f);
	size_open_ = Vector2f(helper::GetScreenSize().x / 6.0f, helper::GetScreenSize().y / 3.0f);

	this->position_ = position;
	last_mouse_pos_ = position;

	this->is_selectable_ = is_selectable;

	texture_closed_offset = Vector2f(size_closed_.x / 2.0f, size_closed_.y / 1.7f);
	texture_open_offset = Vector2f(size_open_.x / 2.0f, size_open_.y / 1.7f);

	bag_sprite_chain_.closed_bag->size = size_closed_; bag_sprite_chain_.closed_bag_selected->size = size_closed_; bag_sprite_chain_.closed_bag_big->size = size_closed_;
	bag_sprite_chain_.opened_bag->size = size_open_; bag_sprite_chain_.opened_bag_selected->size = size_open_;
	bag_sprite_chain_.closed_bag->offset = texture_closed_offset; bag_sprite_chain_.closed_bag_selected->offset = texture_closed_offset; bag_sprite_chain_.closed_bag_big->offset = texture_closed_offset;
	bag_sprite_chain_.opened_bag->offset = texture_open_offset; bag_sprite_chain_.opened_bag_selected->offset = texture_open_offset;
	bag_sprite_chain_.closed_bag->position = position; bag_sprite_chain_.closed_bag_selected->position = position; bag_sprite_chain_.closed_bag_big->position = position;
	bag_sprite_chain_.opened_bag->position = position; bag_sprite_chain_.opened_bag_selected->position = position;
	bag_sprite_chain_.closed_bag->initialize(); bag_sprite_chain_.closed_bag_selected->initialize(); bag_sprite_chain_.closed_bag_big->initialize();
	bag_sprite_chain_.opened_bag->initialize(); bag_sprite_chain_.opened_bag_selected->initialize();

	this->min_dist_to_border = std::max(size_closed_.y - texture_closed_offset.y, texture_closed_offset.y);
	closed_radius = (size_closed_.x + size_closed_.y) / 4;
	opened_radius = (size_open_.x + size_open_.y) / 10;

	this->selection_zone_closed_offset = Vector2f(0, -size_closed_.y * 0.15f);
	this->selection_zone_opened_offset = Vector2f(0, -texture_open_offset.y + size_open_.y * 0.15f);

	if (inventory.empty())
		inventory = empty_inventory;

	for (auto i = 0; i < 7; i++)
	{
		cells_pos_[i].x *= size_open_.x; cells_pos_[i].y *= size_open_.y;
		cells.push_back(create_cell(Vector2f(position.x + cells_pos_[i].x, position.y + cells_pos_[i].y), inventory[i]));
	}

	// set sprites from pack
	if (bag_sprite_chain_.closed_bag->pack_tag == pack_tag::empty)
		bag_sprite_chain_.closed_bag->set_draw_info(pack_tag::inventory, pack_part::bag1, direction::DOWN, 1);
	if (bag_sprite_chain_.closed_bag_selected->pack_tag == pack_tag::empty)
		bag_sprite_chain_.closed_bag_selected->set_draw_info(pack_tag::inventory, pack_part::bag1, direction::DOWN, 2);
	if (bag_sprite_chain_.closed_bag_big->pack_tag == pack_tag::empty)
		bag_sprite_chain_.closed_bag_big->set_draw_info(pack_tag::inventory, pack_part::bag1, direction::DOWN, 3);
	if (bag_sprite_chain_.opened_bag->pack_tag == pack_tag::empty)
		bag_sprite_chain_.opened_bag->set_draw_info(pack_tag::inventory, pack_part::bag1, direction::DOWN, 4);
	if (bag_sprite_chain_.opened_bag_selected->pack_tag == pack_tag::empty)
		bag_sprite_chain_.opened_bag_selected->set_draw_info(pack_tag::inventory, pack_part::bag1, direction::DOWN, 5);
	//----------------------
}

cell hero_bag::create_cell(const Vector2f position, const std::pair<entity_tag, int> content)
{
	cell cell;
	cell.position = position;
	cell.content = content;
	return cell;
}

int hero_bag::get_selected_cell(const Vector2f position)
{
	for (auto i = 0u; i < cells.size(); i++)
	{
		if (helper::getDist(position, cells[i].position) <= sprite_pack::icon_size.x / 2)
			return i;
	}
	return -1;
}

float hero_bag::get_radius() const
{
	if (current_state == bag_closed)
		return bag_sprite_chain_.closed_bag->size.x / 2;

	if (current_state == bag_opening)
		return bag_sprite_chain_.closed_bag_selected->size.x / 2;

	if (current_state == bag_open)
		return bag_sprite_chain_.opened_bag->size.x / 2;

	return 0;
}

void hero_bag::draw_circuit(RenderWindow* window)
{
	const auto i_end = bag_sprite_chain_.opened_bag->position.x + bag_sprite_chain_.opened_bag->size.x;
	const auto j_end = bag_sprite_chain_.opened_bag->position.y + bag_sprite_chain_.opened_bag->size.y;

	auto i = bag_sprite_chain_.opened_bag->position.x;
	while (i <= i_end)
	{
		auto j = bag_sprite_chain_.opened_bag->position.y;
		while (j <= j_end)
		{
			if (get_selected_cell(Vector2f(i, j)) != -1)
			{
				RectangleShape rec;
				rec.setPosition(i, j);
				rec.setSize(Vector2f(5, 5));
				rec.setFillColor(sf::Color::Red);
				window->draw(rec);
			}
			j += 5.0f;
		}
		i += 5.0f;
	}
}

void hero_bag::fix_cells()
{
	for (auto i = 0u; i < cells.size(); i++)
		cells[i].position = Vector2f(position_.x + cells_pos_[i].x, position_.y + cells_pos_[i].y);
}

void hero_bag::fix_pos()
{
	if (move_position == position_)
		move_position = { -1, -1 };

	if (move_position.x < min_dist_to_border)
		move_position.x = -1;
	if (move_position.x > helper::GetScreenSize().x - min_dist_to_border)
		move_position.x = -1;

	if (move_position.y < min_dist_to_border)
		move_position.y = -1;
	if (move_position.y > helper::GetScreenSize().y - min_dist_to_border)
		move_position.y = -1;

	if (position_.x + shift_vector.x < min_dist_to_border || position_.x + shift_vector.x > helper::GetScreenSize().x - min_dist_to_border)
		shift_vector.x = 0;
	if (position_.y + shift_vector.y < min_dist_to_border || position_.y + shift_vector.y > helper::GetScreenSize().y - min_dist_to_border)
		shift_vector.y = 0;
}

std::vector<std::pair<entity_tag, int>> hero_bag::cells_to_inventory(const std::vector<cell>& cells)
{
	std::vector<std::pair<entity_tag, int>> ans(cells.size());
	for (auto i = 0u; i < cells.size(); i++)
	{
		ans[i] = { cells[i].content.first, cells[i].content.second };
	}

	return ans;
}

bool hero_bag::can_afford(std::vector<std::pair<entity_tag, int>> recipe, std::vector<hero_bag>* bags, cell* held_item)
{
	for (auto& item : recipe)
	{
		if (held_item != nullptr)
		{
			if (item.first == held_item->content.first)
				item.second -= held_item->content.second;
		}
		for (auto& bag : *bags)
		{
			if (item.second <= 0)
				break;
			for (auto& cell : bag.cells)
				if (item.first == cell.content.first)
					item.second -= cell.content.second;
		}
		if (item.second > 0)
			return false;
	}
	return true;
}

void hero_bag::take_items(std::vector<std::pair<entity_tag, int>> recipe, std::vector<hero_bag>* bags, cell* held_item)
{
	for (auto& item : recipe)
	{
		if (held_item != nullptr)
		{
			if (item.second < held_item->content.second)
			{
				held_item->content.second -= item.second;
				item.second = 0;
				continue;
			}
			item.second -= held_item->content.second;
			held_item->content.second = 0;
			held_item->content.first = entity_tag::emptyCell;
		}
		for (auto& bag : *bags)
		{
			auto is_break = true;
			for (auto& cell : bag.cells)
				if (item.first == cell.content.first)
				{
					if (item.second < cell.content.second)
					{
						cell.content.second -= item.second;
						item.second = 0;
						is_break = true;
					}
					else
					{
						item.second -= cell.content.second;
						cell.content.second = 0;
						cell.content.first = entity_tag::emptyCell;
					}
				}
			if (is_break)
				break;
		}
	}
}

bool hero_bag::put_items_in(std::vector<std::pair<entity_tag, int>>* loot, std::vector<hero_bag>* bags)
{
	auto result = true;
	for (auto& item : *loot)
	{
		for (auto& bag : *bags)
		{
			auto is_break = false;
			for (auto& cell : bag.cells)
				if (item.first == cell.content.first || cell.content.first == entity_tag::emptyCell)
				{
					if (item.second <= (items_max_count[item.first] - cell.content.second))
					{
						cell.content.second += item.second;
						cell.content.first = item.first;
						item = std::make_pair(entity_tag::emptyCell, 0);
						is_break = true;
						break;
					}
					item.second -= (items_max_count[item.first] - cell.content.second);
					cell.content.second = items_max_count[item.first];
					cell.content.first = item.first;
				}
			if (is_break)
				break;
		}
		if (item.second != 0)
			result = false;
	}
	return result;
}

bool hero_bag::put_item_in(std::pair<entity_tag, int>* loot, std::vector<hero_bag>* bags)
{
	auto result = true;

	for (auto& bag : *bags)
	{
		auto is_break = false;
		for (auto& cell : bag.cells)
			if (loot->first == cell.content.first || cell.content.first == entity_tag::emptyCell)
			{
				if (loot->second <= (items_max_count[loot->first] - cell.content.second))
				{
					cell.content.second += loot->second;
					cell.content.first = loot->first;
					loot->first = entity_tag::emptyCell;
					loot->second = 0;
					is_break = true;
					break;
				}
				loot->second -= (items_max_count[loot->first] - cell.content.second);
				cell.content.second = items_max_count[loot->first];
				cell.content.first = loot->first;
			}
		if (is_break)
			break;
	}
	if (loot->second != 0)
		result = false;
	return result;
}

void hero_bag::mouse_move()
{
	if (current_state == bag_closed && (ready_to_change_state || was_moved))
	{
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			shift_vector = { 0, 0 };
			if (last_mouse_pos_ != Vector2f(0, 0))
				shift_vector = Vector2f(Mouse::getPosition().x - last_mouse_pos_.x, Mouse::getPosition().y - last_mouse_pos_.y);
			fix_cells();
			fix_pos();
			position_.x += shift_vector.x; position_.y += shift_vector.y;
			if (shift_vector != Vector2f(0, 0))
				was_moved = true;
		}
	}

	last_mouse_pos_ = Vector2f(Mouse::getPosition());
}

sprite_chain_element* hero_bag::prepare_sprite(const long long elapsed_time, std::map<pack_tag, sprite_pack> * packs_map)
{
	const auto screen_center = Vector2f(helper::GetScreenSize().x / 2, helper::GetScreenSize().y / 2);
	bag_sprite_chain_.closed_bag->size = size_closed_; bag_sprite_chain_.closed_bag_selected->size = size_closed_; bag_sprite_chain_.closed_bag_big->size = size_closed_;
	bag_sprite_chain_.opened_bag->size = size_open_; bag_sprite_chain_.opened_bag_selected->size = size_open_;
	bag_sprite_chain_.closed_bag->offset = texture_closed_offset; bag_sprite_chain_.closed_bag_selected->offset = texture_closed_offset; bag_sprite_chain_.closed_bag_big->offset = texture_closed_offset;
	bag_sprite_chain_.opened_bag->offset = texture_open_offset; bag_sprite_chain_.opened_bag_selected->offset = texture_open_offset;
	bag_sprite_chain_.closed_bag->position = position_; bag_sprite_chain_.closed_bag_selected->position = position_; bag_sprite_chain_.closed_bag_big->position = position_;
	bag_sprite_chain_.opened_bag->position = position_; bag_sprite_chain_.opened_bag_selected->position = position_;

	if (current_state == bag_open)
	{
		bag_sprite_chain_.opened_bag->offset = texture_open_offset;
		bag_sprite_chain_.opened_bag_selected->offset = texture_open_offset;
		
		if (ready_to_change_state)
			return bag_sprite_chain_.opened_bag_selected;
		
		return bag_sprite_chain_.opened_bag;
	}

	if (current_state == bag_closed)
	{
		bag_sprite_chain_.closed_bag->offset = texture_closed_offset;
		bag_sprite_chain_.closed_bag_selected->offset = texture_closed_offset;
		
		if (ready_to_change_state)
			return bag_sprite_chain_.closed_bag_selected;
		
		return bag_sprite_chain_.closed_bag;
	}

	const auto elapsed_to_change_time = float(elapsed_time) / float(state_change_time);

	if (current_state == bag_opening)
	{
		state_changing_time += elapsed_time;
		if (state_changing_time >= state_change_time)
		{
			current_state = bag_open;
			state_changing_time = 0;
		}
		else
		{
			const auto to_center_vector = Vector2f(screen_center.x - position_.x, screen_center.y - position_.y);
			const auto cut_coefficient = sqrt(pow((size_open_.x - size_closed_.x) / 2 * elapsed_to_change_time, 2) + pow((size_open_.y - size_closed_.y) / 2 * elapsed_to_change_time, 2)) /
				sqrt(pow(to_center_vector.x, 2) + pow(to_center_vector.y, 2));
			shift_vector = Vector2f(to_center_vector.x * cut_coefficient, to_center_vector.y * cut_coefficient);
			fix_pos();
			position_.x += shift_vector.x; position_.y += shift_vector.y;

			const auto texture_size = packs_map->at(pack_tag::inventory).get_original_info(pack_part::bag1, direction::DOWN, 4).source_size;
			const auto changing_to_change_time = float(state_changing_time)/float(state_change_time);
			const Vector2f scale_value = {
				(size_closed_.x + (size_open_.x - size_closed_.x) * changing_to_change_time) / float(texture_size.w),
				(size_closed_.y + (size_open_.y - size_closed_.y) * changing_to_change_time) / float(texture_size.h)
			};
			bag_sprite_chain_.opened_bag->size.x *= scale_value.x; bag_sprite_chain_.opened_bag->size.y *= scale_value.y;

			bag_sprite_chain_.opened_bag->offset = Vector2f(bag_sprite_chain_.opened_bag->size.x / 2.0f, bag_sprite_chain_.opened_bag->size.y / 1.7f);
			bag_sprite_chain_.opened_bag->position = position_;

			fix_cells();
		}
		return bag_sprite_chain_.opened_bag;
	}

	if (current_state == bag_closing)
	{
		state_changing_time += elapsed_time;
		if (state_changing_time >= state_change_time)
		{
			current_state = bag_closed;
			state_changing_time = 0;
		}
		else
		{
			const auto from_center_vector = Vector2f(position_.x - screen_center.x, position_.y - screen_center.y);
			const auto cut_coefficient = 
				sqrt(pow((size_open_.x - size_closed_.x) / 2 * elapsed_to_change_time, 2) + pow((size_open_.y - size_closed_.y) / 2 * elapsed_to_change_time, 2)) /
				sqrt(pow(from_center_vector.x, 2) + pow(from_center_vector.y, 2));
			shift_vector = Vector2f(from_center_vector.x * cut_coefficient, from_center_vector.y * cut_coefficient);
			fix_pos();
			position_.x += shift_vector.x; position_.y += shift_vector.y;

			const auto delta_time = float(state_change_time - state_changing_time) / float(state_change_time);
			const Vector2f scale_value = {
				(size_closed_.x + (size_open_.x - size_closed_.x) * delta_time) / bag_sprite_chain_.closed_bag->size.x,
				(size_closed_.y + (size_open_.y - size_closed_.y) * delta_time) / bag_sprite_chain_.closed_bag->size.y
			};
			bag_sprite_chain_.closed_bag->size = size_closed_;
			bag_sprite_chain_.closed_bag->size.x *= scale_value.x; bag_sprite_chain_.closed_bag->size.y *= scale_value.y;

			bag_sprite_chain_.closed_bag->offset = Vector2f(bag_sprite_chain_.closed_bag->size.x / 2.0f, bag_sprite_chain_.closed_bag->size.y / 1.7f);
			bag_sprite_chain_.closed_bag->position = position_;

			fix_cells();
		}
		return  bag_sprite_chain_.closed_bag;
	}
	return new sprite_chain_element();
}