#include "inventory_system.h"

#include<fstream>

#include "helper.h"


inventory_system::inventory_system()
{
	heldItemSpeed = 0.00005f;
}

inventory_system::~inventory_system()
= default;

void inventory_system::initMaxCounts(const std::string& filePath)
{
	std::ifstream file(filePath);

	auto tagNumber = 508, maxCount = 1;
	while (file >> tagNumber >> maxCount)	
		hero_bag::itemsMaxCount[entity_tag(tagNumber)] = maxCount;

	file.close();
}

void inventory_system::init()
{
	dropZoneRadius = helper::GetScreenSize().y * 2 / 7;
	heldItem.content = { entity_tag::emptyCell, 0 };
	dropZoneTexture.loadFromFile("Game/inventorySprites/dropZone.png");
	dropZone.setTexture(dropZoneTexture); dropZone.setScale(helper::GetScreenSize().x / dropZoneTexture.getSize().x, helper::GetScreenSize().y / dropZoneTexture.getSize().y);
	bagPosDot.setRadius(helper::GetScreenSize().y / 288);
	bagPosDot.setFillColor(sf::Color(53, 53, 53, 200));
	initMaxCounts();
	successInit = true;
}

void inventory_system::moveOtherBags(const int cur) const
{
	const auto lapsCount = 20;
	//if (boundBags->at(cur).currentState != bagClosed)
	//{
		for (auto& anotherBag : *boundBags)
		{
			if (anotherBag.getPosition() == boundBags->at(cur).getPosition() || anotherBag.wasMoved)
				continue;
			const auto newPos = Vector2f(boundBags->at(cur).getPosition().x + boundBags->at(cur).shiftVector.x, boundBags->at(cur).getPosition().y + boundBags->at(cur).shiftVector.y);

			if (helper::getDist(newPos, anotherBag.getPosition()) < boundBags->at(cur).getRadius() + anotherBag.getRadius())
			{
				const auto k = 0.05f * lapsCount * ((boundBags->at(cur).getRadius() + anotherBag.getRadius()) - helper::getDist(newPos, anotherBag.getPosition())) / helper::getDist(newPos, anotherBag.getPosition());
				anotherBag.shiftVector = Vector2f((anotherBag.getPosition().x - newPos.x) * k, (anotherBag.getPosition().y - newPos.y) * k);

				const auto tempNewPos = Vector2f(anotherBag.getPosition().x + anotherBag.shiftVector.x, anotherBag.getPosition().y + (anotherBag.shiftVector.y));
				anotherBag.movePosition = tempNewPos;
				anotherBag.shiftVector.x = 0; anotherBag.shiftVector.y = 0;
				//break;
			}
		}
	//}
}

