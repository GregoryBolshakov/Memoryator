#pragma once
#ifndef SHAPE_CHAIN_ELEMENT_H
#define SHAPE_CHAIN_ELEMENT_H

#include <SFML/Graphics.hpp>
#include <string>

#include "drawable_chain_element.h"
#include "helper.h"

enum class shape_type { circle = 1, rectangle = 2 };

using Color = sf::Color;

class shape_chain_element : public drawable_chain_element
{
public:
	static const float default_character_size;
	shape_chain_element();
	shape_chain_element(
		Vector2f position,
		float radius,
		Vector2f offset = { 0, 0 },
		sf::Color color = sf::Color::White,
		float rotation = 0);
	shape_chain_element(
		Vector2f position,
		Vector2f offset = { 0, 0 },
		sf::Color color = sf::Color::White,		
		float rotation = 0);
	virtual ~shape_chain_element();
	drawable_chain_element* up_cast() { drawable_chain_element* result = this; return result; }

	shape_type type = shape_type::circle;
	float radius = 0;
	Vector2f size = { 0, 0 };
};

#endif