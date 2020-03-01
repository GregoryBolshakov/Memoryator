#include "GridList.h"

#include <cmath>
#include <queue>

#include "Helper.h"

GridList::GridList() : width(0), height(0)
{
}

Vector2f GridList::getBlockSize() const
{
	return size;
}

GridList::GridList(const int width, const int height, const Vector2f size, const Vector2f microSize)
{
	this->width = width;
	this->height = height;
	this->size = size;
	this->microSize = microSize;

	width_to_size_x_ = int(ceil(float(width) / size.x));
	height_to_size_y_ = int(ceil(float(height) / size.y));
	width_to_micro_x_ = int(ceil(float(width) / microSize.x));
	height_to_micro_y_ = int(ceil(float(height) / microSize.y));
	
	const auto vectorSize = width_to_size_x_ * height_to_size_y_;
	cells.resize(vectorSize);
	for (auto& arr : cells)
	{
		arr.resize(0);
	}
	microBlockMatrix.resize(4000, std::vector<bool>(4000, true));	
	distances.resize(4000, std::vector<float>(4000, 1000));
	previous.resize(4000, std::vector<std::pair<int, int>>(4000, { -1, -1 }));
}

GridList::~GridList()
{
	for (auto cell : cells)
	{
		for (auto ptr : cell)
		{
			delete ptr;
		}
		cell.clear();
	}
	cells.clear();
	items.clear();
}

int GridList::getIndexByPoint(const float x, const float y) const
{
	const auto x1 = width / int(size.x);
	const auto y1 = int(y / size.y);
	const auto result = x1 * y1 + int(x / size.x);
	
	return int(result);
}

Vector2f GridList::getPointByIndex(const int index) const
{
	const auto inLineNumber = width / int(size.x);
	const auto inRawNumber = index / inLineNumber;

	const auto result= Vector2f(
		float(index % inLineNumber) * size.x,
		float(inRawNumber) * size.y);

	return result;
}

int GridList::getMicroBlockByPoint(const float x, const float y) const
{
	const auto x1 = width / int(microSize.x);
	const auto y1 = int(y / microSize.y);
	const auto result = x1 * y1 + int(x / microSize.x);
	
	return int(result);
}

Vector2f GridList::getPointByMicroBlock(const int microBlockIndex) const
{
	const auto inLineNumber = width / int(microSize.x);
	const auto inRawNumber = microBlockIndex / inLineNumber;

	const auto result = Vector2f(
		float(microBlockIndex % inLineNumber) * microSize.x,
		float(inRawNumber) * microSize.y);

	return result;
}