void inventory_system::interact(const long long elapsedTime)
{
	const auto mousePos = Vector2f(Mouse::getPosition());

	auto cnt = -1; minDistToClosed = 10e4; minDistToOpen = 10e4;

	for (auto& bag : *boundBags)
	{
		cnt++;

		if (cursorText == "throw away" && helper::getDist(bag.getPosition(), Vector2f(helper::GetScreenSize().x / 2, helper::GetScreenSize().y / 2)) <= dropZoneRadius ||
			cursorText.empty())
			if (cnt == currentMovingBag || currentMovingBag == -1)
				bag.mouseMove();

		//move other bags while opening
		moveOtherBags(cnt);
		//-----------------------------

		// bag auto-moving
		bag.fixPos();
		auto newPos = bag.getPosition();
		const Vector2f shift = { bag.movePosition.x - bag.getPosition().x, bag.movePosition.y - bag.getPosition().y };
		const auto time = float(elapsedTime);
		
		if (bag.movePosition.x != -1 && bag.movePosition.y != -1)
		{
			const auto k = bag.speed * time / helper::getDist(bag.getPosition(), bag.movePosition);			
			newPos.x = bag.getPosition().x + shift.x * k;
			newPos.y = bag.getPosition().y + shift.y * k;
		}
		else
		{
			if (bag.movePosition.x == -1 && bag.movePosition.y != -1)
			{
				newPos.y += bag.speed * time / shift.y * abs(shift.y) / 2;
				bag.movePosition.x = bag.getPosition().x;
			}
			if (bag.movePosition.y == -1 && bag.movePosition.x != -1)
			{
				newPos.x += bag.speed * time / shift.x * abs(shift.x) / 2;
				bag.movePosition.y = bag.getPosition().y;
			}
		}
		if (helper::getDist(bag.getPosition(), bag.movePosition) <= bag.speed * time)
			newPos = bag.movePosition;
		bag.setPosition(newPos);
		bag.fixCells();
		//----------------

		// bag selection
		if (bag.currentState == bagClosed)
		{
			const auto selectionPos = Vector2f(bag.getPosition().x + bag.selectionZoneClosedOffset.x, bag.getPosition().y + bag.selectionZoneClosedOffset.y);
			bag.readyToChangeState = (helper::getDist(mousePos, selectionPos) <= bag.closedRadius);
		} else
		if (bag.currentState == bagOpen)
		{
			const auto selectionPos = Vector2f(bag.getPosition().x + bag.selectionZoneOpenedOffset.x, bag.getPosition().y + bag.selectionZoneOpenedOffset.y);
			bag.readyToChangeState = (helper::getDist(mousePos, selectionPos) <= bag.openedRadius);
		}
		//--------------
	}

	// nearest bag
	if (currentMovingBag == -1)
	{
		for (auto& boundBag : *boundBags)
		{
			if (helper::getDist(mousePos, boundBag.getPosition()) <= minDistToClosed && boundBag.currentState == bagClosed)
			{
				minDistToClosed = helper::getDist(mousePos, boundBag.getPosition());
			}
			if (helper::getDist(mousePos, boundBag.getPosition()) <= minDistToOpen && boundBag.currentState == bagOpen)
			{
				minDistToOpen = helper::getDist(mousePos, boundBag.getPosition());
			}
		}		
	}

	if (heldItem.content.first != entity_tag::emptyCell)
	{
		const auto shiftVector = Vector2f(Mouse::getPosition().x - heldItem.position.x, Mouse::getPosition().y - heldItem.position.y);
		heldItem.position.x += shiftVector.x; heldItem.position.y += shiftVector.y;
	}

	effectsSystem.interact(elapsedTime);
}

void inventory_system::crashIntoOtherBags(const int cnt) const
{
	auto lapsCount = 20.0f;
	while (true)
	{
		auto isBreak = true;
		for (auto& anotherBag : *boundBags)
		{
			if (anotherBag.getPosition() == boundBags->at(cnt).getPosition())
				continue;
			const auto newPos = Vector2f(boundBags->at(cnt).getPosition().x + boundBags->at(cnt).shiftVector.x, boundBags->at(cnt).getPosition().y + boundBags->at(cnt).shiftVector.y);

			if (helper::getDist(newPos, anotherBag.getPosition()) < boundBags->at(cnt).getRadius() + anotherBag.getRadius())
			{
				if (boundBags->at(cnt).currentState == bagClosed)
				{
					const auto k = 0.05f * lapsCount * ((boundBags->at(cnt).getRadius() + anotherBag.getRadius()) - helper::getDist(newPos, anotherBag.getPosition())) / helper::getDist(newPos, anotherBag.getPosition());
					boundBags->at(cnt).shiftVector = Vector2f((newPos.x - anotherBag.getPosition().x) * k, (newPos.y - anotherBag.getPosition().y) * k);
					isBreak = false;
					break;
				}
			}
		}
		if (isBreak)
			break;
		lapsCount++;
	}

	const auto newPos = Vector2f(boundBags->at(cnt).getPosition().x + boundBags->at(cnt).shiftVector.x, boundBags->at(cnt).getPosition().y + (boundBags->at(cnt).shiftVector.y));
	boundBags->at(cnt).movePosition = newPos;
	boundBags->at(cnt).shiftVector.x = 0; boundBags->at(cnt).shiftVector.y = 0;
}

