#pragma once

#include "world_object.h"

class static_object : public world_object
{
public:
	static_object(std::string name, sf::Vector2f position, int kind);
	virtual ~static_object();
};
