#pragma once
#ifndef GRIDLIST_CPP
#define GRIDLIST_CPP

#include <cmath>
#include <queue>
#include "GridList.h"
#include "Helper.h"

GridList::GridList() : width(0), height(0), size(0, 0)
{
}

Vector2i GridList::getBlockSize() const
{
	return size;
}

GridList::GridList(int width, int height, Vector2i size, Vector2i microSize)
{
	this->width = width;
	this->height = height;
	this->size = size;
	this->microSize = microSize;
	auto vectorSize = int(ceil(double(height) / size.y) * ceil(double(width) / size.x));
	cells.resize(vectorSize);
	for (auto&arr : cells)
	{
		arr.resize(0);
	}
	microBlockMatrix.resize(4000, std::vector<bool>(4000, true));	
	distances.resize(4000, std::vector<float>(4000, 1000));
	previous.resize(4000, std::vector<std::pair<int, int>>(4000, { -1, -1 }));
}

GridList::~GridList()
{
	for (std::vector<WorldObject*> cell : cells)
	{
		for (WorldObject* ptr : cell)
		{
			delete ptr;
		}
		cell.clear();
	}
	cells.clear();
	items.clear();
}

int GridList::getIndexByPoint(int x, int y) const
{
	auto y1 = y / size.y;
	auto x1 = ceil(double(width) / size.x);
	auto result = x1 * y1 + x / size.x;
	return int(result);
}

Vector2f GridList::getPointByIndex(int index) const
{
	int inLineNumber = ceil(double(width) / size.x);
	int inRawNumber = (index) / inLineNumber;

	Vector2f result;
	result.x = (index % inLineNumber) * size.x;
	result.y = (inRawNumber * size.y);

	return result;
}

int GridList::getMicroblockByPoint(int x, int y) const
{
	auto y1 = y / microSize.y;
	auto x1 = ceil(double(width) / microSize.x);
	auto result = x1 * y1 + x / microSize.x;
	return int(result);
}

Vector2f GridList::getPointByMicroblock(int microBlockIndex) const
{
	int inLineNumber = ceil(double(width) / microSize.x);
	int inRawNumber = (microBlockIndex) / inLineNumber;

	Vector2f result = Vector2f((microBlockIndex % inLineNumber) * microSize.x, result.y = (inRawNumber * microSize.y));

	return result;
}

void GridList::makeRoute(Vector2f startPos, Vector2f finishPos, int upperLeftX, int upperLeftY, int bottomRightX, int bottomRightY, int permissibleDistance)
{
	route.clear();

	if (abs(startPos.x - finishPos.x) / microSize.x + abs(startPos.y - finishPos.y) / microSize.y < 1)
		return;

	const int xMicroblocksCount = ceil((bottomRightX - upperLeftX) / microSize.x);
	const int yMicroblocksCount = ceil((bottomRightY - upperLeftY) / microSize.y);

	const int startXInd = upperLeftX / microSize.x;
	const int startYInd = upperLeftY / microSize.y;

	int curMicroblockX = startPos.x / microSize.x, curMicroblockY = startPos.y / microSize.y;
	int lastMicroblockX = finishPos.x / microSize.x, lastMicroblockY = finishPos.y / microSize.y;

	bool isBreak = false;
	if (!microBlockMatrix[curMicroblockX][curMicroblockY]/* || !dynamicMicroBlockMatrix->at(curMicroblockX)[curMicroblockY]*/)
	{
		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
				if (curMicroblockX + i > 0 && curMicroblockX + i < width / microSize.x &&
					curMicroblockY + j > 0 && curMicroblockY + j < height / microSize.y && microBlockMatrix[curMicroblockX + i][curMicroblockY + j]/* && dynamicMicroBlockMatrix->at(curMicroblockX + i)[curMicroblockY + j]*/)
				{
					curMicroblockX += i;
					curMicroblockY += j;
					isBreak = true;
					break;
				}
			if (isBreak)
				break;
		}
	}

	if (!microBlockMatrix[lastMicroblockX][lastMicroblockY]/* || !dynamicMicroBlockMatrix->at(lastMicroblockX)[lastMicroblockY]*/)
	{
		for (int i = -permissibleDistance; i <= permissibleDistance; i++)
		{
			isBreak = false;
			for (int j = -permissibleDistance; j <= permissibleDistance; j++)
				if (lastMicroblockX + i > 0 && lastMicroblockX + i < width / microSize.x &&
					lastMicroblockY + j > 0 && lastMicroblockY + j < height / microSize.y && microBlockMatrix[lastMicroblockX + i][lastMicroblockY + j]/* && dynamicMicroBlockMatrix->at(lastMicroblockX + i)[lastMicroblockY + j]*/)
				{
					lastMicroblockX += i;
					lastMicroblockY += j;
					isBreak = true;
					break;
				}
			if (isBreak)
				break;
		}
	}	
	for (int i = startXInd; i < startXInd + xMicroblocksCount; i++)	
		for (int j = startYInd; j < startYInd + yMicroblocksCount; j++)		
			distances[i][j] = inf;

	distances[curMicroblockX][curMicroblockY] = 0;
	bfs(startXInd + xMicroblocksCount, startYInd + yMicroblocksCount, curMicroblockX, curMicroblockY, lastMicroblockX, lastMicroblockY, permissibleDistance);
}