void inventory_system::onMouseUp()
{
	usedMouse = false;
	const auto mousePos = Vector2f(Mouse::getPosition());

	if (pickedCell != nullptr)	
		hero_bag::putItemIn(pickedCell, boundBags);

	auto cnt = -1;
	for (auto& bag : *boundBags)
	{
		cnt++;
		// crash into other bags
		if (bag.currentState == bagClosed && bag.wasMoved)
		{
			crashIntoOtherBags(cnt);
		}
		//----------------------

		// bag state changing
		if (cnt == currentMovingBag || currentMovingBag == -1)
		{
			usedMouse = true;
			if (bag.readyToEject)
				bag.currentState = ejected;
			if (bag.currentState != bagOpen)
			{
				if (bag.currentState == bagClosed && bag.readyToChangeState && !bag.wasMoved)
				{
					bag.currentState = bagOpening;
					bag.wasMoved = false;
					continue;
				}
			}
			else
			{
				if (bag.currentState == bagOpen && bag.readyToChangeState)
				{
					bag.currentState = bagClosing;
					bag.wasMoved = false;
					continue;
				}
			}
		}
		bag.wasMoved = false;
		if (bag.currentState != bagOpen)
			continue;
		//-------------------

		// put cursor item to bag
		if (heldItem.content.first != entity_tag::emptyCell)
		{
			const auto curIndex = bag.getSelectedCell(mousePos);
			if (curIndex == -1)
				continue;
			auto& item = bag.cells[curIndex];
			if (item.content.first == entity_tag::emptyCell || item.content.first == heldItem.content.first)
			{
				item.content.first = heldItem.content.first;
				item.content.second += heldItem.content.second;
				if (item.content.second > hero_bag::itemsMaxCount.at(entity_tag(item.content.first)))
				{
					heldItem.content.second = item.content.second % hero_bag::itemsMaxCount.at(entity_tag(item.content.first));
					item.content.second = hero_bag::itemsMaxCount.at(entity_tag(item.content.first));
				}
				else
					heldItem.content = { entity_tag::emptyCell, 0 };
				break;
			}
			else
			{
				const auto temp = heldItem.content;
				heldItem.content = item.content;
				item.content = temp;
			}
		}
		else
		{
			const auto curIndex = bag.getSelectedCell(mousePos);
			if (curIndex != -1)
			{
				heldItem.content = bag.cells[curIndex].content;
				heldItem.position = bag.cells[curIndex].position;
				bag.cells[curIndex].content = { entity_tag::emptyCell, 0 };
			}
		}
		//-----------------------
	}
	currentMovingBag = -1;
}

std::vector<drawable_chain_element*> inventory_system::prepareSprites(long long elapsedTime, std::map<pack_tag, sprite_pack>* packsMap)
{
	std::vector<drawable_chain_element*> result = {};
	usedMouse = false;
	const auto mousePos = Vector2f(Mouse::getPosition());
	// draw bags
	auto cnt = -1;
	auto cursorTurnedOn = false;
	//cursorText = "";
	bagPosDot.setPosition(0, 0);
	for (auto& bag : *boundBags)
	{
		cnt++;
		result.push_back(bag.prepareSprite(elapsedTime, packsMap));
		bag.readyToEject = false;
		if (bag.wasMoved)
			currentMovingBag = cnt;
		const auto curIndex = bag.getSelectedCell(mousePos);

		if (bag.readyToChangeState || heldItem.content.first != entity_tag::emptyCell || curIndex != -1)
			usedMouse = true;

		// dropping bag
		if (helper::getDist(bag.getPosition(), Vector2f(helper::GetScreenSize().x / 2, helper::GetScreenSize().y / 2)) <= dropZoneRadius && bag.currentState == bagClosed)
		{
			cursorTurnedOn = true;
			if (cursorText.empty())
			{
				effectsSystem.add_effect(effects::transparencyRemoval, &dropZone, "dropZone", long(3 * 10e4));
			}
			cursorText = "throw away";
			cursorTextPos = bag.getPosition();
			bag.readyToEject = true;
			bagPosDot.setPosition(bag.getPosition());
		}
		//-------------

		if (cnt == currentMovingBag)
			bagPosDot.setPosition(bag.getPosition());

		// drawing bag content
		if (bag.currentState != bagOpen)
			continue;
		for (auto& cell : bag.cells)
		{
			const auto item = cell;

			//drawing cell background
			auto iconBackground = sprite_pack::tagToIcon(entity_tag::emptyObject, false);
			iconBackground->position = item.position;
			result.push_back(iconBackground);
			//-----------------------

			if (cell.content.first == entity_tag::emptyCell)
				continue;

			auto icon = sprite_pack::tagToIcon(item.content.first, helper::isIntersects(mousePos, cell.position, sprite_pack::iconSize.x / 2), 1);			
			icon->position = item.position;
			result.push_back(icon);

			if (hero_bag::itemsMaxCount.at(item.content.first) != 1)
				result.push_back(new text_chain_element(
					icon->position,
					/*{ -SpritePack::iconWithoutSpaceSize.x / 2, -SpritePack::iconWithoutSpaceSize.y / 2 },*/ { 0, 0 },
					sf::Color(255, 255, 255, 180),
					std::to_string(item.content.second),
					text_chain_element::defaultCharacterSize * 1.5f));
		}
		//--------------------
	}
	//----------

	//drawing held item
	if (heldItem.content.first != entity_tag::emptyCell)
	{
		auto heldItemIcon = sprite_pack::tagToIcon(heldItem.content.first, true, 1);
		heldItemIcon->position = heldItem.position;
		result.push_back(heldItemIcon);
		result.push_back(heldItemIcon);
		if (hero_bag::itemsMaxCount.at(heldItem.content.first) != 1)
			result.push_back(new text_chain_element(
				heldItemIcon->position,
				/*{ -SpritePack::iconWithoutSpaceSize.x / 2, -SpritePack::iconWithoutSpaceSize.y / 2 },*/ { 0, 0 },
				sf::Color(255, 255, 255, 180),
				std::to_string(heldItem.content.second),
				text_chain_element::defaultCharacterSize * 1.5f));
;	}
	//-----------------

	// draw cursor text
	if (!cursorTurnedOn)
	{
		cursorText = "";
		effectsSystem.resetEffects({ "dropZone" });
	}
	if (cursorText == "throw away")
	{
		result.push_back(new text_chain_element(
			cursorTextPos,
			{ text_system::getTextBoxSize(cursorText, text_chain_element::defaultCharacterSize * 1.5f, FontName::NormalFont).x / 2.0f, 0 },
			sf::Color(0, 0, 0, 180),
			cursorText, 
			text_chain_element::defaultCharacterSize * 1.5f));
		result.push_back(new sprite_chain_element(pack_tag::inventory, pack_part::areas, Direction::DOWN, 2, { 0, 0 }, helper::GetScreenSize()));
	}
	//if (bagPosDot.getPosition() != Vector2f(0, 0))
		//window.draw(bagPosDot);
	return result;
}

