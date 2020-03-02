#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector>

#include "world_object.h"

using namespace sf;

const float inf = 1e3;

class grid_list
{
public:
	[[nodiscard]] std::unordered_map<std::string, std::pair<int, int>> getItemsOfGreed() const { return items; }
	grid_list();
	grid_list(int width, int height, Vector2f size, Vector2f microSize);
	~grid_list();
	[[nodiscard]] Vector2f getBlockSize() const;
	size_t getBlockItemsAmount(const int index) { return cells[index].size(); }
	[[nodiscard]] int getIndexByPoint(float x, float y) const;
	[[nodiscard]] Vector2f getPointByIndex(int index) const;
	[[nodiscard]] int getMicroBlockByPoint(float x, float y) const;
	[[nodiscard]] Vector2f getPointByMicroBlock(int microBlockIndex) const;
	
	void makeRoute(Vector2f startPos, Vector2f finishPos, float upperLeftX, float upperLeftY, float bottomRightX, float bottomRightY, int permissibleDistance = 1);
	void addItem(world_object* item, const std::string& name, float x, float y);
	world_object* getItemByName(const std::string& name);
	std::vector<world_object*> getItems(float upperLeftX, float upperLeftY, float bottomRightX, float bottomRightY);
	std::vector<world_object*> getItems(int blockIndex);
	void updateItemPosition(const std::string& name, const float x, const float y);

	[[nodiscard]] std::vector<std::vector<world_object*>> getCells() const { return cells; }
	
	void clearCell(int cellIndex);
	void deleteItem(const std::string& name);
	void setLockedMicroBlocks(world_object* item, bool value = false, bool dynamicMatrix = false);
	void boundDynamicMatrix(std::vector<std::vector<bool>>* matrix) { this->dynamicMicroBlockMatrix = matrix; }
	
	[[nodiscard]] size_t getSize() const { return items.size(); }

	[[nodiscard]] std::vector<int> getBlocksAround(float upperLeftX, float upperLeftY, float bottomRightX, float bottomRightY) const;
	[[nodiscard]] std::vector<int> getBlocksInSight(float upperLeftX, float upperLeftY, float bottomRightX, float bottomRightY) const;
	bool isIntersectWithOthers(world_object* object, const std::vector<world_object*>& visibleTerrain, bool isDotAdjusted = false) const;

	std::vector<std::vector<bool>> microBlockMatrix;
	std::vector<std::vector<bool>>* dynamicMicroBlockMatrix = nullptr;
	std::vector<std::vector<float>> distances;
	std::vector<std::vector<std::pair<int, int>>> previous;
	std::vector<std::pair<int, int>> route = {{}};
	std::pair<int, int> reserveDestination = std::make_pair(-1, -1);

private:
	int width, height;
	Vector2f size = { 0, 0 }, microSize = { 0, 0 };
	std::vector<std::vector<world_object*>> cells;
	std::unordered_map<std::string, std::pair<int, int>> items;	
	void bfs(int xBorder, int yBorder, int startX, int startY, int finishX, int finishY);
	int width_to_size_x_{};
	int height_to_size_y_{};
	int width_to_micro_x_{};
	int height_to_micro_y_{};
};


