#include "move_system.h"

#include "helper.h"
#include "math_constants.h"

move_system::move_system()
= default;

move_system::~move_system()
= default;

void move_system::init(entity_tag * tag, float* radius, Vector2f* position, sf::Color* color, actions* current_action, direction_system* direction_system)
{
	this->tag = tag;
	this->radius_ = radius;
	this->position_ = position;
	this->color_ = color;
	this->current_action_ = current_action;
	this->direction_system_ = direction_system;
	turned_on = true;
}

// push interactions
void move_system::push_by_bumping(Vector2f position, float radius, bool can_crash)
{
	if ((!can_crash_into_dynamic && !can_crash) || push_damage != 0)
		return;
	bumped_positions.emplace_back(position, false);
	bump_distance += radius;
}

void move_system::end_push()
{
	push_direction = { 0, 0 };
	push_duration = 0;
	push_vector = { 0, 0 };
	push_damage = 0;
	bumped_positions.clear();
	bump_distance = 0;
}

void move_system::push_away(long long elapsed_time, float push_speed)
{
	if (push_speed == 0)
		push_speed = default_push_speed;

	const auto push_color = sf::Color(255, 100, 100, 255);

	if (!bumped_positions.empty() && this->can_crash_into_dynamic && push_damage == 0)
	{
		Vector2f bump_center = { 0, 0 };
		auto bump_count = 0;
		for (const auto bump : bumped_positions)
		{
			bump_count++;
			bump_center.x += bump.position.x;
			bump_center.y += bump.position.y;
		}
		if (bump_count == 0 || push_speed == 0)
			return;
		bump_center.x /= float(bump_count);
		bump_center.y /= float(bump_count);
		push_distance = (*radius_ + bump_distance) - helper::getDist(*position_, bump_center);
		push_direction = Vector2f(position_->x - bump_center.x, position_->y - bump_center.y);
		push_duration = long(push_distance / push_speed);
		push_rest_duration = push_duration;
		bump_distance = 0;
		bumped_positions.clear();
		*color_ = push_color;
	}

	if (red_rest_duration > 0)
		*color_ = sf::Color(
			255,
			push_color.g + Uint8((255.0f - float(push_color.g)) * (1.0f - float(red_rest_duration) / float(red_duration))),
			push_color.b + Uint8((255.0f - float(push_color.b)) * (1.0f - float(red_rest_duration) / float(red_duration))),
			255);
	else
	{
		*color_ = sf::Color::White;
		red_rest_duration = 0;
	}

	red_rest_duration -= elapsed_time;

	if (push_rest_duration <= 0 || !turned_on)
	{
		end_push();
		return;
	}

	push_rest_duration -= elapsed_time;

	const float elongation_coefficient = push_speed * float(elapsed_time) / sqrt(pow(push_direction.x, 2) + pow(push_direction.y, 2));
	if (push_direction != Vector2f(0, 0))
		push_vector = { elongation_coefficient * push_direction.x, elongation_coefficient * push_direction.y };
	else
		push_vector = { 0, 0 };
}
//------------------