void GridList::bfs(int xBorder, int yBorder, int startX, int startY, int finishX, int finishY, int permissibleDistance)
{
	std::queue<std::pair<int, int>> q;
	q.push(std::make_pair(startX, startY));	

	int minDistToFinish = 1000;
	float step = 1;
	while (!q.empty()) 
	{
		std::pair<int, int> v = q.front();
		q.pop();
		bool isBreak = false;

		for (int i = -1; i <= 1; i++)
			for (int j = -1; j <= 1; j++)
			{				
				if (i == 0 && j == 0)
					continue;
				if (abs(i) == abs(j))
					step = 1.42f;
				else
					step = 1.0f;

				std::pair<int, int> to = std::make_pair(v.first + i, v.second + j);
				if (to.first < 0 || to.first > xBorder || to.second < 0 || to.second > yBorder)
					continue;

				if (abs(previous[v.first][v.second].first - v.first) + abs(previous[v.first][v.second].second - v.second) <= 2 &&
					v.first - previous[v.first][v.second].first == to.first - v.first && v.second - previous[v.first][v.second].second == to.second - v.second)
					step -= 0.0001;


				if (distances[to.first][to.second] > distances[v.first][v.second] + step && ((microBlockMatrix[to.first][to.second] != 0 && dynamicMicroBlockMatrix->at(to.first)[to.second] != 0) || (to.first == finishX && to.second == finishY)))
				{
					distances[to.first][to.second] = distances[v.first][v.second] + step;
					previous[to.first][to.second] = v;
					q.push(to);
				}
				if (to.first == finishX && to.second == finishY)
				{
					isBreak = true;
					break;
				}
			}
		if (isBreak)
			break;
	}

	bool canCreateRoute = true;

	if ((finishX == -1) || distances[finishX][finishY] == inf)
	{
		canCreateRoute = false;
		route.clear();		
	}

	if (canCreateRoute)
	{
		std::vector<std::pair<int, int>> path;

		std::pair<int, int> currentMatrixCeil = previous[finishX][finishY];
		float iterations = 0;
		while (true)
		{
			iterations++;
			if (currentMatrixCeil == std::make_pair(-1, -1) || iterations > distances[finishX][finishY])
			{
				route.clear();
				return;
			}

			if (abs(previous[currentMatrixCeil.first][currentMatrixCeil.second].first - currentMatrixCeil.first) + abs(previous[currentMatrixCeil.first][currentMatrixCeil.second].second - currentMatrixCeil.second) > 2)
			{
				route.clear();
				return;
			}

			path.push_back(currentMatrixCeil);			
			currentMatrixCeil = previous[currentMatrixCeil.first][currentMatrixCeil.second];
			if (currentMatrixCeil == std::make_pair(startX, startY))
				break;
		}
		if (!path.empty())
			reverse(path.begin(), path.end());

		if (!path.empty() && path[0] == std::make_pair(startX, startY))
			path.erase(path.begin() + 0);

		// cut corners
		int cnt = 0;
		while (cnt < path.size() - 3)
			if (path[cnt + 1].first - path[cnt].first == path[cnt + 2].first - path[cnt + 1].first && path[cnt + 1].second - path[cnt].second == path[cnt + 2].second - path[cnt + 1].second)
				path.erase(path.begin() + cnt);
			else
				cnt++;
		//------------

		route = path;
	}
}

