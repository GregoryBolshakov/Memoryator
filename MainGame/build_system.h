#pragma once

#include <SFML/Graphics.hpp>

#include "grid_list.h"
#include "helper.h"
#include "inventory_system.h"
#include "sprite_pack.h"
#include "world_object.h"

using namespace sf;

struct object_info {
	std::string type, icon;
	Sprite sprite, iconSprite;
	Texture iconTexture;
	std::vector<std::pair<entity_tag, int>> recipe;
};

class build_system
{
public:	
	build_system();
	~build_system();
	void inventoryBounding(std::vector<hero_bag>* boundBags);
	void init();	
	void onMouseUp();
	void buildHeldItem(Vector2f focusedObjectPosition, float scaleFactor);
	void interact(Vector2f cameraPosition = {0, 0}, float scaleFactor = 1);
	void clearHareBags(int block, grid_list& staticGrid, std::vector<world_object*>* visibleItems);
	void wasPlaced();
	std::vector<sprite_chain_element*> prepareSprites(grid_list& staticGrid, const std::vector<world_object*>& visibleItems, std::map<pack_tag, sprite_pack>* packsMap);

	int getCurrentObject() const { return currentObject; }
	bool getUsedMouse() const { return usedMouse; }
	bool getSuccessInit() const { return successInit; }

	void setCurrentObject(int n) { currentObject = n; }
	void setHeldItem(std::pair<entity_tag, int>& heldItem) { if (nullptr != &heldItem) this->heldItem = &heldItem; }
	
	bool canAfford();

	std::vector<hero_bag>* boundBags;
	bool canBePlaced = false, instantBuild = false;
	entity_tag selectedObject = entity_tag::emptyCell;
	Vector2f buildingPosition = { -1, -1 };
	std::map<entity_tag, bool> droppedLootIdList = { {entity_tag::heroBag, 1} };
	int buildType = 1;
private:
	//std::unordered_map<Tag, CellSprite> craftIngredientsSpriteList;
	std::pair<entity_tag, int> *heldItem = nullptr;
	Vector2f mouseWorldPos = { 0, 0 };
	Vector2f spriteBuildPos = Vector2f (-1, -1);
	void initializeObjectsInfo();
	void animator(long long elapsedTime);	
	float originalAnimationSpeed = 0.001f, animationSpeed = 0.001f;
	bool isPressed = false, usedMouse = false, successInit = false;
	std::vector<object_info> builtObjects;
	int currentObject = -1;
	std::string BuildSystemObjectsInfoFileDirectory = "Game/buildSystem/buildSystemObjectsInfo.txt";
	Font font;
};
