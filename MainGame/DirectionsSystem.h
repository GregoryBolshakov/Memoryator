#pragma once
#include <string>
#include "Tags.h"

class DirectionsSystem
{
public:
	DirectionsSystem();
	~DirectionsSystem();
	static std::string sideToString(Side side);
	static std::string directionToString(Direction direction);
	static Direction cutRights(Direction direction);
	static Side cutRights(Side side);
	static Direction sideToDirection(Side side);
	static Direction invertDirection(Direction dir);
};