void GridList::makeRoute(
	const Vector2f startPos,
	const Vector2f finishPos,
	const float upperLeftX,
	const float upperLeftY,
	const float bottomRightX,
	const float bottomRightY,
	const int permissibleDistance)
{
	route.clear();

	if (abs(startPos.x - finishPos.x) / microSize.x + abs(startPos.y - finishPos.y) / microSize.y < 1)
		return;

	const auto xMicroBlocksCount = int(ceil((bottomRightX - upperLeftX) / microSize.x));
	const auto yMicroBlocksCount = int(ceil((bottomRightY - upperLeftY) / microSize.y));

	const auto startXInd = int(round(upperLeftX / microSize.x));
	const auto startYInd = int(round(upperLeftY / microSize.y));

	auto curMicroBlockX = int(round(startPos.x / microSize.x));
	auto curMicroBlockY = int(round(startPos.y / microSize.y));

	auto lastMicroBlockX = int(round(finishPos.x / microSize.x));
	auto lastMicroBlockY = int(round(finishPos.y / microSize.y));

	auto isBreak = false;
	
	if (!microBlockMatrix[curMicroBlockX][curMicroBlockY]/* || !dynamicMicroBlockMatrix->at(curMicroBlockX)[curMicroBlockY]*/)
	{
		for (auto i = -1; i <= 1; i++)
		{
			for (auto j = -1; j <= 1; j++)
				if (curMicroBlockX + i > 0 && curMicroBlockX + i < width_to_micro_x_ &&
					curMicroBlockY + j > 0 && curMicroBlockY + j < height_to_micro_y_ && 
					microBlockMatrix[curMicroBlockX + i][curMicroBlockY + j]/* && dynamicMicroBlockMatrix->at(curMicroBlockX + i)[curMicroBlockY + j]*/)
				{
					curMicroBlockX += i;
					curMicroBlockY += j;
					isBreak = true;
					break;
				}
			if (isBreak)
				break;
		}
	}

	if (!microBlockMatrix[lastMicroBlockX][lastMicroBlockY]/* || !dynamicMicroBlockMatrix->at(lastMicroBlockX)[lastMicroBlockY]*/)
	{
		for (auto i = -permissibleDistance; i <= permissibleDistance; i++)
		{
			isBreak = false;
			for (auto j = -permissibleDistance; j <= permissibleDistance; j++)
				if (lastMicroBlockX + i > 0 && lastMicroBlockX + i < width_to_micro_x_ &&
					lastMicroBlockY + j > 0 && lastMicroBlockY + j < height_to_micro_y_ && 
					microBlockMatrix[lastMicroBlockX + i][lastMicroBlockY + j]/* && dynamicMicroBlockMatrix->at(lastMicroBlockX + i)[lastMicroBlockY + j]*/)
				{
					lastMicroBlockX += i;
					lastMicroBlockY += j;
					isBreak = true;
					break;
				}
			if (isBreak)
				break;
		}
	}

	if (!microBlockMatrix[lastMicroBlockX][lastMicroBlockY])
	{
		route.clear();
		return;
	}

	for (auto i = startXInd; i < startXInd + xMicroBlocksCount; i++)	
		for (auto j = startYInd; j < startYInd + yMicroBlocksCount; j++)		
			distances[i][j] = inf;

	distances[curMicroBlockX][curMicroBlockY] = 0;
	bfs(startXInd + xMicroBlocksCount, startYInd + yMicroBlocksCount, curMicroBlockX, curMicroBlockY, lastMicroBlockX, lastMicroBlockY);
}

