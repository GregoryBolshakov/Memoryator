#pragma once

#include "terrain_object.h"
#include "build_system.h"

class brazier : public terrain_object
{
public:
	brazier(std::string objectName, Vector2f centerPosition, int typeOfObject);
	Vector2f calculateTextureOffset() override;
	void initPedestal() override;
	void initCraftRecipes();
	std::vector<sprite_chain_element*> prepareSprites(long long elapsedTime) override;
	void setType(int typeOfObject) override;
	Vector2f getBuildPosition(std::vector<world_object*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	int getBuildType(Vector2f ounPos, Vector2f otherPos) override;

	// craft
	//void linkWithBuildSystem(BuildSystem* buildSystem) { this->buildSystem = buildSystem; }
	void putItemToCraft(Tag id);
	void clearCurrentCraft() { currentCraft.clear(); }
	[[nodiscard]] Vector2f getPlatePosition() const { return { position.x + conditionalSizeUnits.x * 0.06f, position.y - conditionalSizeUnits.x * 0.025f }; }
	Tag checkCraftResult();

	[[nodiscard]] float getPlateRadius() const { return plateRadius; }

	//BuildSystem* buildSystem = nullptr;
	//------
private:
	void resultAnalysis() const;

	std::string craftRecipesPath = "Game/craftRecipes.txt";
	std::map<Tag, std::vector<std::vector<std::pair<Tag, int>>>> craftRecipes;
	std::map<Tag, int> currentCraft;
	Tag craftResult = Tag::emptyCell;
	float plateRadius;
};
