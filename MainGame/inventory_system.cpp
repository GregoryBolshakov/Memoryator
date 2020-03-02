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
		hero_bag::items_max_count[entity_tag(tagNumber)] = maxCount;

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
			if (anotherBag.get_position() == boundBags->at(cur).get_position() || anotherBag.was_moved)
				continue;
			const auto newPos = Vector2f(boundBags->at(cur).get_position().x + boundBags->at(cur).shift_vector.x, boundBags->at(cur).get_position().y + boundBags->at(cur).shift_vector.y);

			if (helper::getDist(newPos, anotherBag.get_position()) < boundBags->at(cur).get_radius() + anotherBag.get_radius())
			{
				const auto k = 0.05f * lapsCount * ((boundBags->at(cur).get_radius() + anotherBag.get_radius()) - helper::getDist(newPos, anotherBag.get_position())) / helper::getDist(newPos, anotherBag.get_position());
				anotherBag.shift_vector = Vector2f((anotherBag.get_position().x - newPos.x) * k, (anotherBag.get_position().y - newPos.y) * k);

				const auto tempNewPos = Vector2f(anotherBag.get_position().x + anotherBag.shift_vector.x, anotherBag.get_position().y + (anotherBag.shift_vector.y));
				anotherBag.move_position = tempNewPos;
				anotherBag.shift_vector.x = 0; anotherBag.shift_vector.y = 0;
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

		if (cursorText == "throw away" && helper::getDist(bag.get_position(), Vector2f(helper::GetScreenSize().x / 2, helper::GetScreenSize().y / 2)) <= dropZoneRadius ||
			cursorText.empty())
			if (cnt == currentMovingBag || currentMovingBag == -1)
				bag.mouse_move();

		//move other bags while opening
		moveOtherBags(cnt);
		//-----------------------------

		// bag auto-moving
		bag.fix_pos();
		auto newPos = bag.get_position();
		const Vector2f shift = { bag.move_position.x - bag.get_position().x, bag.move_position.y - bag.get_position().y };
		const auto time = float(elapsedTime);
		
		if (bag.move_position.x != -1 && bag.move_position.y != -1)
		{
			const auto k = bag.speed * time / helper::getDist(bag.get_position(), bag.move_position);			
			newPos.x = bag.get_position().x + shift.x * k;
			newPos.y = bag.get_position().y + shift.y * k;
		}
		else
		{
			if (bag.move_position.x == -1 && bag.move_position.y != -1)
			{
				newPos.y += bag.speed * time / shift.y * abs(shift.y) / 2;
				bag.move_position.x = bag.get_position().x;
			}
			if (bag.move_position.y == -1 && bag.move_position.x != -1)
			{
				newPos.x += bag.speed * time / shift.x * abs(shift.x) / 2;
				bag.move_position.y = bag.get_position().y;
			}
		}
		if (helper::getDist(bag.get_position(), bag.move_position) <= bag.speed * time)
			newPos = bag.move_position;
		bag.set_position(newPos);
		bag.fix_cells();
		//----------------

		// bag selection
		if (bag.current_state == bag_closed)
		{
			const auto selectionPos = Vector2f(bag.get_position().x + bag.selection_zone_closed_offset.x, bag.get_position().y + bag.selection_zone_closed_offset.y);
			bag.ready_to_change_state = (helper::getDist(mousePos, selectionPos) <= bag.closed_radius);
		} else
		if (bag.current_state == bag_open)
		{
			const auto selectionPos = Vector2f(bag.get_position().x + bag.selection_zone_opened_offset.x, bag.get_position().y + bag.selection_zone_opened_offset.y);
			bag.ready_to_change_state = (helper::getDist(mousePos, selectionPos) <= bag.opened_radius);
		}
		//--------------
	}

	// nearest bag
	if (currentMovingBag == -1)
	{
		for (auto& boundBag : *boundBags)
		{
			if (helper::getDist(mousePos, boundBag.get_position()) <= minDistToClosed && boundBag.current_state == bag_closed)
			{
				minDistToClosed = helper::getDist(mousePos, boundBag.get_position());
			}
			if (helper::getDist(mousePos, boundBag.get_position()) <= minDistToOpen && boundBag.current_state == bag_open)
			{
				minDistToOpen = helper::getDist(mousePos, boundBag.get_position());
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
			if (anotherBag.get_position() == boundBags->at(cnt).get_position())
				continue;
			const auto newPos = Vector2f(boundBags->at(cnt).get_position().x + boundBags->at(cnt).shift_vector.x, boundBags->at(cnt).get_position().y + boundBags->at(cnt).shift_vector.y);

			if (helper::getDist(newPos, anotherBag.get_position()) < boundBags->at(cnt).get_radius() + anotherBag.get_radius())
			{
				if (boundBags->at(cnt).current_state == bag_closed)
				{
					const auto k = 0.05f * lapsCount * ((boundBags->at(cnt).get_radius() + anotherBag.get_radius()) - helper::getDist(newPos, anotherBag.get_position())) / helper::getDist(newPos, anotherBag.get_position());
					boundBags->at(cnt).shift_vector = Vector2f((newPos.x - anotherBag.get_position().x) * k, (newPos.y - anotherBag.get_position().y) * k);
					isBreak = false;
					break;
				}
			}
		}
		if (isBreak)
			break;
		lapsCount++;
	}

	const auto newPos = Vector2f(boundBags->at(cnt).get_position().x + boundBags->at(cnt).shift_vector.x, boundBags->at(cnt).get_position().y + (boundBags->at(cnt).shift_vector.y));
	boundBags->at(cnt).move_position = newPos;
	boundBags->at(cnt).shift_vector.x = 0; boundBags->at(cnt).shift_vector.y = 0;
}

