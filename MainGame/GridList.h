#pragma once
#ifndef GRIDLIST_H
#define GRIDLIST_H

#include <vector>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "WorldObject.h"

using namespace sf;

const int inf = 1e3;

class GridList
{
private:
	int width, height;
	Vector2f size = { 0, 0 }, microSize = { 0, 0 };
	std::vector<std::vector<WorldObject*>> cells;
	std::unordered_map<std::string, std::pair<int, int>> items;	
	void GridList::bfs(int xBorder, int yBorder, int startX, int startY, int finishX, int finishY, int permissibleDistance);
public:
	std::unordered_map<std::string, std::pair<int, int>> getItemsOfGreed() { return items; }
	GridList();
	GridList(int width, int height, Vector2f size, Vector2f microSize);
	~GridList();
	Vector2f getBlockSize() const;
	int getBlockItemsAmount(int index) { return cells[index].size(); }
	int getIndexByPoint(int x, int y) const;
	Vector2f getPointByIndex(int index) const;
	int getMicroBlockByPoint(int x, int y) const;
	Vector2f getPointByMicroBlock(int microBlockIndex) const;
	
	void makeRoute(Vector2f startPos, Vector2f finishPos, int upperLeftX, int upperLeftY, int bottomRightX, int bottomRightY, int permissibleDistance = 1);
	void addItem(WorldObject* item, const std::string& name, int x, int y);
	WorldObject* getItemByName(const std::string& name);
	std::vector<WorldObject*> getItems(int upperLeftX, int upperLeftY, int bottomRightX, int bottomRightY);
	std::vector<WorldObject*> getItems(int blockIndex);
	void updateItemPosition(const std::string name, int x, int y);
	std::vector<std::vector<WorldObject*>> getCells(){ return cells; }
	void clearCell(int cellIndex);
	void deleteItem(std::string name);
	void setLockedMicroBlocks(WorldObject* item, bool value = false, bool dynamicMatrix = false);
	void boundDynamicMatrix(std::vector<std::vector<bool>>* matrix) { this->dynamicMicroBlockMatrix = matrix; }
	int getSize() const { return items.size(); }
	std::vector<int> getBlocksAround(int upperLeftX, int upperLeftY, int bottomRightX, int bottomRightY, int offset);
	std::vector<int> GridList::getBlocksInSight(int upperLeftX, int upperLeftY, int bottomRightX, int bottomRightY);
	bool isIntersectWithOthers(WorldObject* object, std::vector<WorldObject*> visibleTerrain, bool isDotAdjustded = false) const;

	std::vector<std::vector<bool>> microBlockMatrix;
	std::vector<std::vector<bool>>* dynamicMicroBlockMatrix = nullptr;
	std::vector<std::vector<float>> distances;
	std::vector<std::vector<std::pair<int, int>>> previous;
	std::vector<std::pair<int, int>> route = {{}};
	std::pair<int, int> reserveDestination = std::make_pair(-1, -1);
};

//#include "GridList.cpp"
#endif

