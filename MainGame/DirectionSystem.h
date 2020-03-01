#pragma once
#ifndef DIRECTIONSYSTEM_H
#define DIRECTIONSYSTEM_H

#include <string>
#include "Tags.h"
#include <SFML/System/Vector2.hpp>

const float pi = 3.14159265358979323846f;

using Vector2f = sf::Vector2f;

class DirectionSystem
{
public:
	DirectionSystem();
	~DirectionSystem();
	void init(Vector2f* position, Vector2f* movePosition) { this->position = position; this->movePosition = movePosition; }
	void calculateDirection();
	static Side calculateSide(Vector2f position, sf::Vector2f otherObjectPosition, long long elapsedTime);
	static Side invertSide(Side side);
	static std::string sideToString(Side side);
	static std::string directionToString(Direction direction);
	static Direction cutRights(Direction direction);
	static Side cutRights(Side side);
	static Direction sideToDirection(Side side);
	static Direction invertDirection(Direction dir);
	static Direction cutDiagonals(Direction dir);

	Side side = down;
	Direction direction = Direction::STAND, lastDirection = Direction::DOWN;

	// owner info
	Vector2f* position = nullptr;
	Vector2f* movePosition = nullptr;
	//-----------
};

#endif