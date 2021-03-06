#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "drawable_chain_element.h"
#include "helper.h"

enum class shape_type { circle = 1, rectangle = 2 };

class shape_chain_element : public drawable_chain_element
{
public:
	static const float default_character_size;
	shape_chain_element();
	shape_chain_element(
		sf::Vector2f position,
		float radius,
		sf::Vector2f offset = { 0, 0 },
		bool hollow = false,
		sf::Color color = sf::Color::White,
		float rotation = 0);
	shape_chain_element(
		sf::Vector2f position,
		sf::Vector2f size,
		sf::Vector2f offset = { 0, 0 },
		bool hollow = false,
		sf::Color color = sf::Color::White,
		float rotation = 0);
	shape_chain_element(shape_chain_element&& element) noexcept : drawable_chain_element(std::move(element))
	{
		this->type = element.type;
		this->radius = element.radius;
	}
	shape_chain_element(shape_chain_element& element) noexcept : drawable_chain_element(element)
	{
		if (this == &element)
			return;
		this->type = element.type;
		this->radius = element.radius;
	}
	virtual ~shape_chain_element() = default;
	//drawable_chain_element* up_cast() { drawable_chain_element* result = this; return result; }

	shape_type type;
	float radius = 0;
	bool hollow;
};
