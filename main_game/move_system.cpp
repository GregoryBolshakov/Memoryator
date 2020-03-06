#include "move_system.h"

#include "helper.h"

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

void move_system::end_push(long long elapsed_time)
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
		end_push(elapsed_time);
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

Vector2f move_system::ellipseSlip(Vector2f newPos, Vector2f destination, Vector2f f1, Vector2f f2, float ellipseSize, float height, long long elapsedTime) const
{
	const auto dynamicPos = Vector2f(position_->x, height - position_->y);
	const auto focus1 = Vector2f(f1.x, height - f1.y);
	const auto focus2 = Vector2f(f2.x, height - f2.y);
	const auto newPosition = Vector2f(newPos.x, height - newPos.y);
	const auto destinationPos = Vector2f(destination.x, height - destination.y);

	const auto a1 = newPosition.x - dynamicPos.x;
	const auto b1 = newPosition.y - dynamicPos.y;
	const auto c1 = -a1 * newPosition.x - b1 * newPosition.y;

	const auto l = (ellipseSize - abs(focus2.x - focus1.x)) / 2;
	const auto a2 = ellipseSize / 2;
	const auto b2 = sqrt(pow((2 * l + abs(focus2.x - focus1.x)) / 2, 2) - pow(abs(focus2.x - focus1.x) / 2, 2));
	const auto x0 = (focus1.x + focus2.x) / 2;
	const auto y0 = (focus1.y + focus2.y) / 2;

	float x1, yl, x2, y2, d;

	if (a1 == 0 && b1 != 0)
	{
		d = pow(2 * x0 * b1 * b1 * b2 * b2, 2) - 4 * (b1 * b1 * b2 * b2) * (x0 * x0 * b1 * b1 * b2 * b2 + c1 * c1 * a2 * a2 + y0 * y0 * a2 * a2 * b1 * b1 + 2 * c1 * y0 * a2 * a2 * b1 - a2 * a2 * b1 * b1 * b2 * b2);

		if (b1 * b1 * b2 * b2 == 0 || d < 0)
			return { -1, -1 };

		x1 = -((-(2 * x0 * b1 * b1 * b2 * b2) + sqrt(d)) / (2 * (b1 * b1 * b2 * b2)));
		x2 = -((-(2 * x0 * b1 * b1 * b2 * b2) - sqrt(d)) / (2 * (b1 * b1 * b2 * b2)));
		yl = -c1 / b1;
		y2 = -c1 / b1;
	}
	else
		if (a1 != 0 && b1 == 0)
		{
			d = pow(-2 * y0 * a1 * a1 * a2 * a2, 2) - 4 * (a1 * a1 * a2 * a2) * (c1 * c1 * b2 * b2 + x0 * x0 * a1 * a1 * b2 * b2 + 2 * c1 * x0 * a1 * b2 * b2 + y0 * y0 * a1 * a1 * a2 * a2 - a1 * a1 * a2 * a2 * b2 * b2);

			if (a1 * a1 * a2 * a2 == 0 || d < 0)
				return { -1, -1 };

			yl = (2 * y0 * a1 * a1 * a2 * a2 + sqrt(d)) / (2 * a1 * a1 * a2 * a2);
			y2 = (2 * y0 * a1 * a1 * a2 * a2 - sqrt(d)) / (2 * a1 * a1 * a2 * a2);
			x1 = -c1 / a1;
			x2 = -c1 / a1;
		}
		else
			if (a1 != 0 && b1 != 0)
			{
				d = pow(2 * b1 * b2 * b2 * c1 + 2 * a1 * b1 * b2 * b2 * x0 - 2 * a1 * a1 * a2 * a2 * y0, 2) - 4 * (b1 * b1 * b2 * b2 + a1 * a1 * a2 * a2) * (b2 * b2 * c1 * c1 + a1 * a1 * b2 * b2 * x0 * x0 + 2 * a1 * b2 * b2 * c1 * x0 + a1 * a1 * a2 * a2 * y0 * y0 - a1 * a1 * a2 * a2 * b2 * b2);

				if (b1 * b1 * b2 * b2 + a1 * a1 * a2 * a2 == 0 || d < 0)
					return { -1, -1 };

				yl = (-(2 * b1 * b2 * b2 * c1 + 2 * a1 * b1 * b2 * b2 * x0 - 2 * a1 * a1 * a2 * a2 * y0) + sqrt(d)) / (2 * (b1 * b1 * b2 * b2 + a1 * a1 * a2 * a2));
				y2 = (-(2 * b1 * b2 * b2 * c1 + 2 * a1 * b1 * b2 * b2 * x0 - 2 * a1 * a1 * a2 * a2 * y0) - sqrt(d)) / (2 * (b1 * b1 * b2 * b2 + a1 * a1 * a2 * a2));
				x1 = -1 * (b1 * yl + c1) / a1;
				x2 = -1 * (b1 * y2 + c1) / a1;
			}
			else
				return { -1, -1 };

	float dist1, dist2;
	const auto time = float(elapsedTime);

	if (destination == Vector2f(-1, -1))
		dist1 = helper::getDist(newPosition, Vector2f(x1, yl)), dist2 = helper::getDist(newPosition, Vector2f(x2, y2));
	else
		dist1 = helper::getDist(destinationPos, Vector2f(x1, yl)), dist2 = helper::getDist(destinationPos, Vector2f(x2, y2));

	if (dist1 <= dist2 && d >= 0 && sqrt(pow(x1 - position_->x, 2) + pow(height - yl - position_->y, 2)) != 0)
	{
		const auto k = ((speed * time) / sqrt(pow(x1 - position_->x, 2) + pow(height - yl - position_->y, 2)));
		return { (x1 - position_->x) * k, (height - yl - position_->y) * k };
	}
	if (dist2 <= dist1 && d >= 0 && sqrt(pow(x2 - position_->x, 2) + pow(height - y2 - position_->y, 2)) != 0)
	{
		const auto k = ((speed * time) / sqrt(pow(x2 - position_->x, 2) + pow(height - y2 - position_->y, 2)));
		return { (x2 - position_->x) * k, (height - y2 - position_->y) * k };
	}

	return { -1, -1 };
}

