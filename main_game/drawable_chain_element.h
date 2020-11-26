#pragma once

#include <SFML/Graphics.hpp>

class drawable_chain_element
{
public:
	drawable_chain_element();
	drawable_chain_element(
		sf::Vector2f position,
		sf::Vector2f size,
		sf::Vector2f offset,
		sf::Color color = sf::Color(255, 255, 255, 255),
		float rotation = 0);
	virtual ~drawable_chain_element() = default;
	drawable_chain_element(drawable_chain_element&& element) noexcept
	{
		this->offset = element.offset;
		this->position = element.position;
		this->size = element.size;
		this->rotation = element.rotation;
		this->color = element.color;
		this->initialized = element.initialized;
	}
	void initialize() { this->initialized = true; }

	sf::Vector2f offset = { 0, 0 }, position = { 0, 0 }, size = { 0, 0 };
	float rotation = 0;
	sf::Color color = sf::Color(255, 255, 255);
	bool initialized = false;
};