Vector2f move_system::ellipse_slip(const Vector2f new_pos, const Vector2f destination, Vector2f f1, const Vector2f f2, float ellipseSize, const float height, const long long elapsed_time) const
{
	const auto terrain_pos = Vector2f((f1.x + f2.x) / 2, ((height - f1.y) + (height - f2.y)) / 2);
	const auto dynamic_pos = Vector2f(position_->x, height - position_->y);
	const auto focus1 = Vector2f(f1.x, height - f1.y);
	const auto focus2 = Vector2f(f2.x, height - f2.y);
	const auto new_position = Vector2f(new_pos.x, height - new_pos.y);
	const auto destination_pos = Vector2f(destination.x, height - destination.y);

	float c1 = 0, x0 = terrain_pos.x, y0 = terrain_pos.y;

	const auto a1 = (new_position.x - dynamic_pos.x);
	const auto b1 = (new_position.y - dynamic_pos.y);
	c1 = -a1 * new_position.x - b1 * new_position.y;

	const auto l = (ellipseSize - abs(focus2.x - focus1.x)) / 2;
	const auto a2 = ellipseSize / 2;
	const auto b2 = sqrt(pow((2 * l + (abs(focus2.x - focus1.x))) / 2, 2) - pow(abs(focus2.x - focus1.x) / 2, 2));
	x0 = (focus1.x + focus2.x) / 2;
	y0 = (focus1.y + focus2.y) / 2;

	float x1, yl, x2, y2, D = 0;

	if (a1 == 0 && b1 != 0)
	{
		D = pow(2 * x0 * b1 * b1 * b2 * b2, 2) - 4 * (b1 * b1 * b2 * b2) * (x0 * x0 * b1 * b1 * b2 * b2 + c1 * c1 * a2 * a2 + y0 * y0 * a2 * a2 * b1 * b1 + 2 * c1 * y0 * a2 * a2 * b1 - a2 * a2 * b1 * b1 * b2 * b2);

		if (b1 * b1 * b2 * b2 == 0 || D < 0)
			return { -1, -1 };

		x1 = -((-(2 * x0 * b1 * b1 * b2 * b2) + sqrt(D)) / (2 * (b1 * b1 * b2 * b2)));
		x2 = -((-(2 * x0 * b1 * b1 * b2 * b2) - sqrt(D)) / (2 * (b1 * b1 * b2 * b2)));
		yl = -c1 / b1;
		y2 = -c1 / b1;
	}
	else
		if (a1 != 0 && b1 == 0)
		{
			D = pow(-2 * y0 * a1 * a1 * a2 * a2, 2) - 4 * (a1 * a1 * a2 * a2) * (c1 * c1 * b2 * b2 + x0 * x0 * a1 * a1 * b2 * b2 + 2 * c1 * x0 * a1 * b2 * b2 + y0 * y0 * a1 * a1 * a2 * a2 - a1 * a1 * a2 * a2 * b2 * b2);

			if (a1 * a1 * a2 * a2 == 0 || D < 0)
				return { -1, -1 };

			yl = (2 * y0 * a1 * a1 * a2 * a2 + sqrt(D)) / (2 * a1 * a1 * a2 * a2);
			y2 = (2 * y0 * a1 * a1 * a2 * a2 - sqrt(D)) / (2 * a1 * a1 * a2 * a2);
			x1 = -c1 / a1;
			x2 = -c1 / a1;
		}
		else
			if (a1 != 0 && b1 != 0)
			{
				D = pow(2 * b1 * b2 * b2 * c1 + 2 * a1 * b1 * b2 * b2 * x0 - 2 * a1 * a1 * a2 * a2 * y0, 2) - 4 * (b1 * b1 * b2 * b2 + a1 * a1 * a2 * a2) * (b2 * b2 * c1 * c1 + a1 * a1 * b2 * b2 * x0 * x0 + 2 * a1 * b2 * b2 * c1 * x0 + a1 * a1 * a2 * a2 * y0 * y0 - a1 * a1 * a2 * a2 * b2 * b2);

				if (b1 * b1 * b2 * b2 + a1 * a1 * a2 * a2 == 0 || D < 0)
					return { -1, -1 };

				yl = (-(2 * b1 * b2 * b2 * c1 + 2 * a1 * b1 * b2 * b2 * x0 - 2 * a1 * a1 * a2 * a2 * y0) + sqrt(D)) / (2 * (b1 * b1 * b2 * b2 + a1 * a1 * a2 * a2));
				y2 = (-(2 * b1 * b2 * b2 * c1 + 2 * a1 * b1 * b2 * b2 * x0 - 2 * a1 * a1 * a2 * a2 * y0) - sqrt(D)) / (2 * (b1 * b1 * b2 * b2 + a1 * a1 * a2 * a2));
				x1 = -1 * (b1 * yl + c1) / a1;
				x2 = -1 * (b1 * y2 + c1) / a1;
			}
			else
				return { -1, -1 };

	float dist1, dist2;

	if (destination == Vector2f(-1, -1))
		dist1 = helper::getDist(new_position, Vector2f(x1, yl)), dist2 = helper::getDist(new_position, Vector2f(x2, y2));
	else
		dist1 = helper::getDist(destination_pos, Vector2f(x1, yl)), dist2 = helper::getDist(destination_pos, Vector2f(x2, y2));

	if (dist1 <= dist2 && D >= 0 && sqrt(pow(x1 - position_->x, 2) + pow((height - yl) - position_->y, 2)) != 0)
	{
		const auto k = ((speed * float(elapsed_time)) / sqrt(pow(x1 - position_->x, 2) + pow((height - yl) - position_->y, 2)));
		return { (x1 - position_->x) * k, ((height - yl) - position_->y) * k };
	}
	if (dist2 <= dist1 && D >= 0 && sqrt(pow(x2 - position_->x, 2) + pow((height - y2) - position_->y, 2)) != 0)
	{
		const auto k = ((speed * float(elapsed_time)) / sqrt(pow(x2 - position_->x, 2) + pow((height - y2) - position_->y, 2)));
		return { (x2 - position_->x) * k, ((height - y2) - position_->y) * k };
	}

	return { -1, -1 };
}

