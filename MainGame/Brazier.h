#pragma once
#ifndef BRAZIER_H
#define BRAZIER_H

#include "TerrainObject.h"
#include "BuildSystem.h"

class Brazier : public TerrainObject
{
public:
	Brazier(std::string objectName, Vector2f centerPosition, int typeOfObject);
	Vector2f calculateTextureOffset() override;
	void initPedestal() override;
	void initCraftRecipes();
	std::vector<SpriteChainElement*> prepareSprites(long long elapsedTime) override;
	void setType(int typeOfObject) override;
	Vector2f getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	int getBuildType(Vector2f ounPos, Vector2f otherPos) override;

	// craft
	//void linkWithBuildSystem(BuildSystem* buildSystem) { this->buildSystem = buildSystem; }
	void putItemToCraft(Tag id);
	void clearCurrentCraft() { currentCraft.clear(); }
	Vector2f getPlatePosition() const { return { position.x + conditionalSizeUnits.x * 0.06f, position.y - conditionalSizeUnits.x * 0.025f }; }
	Tag checkCraftResult();	

	int getPlateRadius() const { return plateRadius; }

	//BuildSystem* buildSystem = nullptr;
	//------
private:
	void resultAnalysis();

	std::string craftRecipesPath = "Game/craftRecipes.txt";
	std::map<Tag, std::vector<std::vector<std::pair<Tag, int>>>> craftRecipes;
	std::map<Tag, int> currentCraft;
	Tag craftResult = Tag::emptyCell;
	int plateRadius;
};

#endif