void inventory_system::onMouseUp()
{
	usedMouse = false;
	const auto mousePos = Vector2f(Mouse::getPosition());

	if (pickedCell != nullptr)	
		hero_bag::put_item_in(pickedCell, boundBags);

	auto cnt = -1;
	for (auto& bag : *boundBags)
	{
		cnt++;
		// crash into other bags
		if (bag.current_state == bag_closed && bag.was_moved)
		{
			crashIntoOtherBags(cnt);
		}
		//----------------------

		// bag state changing
		if (cnt == currentMovingBag || currentMovingBag == -1)
		{
			usedMouse = true;
			if (bag.ready_to_eject)
				bag.current_state = ejected;
			if (bag.current_state != bag_open)
			{
				if (bag.current_state == bag_closed && bag.ready_to_change_state && !bag.was_moved)
				{
					bag.current_state = bag_opening;
					bag.was_moved = false;
					continue;
				}
			}
			else
			{
				if (bag.current_state == bag_open && bag.ready_to_change_state)
				{
					bag.current_state = bag_closing;
					bag.was_moved = false;
					continue;
				}
			}
		}
		bag.was_moved = false;
		if (bag.current_state != bag_open)
			continue;
		//-------------------

		// put cursor item to bag
		if (heldItem.content.first != entity_tag::emptyCell)
		{
			const auto curIndex = bag.get_selected_cell(mousePos);
			if (curIndex == -1)
				continue;
			auto& item = bag.cells[curIndex];
			if (item.content.first == entity_tag::emptyCell || item.content.first == heldItem.content.first)
			{
				item.content.first = heldItem.content.first;
				item.content.second += heldItem.content.second;
				if (item.content.second > hero_bag::items_max_count.at(entity_tag(item.content.first)))
				{
					heldItem.content.second = item.content.second % hero_bag::items_max_count.at(entity_tag(item.content.first));
					item.content.second = hero_bag::items_max_count.at(entity_tag(item.content.first));
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
			const auto curIndex = bag.get_selected_cell(mousePos);
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
		result.push_back(bag.prepare_sprite(elapsedTime, packsMap));
		bag.ready_to_eject = false;
		if (bag.was_moved)
			currentMovingBag = cnt;
		const auto curIndex = bag.get_selected_cell(mousePos);

		if (bag.ready_to_change_state || heldItem.content.first != entity_tag::emptyCell || curIndex != -1)
			usedMouse = true;

		// dropping bag
		if (helper::getDist(bag.get_position(), Vector2f(helper::GetScreenSize().x / 2, helper::GetScreenSize().y / 2)) <= dropZoneRadius && bag.current_state == bag_closed)
		{
			cursorTurnedOn = true;
			if (cursorText.empty())
			{
				effectsSystem.add_effect(effects::transparency_removal, &dropZone, "dropZone", long(3 * 10e4));
			}
			cursorText = "throw away";
			cursorTextPos = bag.get_position();
			bag.ready_to_eject = true;
			bagPosDot.setPosition(bag.get_position());
		}
		//-------------

		if (cnt == currentMovingBag)
			bagPosDot.setPosition(bag.get_position());

		// drawing bag content
		if (bag.current_state != bag_open)
			continue;
		for (auto& cell : bag.cells)
		{
			const auto item = cell;

			//drawing cell background
			auto iconBackground = sprite_pack::tag_to_icon(entity_tag::emptyObject, false);
			iconBackground->position = item.position;
			result.push_back(iconBackground);
			//-----------------------

			if (cell.content.first == entity_tag::emptyCell)
				continue;

			auto icon = sprite_pack::tag_to_icon(item.content.first, helper::isIntersects(mousePos, cell.position, sprite_pack::icon_size.x / 2), 1);			
			icon->position = item.position;
			result.push_back(icon);

			if (hero_bag::items_max_count.at(item.content.first) != 1)
				result.push_back(new text_chain_element(
					icon->position,
					/*{ -SpritePack::iconWithoutSpaceSize.x / 2, -SpritePack::iconWithoutSpaceSize.y / 2 },*/ { 0, 0 },
					sf::Color(255, 255, 255, 180),
					std::to_string(item.content.second),
					text_chain_element::default_character_size * 1.5f));
		}
		//--------------------
	}
	//----------

	//drawing held item
	if (heldItem.content.first != entity_tag::emptyCell)
	{
		auto heldItemIcon = sprite_pack::tag_to_icon(heldItem.content.first, true, 1);
		heldItemIcon->position = heldItem.position;
		result.push_back(heldItemIcon);
		result.push_back(heldItemIcon);
		if (hero_bag::items_max_count.at(heldItem.content.first) != 1)
			result.push_back(new text_chain_element(
				heldItemIcon->position,
				/*{ -SpritePack::iconWithoutSpaceSize.x / 2, -SpritePack::iconWithoutSpaceSize.y / 2 },*/ { 0, 0 },
				sf::Color(255, 255, 255, 180),
				std::to_string(heldItem.content.second),
				text_chain_element::default_character_size * 1.5f));
;	}
	//-----------------

	// draw cursor text
	if (!cursorTurnedOn)
	{
		cursorText = "";
		effectsSystem.reset_effects({ "dropZone" });
	}
	if (cursorText == "throw away")
	{
		result.push_back(new text_chain_element(
			cursorTextPos,
			{ text_system::getTextBoxSize(cursorText, text_chain_element::default_character_size * 1.5f, font_name::normal_font).x / 2.0f, 0 },
			sf::Color(0, 0, 0, 180),
			cursorText, 
			text_chain_element::default_character_size * 1.5f));
		result.push_back(new sprite_chain_element(pack_tag::inventory, pack_part::areas, direction::DOWN, 2, { 0, 0 }, helper::GetScreenSize()));
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