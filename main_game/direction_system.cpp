#include "direction_system.h"
#include "cmath"

#include "math_constants.h"

direction_system::direction_system(sf::Vector2f& position, sf::Vector2f& move_position, bool& mirrored) :
	  position(position)
	, move_position(move_position)
	, mirrored(mirrored)
{
}

void direction_system::calculate_direction(const long long elapsed_time)
{
	time_after_new_direction += elapsed_time;
	
	if (time_after_new_direction < time_for_new_direction)
		return;
	else
		time_after_new_direction = 0;
	
	const auto cur_pos = position;
	const auto tar_pos = move_position;

	if (tar_pos == sf::Vector2f(-1, -1))
	{
		direction = direction::STAND;
		return;
	}

	const float direction_corridor = 50;

	if (abs(cur_pos.x - tar_pos.x) <= direction_corridor && abs(cur_pos.y - tar_pos.y) <= direction_corridor)
	{
		if (abs(cur_pos.x - tar_pos.x) > abs(cur_pos.y - tar_pos.y))
		{
			if (cur_pos.x >= tar_pos.x)
				direction = direction::LEFT;
			else
				direction = direction::RIGHT;
		}
		else
		{
			if (cur_pos.y >= tar_pos.y)
				direction = direction::UP;
			else
				direction = direction::DOWN;
		}
		last_direction = direction;
		return;
	}

	if (abs(cur_pos.x - tar_pos.x) <= direction_corridor)
	{
		if (cur_pos.y < tar_pos.y)
			direction = direction::DOWN;
		else
			direction = direction::UP;
	}
	else
		if (abs(cur_pos.y - tar_pos.y) <= direction_corridor)
		{
			if (cur_pos.x < tar_pos.x)
				direction = direction::RIGHT;
			else
				direction = direction::LEFT;
		}
		else
			if (cur_pos.x < tar_pos.x)
			{
				if (cur_pos.y < tar_pos.y)
					direction = direction::DOWNRIGHT;
				else
					if (cur_pos.y > tar_pos.y)
						direction = direction::UPRIGHT;
			}
			else
				if (cur_pos.x > tar_pos.x)
				{
					if (cur_pos.y < tar_pos.y)
						direction = direction::DOWNLEFT;
					else
						if (cur_pos.y > tar_pos.y)
							direction = direction::UPLEFT;
				}

	if (direction != direction::STAND)
		last_direction = direction;
}

void direction_system::set_direction_from_8(const sf::Vector2f move_offset, const long long elapsed_time)
{
	time_after_new_direction += elapsed_time;

	if (time_after_new_direction < time_for_new_direction)
		return;
	
	time_after_new_direction = 0;

	const auto angle = calculate_angle(move_offset);
	
	if (angle >= 0 && angle < 45)
	{
		direction = direction::UPLEFT;
		mirrored = true;
	}
	else
		if (angle >= 45 && angle < 90)
		{
			direction = direction::UP;
			mirrored = true;
		}
		else
			if (angle >= 90 && angle < 135)
			{
				direction = direction::UP;
				mirrored = false;
			}
			else
				if (angle >= 135 && angle < 180)
				{
					direction = direction::UPLEFT;
					mirrored = false;
				}
				else
					if (angle >= 180 && angle < 225)
					{
						direction = direction::DOWNLEFT;
						mirrored = false;
					}
					else
						if (angle >= 225 && angle < 270)
						{
							direction = direction::DOWN;
							mirrored = false;
						}
						else
							if (angle >= 270 && angle < 315)
							{
								direction = direction::DOWN;
								mirrored = true;
							}
							else
								if (angle >= 315 && angle < 360)
								{
									direction = direction::DOWNLEFT;
									mirrored = true;
								}
}

void direction_system::set_direction_from_12(const sf::Vector2f move_offset, const long long elapsed_time)
{
	time_after_new_direction += elapsed_time;

	if (time_after_new_direction < time_for_new_direction)
		return;

	time_after_new_direction = 0;

	const auto angle = calculate_angle(move_offset);

	if (angle >= 0 && angle < 22.5 || angle >= 337.5 && angle < 0)
	{
		direction = direction::LEFT;
		mirrored = true;
	}
	else
		if (angle >= 22.5 && angle < 67.5)
		{
			direction = direction::UPLEFT;
			mirrored = true;
		}
		else
			if (angle >= 67.5 && angle < 90 || angle >= 90 && angle < 112.5)
			{
				direction = direction::UP;
				mirrored = false;
			}
			else
				if (angle >= 112.5 && angle < 157.5)
				{
					direction = direction::UPLEFT;
					mirrored = false;
				}
				else
					if (angle >= 157.5 && angle < 202.5)
					{
						direction = direction::LEFT;
						mirrored = false;
					}
					else
						if (angle >= 202.5 && angle < 247.5)
						{
							direction = direction::DOWNLEFT;
							mirrored = false;
						}
						else
							if (angle >= 247.5 && angle < 292.5)
							{
								direction = direction::DOWN;
								mirrored = true;
							}
							else
								if (angle >= 292.5 && angle < 337.5)
								{
									direction = direction::DOWNLEFT;
									mirrored = true;
								}
}

