#pragma once

#include "terrain_object.h"

namespace mr
{
	class log : public terrain_object
	{
	public:
		log(std::string objectName, Vector2f centerPosition, int typeOfObject);
		Vector2f calculateTextureOffset() override;
		void initPedestal() override;
		std::vector<sprite_chain_element*> prepareSprites(long long elapsedTime) override;
		int strength;
		void setType(int typeOfObject) override;
		Vector2f getBuildPosition(std::vector<world_object*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
		int getBuildType(Vector2f ounPos, Vector2f otherPos) override;
	};
}