bool GridList::isIntersectWithOthers(Vector2f position1, float radius1, std::vector<WorldObject*> visibleTerrain, bool isDotAdjustded) const
{
	for (auto&anotherItem : visibleTerrain)
	{
		auto const anotherObject = dynamic_cast<WorldObject*>(anotherItem);
		if (!anotherObject)
			continue;

		if (anotherObject->isBackground || anotherObject->tag == Tag::hero1 || anotherObject->intangible)
			continue;

		if (anotherItem->isDotsAdjusted && isDotAdjustded)
			continue;

		Vector2i const position2 = Vector2i(anotherObject->getPosition());
		float const radius2 = anotherObject->getRadius();

		//if (!(abs(position1.x - position2.x) <= (radius1 + radius2) && abs(position1.y - position2.y) <= (radius1 + radius2)))
		if (int(sqrt(pow(position1.x - position2.x, 2) + pow(position1.y - position2.y, 2))) < radius1 + radius2)
			return true;
	}

	return false;
}

void GridList::setLockedMicroBlocks(WorldObject* item, bool value, bool dynamicMatrix)
{
	const auto worldItem = dynamic_cast<WorldObject*>(item);
	if (worldItem)
		for (const auto block : worldItem->getLockedMicroBlocks())
		{
			if (!(block.x < 0 || block.x > width / microSize.x || block.y < 0 || block.y > height / microSize.y))
			{
				if (dynamicMatrix)
					dynamicMicroBlockMatrix->at(block.x)[block.y] = value;
				else
					microBlockMatrix[block.x][block.y] = value;
			}
		}
}

void GridList::addItem(WorldObject* item, const std::string& name, int x, int y)
{
	setLockedMicroBlocks(item);
	int blocksCount = ceil(width / size.x) * ceil(height / size.y);

	if (items.find(name) != items.end())
		throw std::invalid_argument("The key " + name + " already exists in the Grid.");

	auto index = getIndexByPoint(x, y);	

	auto position = std::make_pair(index, int(cells[index].size()));

	

	cells[index].push_back(item);
	items.insert({ name, position });
}

void GridList::clearCell(int cellIndex)
{
	for (auto& item : cells[cellIndex])
	{
		setLockedMicroBlocks(item, true);		
		items.erase(items.find(item->getName()));
		item->~WorldObject();
		delete item;
	}
	cells[cellIndex].clear();
}

void GridList::deleteItem(std::string name)
{		
	auto position = items.at(name);
	setLockedMicroBlocks(cells[position.first][position.second], true);
	for (int i = position.second; i < cells[position.first].size(); i++)
	{
		auto itemToUpdate = dynamic_cast<WorldObject*>(cells[position.first][i]);
		auto itemName = itemToUpdate->getName();
		items.at(itemName).second -= 1;
	}
	auto itemObject = dynamic_cast<WorldObject*>(cells[position.first][position.second]);
	itemObject->~WorldObject();
	cells[position.first].erase(cells[position.first].begin() + position.second);
	items.erase(items.find(name));
}

WorldObject* GridList::getItemByName(const std::string& name)
{
	auto position = items.at(name);
	return cells[position.first][position.second];
}

std::vector<WorldObject*> GridList::getItems(int blockIndex)
{
	return cells[blockIndex];
}

