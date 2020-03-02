#pragma once

#include "world_object.h"

class static_object : public world_object
{
public:
	static_object(std::string objectName, Vector2f centerPosition);
	virtual ~static_object();
	int getSpriteNumber() override { return currentSprite[0]; }	
	virtual void setType(int newType) = 0;
};
