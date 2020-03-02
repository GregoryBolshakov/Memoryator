#pragma once

#include "picked_object.h"

class hare_trap : public picked_object
{
public:
	hare_trap(std::string objectName, Vector2f centerPosition, int typeOfObject);
	Vector2f calculateTextureOffset() override;
	void initPedestal() override;
	std::vector<sprite_chain_element*> prepareSprites(long long elapsedTime) override;
	void setType(int typeOfObject) override;
	Vector2f getBuildPosition(std::vector<world_object*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	[[nodiscard]] Vector2f getEnterPosition() const;
	int getBuildType(Vector2f ounPos, Vector2f otherPos) override;
};