std::vector<WorldObject*> GridList::getItems(int upperLeftX, int upperLeftY, int bottomRightX, int bottomRightY)
{
	std::vector<WorldObject*> result;
	if (upperLeftX <= 0)
	{
		upperLeftX = 0;
	}
	if (bottomRightX >= width)
	{
		bottomRightX = width;
	}
	if (upperLeftY <= 0)
	{
		upperLeftY = 0;
	}
	if (bottomRightY >= height)
	{
		bottomRightY = height;
	}
	auto rowsCount = int(ceil(double(bottomRightY - upperLeftY) / size.y));
	auto firstColumn = getIndexByPoint(upperLeftX, upperLeftY);
	auto lastColumn = getIndexByPoint(bottomRightX, upperLeftY);
	auto columnsPerRow = int(ceil(double(width) / size.x));
	auto maxColumn = int(cells.size()) - 1;

	for (auto i = 0; i <= rowsCount; i++)
	{
		
		if (lastColumn >= maxColumn)
			lastColumn = maxColumn;

		for (int j = firstColumn; j <= lastColumn; j++)
		{
			result.insert(result.end(), cells[j].begin(), cells[j].end());
		}

		firstColumn += columnsPerRow;
		lastColumn += columnsPerRow;
		
	}
	return result;
}

std::vector<int> GridList::getBlocksAround(int upperLeftX, int upperLeftY, int bottomRightX, int bottomRightY, int offset)
{
	std::vector<int> result;

	if (upperLeftX <= 0)
		upperLeftX = 0;
	if (bottomRightX >= width)
		bottomRightX = width;
	if (upperLeftY <= 0)
		upperLeftY = 0;
	if (bottomRightY >= height)
		bottomRightY = height;

	auto rowsCount = int(ceil(double(bottomRightY - upperLeftY) / size.y));
	auto firstColumn = getIndexByPoint(upperLeftX, upperLeftY);
	auto lastColumn = getIndexByPoint(bottomRightX, upperLeftY);
	auto columnsPerRow = int(ceil(double(width) / size.x));
	auto maxColumn = int(cells.size()) - 1;

	for (auto i = 0; i <= rowsCount; i++)
	{
		if (lastColumn >= maxColumn)
			lastColumn = maxColumn;

		if (i == 0 || i == rowsCount)
			for (int j = firstColumn; j <= lastColumn; j++)
				result.push_back(j);
		else
		{
			result.push_back(firstColumn);
			result.push_back(lastColumn);
		}

		firstColumn += columnsPerRow;
		lastColumn += columnsPerRow;
	}

	return result;
}

std::vector<int> GridList::getBlocksInSight(int upperLeftX, int upperLeftY, int bottomRightX, int bottomRightY)
{
	std::vector<int> result;

	if (upperLeftX <= 0)
		upperLeftX = 0;
	if (bottomRightX >= width)
		bottomRightX = width;
	if (upperLeftY <= 0)
		upperLeftY = 0;
	if (bottomRightY >= height)
		bottomRightY = height;

	auto rowsCount = int(ceil(double(bottomRightY - upperLeftY) / size.y));
	auto firstColumn = getIndexByPoint(upperLeftX, upperLeftY);
	auto lastColumn = getIndexByPoint(bottomRightX, upperLeftY);
	auto columnsPerRow = int(ceil(double(width) / size.x));
	auto maxColumn = int(cells.size()) - 1;

	for (auto i = 0; i <= rowsCount; i++)
	{
		if (lastColumn >= maxColumn)
			lastColumn = maxColumn;

		for (int j = firstColumn; j <= lastColumn; j++)
			result.push_back(j);

		firstColumn += columnsPerRow;
		lastColumn += columnsPerRow;
	}

	return result;
}

void GridList::updateItemPosition(const std::string name, int x, int y)
{
	auto position = items.at(name);
	auto item = cells[position.first][position.second];
	cells[position.first].erase(cells[position.first].begin() + position.second);

	for (int i = position.second; i < cells[position.first].size(); i++)
	{
		auto itemToUpdate = dynamic_cast<WorldObject*>(cells[position.first][i]);
		auto itemName = itemToUpdate->getName();
		items.at(itemName).second -= 1;
	}
	auto index = getIndexByPoint(x, y);
	position = std::make_pair(index, int(cells[index].size()));
	items.at(name) = position;
	cells[index].push_back(item);
}

#endif
