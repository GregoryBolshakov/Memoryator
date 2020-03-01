#include "HeroBag.h"
#include "GridList.h"
#include "Helper.h"

HeroBag::HeroBag()
{
	stateChangeTime = 100000;
}

HeroBag::~HeroBag()
= default;

std::unordered_map<Tag, int> HeroBag::itemsMaxCount = std::unordered_map<Tag, int>();

std::vector<std::pair<Tag, int>> HeroBag::testInventory =
{ {Tag::noose, 1}, {Tag::yarrow, 3}, {Tag::emptyCell, 0}, {Tag::inkyBlackPen, 1}, {Tag::hare, 1}, {Tag::chamomile, 2}, {Tag::emptyCell, 0} };
std::vector<std::pair<Tag, int>> HeroBag::emptyInventory =
{ {Tag::emptyCell, 0}, {Tag::emptyCell, 0}, {Tag::emptyCell, 0}, {Tag::emptyCell, 0}, {Tag::emptyCell, 0}, {Tag::emptyCell, 0}, {Tag::emptyCell, 0} };

void HeroBag::initialize(const Vector2f position, const bool isSelectable, std::vector<std::pair<Tag, int>> inventory)
{
	sizeClosed = Vector2f(Helper::GetScreenSize().x / 12.0f, Helper::GetScreenSize().y / 6.0f);
	sizeOpen = Vector2f(Helper::GetScreenSize().x / 6.0f, Helper::GetScreenSize().y / 3.0f);

	this->position = position;
	lastMousePos = position;

	this->isSelectable = isSelectable;

	textureClosedOffset = Vector2f(sizeClosed.x / 2.0f, sizeClosed.y / 1.7f);
	textureOpenOffset = Vector2f(sizeOpen.x / 2.0f, sizeOpen.y / 1.7f);

	bagSpriteChain.ClosedBag->size = sizeClosed; bagSpriteChain.ClosedBagSelected->size = sizeClosed; bagSpriteChain.ClosedBagBig->size = sizeClosed;
	bagSpriteChain.OpenedBag->size = sizeOpen; bagSpriteChain.OpenedBagSelected->size = sizeOpen;
	bagSpriteChain.ClosedBag->offset = textureClosedOffset; bagSpriteChain.ClosedBagSelected->offset = textureClosedOffset; bagSpriteChain.ClosedBagBig->offset = textureClosedOffset;
	bagSpriteChain.OpenedBag->offset = textureOpenOffset; bagSpriteChain.OpenedBagSelected->offset = textureOpenOffset;
	bagSpriteChain.ClosedBag->position = position; bagSpriteChain.ClosedBagSelected->position = position; bagSpriteChain.ClosedBagBig->position = position;
	bagSpriteChain.OpenedBag->position = position; bagSpriteChain.OpenedBagSelected->position = position;
	bagSpriteChain.ClosedBag->initialize(); bagSpriteChain.ClosedBagSelected->initialize(); bagSpriteChain.ClosedBagBig->initialize();
	bagSpriteChain.OpenedBag->initialize(); bagSpriteChain.OpenedBagSelected->initialize();

	this->minDistToBorder = std::max(sizeClosed.y - textureClosedOffset.y, textureClosedOffset.y);
	closedRadius = (sizeClosed.x + sizeClosed.y) / 4;
	openedRadius = (sizeOpen.x + sizeOpen.y) / 10;

	this->selectionZoneClosedOffset = Vector2f(0, -sizeClosed.y * 0.15f);
	this->selectionZoneOpenedOffset = Vector2f(0, -textureOpenOffset.y + sizeOpen.y * 0.15f);

	if (inventory.empty())
		inventory = emptyInventory;

	for (auto i = 0; i < 7; i++)
	{
		cellsPos[i].x *= sizeOpen.x; cellsPos[i].y *= sizeOpen.y;
		cells.push_back(createCell(Vector2f(position.x + cellsPos[i].x, position.y + cellsPos[i].y), inventory[i]));
	}

	// set sprites from pack
	if (bagSpriteChain.ClosedBag->packTag == PackTag::empty)
		bagSpriteChain.ClosedBag->setDrawInfo(PackTag::inventory, PackPart::bag1, Direction::DOWN, 1);
	if (bagSpriteChain.ClosedBagSelected->packTag == PackTag::empty)
		bagSpriteChain.ClosedBagSelected->setDrawInfo(PackTag::inventory, PackPart::bag1, Direction::DOWN, 2);
	if (bagSpriteChain.ClosedBagBig->packTag == PackTag::empty)
		bagSpriteChain.ClosedBagBig->setDrawInfo(PackTag::inventory, PackPart::bag1, Direction::DOWN, 3);
	if (bagSpriteChain.OpenedBag->packTag == PackTag::empty)
		bagSpriteChain.OpenedBag->setDrawInfo(PackTag::inventory, PackPart::bag1, Direction::DOWN, 4);
	if (bagSpriteChain.OpenedBagSelected->packTag == PackTag::empty)
		bagSpriteChain.OpenedBagSelected->setDrawInfo(PackTag::inventory, PackPart::bag1, Direction::DOWN, 5);
	//----------------------
}