void GridList::bfs(const int xBorder, const int yBorder, int startX, int startY, const int finishX, const int finishY)
{
	std::queue<std::pair<int, int>> q;
	q.push(std::make_pair(startX, startY));

	float step;
	while (!q.empty()) 
	{
		const auto v = q.front();
		q.pop();
		auto isBreak = false;

		for (auto i = -1; i <= 1; i++)
			for (auto j = -1; j <= 1; j++)
			{				
				if (i == 0 && j == 0)
					continue;
				if (abs(i) == abs(j))
					step = 1.42f;
				else
					step = 1.0f;

				auto to = std::make_pair(v.first + i, v.second + j);
				if (to.first < 0 || to.first > xBorder || to.second < 0 || to.second > yBorder)
					continue;

				if (abs(previous[v.first][v.second].first - v.first) + abs(previous[v.first][v.second].second - v.second) <= 2 &&
					v.first - previous[v.first][v.second].first == to.first - v.first && v.second - previous[v.first][v.second].second == to.second - v.second)
					step -= 0.0001f;


				if (distances[to.first][to.second] > distances[v.first][v.second] + step && (microBlockMatrix[to.first][to.second] != 0 && dynamicMicroBlockMatrix->at(to.first)[to.second] != 0 || to.first == finishX && to.second == finishY))
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

	auto canCreateRoute = true;

	if (finishX == -1 || distances[finishX][finishY] == inf)
	{
		canCreateRoute = false;
		route.clear();		
	}

	if (canCreateRoute)
	{
		std::vector<std::pair<int, int>> path;

		auto currentMatrixCeil = previous[finishX][finishY];
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
		auto cnt = 0;
		while (cnt < int(path.size() - 3))
			if (path[cnt + 1].first - path[cnt].first == path[cnt + 2].first - path[cnt + 1].first && path[cnt + 1].second - path[cnt].second == path[cnt + 2].second - path[cnt + 1].second)
				path.erase(path.begin() + cnt);
			else
				cnt++;
		//------------

		route = path;
	}
}

bool GridList::isIntersectWithOthers(WorldObject* object, const std::vector<WorldObject*>& visibleTerrain, bool isDotAdjusted) const
{
	/*for (auto&anotherItem : visibleTerrain)
	{
		auto const anotherObject = dynamic_cast<WorldObject*>(anotherItem);
		if (!anotherObject)
			continue;

		if (anotherObject->isBackground || anotherObject->tag == Tag::hero || anotherObject->intangible)
			continue;

		if (anotherItem->isDotsAdjusted && isDotAdjusted)
			continue;
		
		if (Helper::getDist(position, anotherObject->getPosition()) < radius + anotherObject->getRadius())
			return true;
	}*/

	std::map<std::pair<int, int>, bool> checkBlocks = {};

	const auto i_start = int(round((object->getPosition().x - object->getMicroBlockCheckAreaBounds().x) / microSize.x));
	const auto i_end = int(round((object->getPosition().x + object->getMicroBlockCheckAreaBounds().x) / microSize.x));

	const auto j_start = int(round((object->getPosition().y - object->getMicroBlockCheckAreaBounds().y) / microSize.y));
	const auto j_end = int(round((object->getPosition().y + object->getMicroBlockCheckAreaBounds().y) / microSize.y));

	for (auto i = i_start; i < i_end; i++)
		for (auto j = j_start; j < j_end; j++)
			if (!(i < 0 || i > width_to_micro_x_ || j < 0 || j > height_to_micro_y_) && !microBlockMatrix[i][j])
				checkBlocks[{i, j}] = true;

	return object->isLockedPlace(checkBlocks);
}

void GridList::setLockedMicroBlocks(WorldObject* item, const bool value, const bool dynamicMatrix)
{
	const auto worldItem = dynamic_cast<WorldObject*>(item);
	if (worldItem)
		for (const auto block : worldItem->getLockedMicroBlocks())
		{
			if (!(block.x < 0 || block.x > width_to_micro_x_ || block.y < 0 || block.y > height_to_micro_y_))
			{
				if (dynamicMatrix)
					dynamicMicroBlockMatrix->at(block.x)[block.y] = value;
				else
					microBlockMatrix[block.x][block.y] = value;
			}
		}
}

void GridList::addItem(WorldObject* item, const std::string& name, const float x, const float y)
{
	setLockedMicroBlocks(item);

	if (items.find(name) != items.end())
		throw std::invalid_argument("The key " + name + " already exists in the Grid.");

	auto index = getIndexByPoint(x, y);	

	auto position = std::make_pair(index, int(cells[index].size()));

	cells[index].push_back(item);
	
	items.insert({ name, position });
}

void GridList::clearCell(const int cellIndex)
{
	for (auto& item : cells[cellIndex])
	{
		setLockedMicroBlocks(item, true);
		items.erase(items.find(item->getName()));		
	}
	cells[cellIndex].clear();
}

void GridList::deleteItem(const std::string& name)
{
	const auto position = items.at(name);
	setLockedMicroBlocks(cells[position.first][position.second], true);
	
	for (unsigned int i = position.second; i < cells[position.first].size(); i++)
	{
		const auto itemToUpdate = dynamic_cast<WorldObject*>(cells[position.first][i]);
		auto itemName = itemToUpdate->getName();
		items.at(itemName).second -= 1;
	}
	
	cells[position.first].erase(cells[position.first].begin() + position.second);
	items.erase(items.find(name));
}

WorldObject* GridList::getItemByName(const std::string& name)
{
	const auto position = items.at(name);
	return cells[position.first][position.second];
}

std::vector<WorldObject*> GridList::getItems(const int blockIndex)
{
	return cells[blockIndex];
}

std::vector<WorldObject*> GridList::getItems(float upperLeftX, float upperLeftY, float bottomRightX, float bottomRightY)
{
	std::vector<WorldObject*> result = {};
	
	if (upperLeftX < 0)
	{
		upperLeftX = 0;
	}
	if (bottomRightX > float(width))
	{
		bottomRightX = float(width);
	}
	if (upperLeftY < 0)
	{
		upperLeftY = 0;
	}
	if (bottomRightY > float(height))
	{
		bottomRightY = float(height);
	}
	const auto rowsCount = int(ceil(double(bottomRightY - upperLeftY) / int(size.y)));
	auto firstColumn = int(getIndexByPoint(upperLeftX, upperLeftY));
	auto lastColumn = int(getIndexByPoint(bottomRightX, upperLeftY));
	const auto columnsPerRow = width_to_size_x_;
	const auto maxColumn = int(cells.size()) - 1;

	for (auto i = 0; i <= rowsCount; i++)
	{
		if (lastColumn > maxColumn)
			lastColumn = maxColumn;
		
		for (auto j = firstColumn; j <= lastColumn; j++)
		{
			result.insert(result.end(), cells[j].begin(), cells[j].end());
		}

		firstColumn += columnsPerRow;
		lastColumn += columnsPerRow;
	}

	return result;
}

std::vector<int> GridList::getBlocksAround(float upperLeftX, float upperLeftY, float bottomRightX, float bottomRightY) const
{
	std::vector<int> result;

	if (upperLeftX < 0)
		upperLeftX = 0;
	if (bottomRightX > float(width))
		bottomRightX = float(width);
	if (upperLeftY < 0)
		upperLeftY = 0;
	if (bottomRightY > float(height))
		bottomRightY = float(height);

	const auto rowsCount = int(ceil(double(bottomRightY - upperLeftY) / size.y));
	auto firstColumn = getIndexByPoint(upperLeftX, upperLeftY);
	auto lastColumn = getIndexByPoint(bottomRightX, upperLeftY);
	const auto columnsPerRow = width_to_size_x_;
	const auto maxColumn = int(cells.size()) - 1;

	for (auto i = 0; i <= rowsCount; i++)
	{
		if (lastColumn >= maxColumn)
			lastColumn = maxColumn;

		if (i == 0 || i == rowsCount)
			for (auto j = firstColumn; j <= lastColumn; j++)
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

std::vector<int> GridList::getBlocksInSight(float upperLeftX, float upperLeftY, float bottomRightX, float bottomRightY) const
{
	std::vector<int> result;

	if (upperLeftX < 0)
		upperLeftX = 0;
	if (bottomRightX > float(width))
		bottomRightX = float(width);
	if (upperLeftY < 0)
		upperLeftY = 0;
	if (bottomRightY > float(height))
		bottomRightY = float(height);

	const auto rowsCount = int(ceil(double(bottomRightY - upperLeftY) / size.y));
	auto firstColumn = getIndexByPoint(upperLeftX, upperLeftY);
	auto lastColumn = getIndexByPoint(bottomRightX, upperLeftY);
	const auto columnsPerRow = width_to_size_x_;
	const auto maxColumn = int(cells.size()) - 1;

	for (auto i = 0; i <= rowsCount; i++)
	{
		if (lastColumn >= maxColumn)
			lastColumn = maxColumn;

		for (auto j = firstColumn; j <= lastColumn; j++)
			result.push_back(j);

		firstColumn += columnsPerRow;
		lastColumn += columnsPerRow;
	}

	return result;
}

void GridList::updateItemPosition(const std::string& name, const float x, const float y)
{
	auto position = items.at(name);
	const auto item = cells[position.first][position.second];
	cells[position.first].erase(cells[position.first].begin() + position.second);

	for (unsigned int i = position.second; i < cells[position.first].size(); i++)
	{
		const auto itemToUpdate = dynamic_cast<WorldObject*>(cells[position.first][i]);
		auto itemName = itemToUpdate->getName();
		items.at(itemName).second -= 1;
	}
	auto index = getIndexByPoint(x, y);
	position = std::make_pair(index, int(cells[index].size()));
	items.at(name) = position;
	cells[index].push_back(item);
}

