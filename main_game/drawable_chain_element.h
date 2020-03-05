#pragma once

#include <SFML/Graphics.hpp>

using Vector2f = sf::Vector2f;
using Color = sf::Color;

class drawable_chain_element
{
public:
	drawable_chain_element();
	drawable_chain_element(
		Vector2f position,
		Vector2f size,
		Vector2f offset,
		Color color = Color(255, 255, 255, 255),		
		float rotation = 0);
	virtual ~drawable_chain_element();
	void initialize() { this->initialized = true; }

	Vector2f offset = { 0, 0 }, position = { 0, 0 }, size = { 0, 0 };
	float rotation = 0;
	Color color = Color(255, 255, 255);
	bool initialized = false;
};
