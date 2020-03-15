#pragma once
#ifndef WORLDHANDLER_H
#define WORLDHANDLER_H

#include <unordered_map>
#include <windows.h>

#include "brazier.h"
#include "camera_system.h"
#include "dynamic_object.h"
#include "effects_system.h"
#include "empty_object.h"
#include "helper.h"
#include "inventory_system.h"
#include "light_system.h"
#include "pedestal_controller.h"
#include "time_system.h"
#include "world_generator.h"
#include "shape_chain_element.h"

using namespace sf;

class world_handler
{
public:
	world_handler(int width, int height, std::map<pack_tag, sprite_pack>* packsMap);
	~world_handler();

	//adding to the grid
	void birthObjects();

	// Getters
	grid_list& getStaticGrid() { return staticGrid; }
	grid_list& getDynamicGrid() { return dynamicGrid; }
	Vector2f getCameraPosition() const	{ return cameraSystem.position; }
	camera_system& get_camera_system() {return cameraSystem; }
	inventory_system& getInventorySystem() { return inventorySystem; }
	build_system& getBuildSystem() { return buildSystem; }
	time_system& getTimeSystem() { return timeSystem; }
	world_generator& getWorldGenerator() { return worldGenerator; }
	light_system& getLightSystem() { return lightSystem; }
	std::string getMouseDisplayName() const { return mouseDisplayName; }
	world_object* getSelectedObject() const { return selectedObject; }
	std::vector<world_object*> getLocalTerrain() const { return localTerrain; }

	// Save-load logic
	void clearWorld();
	void Load();
	void Save();
	
	// Base (draw, interact)
	std::map<pack_tag, sprite_pack>* packsMap;
	void interact(Vector2f render_target_size, long long elapsedTime, Event event);
	void handleEvents(Event& event);
    std::vector<drawable_chain_element*> prepareSprites(long long elapsedTime, bool onlyBackground = false);
	//void draw(RenderWindow& window, long long elapsedTime);
	void setItemFromBuildSystem();
	//void drawVisibleItems(RenderWindow& window, long long elapsedTime, std::vector<SpriteChainElement*> sprites);
	Vector2f worldUpperLeft, worldBottomRight;
	//void runBuildSystemDrawing(RenderWindow& window, long long elapsedTime);
	//void runInventorySystemDrawing(RenderWindow& window, long long elapsedTime);
	void runWorldGenerator();
	pedestal_controller pedestalController;

	// Zoom	
	void setScaleFactor(int delta);
	float get_scale_delta_normalized() const;
	void scaleSmoothing();
	float scaleDecrease{}, timeForScaleDecrease = 0;
	Clock scaleDecreaseClock;

	// Hero
	dynamic_object* focusedObject = nullptr;
	brazier* brazier{};
	Vector2f focused_object_screen_position_normalized() const;

	// Events
	void onMouseUp(int currentMouseButton);
	bool getHeroBookVisibility() const { return isHeroBookVisible; }
	void changeBookVisibility() { isHeroBookVisible = !isHeroBookVisible; }

	void setObjectToBuild(entity_tag tag, int type = 1, bool instantBuild = false) { buildSystem.selected_object = tag; buildSystem.build_type = type; buildSystem.instant_build = instantBuild; }
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
	std::string spriteNameFileDirectory = "Game/objects.txt";
	bool fixedClimbingBeyond(Vector2f &pos) const;
	Vector2f object_screen_position(dynamic_object* obj) const;
	
	world_generator worldGenerator;

	// Time logic
	Clock timer;
	long long timeForNewSave = 0;
	long long timeAfterSave = long(6e7);

	// Selection logic
	void setTransparent(std::vector<world_object*>& visibleItems);
	std::string mouseDisplayName;
	world_object* selectedObject = focusedObject;

	// Shaders
	/*Shader spiritWorldShader;
	Texture distortionMap;
	void initShaders();*/

	effects_system effectSystem;

	// Systems
	camera_system cameraSystem;
	inventory_system inventorySystem;
	build_system buildSystem;
	time_system timeSystem;
	light_system lightSystem;

	// Grids
	grid_list staticGrid;
	grid_list dynamicGrid;
	std::vector<sprite_chain_element*> visibleBackground, visibleTerrain;
	std::vector<world_object*> localTerrain;

	// Test
	std::vector<std::pair<entity_tag, int>>* testInv = new std::vector<std::pair<entity_tag, int>>({ {entity_tag::chamomile, 2}, {entity_tag::chamomile, 2}, {entity_tag::chamomile, 2} });
};

#endif