void move_system::set_move_offset(long long elapsedTime)
{
	if (!(*current_action_ == move ||
		*current_action_ == move_hit ||
		*current_action_ == move_end ||
		*current_action_ == jerking ||
		*current_action_ == throw_noose ||
		*current_action_ == move_slowly ||
		*current_action_ == soar ||
		*current_action_ == flap))
	{
		move_offset = { -1, -1 };
		return;
	}

	if (move_position == Vector2f(-1, -1))
	{
		if (direction_system_->direction != direction::STAND)
		{
			const auto angle = double(direction_system_->direction) * pi / 180;
			const auto k = speed * double(elapsedTime) / sqrt(pow(cos(angle), 2) + pow(sin(angle), 2));
			move_offset.x = float(k * cos(angle));
			move_offset.y = float(k * -sin(angle));
			return;
		}
		move_offset = Vector2f(-1, -1);
		return;
	}

	const auto distanceToTarget = float(sqrt(pow(move_position.x - position_->x, 2) + pow(move_position.y - position_->y, 2)));
	if (distanceToTarget == 0)
	{
		move_offset = Vector2f(-1, -1);
		return;
	}

	const auto k = speed * float(elapsedTime) / sqrt(pow(move_position.x - position_->x, 2) + pow(move_position.y - position_->y, 2));
	/*if (distanceToTarget <= k)
	{
		moveOffset = Vector2f(-1, -1);
		return;
	}*/
	move_offset = { (move_position.x - position_->x) * k, (move_position.y - position_->y) * k };
}

Vector2f move_system::do_move(long long elapsedTime)
{
	set_move_offset(elapsedTime);
	Vector2f position = *position_;
	position.x += push_vector.x; position.y += push_vector.y;

	//if (this->direction == Direction::STAND)
		//return position;

	if (move_offset != Vector2f(-1, -1))
	{
		position.x += move_offset.x;
		position.y += move_offset.y;
	}

	return position;
}

