#pragma once
#ifndef INVENTORYSYSTEM_H
#define INVENTORYSYSTEM_H

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include<fstream>
#include "TextSystem.h"
#include "EffectsSystem.h"
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
	std::vector<DrawableChainElement*> prepareSprites(long long elapsedTime, std::map<PackTag, SpritePack>* packsMap);
	//void drawInventory(std::vector<std::pair<Tag, int>>* inventory, Vector2f position, RenderWindow& window);
	void resetAnimationValues();
	void onMouseUp();
	void inventoryBounding(std::vector<HeroBag>* bags) { boundBags = bags; }
	void interact(long long elapsedTime);
	void resetPickedCell() { pickedCell = nullptr; }
	bool getUsedMouse() const { return usedMouse; }	
	Cell &getHeldItem() { return heldItem; }

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
	
	void crashIntoOtherBags(int cnt);
	void moveOtherBags(int cur, std::vector<int> ancestors = {});
	void initMaxCounts(std::string filePath = "Game/maxCounts.txt");
	std::vector<bool> visitedInDisplacement;
	bool usedMouse = false, cursorBlurUsing = false, successInit = false;
	Vector2f cursorTextPos = {0, 0};
	TextSystem textWriter;
	EffectsSystem effectsSystem;
};

#endif