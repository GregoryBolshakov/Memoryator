#pragma once
#ifndef WORLDHANDLER_H
#define WORLDHANDLER_H

#include <unordered_map>
#include <windows.h>

#include "Helper.h"
#include "EffectsSystem.h"
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
	GridList& getStaticGrid() { return staticGrid; }
	GridList& getDynamicGrid() { return dynamicGrid; }
	Vector2f getCameraPosition() const { return cameraPosition; }
	InventorySystem& getInventorySystem() { return inventorySystem; }
	BuildSystem& getBuildSystem() { return buildSystem; }
	TimeSystem& getTimeSystem() { return timeSystem; }
	WorldGenerator& getWorldGenerator() { return worldGenerator; }
	LightSystem& getLightSystem() { return lightSystem; }
	std::string getMouseDisplayName() const { return mouseDisplayName; }
	WorldObject* getSelectedObject() const { return selectedObject; }
	std::vector<WorldObject*> getLocalTerrain() const { return localTerrain; }

	// Save-load logic
	void clearWorld();
	void Load();
	void Save();
	
	// Base (draw, interact)
	std::map<PackTag, SpritePack>* packsMap;
	void interact(Vector2f render_target_size, long long elapsedTime, Event event);
	void handleEvents(Event& event);
    std::vector<SpriteChainElement*> prepareSprites(long long elapsedTime, bool onlyBackground = false);
	//void draw(RenderWindow& window, long long elapsedTime);
	void setItemFromBuildSystem();
	//void drawVisibleItems(RenderWindow& window, long long elapsedTime, std::vector<SpriteChainElement*> sprites);
	Vector2f worldUpperLeft, worldBottomRight;
	//void runBuildSystemDrawing(RenderWindow& window, long long elapsedTime);
	//void runInventorySystemDrawing(RenderWindow& window, long long elapsedTime);
	void runWorldGenerator();
	PedestalController pedestalController;

	// Zoom	
	void setScaleFactor(int delta);
	void scaleSmoothing();
	float scaleDecrease{}, timeForScaleDecrease = 0;
	Clock scaleDecreaseClock;

	// Hero
	DynamicObject* focusedObject = nullptr;
	Brazier* brazier{};

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

	int width;
	int height;
	Vector2f blockSize = { 1000, 1000 };
	Vector2f microBlockSize = { 20, 20 };
	Vector2f cameraPosition;
	const Vector2f maxCameraDistance = Vector2f(250, 250), camOffset = { 0, -0.04f };
	std::string spriteNameFileDirectory = "Game/objects.txt";
	bool fixedClimbingBeyond(Vector2f &pos) const;
	void makeCameraShake();
	void cameraShakeInteract(long long elapsedTime);
	Vector2f cameraShakeVector = { 0, 0 };
	long long cameraShakeDuration = 0;
	float cameraShakePower = 0.0002f;
	WorldGenerator worldGenerator;

	// Time logic
	Clock timer;
	long long timeForNewSave = 0;
	long long timeAfterSave = long(6e7);
	long long timeForNewRoute = long(3e5);
	long long timeAfterNewRoute = long(3e5);

	// Selection logic
	void setTransparent(std::vector<WorldObject*>& visibleItems);
	std::string mouseDisplayName;
	WorldObject* selectedObject = focusedObject;

	// Shaders
	/*Shader spiritWorldShader;
	Texture distortionMap;
	void initShaders();*/

	EffectsSystem effectSystem;

	// Systems
	InventorySystem inventorySystem;
	BuildSystem buildSystem;
	TimeSystem timeSystem;
	LightSystem lightSystem;

	// Grids
	GridList staticGrid;
	GridList dynamicGrid;
	std::vector<SpriteChainElement*> visibleBackground, visibleTerrain;
	std::vector<WorldObject*> localTerrain;

	// Test
	std::vector<std::pair<Tag, int>>* testInv = new std::vector<std::pair<Tag, int>>({ {Tag::chamomile, 2}, {Tag::chamomile, 2}, {Tag::chamomile, 2} });
};

#endif