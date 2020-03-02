#include "hero_bag.h"

#include "grid_list.h"
#include "helper.h"

hero_bag::hero_bag()
{
	stateChangeTime = 100000;
}

hero_bag::~hero_bag()
= default;

std::unordered_map<entity_tag, int> hero_bag::itemsMaxCount = std::unordered_map<entity_tag, int>();

std::vector<std::pair<entity_tag, int>> hero_bag::testInventory =
{ {entity_tag::noose, 1}, {entity_tag::yarrow, 3}, {entity_tag::emptyCell, 0}, {entity_tag::inkyBlackPen, 1}, {entity_tag::hare, 1}, {entity_tag::chamomile, 2}, {entity_tag::emptyCell, 0} };
std::vector<std::pair<entity_tag, int>> hero_bag::emptyInventory =
{ {entity_tag::emptyCell, 0}, {entity_tag::emptyCell, 0}, {entity_tag::emptyCell, 0}, {entity_tag::emptyCell, 0}, {entity_tag::emptyCell, 0}, {entity_tag::emptyCell, 0}, {entity_tag::emptyCell, 0} };

void hero_bag::initialize(const Vector2f position, const bool isSelectable, std::vector<std::pair<entity_tag, int>> inventory)
{
	sizeClosed = Vector2f(helper::GetScreenSize().x / 12.0f, helper::GetScreenSize().y / 6.0f);
	sizeOpen = Vector2f(helper::GetScreenSize().x / 6.0f, helper::GetScreenSize().y / 3.0f);

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
	if (bagSpriteChain.ClosedBag->pack_tag == pack_tag::empty)
		bagSpriteChain.ClosedBag->set_draw_info(pack_tag::inventory, pack_part::bag1, direction::DOWN, 1);
	if (bagSpriteChain.ClosedBagSelected->pack_tag == pack_tag::empty)
		bagSpriteChain.ClosedBagSelected->set_draw_info(pack_tag::inventory, pack_part::bag1, direction::DOWN, 2);
	if (bagSpriteChain.ClosedBagBig->pack_tag == pack_tag::empty)
		bagSpriteChain.ClosedBagBig->set_draw_info(pack_tag::inventory, pack_part::bag1, direction::DOWN, 3);
	if (bagSpriteChain.OpenedBag->pack_tag == pack_tag::empty)
		bagSpriteChain.OpenedBag->set_draw_info(pack_tag::inventory, pack_part::bag1, direction::DOWN, 4);
	if (bagSpriteChain.OpenedBagSelected->pack_tag == pack_tag::empty)
		bagSpriteChain.OpenedBagSelected->set_draw_info(pack_tag::inventory, pack_part::bag1, direction::DOWN, 5);
	//----------------------
}

cell hero_bag::createCell(const Vector2f position, const std::pair<entity_tag, int> content)
{
	cell cell;
	cell.position = position;
	cell.content = content;
	return cell;
}

int hero_bag::getSelectedCell(const Vector2f position)
{
	for (auto i = 0u; i < cells.size(); i++)
	{
		if (helper::getDist(position, cells[i].position) <= sprite_pack::iconSize.x / 2)
			return i;
	}
	return -1;
}

float hero_bag::getRadius() const
{
	if (currentState == bagClosed)
		return bagSpriteChain.ClosedBag->size.x / 2;

	if (currentState == bagOpening)
		return bagSpriteChain.ClosedBagSelected->size.x / 2;

	if (currentState == bagOpen)
		return bagSpriteChain.OpenedBag->size.x / 2;

	return 0;
}

void hero_bag::drawCircuit(RenderWindow* window)
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

void hero_bag::fixCells()
{
	for (auto i = 0u; i < cells.size(); i++)
		cells[i].position = Vector2f(position.x + cellsPos[i].x, position.y + cellsPos[i].y);
}

void hero_bag::fixPos()
{
	if (movePosition == position)
		movePosition = { -1, -1 };

	if (movePosition.x < minDistToBorder)
		movePosition.x = -1;
	if (movePosition.x > helper::GetScreenSize().x - minDistToBorder)
		movePosition.x = -1;

	if (movePosition.y < minDistToBorder)
		movePosition.y = -1;
	if (movePosition.y > helper::GetScreenSize().y - minDistToBorder)
		movePosition.y = -1;

	if (position.x + shiftVector.x < minDistToBorder || position.x + shiftVector.x > helper::GetScreenSize().x - minDistToBorder)
		shiftVector.x = 0;
	if (position.y + shiftVector.y < minDistToBorder || position.y + shiftVector.y > helper::GetScreenSize().y - minDistToBorder)
		shiftVector.y = 0;
}

std::vector<std::pair<entity_tag, int>> hero_bag::cellsToInventory(const std::vector<cell>& cells)
{
	std::vector<std::pair<entity_tag, int>> ans(cells.size());
	for (auto i = 0u; i < cells.size(); i++)
	{
		ans[i] = { cells[i].content.first, cells[i].content.second };
	}

	return ans;
}

bool hero_bag::canAfford(std::vector<std::pair<entity_tag, int>> recipe, std::vector<hero_bag>* bags, cell* heldItem)
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

void hero_bag::takeItems(std::vector<std::pair<entity_tag, int>> recipe, std::vector<hero_bag>* bags, cell* heldItem)
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
			heldItem->content.first = entity_tag::emptyCell;
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
						cell.content.first = entity_tag::emptyCell;
					}
				}
			if (isBreak)
				break;
		}
	}
}

bool hero_bag::putItemsIn(std::vector<std::pair<entity_tag, int>>* loot, std::vector<hero_bag>* bags)
{
	auto result = true;
	for (auto& item : *loot)
	{
		for (auto& bag : *bags)
		{
			auto isBreak = false;
			for (auto& cell : bag.cells)
				if (item.first == cell.content.first || cell.content.first == entity_tag::emptyCell)
				{
					if (item.second <= (itemsMaxCount[item.first] - cell.content.second))
					{
						cell.content.second += item.second;
						cell.content.first = item.first;
						item = std::make_pair(entity_tag::emptyCell, 0);
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

bool hero_bag::putItemIn(std::pair<entity_tag, int>* loot, std::vector<hero_bag>* bags)
{
	auto result = true;

	for (auto& bag : *bags)
	{
		auto isBreak = false;
		for (auto& cell : bag.cells)
			if (loot->first == cell.content.first || cell.content.first == entity_tag::emptyCell)
			{
				if (loot->second <= (itemsMaxCount[loot->first] - cell.content.second))
				{
					cell.content.second += loot->second;
					cell.content.first = loot->first;
					loot->first = entity_tag::emptyCell;
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

void hero_bag::mouseMove()
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

sprite_chain_element* hero_bag::prepareSprite(const long long elapsedTime, std::map<pack_tag, sprite_pack> * packsMap)
{
	const auto screenCenter = Vector2f(helper::GetScreenSize().x / 2, helper::GetScreenSize().y / 2);
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

			const auto textureSize = packsMap->at(pack_tag::inventory).getOriginalInfo(pack_part::bag1, direction::DOWN, 4).source_size;
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
	return new sprite_chain_element();
}