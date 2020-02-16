#pragma once
#ifndef INVENTORYSYSTEM_H
#define INVENTORYSYSTEM_H

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include<fstream>
#include "TextSystem.h"
#include "UIEffectsSystemMaker.h"
#include "WorldObject.h"
#include "HeroBag.h"
#include "SpriteStructures.h"

using namespace sf;

class InventorySystem
{
public:
	InventorySystem();
	~InventorySystem();
	void init();
	void drawHeroInventory(long long elapsedTime, RenderWindow& window);
	void drawInventory(std::vector<std::pair<Tag, int>>* inventory, Vector2f position, RenderWindow& window);
	void resetAnimationValues();
	void onMouseUp();
	void inventoryBounding(std::vector<HeroBag>* bags);
	void interact(long long elapsedTime);
	void resetPickedCell() { pickedCell = nullptr; }
	bool getUsedMouse() const { return usedMouse; }	
	Cell &getHeldItem() { return heldItem; }
	std::unordered_map<Tag, CellSprite> getSpriteList() const { return cellsSpriteList; }

	bool wasDrawing = false;
	std::string debugInfo = "", cursorText = "";

	Sprite *selectedCellBackground{}, dropZone;
	CircleShape bagPosDot;
	Texture dropZoneTexture;
private:
	//hero bags
	std::vector<HeroBag>* boundBags{};
	int currentMovingBag = -1;
	float minDistToClosed = 10e4, minDistToOpen = 10e4;

	//another inventories 
	int animationCounter = 1;
	//held item
	Cell heldItem;
	float heldItemSpeed = 0, dropZoneRadius{};
	std::pair <Tag, int>* pickedCell = nullptr; 
	//drawing
	std::string cellsFileDirectory = "Game/inventorySprites/cellsPath.txt", 
		bagsFileDirectory = "Game/inventorySprites/bagsPath.txt";
	std::unordered_map<Tag, BagSprite> bagsSpriteList;
	std::unordered_map<Tag, CellSprite> cellsSpriteList;
	void initBags();
	void initCells();	
	void initSpriteLists();
	
	void crashIntoOtherBags(int cnt);
	void moveOtherBags(int cur, std::vector<int> ancestors = {});
	std::vector<bool> visitedInDisplacement;
	bool usedMouse = false, cursorBlurUsing = false;
	Vector2f cursorTextPos = {0, 0};
	TextSystem textWriter;
	UIEffectsSystemMaker uiEffectsSystem;
	//std::vector<std::pair<Vector2f, std::string>> temporaryPositions;
};

#endif