Vector2f move_system::do_slip(Vector2f new_position, std::vector<shared_ptr<static_object>>& local_static_items, const float height, const long long elapsed_time) const
{
	if (move_position != Vector2f(-1, -1))
		auto a = 123;
	bool crashed = false;
	if (!can_crash_into_static)
		return new_position;

	for (auto& staticItem : local_static_items)
	{
		auto terrain = dynamic_pointer_cast<terrain_object>(staticItem);
		if (!terrain || staticItem->is_background || staticItem->get_radius() == 0)
			continue;

		if (*tag != entity_tag::hero && staticItem->is_multi_ellipse)
			continue;

		if (terrain->is_multi_ellipse)
		{
			auto curEllipses = terrain->get_multi_ellipse_intersect(new_position);
			Vector2f motionAfterSlipping;

			if (!curEllipses.empty())
				for (auto& curEllipse : curEllipses)
				{
					if (crashed)
						return Vector2f(-1, -1);

					crashed = true;
					motionAfterSlipping = this->ellipse_slip(new_position, move_position, terrain->internal_ellipses[curEllipse].first, terrain->internal_ellipses[curEllipse].second, terrain->get_ellipse_size(curEllipse), height, elapsed_time);

					if (motionAfterSlipping != Vector2f(-1, -1))
					{
						new_position = Vector2f(position_->x + motionAfterSlipping.x, position_->y + motionAfterSlipping.y);
						continue;
					}

					return Vector2f(-1, -1);
				}
		}
		else
			if (terrain->is_intersected(*position_, new_position))
			{
				if (crashed)
					return Vector2f(-1, -1);

				crashed = true;
				Vector2f motionAfterSlipping;

				if (staticItem->is_dots_adjusted)
					motionAfterSlipping = terrain->new_slipping_position_for_dots_adjusted(*position_, speed, elapsed_time);
				else
					motionAfterSlipping = ellipse_slip(new_position, move_position, terrain->get_focus1(), terrain->get_focus2(), terrain->get_ellipse_size(), height, elapsed_time);

				if (motionAfterSlipping != Vector2f(-1, -1))
				{
					new_position = Vector2f(position_->x + motionAfterSlipping.x, position_->y + motionAfterSlipping.y);
					continue;
				}

				return Vector2f(-1, -1);
			}
	}

	return new_position;
}

void move_system::is_route_needed(std::vector<std::vector<bool>>& micro_block_matrix, Vector2f& micro_block_size)
{		
	need_route = false;
	auto is_break = false;

	if (!can_crash_into_static)
		return;
	
	for (auto i = long(position_->x - *radius_);
	     i <= long(position_->x + *radius_); i++)
	{
		for (auto j = long(position_->y - *radius_); j <=
		     long(position_->y + *radius_); j++)
			if (!micro_block_matrix[i / long(micro_block_size.x)][j / long(micro_block_size.y)])
			{
				need_route = true;
				is_break = true;
				break;
			}
		if (is_break)
			break;
	}
	time_after_new_route = time_for_new_route;
}

void move_system::make_route(long long elapsed_time, grid_list& grid_list, float zone_offset)
{
	if (!can_crash_into_static)
		return;
	
	time_after_new_route += elapsed_time;
	if (time_after_new_route >= time_for_new_route)
	{
		if (need_route)
		{
			time_after_new_route = 0;
			if (route_generation_ability && lax_move_position != Vector2f(-1, -1) && *current_action_ != jerking && *tag != entity_tag::hero)
			{
				time_after_new_route = 0;
				route = grid_list.make_route(
					*position_, 
					lax_move_position, 
					position_->x - zone_offset,
					position_->y - zone_offset,
					position_->x + zone_offset,
					position_->y + zone_offset);
			}
		}
		else
		{
			route.clear();
			move_position = lax_move_position;
		}
	}
}

void move_system::pass_route_beginning(Vector2f micro_block_size)
{
	if (!can_crash_into_static)
	{
		move_position = lax_move_position;
		return;
	}
	
	if (need_route)
	{
		if (!route.empty())
		{
			std::pair<int, int> routeMicroBlock;
			do
			{
				routeMicroBlock = route[0];

				const auto route_pos = Vector2f(routeMicroBlock.first * micro_block_size.x, routeMicroBlock.second * micro_block_size.y);
				if (helper::getDist(*position_, route_pos) > sqrt(pow(micro_block_size.x, 2) + pow(micro_block_size.y, 2)))
					break;

				route.erase(route.begin());
			} while (!route.empty());

			move_position = Vector2f(routeMicroBlock.first * micro_block_size.x, routeMicroBlock.second * micro_block_size.y);
		}
		else
			move_position = { -1, -1 };
	}
}
