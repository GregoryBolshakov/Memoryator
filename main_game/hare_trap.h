#pragma once

#include "picked_object.h"

class hare_trap : public picked_object
{
public:
	hare_trap(std::string objectName, Vector2f centerPosition, int typeOfObject);
	Vector2f calculate_texture_offset() override;
	void init_pedestal() override;
	std::vector<sprite_chain_element*> prepare_sprites(long long elapsedTime) override;
	void setType(int typeOfObject) override;
	Vector2f get_build_position(std::vector<world_object*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	[[nodiscard]] Vector2f getEnterPosition() const;
	int get_build_type(Vector2f ounPos, Vector2f otherPos) override;
};
