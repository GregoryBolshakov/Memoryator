#pragma once

#include "world_object.h"

class static_object : public world_object
{
public:
	static_object(std::string objectName, sf::Vector2f centerPosition);
	virtual ~static_object();
	void set_position(sf::Vector2f new_position) override;
	int get_sprite_number() override { return current_sprite_[0]; }	
	virtual void setType(int newType) = 0;
};