Cell HeroBag::createCell(const Vector2f position, const std::pair<Tag, int> content)
{
	Cell cell;
	cell.position = position;
	cell.content = content;
	return cell;
}

int HeroBag::getSelectedCell(const Vector2f position)
{
	for (auto i = 0u; i < cells.size(); i++)
	{
		if (Helper::getDist(position, cells[i].position) <= SpritePack::iconSize.x / 2)
			return i;
	}
	return -1;
}

float HeroBag::getRadius() const
{
	if (currentState == bagClosed)
		return bagSpriteChain.ClosedBag->size.x / 2;

	if (currentState == bagOpening)
		return bagSpriteChain.ClosedBagSelected->size.x / 2;

	if (currentState == bagOpen)
		return bagSpriteChain.OpenedBag->size.x / 2;

	return 0;
}

void HeroBag::drawCircuit(RenderWindow* window)
{
	const auto i_end = bagSpriteChain.OpenedBag->position.x + bagSpriteChain.OpenedBag->size.x;
	const auto j_end = bagSpriteChain.OpenedBag->position.y + bagSpriteChain.OpenedBag->size.y;

	auto i = bagSpriteChain.OpenedBag->position.x;
	while (i <= i_end)
	{
		auto j = bagSpriteChain.OpenedBag->position.y;
		while (j <= j_end)
		{
			if (getSelectedCell(Vector2f(i, j)) != -1)
			{
				RectangleShape rec;
				rec.setPosition(i, j);
				rec.setSize(Vector2f(5, 5));
				rec.setFillColor(sf::Color::Red);
				window->draw(rec);
			}
			j += 5.0f;
		}
		i += 5.0f;
	}
}

void HeroBag::fixCells()
{
	for (auto i = 0u; i < cells.size(); i++)
		cells[i].position = Vector2f(position.x + cellsPos[i].x, position.y + cellsPos[i].y);
}

void HeroBag::fixPos()
{
	if (movePosition == position)
		movePosition = { -1, -1 };

	if (movePosition.x < minDistToBorder)
		movePosition.x = -1;
	if (movePosition.x > Helper::GetScreenSize().x - minDistToBorder)
		movePosition.x = -1;

	if (movePosition.y < minDistToBorder)
		movePosition.y = -1;
	if (movePosition.y > Helper::GetScreenSize().y - minDistToBorder)
		movePosition.y = -1;

	if (position.x + shiftVector.x < minDistToBorder || position.x + shiftVector.x > Helper::GetScreenSize().x - minDistToBorder)
		shiftVector.x = 0;
	if (position.y + shiftVector.y < minDistToBorder || position.y + shiftVector.y > Helper::GetScreenSize().y - minDistToBorder)
		shiftVector.y = 0;
}

std::vector<std::pair<Tag, int>> HeroBag::cellsToInventory(const std::vector<Cell>& cells)
{
	std::vector<std::pair<Tag, int>> ans(cells.size());
	for (auto i = 0u; i < cells.size(); i++)
	{
		ans[i] = { cells[i].content.first, cells[i].content.second };
	}

	return ans;
}

bool HeroBag::canAfford(std::vector<std::pair<Tag, int>> recipe, std::vector<HeroBag>* bags, Cell* heldItem)
{
	for (auto& item : recipe)
	{
		if (heldItem != nullptr)
		{
			if (item.first == heldItem->content.first)
				item.second -= heldItem->content.second;
		}
		for (auto& bag : *bags)
		{
			if (item.second <= 0)
				break;
			for (auto& cell : bag.cells)
				if (item.first == cell.content.first)
					item.second -= cell.content.second;
		}
		if (item.second > 0)
			return false;
	}
	return true;
}

