#pragma once

#include <SFML/Graphics.hpp>

#include "world_object.h"

class empty_object : public world_object
{
public:
	empty_object(std::string objectName, Vector2f centerPosition);
	~empty_object();
	std::vector<sprite_chain_element*> prepareSprites(long long elapsedTime) override;
	int getSpriteNumber() override;
	Vector2f getBuildPosition(std::vector<world_object*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	int getBuildType(Vector2f ounPos, Vector2f otherPos) override;
	void setTextureSize(Vector2f textureSize) override;
	Vector2f calculateTextureOffset() override;
	void initPedestal() override;
};

