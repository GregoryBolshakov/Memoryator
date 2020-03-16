#pragma once

#include <unordered_map>
#include <windows.h>

#include "brazier.h"
#include "camera_system.h"
#include "dynamic_object.h"
#include "effects_system.h"
#include "empty_object.h"
#include "inventory_system.h"
#include "light_system.h"
#include "pedestal_controller.h"
#include "scale_system.h"
#include "world_generator.h"
#include "shape_chain_element.h"

using namespace sf;

class world_handler
{
public:
	world_handler(
		int width,
		int height,
		camera_system& camera_system,
		std::map<pack_tag, sprite_pack>* packsMap);
	
	~world_handler();

	//adding to the grid
	void birthObjects();

	// Getters
	grid_list& getStaticGrid() { return staticGrid; }
	grid_list& getDynamicGrid() { return dynamicGrid; }
	inventory_system& getInventorySystem() { return inventorySystem; }
	build_system& getBuildSystem() { return buildSystem; }
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

	void setScaleFactor(int delta);
	
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
	Vector2f  world_handler::mouse_position() const;

	// Systems
	camera_system cameraSystem;
	scale_system& scale_system_;
	effects_system effectSystem;
	inventory_system inventorySystem;
	build_system buildSystem;
	light_system lightSystem;

	// Grids
	grid_list staticGrid;
	grid_list dynamicGrid;
	std::vector<sprite_chain_element*> visibleBackground, visibleTerrain;
	std::vector<world_object*> localTerrain;

	// Test
	std::vector<std::pair<entity_tag, int>>* testInv = new std::vector<std::pair<entity_tag, int>>({ {entity_tag::chamomile, 2}, {entity_tag::chamomile, 2}, {entity_tag::chamomile, 2} });
};
