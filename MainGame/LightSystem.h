#pragma once
#ifndef LIGHTSYSTEM_H
#define LIGHTSYSTEM_H

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

using Color = sf::Color;
using Vector2f = sf::Vector2f;
using FloatRect = sf::FloatRect;

class LightSystem
{
public:
	LightSystem();
	~LightSystem();
	void init(FloatRect visibleArea);
	Vector2f calculateSunPosition(float dayPart = 0); // from 0 to 1
private:
	float calculateHorizontalOffset(float dayPart);

	FloatRect visibleArea = { 0, 0, 0, 0 };
	Vector2f circleCenter = { 0, 0 }, calculationsCenter = { 0, 0 };
	float calculationsRadius = 0;
};

#endif
