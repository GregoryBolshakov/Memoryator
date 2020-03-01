#pragma once
#ifndef MOVESYSTEM_H
#define MOVESYSTEM_H

#include "DirectionSystem.h"
#include <vector>
#include <SFML/Graphics/Color.hpp>

struct BumpChainElement
{
	BumpChainElement(Vector2f position, bool cancelable) : position{ position }, cancelable{ cancelable } {};
	Vector2f position;
	bool cancelable = true;
};

class MoveSystem
{
public:
	MoveSystem();
	virtual ~MoveSystem();
	void init(float* radius, Vector2f* position, sf::Color* color);
	void turnOff() { turnedOn = false; }
	void pushAway(long long elapsedTime, float pushSpeed = 0);
	void endPush(long long elapsedTime);
	void pushByBumping(Vector2f position, float radius, bool canCrash);

	bool turnedOn = false;
	bool canCrashIntoDynamic = true, canCrashIntoStatic = false;
	Vector2f moveOffset = Vector2f(-1, -1);
	float defaultSpeed = 0, speed = 0;

	std::vector<BumpChainElement> bumpedPositions = {};
	float bumpDistance = 0;
	float pushDamage = 0, pushDistance = 0;
	Vector2f pushDirection = { 0, 0 }, pushVector = { 0, 0 };
	long long pushDuration = 0;
	long long pushRestDuration = 0;
	long long redDuration = 0;
	long long redRestDuration = 0;
	const long long DEFAULT_PUSH_DURATION = long(2e5);
	const float DEFAULT_PUSH_SPEED = 0.00025f;

	// owner info
	float* radius = nullptr;
	Vector2f* position = nullptr;
	sf::Color* color = nullptr;
	//-----------
};

#endif

