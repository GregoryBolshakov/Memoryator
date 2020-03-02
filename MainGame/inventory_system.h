#pragma once

#include <SFML/Graphics.hpp>

#include "text_system.h"
#include "effects_system.h"
#include "hero_bag.h"
#include "sprite_structures.h"

using namespace sf;

class inventory_system
{
public:
	inventory_system();
	~inventory_system();
	void init();
	std::vector<drawable_chain_element*> prepareSprites(long long elapsedTime, std::map<PackTag, sprite_pack>* packsMap);
	//void drawInventory(std::vector<std::pair<Tag, int>>* inventory, Vector2f position, RenderWindow& window);
	void resetAnimationValues();
	void onMouseUp();
	void inventoryBounding(std::vector<hero_bag>* bags) { boundBags = bags; }
	void interact(long long elapsedTime);
	void resetPickedCell() { pickedCell = nullptr; }
	bool getUsedMouse() const { return usedMouse; }	
	cell &getHeldItem() { return heldItem; }

	bool wasDrawing = false;
	std::string debugInfo = "", cursorText = "";

	Sprite *selectedCellBackground{}, dropZone;
	CircleShape bagPosDot;
	Texture dropZoneTexture;
private:
	//hero bags
	std::vector<hero_bag>* boundBags{};
	int currentMovingBag = -1;
	float minDistToClosed = 10e4, minDistToOpen = 10e4;

	//another inventories 
	int animationCounter = 1;

	//held item
	cell heldItem;
	float heldItemSpeed = 0, dropZoneRadius{};
	std::pair <Tag, int>* pickedCell = nullptr; 
	//drawing
	std::string cellsFileDirectory = "Game/inventorySprites/cellsPath.txt", 
		bagsFileDirectory = "Game/inventorySprites/bagsPath.txt";
	
	void crashIntoOtherBags(int cnt) const;
	void moveOtherBags(int cur) const;
	static void initMaxCounts(const std::string& filePath = "Game/maxCounts.txt");
	std::vector<bool> visitedInDisplacement;
	bool usedMouse = false, cursorBlurUsing = false, successInit = false;
	Vector2f cursorTextPos = {0, 0};
	text_system textWriter;
	effects_system effectsSystem;
};
