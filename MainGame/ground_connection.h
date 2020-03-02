#pragma once

#include "terrain_object.h"

class ground_connection : public static_object
{
public:
	ground_connection(std::string object_name, Vector2f center_position, int type_of_object);
	Vector2f calculateTextureOffset() override;
	std::vector<sprite_chain_element*> prepareSprites(long long elapsedTime) override;
	int strength{};
	void setType(int type_of_object) override;
	Vector2f getBuildPosition(std::vector<world_object*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	int getBuildType(Vector2f ounPos, Vector2f otherPos) override;
};
