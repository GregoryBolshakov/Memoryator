#pragma once
#ifndef WORLDHANDLER_H
#define WORLDHANDLER_H

#include <unordered_map>
#include <ltbl/lighting/LightSystem.h>
#include <windows.h>

#include "Helper.h"
#include "EffectsSystemMaker.h"
#include "WorldGenerator.h"
#include "PedestalController.h"
#include "DynamicObject.h"
#include "EmptyObject.h"
#include "Brazier.h"
#include "InventorySystem.h"
#include "TimeSystem.h"
#include "LightSystem.h"

using namespace sf;

class WorldHandler
{
public:
	WorldHandler(int width, int height, std::map<PackTag, SpritePack>* packsMap);
	~WorldHandler();

	//adding to the grid
	void birthObjects();

	// Getters
	Vector2f getWorldSize() { return Vector2f (width, height); }
	GridList& getStaticGrid() { return staticGrid; }
	GridList& getDynamicGrid() { return dynamicGrid; }
	Vector2f getCameraPosition() { return cameraPosition; }
	InventorySystem& getInventorySystem() { return inventorySystem; }
	BuildSystem& getBuildSystem() { return buildSystem; }
	TimeSystem& getTimeSystem() { return timeSystem; }
	WorldGenerator& getWorldGenerator() { return worldGenerator; }
	LightSystem& getLightSystem() { return lightSystem; }
	std::string getMouseDisplayName() const { return mouseDisplayName; }
	WorldObject* getSelectedObject() const { return selectedObject; }
	std::vector<WorldObject*> getLocalTerrain() { return localTerrain; }

	// Save-load logic
	void clearWorld();
	void Load();
	void Save();
	
	// Base (draw, interact)
	std::map<PackTag, SpritePack>* packsMap;
	void interact(RenderWindow& window, long long elapsedTime, Event event);
	void handleEvents(Event& event);
    std::vector<SpriteChainElement> prepareSprites(long long elapsedTime, bool onlyBackground = false);
	//void draw(RenderWindow& window, long long elapsedTime);
	void setItemFromBuildSystem();
	//void drawVisibleItems(RenderWindow& window, long long elapsedTime, std::vector<SpriteChainElement> sprites);
	Vector2f worldUpperLeft, worldBottomRight;
	//void runBuildSystemDrawing(RenderWindow& window, long long elapsedTime);
	//void runInventorySystemDrawing(RenderWindow& window, long long elapsedTime);
	void runWorldGenerator();
	PedestalController pedestalController;

	// Zoom	
	void setScaleFactor(int delta);
	void scaleSmoothing();
	float scaleDecrease, timeForScaleDecrease = 0;
	Clock scaleDecreaseClock;

	// Hero
	DynamicObject* focusedObject = nullptr;
	Brazier* brazier;

	// Events
	void onMouseUp(int currentMouseButton);
	bool getHeroBookVisibility() const { return isHeroBookVisible; }
	void changeBookVisibility() { isHeroBookVisible = !isHeroBookVisible; }

	void setObjectToBuild(Tag tag, int type = 1, bool instantBuild = false) { buildSystem.selectedObject = tag; buildSystem.buildType = type; buildSystem.instantBuild = instantBuild; }
	Vector2i currentTransparentPos = Vector2i(0, 0);
	std::string debugInfo = "";
private:
	// Hero
	const std::string heroTextureName = "Game/worldSprites/hero/move/body/down/1.png";
	bool isHeroBookVisible = false;

	// World base
	float width, height;
	Vector2i blockSize = { 1000, 1000 }, microblockSize = { 20, 20 };
	Vector2f cameraPosition, maxCameraDistance = Vector2f(100, 100), camOffset = { 0, -0.04f};
	std::string spriteNameFileDirectory = "Game/objects.txt";
	const float heroToScreenRatio = 0.25f;
	bool fixedClimbingBeyond(Vector2f &pos);
	void makeCameraShake(float power = 0.0002f);
	void cameraShakeInteract(long long elapsedTime);
	Vector2f cameraShakeVector = { 0, 0 };
	float cameraShakeDuration = 0, cameraShakePower = 0.0002f;
	WorldGenerator worldGenerator;

	// Time logic
	Clock timer;
	int newNameId = 10;	
	float timeForNewSave = 0, timeAfterSave = 6e7;
	float timeForNewRoute = 3e5, timeAfterNewRoute = 3e5;

	// Selection logic
	void setTransparent(std::vector<WorldObject*>& visibleItems, long long elapsedTime);
	std::string mouseDisplayName;
	WorldObject *selectedObject = focusedObject;

	// Shaders
	sf::Shader spiritWorldShader;
	sf::Texture distortionMap;
	void initShaders();
	EffectsSystemMaker effectSystem;
	UIEffectsSystemMaker uiEffectSystem;

	// Systems
	InventorySystem inventorySystem;
	BuildSystem buildSystem;
	TimeSystem timeSystem;
	LightSystem lightSystem;

	// Grids
	GridList staticGrid;
	GridList dynamicGrid;
	std::vector<SpriteChainElement> visibleBackground, visibleTerrain;
	std::vector<WorldObject*> localTerrain;

	// Test
	std::vector<std::pair<Tag, int>>* testInv = new std::vector<std::pair<Tag, int>>({ {Tag::chamomile, 2}, {Tag::chamomile, 2}, {Tag::chamomile, 2} });
};

#endif