#include "DirectionsSystem.h"



DirectionsSystem::DirectionsSystem()
= default;


DirectionsSystem::~DirectionsSystem()
= default;

std::string DirectionsSystem::sideToString(Side side)
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

std::string DirectionsSystem::directionToString(Direction direction)
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

Direction DirectionsSystem::sideToDirection(Side side)
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

Direction DirectionsSystem::invertDirection(Direction dir)
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

Direction DirectionsSystem::cutRights(Direction direction)
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

Side DirectionsSystem::cutRights(Side side)
{
	if (side == right)
		return left;
	return side;
}