void HeroBag::takeItems(std::vector<std::pair<Tag, int>> recipe, std::vector<HeroBag>* bags, Cell* heldItem)
{
	for (auto& item : recipe)
	{
		if (heldItem != nullptr)
		{
			if (item.second < heldItem->content.second)
			{
				heldItem->content.second -= item.second;
				item.second = 0;
				continue;
			}
			item.second -= heldItem->content.second;
			heldItem->content.second = 0;
			heldItem->content.first = Tag::emptyCell;
		}
		for (auto& bag : *bags)
		{
			auto isBreak = true;
			for (auto& cell : bag.cells)
				if (item.first == cell.content.first)
				{
					if (item.second < cell.content.second)
					{
						cell.content.second -= item.second;
						item.second = 0;
						isBreak = true;
					}
					else
					{
						item.second -= cell.content.second;
						cell.content.second = 0;
						cell.content.first = Tag::emptyCell;
					}
				}
			if (isBreak)
				break;
		}
	}
}

bool HeroBag::putItemsIn(std::vector<std::pair<Tag, int>>* loot, std::vector<HeroBag>* bags)
{
	auto result = true;
	for (auto& item : *loot)
	{
		for (auto& bag : *bags)
		{
			auto isBreak = false;
			for (auto& cell : bag.cells)
				if (item.first == cell.content.first || cell.content.first == Tag::emptyCell)
				{
					if (item.second <= (itemsMaxCount[item.first] - cell.content.second))
					{
						cell.content.second += item.second;
						cell.content.first = item.first;
						item = std::make_pair(Tag::emptyCell, 0);
						isBreak = true;
						break;
					}
					item.second -= (itemsMaxCount[item.first] - cell.content.second);
					cell.content.second = itemsMaxCount[item.first];
					cell.content.first = item.first;
				}
			if (isBreak)
				break;
		}
		if (item.second != 0)
			result = false;
	}
	return result;
}

bool HeroBag::putItemIn(std::pair<Tag, int>* loot, std::vector<HeroBag>* bags)
{
	auto result = true;

	for (auto& bag : *bags)
	{
		auto isBreak = false;
		for (auto& cell : bag.cells)
			if (loot->first == cell.content.first || cell.content.first == Tag::emptyCell)
			{
				if (loot->second <= (itemsMaxCount[loot->first] - cell.content.second))
				{
					cell.content.second += loot->second;
					cell.content.first = loot->first;
					loot->first = Tag::emptyCell;
					loot->second = 0;
					isBreak = true;
					break;
				}
				loot->second -= (itemsMaxCount[loot->first] - cell.content.second);
				cell.content.second = itemsMaxCount[loot->first];
				cell.content.first = loot->first;
			}
		if (isBreak)
			break;
	}
	if (loot->second != 0)
		result = false;
	return result;
}

void HeroBag::mouseMove()
{
	if (currentState == bagClosed && (readyToChangeState || wasMoved))
	{
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			shiftVector = { 0, 0 };
			if (lastMousePos != Vector2f(0, 0))
				shiftVector = Vector2f(Mouse::getPosition().x - lastMousePos.x, Mouse::getPosition().y - lastMousePos.y);
			fixCells();
			fixPos();
			position.x += shiftVector.x; position.y += shiftVector.y;
			if (shiftVector != Vector2f(0, 0))
				wasMoved = true;
		}
	}

	lastMousePos = Vector2f(Mouse::getPosition());
}

