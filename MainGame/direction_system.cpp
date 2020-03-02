#include "direction_system.h"

direction_system::direction_system()
= default;


direction_system::~direction_system()
= default;

void direction_system::calculateDirection()
{
	const Vector2f curPos = *position;
	const Vector2f tarPos = *movePosition;

	if (tarPos == Vector2f(-1, -1))
	{
		direction = Direction::STAND;
		return;
	}

	const float directionCorridor = 50;

	if (abs(curPos.x - tarPos.x) <= directionCorridor && abs(curPos.y - tarPos.y) <= directionCorridor)
	{
		if (abs(curPos.x - tarPos.x) > abs(curPos.y - tarPos.y))
		{
			if (curPos.x >= tarPos.x)
				direction = Direction::LEFT;
			else
				direction = Direction::RIGHT;
		}
		else
		{
			if (curPos.y >= tarPos.y)
				direction = Direction::UP;
			else
				direction = Direction::DOWN;
		}
		lastDirection = direction;
		return;
	}

	if (abs(curPos.x - tarPos.x) <= directionCorridor)
	{
		if (curPos.y < tarPos.y)
			direction = Direction::DOWN;
		else
			direction = Direction::UP;
	}
	else
		if (abs(curPos.y - tarPos.y) <= directionCorridor)
		{
			if (curPos.x < tarPos.x)
				direction = Direction::RIGHT;
			else
				direction = Direction::LEFT;
		}
		else
			if (curPos.x < tarPos.x)
			{
				if (curPos.y < tarPos.y)
					direction = Direction::DOWNRIGHT;
				else
					if (curPos.y > tarPos.y)
						direction = Direction::UPRIGHT;
			}
			else
				if (curPos.x > tarPos.x)
				{
					if (curPos.y < tarPos.y)
						direction = Direction::DOWNLEFT;
					else
						if (curPos.y > tarPos.y)
							direction = Direction::UPLEFT;
				}

	if (direction != Direction::STAND)
		lastDirection = direction;
}

Side direction_system::calculateSide(Vector2f position, Vector2f otherObjectPosition, long long elapsedTime)
{
	Side answer = down;

	const float alpha = atan((float(otherObjectPosition.y) - position.y) / (float(otherObjectPosition.x) - position.x)) * 180 / pi;
	if (position.y >= otherObjectPosition.y && abs(alpha) >= 45 && abs(alpha) <= 90)
		answer = up;
	else
		if (position.x <= otherObjectPosition.x && abs(alpha) >= 0 && abs(alpha) <= 45)
			answer = right;
		else
			if (position.y <= otherObjectPosition.y && abs(alpha) >= 45 && abs(alpha) <= 90)
				answer = down;
			else
				if (position.x >= otherObjectPosition.x && abs(alpha) >= 0 && abs(alpha) <= 45)
					answer = left;
	return answer;
}

Side direction_system::invertSide(Side side)
{
	switch (side)
	{
	case up:
		return down;
	case down:
		return up;
	case left:
		return right;
	case right:
		return left;
	}
	return down;
}

std::string direction_system::sideToString(Side side)
{
	switch (side)
	{
	case up:
		return "up";
	case right:
		return "left";
	case down:
		return "down";
	case  left:
		return "left";
	}
	return "";
}

std::string direction_system::directionToString(Direction direction)
{
	switch (direction)
	{
	case Direction::UP:
		return "up";
	case Direction::UPRIGHT:
		return "up-left";
	case Direction::RIGHT:
		return "left";
	case Direction::DOWNRIGHT:
		return "down-left";
	case Direction::DOWN:
		return "down";
	case Direction::DOWNLEFT:
		return "down-left";
	case Direction::LEFT:
		return "left";
	case Direction::UPLEFT:
		return "up-left";
	default: ;
	}
	return "";
}

Direction direction_system::sideToDirection(Side side)
{
	switch (side)
	{
	case up:
		return Direction::UP;
	case right:
		return Direction::RIGHT;
	case down:
		return Direction::DOWN;
	case  left:
		return Direction::LEFT;
	}
	return Direction::DOWN;
}

Direction direction_system::invertDirection(Direction dir)
{
	switch (dir)
	{
	case Direction::UP:
		return Direction::DOWN;
	case Direction::DOWN:
		return Direction::UP;
	case Direction::LEFT:
		return Direction::RIGHT;
	case Direction::RIGHT:
		return Direction::LEFT;
	case Direction::UPLEFT:
		return Direction::DOWNRIGHT;
	case Direction::UPRIGHT:
		return Direction::DOWNLEFT;
	case Direction::DOWNLEFT:
		return Direction::UPRIGHT;
	case Direction::DOWNRIGHT:
		return Direction::UPLEFT;
	default: ;
	}
	return {};
}

Direction direction_system::cutRights(Direction direction)
{
	switch (direction)
	{
	case Direction::UPRIGHT:
		return Direction::UPLEFT;
	case Direction::RIGHT:
		return Direction::LEFT;
	case Direction::DOWNRIGHT:
		return Direction::DOWNLEFT;
	default: return direction;
	}
}

Side direction_system::cutRights(Side side)
{
	if (side == right)
		return left;
	return side;
}

Direction direction_system::cutDiagonals(Direction dir)
{
	if (dir == Direction::UPLEFT || dir == Direction::DOWNLEFT)
		return Direction::LEFT;
	if (dir == Direction::UPRIGHT || dir == Direction::DOWNRIGHT)
		return Direction::RIGHT;

	return dir;
}