side direction_system::calculate_side(const sf::Vector2f position, const sf::Vector2f other_object_position, const long long elapsed_time)
{
	time_after_new_side += elapsed_time;

	if (time_after_new_side < time_for_new_side)
		return side;
	else
		time_after_new_side = 0;
	
	auto answer = side::down;

	const auto alpha = atan((float(other_object_position.y) - position.y) / (float(other_object_position.x) - position.x)) * 180 / pi;
	if (position.y >= other_object_position.y && abs(alpha) >= 45 && abs(alpha) <= 90)
		answer = side::up;
	else
		if (position.x <= other_object_position.x && abs(alpha) >= 0 && abs(alpha) <= 45)
			answer = side::right;
		else
			if (position.y <= other_object_position.y && abs(alpha) >= 45 && abs(alpha) <= 90)
				answer = side::down;
			else
				if (position.x >= other_object_position.x && abs(alpha) >= 0 && abs(alpha) <= 45)
					answer = side::left;
	return answer;
}

float direction_system::calculate_angle(sf::Vector2f move_offset)
{
	if (abs(move_offset.x) <= 0.001f)
		move_offset.x = 0;
	if (abs(move_offset.y) <= 0.001f)
		move_offset.y = 0;

	if (move_offset.x == 0)
	{
		if (move_offset.y <= 0)
			return 90;
		return 270;
	}
	
	if (move_offset.x > 0 && move_offset.y <= 0)
		return float(atan(-move_offset.y / move_offset.x) * 180 / pi);
	if (move_offset.x < 0 && move_offset.y <= 0)
		return float(90 - atan(-move_offset.y / abs(move_offset.x)) * 180 / pi + 90);
	if (move_offset.x < 0 && move_offset.y > 0)
		return float(atan(move_offset.y / abs(move_offset.x)) * 180 / pi + 180);
	if (move_offset.x > 0 && move_offset.y > 0)
		return float(90 - atan(move_offset.y / move_offset.x) * 180 / pi + 270);

	return -1;
}

side direction_system::invert_side(const ::side side)
{
	switch (side)
	{
	case side::up:
		return side::down;
	case side::down:
		return side::up;
	case side::left:
		return side::right;
	case side::right:
		return side::left;
	default:
		return side::down;
	}
}

std::string direction_system::side_to_string(const ::side side)
{
	switch (side)
	{
	case side::up:
		return "up";
	case side::right:
		return "left";
	case side::down:
		return "down";
	case  side::left:
		return "left";
	default:
		return "";
	}
}

std::string direction_system::direction_to_string(const ::direction direction)
{
	switch (direction)
	{
	case direction::UP:
		return "up";
	case direction::UPRIGHT:
		return "up-left";
	case direction::RIGHT:
		return "left";
	case direction::DOWNRIGHT:
		return "down-left";
	case direction::DOWN:
		return "down";
	case direction::DOWNLEFT:
		return "down-left";
	case direction::LEFT:
		return "left";
	case direction::UPLEFT:
		return "up-left";
	default:;
	}
	return "";
}

direction direction_system::side_to_direction(const ::side side)
{
	switch (side)
	{
	case side::up:
		return direction::UP;
	case side::right:
		return direction::RIGHT;
	case side::down:
		return direction::DOWN;
	case  side::left:
		return direction::LEFT;
	default:
		return direction::DOWN;
	}
}

direction direction_system::invert_direction(const ::direction dir)
{
	switch (dir)
	{
	case direction::UP:
		return direction::DOWN;
	case direction::DOWN:
		return direction::UP;
	case direction::LEFT:
		return direction::RIGHT;
	case direction::RIGHT:
		return direction::LEFT;
	case direction::UPLEFT:
		return direction::DOWNRIGHT;
	case direction::UPRIGHT:
		return direction::DOWNLEFT;
	case direction::DOWNLEFT:
		return direction::UPRIGHT;
	case direction::DOWNRIGHT:
		return direction::UPLEFT;
	default:;
	}
	return {};
}

direction direction_system::cut_rights(const ::direction direction)
{
	switch (direction)
	{
	case direction::UPRIGHT:
		return direction::UPLEFT;
	case direction::RIGHT:
		return direction::LEFT;
	case direction::DOWNRIGHT:
		return direction::DOWNLEFT;
	default: return direction;
	}
}

side direction_system::cut_rights(const ::side side)
{
	if (side == side::right)
		return side::left;
	
	return side;
}

direction direction_system::cut_diagonals(const ::direction dir)
{
	if (dir == direction::UPLEFT || dir == direction::DOWNLEFT)
		return direction::LEFT;
	if (dir == direction::UPRIGHT || dir == direction::DOWNRIGHT)
		return direction::RIGHT;

	return dir;
}