/*void InventorySystem::drawInventory(std::vector<std::pair<Tag, int>>* inventory, Vector2f position, RenderWindow& window)
{
	for (int i = 0; i < inventory->size(); i++)
		if (inventory->at(i).first == Tag::emptyCell || inventory->at(i).second == 0)
			inventory->erase(inventory->begin() + i);

	const Vector2f mousePos = Vector2f(Mouse::getPosition());
	//const FloatRect cellRect = cellsSpriteList.at(Tag::chamomile).sprite.getGlobalBounds();	
	int maxInRaw;
	if (inventory->size() > 3)
		maxInRaw = sqrt(inventory->size());
	else
		maxInRaw = inventory->size();
	int raw = 0, column = 0;
	while (raw * maxInRaw + column < inventory->size())
	{		
		//drawing cell background		
		if (!(mousePos.x < position.x || mousePos.y < position.y))
			if (int(mousePos.x - position.x) / int(cellRect.width) == column && 
				int(mousePos.y - position.y) / int(cellRect.height) == raw)
			{
				pickedCell = &(inventory->at(raw * maxInRaw + column));
				const Vector2f backgroundOffset = cellsSpriteList.at(Tag::emptyCell).offset;
				selectedCellBackground->setPosition(position.x + column * cellRect.width - backgroundOffset.x, position.y + raw * cellRect.height - backgroundOffset.y);
				window.draw(*selectedCellBackground);
			}
		//-----------------------

		if (inventory->at(raw * maxInRaw + column).first != Tag::emptyCell)
		{
			if (cellsSpriteList.count(Tag(inventory->at(raw * maxInRaw + column).first)) > 0)
			{
				auto sprite = cellsSpriteList.at(Tag(inventory->at(raw * maxInRaw + column).first)).sprite;
				sprite.setScale(HeroBag::itemCommonRadius * 2.6 / sprite.getGlobalBounds().width, HeroBag::itemCommonRadius * 2.6 / sprite.getGlobalBounds().height);
				const Vector2f offset = cellsSpriteList.at(Tag(inventory->at(raw * maxInRaw + column).first)).offset;
				sprite.setPosition(Vector2f(position.x + column * cellRect.width - offset.x, position.y + raw * cellRect.width - offset.y));
				sprite.setColor(Color(sprite.getColor().r, sprite.getColor().g, sprite.getColor().b, 255));
				window.draw(sprite);

				if (HeroBag::itemsMaxCount.at(inventory->at(raw * maxInRaw + column).first) != 1)
					textWriter.drawNumberOfItems(sprite.getPosition(), inventory->at(raw * maxInRaw + column).second, window);
			}
		}
		column++;
		if (column >= maxInRaw)
		{
			raw++;
			column = 0;
		}
	}
}*/

void inventory_system::resetAnimationValues()
{
	animationCounter = 0;
}