SpriteChainElement* HeroBag::prepareSprite(const long long elapsedTime, std::map<PackTag, SpritePack> * packsMap)
{
	const auto screenCenter = Vector2f(Helper::GetScreenSize().x / 2, Helper::GetScreenSize().y / 2);
	bagSpriteChain.ClosedBag->size = sizeClosed; bagSpriteChain.ClosedBagSelected->size = sizeClosed; bagSpriteChain.ClosedBagBig->size = sizeClosed;
	bagSpriteChain.OpenedBag->size = sizeOpen; bagSpriteChain.OpenedBagSelected->size = sizeOpen;
	bagSpriteChain.ClosedBag->offset = textureClosedOffset; bagSpriteChain.ClosedBagSelected->offset = textureClosedOffset; bagSpriteChain.ClosedBagBig->offset = textureClosedOffset;
	bagSpriteChain.OpenedBag->offset = textureOpenOffset; bagSpriteChain.OpenedBagSelected->offset = textureOpenOffset;
	bagSpriteChain.ClosedBag->position = position; bagSpriteChain.ClosedBagSelected->position = position; bagSpriteChain.ClosedBagBig->position = position;
	bagSpriteChain.OpenedBag->position = position; bagSpriteChain.OpenedBagSelected->position = position;

	if (currentState == bagOpen)
	{
		bagSpriteChain.OpenedBag->offset = textureOpenOffset;
		bagSpriteChain.OpenedBagSelected->offset = textureOpenOffset;
		
		if (readyToChangeState)
			return bagSpriteChain.OpenedBagSelected;
		
		return bagSpriteChain.OpenedBag;
	}

	if (currentState == bagClosed)
	{
		bagSpriteChain.ClosedBag->offset = textureClosedOffset;
		bagSpriteChain.ClosedBagSelected->offset = textureClosedOffset;
		
		if (readyToChangeState)
			return bagSpriteChain.ClosedBagSelected;
		
		return bagSpriteChain.ClosedBag;
	}

	const auto elapsed_to_change_time = float(elapsedTime) / float(stateChangeTime);

	if (currentState == bagOpening)
	{
		stateChangingTime += elapsedTime;
		if (stateChangingTime >= stateChangeTime)
		{
			currentState = bagOpen;
			stateChangingTime = 0;
		}
		else
		{
			const auto toCenterVector = Vector2f(screenCenter.x - position.x, screenCenter.y - position.y);
			const auto cutCoefficient = sqrt(pow((sizeOpen.x - sizeClosed.x) / 2 * elapsed_to_change_time, 2) + pow((sizeOpen.y - sizeClosed.y) / 2 * elapsed_to_change_time, 2)) /
				sqrt(pow(toCenterVector.x, 2) + pow(toCenterVector.y, 2));
			shiftVector = Vector2f(toCenterVector.x * cutCoefficient, toCenterVector.y * cutCoefficient);
			fixPos();
			position.x += shiftVector.x; position.y += shiftVector.y;

			const auto textureSize = packsMap->at(PackTag::inventory).getOriginalInfo(PackPart::bag1, Direction::DOWN, 4).source_size;
			const auto changing_to_change_time = float(stateChangingTime)/float(stateChangeTime);
			const Vector2f scaleValue = {
				(sizeClosed.x + (sizeOpen.x - sizeClosed.x) * changing_to_change_time) / textureSize.w,
				(sizeClosed.y + (sizeOpen.y - sizeClosed.y) * changing_to_change_time) / textureSize.h
			};
			bagSpriteChain.OpenedBag->size.x *= scaleValue.x; bagSpriteChain.OpenedBag->size.y *= scaleValue.y;

			bagSpriteChain.OpenedBag->offset = Vector2f(bagSpriteChain.OpenedBag->size.x / 2.0f, bagSpriteChain.OpenedBag->size.y / 1.7f);
			bagSpriteChain.OpenedBag->position = position;

			fixCells();
		}
		return bagSpriteChain.OpenedBag;
	}

	if (currentState == bagClosing)
	{
		stateChangingTime += elapsedTime;
		if (stateChangingTime >= stateChangeTime)
		{
			currentState = bagClosed;
			stateChangingTime = 0;
		}
		else
		{
			const auto fromCenterVector = Vector2f(position.x - screenCenter.x, position.y - screenCenter.y);
			const auto cutCoefficient = 
				sqrt(pow((sizeOpen.x - sizeClosed.x) / 2 * elapsed_to_change_time, 2) + pow((sizeOpen.y - sizeClosed.y) / 2 * elapsed_to_change_time, 2)) /
				sqrt(pow(fromCenterVector.x, 2) + pow(fromCenterVector.y, 2));
			shiftVector = Vector2f(fromCenterVector.x * cutCoefficient, fromCenterVector.y * cutCoefficient);
			fixPos();
			position.x += shiftVector.x; position.y += shiftVector.y;

			const auto delta_time = float(stateChangeTime - stateChangingTime) / float(stateChangeTime);
			const Vector2f scaleValue = {
				(sizeClosed.x + (sizeOpen.x - sizeClosed.x) * delta_time) / bagSpriteChain.ClosedBag->size.x,
				(sizeClosed.y + (sizeOpen.y - sizeClosed.y) * delta_time) / bagSpriteChain.ClosedBag->size.y
			};
			bagSpriteChain.ClosedBag->size = sizeClosed;
			bagSpriteChain.ClosedBag->size.x *= scaleValue.x; bagSpriteChain.ClosedBag->size.y *= scaleValue.y;

			bagSpriteChain.ClosedBag->offset = Vector2f(bagSpriteChain.ClosedBag->size.x / 2.0f, bagSpriteChain.ClosedBag->size.y / 1.7f);
			bagSpriteChain.ClosedBag->position = position;

			fixCells();
		}
		return  bagSpriteChain.ClosedBag;
	}
	return new SpriteChainElement();
}