//void move_system::move_system_.set_move_offset(const long long elapsedTime)
//{
//	if (!(currentAction == move || currentAction == moveHit || currentAction == moveEnd || currentAction == jerking || currentAction == throwNoose))
//	{
//		moveOffset = { -1, -1 };
//		return;
//	}
//
//	const auto time = float(elapsedTime);
//
//	if (move_position_ == Vector2f(-1, -1))
//	{
//		if (direction != Direction::STAND)
//		{
//			const auto angle = float(this->getDirection()) * pi / 180;
//			const auto k = speed * time / sqrt(pow(cos(angle), 2) + pow(sin(angle), 2));
//			moveOffset.x = float(k * cos(angle));
//			moveOffset.y = float(k * -sin(angle));
//			return;
//		}
//		moveOffset = Vector2f(-1, -1);
//		return;
//	}
//
//	const auto distanceToTarget = float(sqrt(pow(move_position_.x - position.x, 2) + pow(move_position_.y - position.y, 2)));
//	if (distanceToTarget == 0)
//	{
//		moveOffset = Vector2f(-1, -1);
//		return;
//	}
//
//	const float k = speed * time / sqrt(pow(move_position_.x - position.x, 2) + pow(move_position_.y - position.y, 2));
//	/*if (distanceToTarget <= k)
//	{
//		moveOffset = Vector2f(-1, -1);
//		return;
//	}*/
//	moveOffset = { (move_position_.x - position.x) * k, (move_position_.y - position.y) * k };
//}

void move_system::set_move_offset(long long elapsedTime)
{
	if (!(*current_action_ == move ||
		*current_action_ == moveHit ||
		*current_action_ == moveEnd ||
		*current_action_ == jerking ||
		*current_action_ == throwNoose ||
		*current_action_ == moveSlowly))
	{
		move_offset = { -1, -1 };
		return;
	}

	if (move_position == Vector2f(-1, -1))
	{
		if (direction_system_->direction != direction::STAND)
		{
			const auto angle = float(direction_system_->direction) * pi / 180;
			const float k = speed * float(elapsedTime) / sqrt(pow(cos(angle), 2) + pow(sin(angle), 2));
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

Vector2f move_system::doMove(long long elapsedTime)
{
	set_move_offset(elapsedTime);
	auto position = this->position_;
	position->x += push_vector.x; position->y += push_vector.y;

	//if (this->direction == Direction::STAND)
		//return position;

	if (move_offset != Vector2f(-1, -1))
	{
		position_->x += move_offset.x;
		position_->y += move_offset.y;
	}

	return *position_;
}

Vector2f move_system::doSlip(Vector2f newPosition, const std::vector<static_object*>& localStaticItems, const float height, const long long elapsedTime)
{
	bool crashed = false;
	if (!can_crash_into_static)
		return newPosition;

	for (auto& staticItem : localStaticItems)
	{
		auto terrain = dynamic_cast<terrain_object*>(staticItem);
		if (!terrain || staticItem->is_background || staticItem->get_radius() == 0)
			continue;

		if (*tag != entity_tag::hero && staticItem->is_multi_ellipse)
			continue;

		if (terrain->is_multi_ellipse)
		{
			auto curEllipses = terrain->getMultiellipseIntersect(newPosition);
			Vector2f motionAfterSlipping;

			if (!curEllipses.empty())
				for (auto& curEllipse : curEllipses)
				{
					if (crashed)
						return Vector2f(-1, -1);

					crashed = true;
					motionAfterSlipping = this->ellipseSlip(newPosition, move_position, terrain->internalEllipses[curEllipse].first, terrain->internalEllipses[curEllipse].second, terrain->getEllipseSize(curEllipse), height, elapsedTime);

					if (motionAfterSlipping != Vector2f(-1, -1))
					{
						newPosition = Vector2f(position_->x + motionAfterSlipping.x, position_->y + motionAfterSlipping.y);
						continue;
					}

					return Vector2f(-1, -1);
				}
		}
		else
			if (terrain->isIntersected(*position_, newPosition))
			{
				if (crashed)
					return Vector2f(-1, -1);

				crashed = true;
				Vector2f motionAfterSlipping;

				if (staticItem->is_dots_adjusted)
					motionAfterSlipping = terrain->newSlippingPositionForDotsAdjusted(*position_, speed, elapsedTime);
				else
					motionAfterSlipping = this->ellipseSlip(newPosition, move_position, terrain->getFocus1(), terrain->getFocus2(), terrain->getEllipseSize(), height, elapsedTime);

				if (motionAfterSlipping != Vector2f(-1, -1))
				{
					newPosition = Vector2f(position_->x + motionAfterSlipping.x, position_->y + motionAfterSlipping.y);
					continue;
				}

				return Vector2f(-1, -1);
			}
	}

	return newPosition;
}