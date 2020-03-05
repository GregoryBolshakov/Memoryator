#pragma once

#include "terrain_object.h"

class root : public terrain_object
{
protected:
public:
	root(std::string objectName, Vector2f centerPosition, int typeOfObject);
	Vector2f calculate_texture_offset() override;
	void init_pedestal() override;
	std::vector<sprite_chain_element*> prepare_sprites(long long elapsedTime) override;
	int strength;
	void setType(int typeOfObject) override;
	Vector2f get_build_position(std::vector<world_object*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	int get_build_type(Vector2f ounPos, Vector2f otherPos) override;
};
