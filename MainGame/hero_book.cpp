#include "hero_book.h"

#include <fstream>

#include "Helper.h"

hero_book::hero_book()
= default;

hero_book::~hero_book()
= default;

void hero_book::init(std::map<PackTag, sprite_pack>* packsMap)
{
	initButtons(packsMap);
	//initContent();
	//somePage.initAuxiliarySpriteMap();
}

void hero_book::initButtons(std::map<PackTag, sprite_pack>* packsMap)
{
	const auto screenSize = Helper::GetScreenSize();

	std::string packTag = "empty";
	std::string packPart = "full";
	int number = 1;
	int selectedNumber = 1;
	int pressedNumber = 1;
	Vector2f buttonPosition;
	Vector2f buttonSize;
	Vector2f offset = {0, 0};
	int tag;
	bool isSelectable;

	std::ifstream fin(buttonsInfoFileDirectory);

	while (fin >> isSelectable)
	{
		if (!isSelectable)
			fin >> packTag >> packPart >> number;		
		else
			fin >> packTag >> packPart >> number >> selectedNumber >> pressedNumber;

		fin >> buttonPosition.x >> buttonPosition.y >> buttonSize.y >> offset.x >> offset.y >> tag;

		buttonPosition.x = buttonPosition.x * screenSize.x / 100;
		buttonPosition.y = buttonPosition.y * screenSize.y / 100;
		const auto textureSize = packsMap->at(sprite_pack::mappedPackTag.at(packTag)).getOriginalInfo(sprite_pack::mappedPackPart.at(packPart), Direction::DOWN, number).source_size;
		buttonSize.y = buttonSize.y * screenSize.y / 100;
		buttonSize.x = float(textureSize.w) * buttonSize.y / float(textureSize.h);

		buttonList[ButtonTag(tag)].initialize(sprite_pack::mappedPackTag.at(packTag), sprite_pack::mappedPackPart.at(packPart), number, selectedNumber, pressedNumber, buttonPosition, buttonSize, isSelectable, ButtonTag(tag), offset);
	}

	fin.close();

	// bind all the buttons to page
	somePage.buttonListBounding(&buttonList);
	//-----------------------------

	// positioning interface elements
	buttonList.at(ButtonTag::bookButtonTag).set_position(Vector2f(Helper::GetScreenSize().x * 2 / 5 - buttonList.at(ButtonTag::bookButtonTag).get_global_bounds().width, Helper::GetScreenSize().y * 14 / 15 - buttonList.at(ButtonTag::bookButtonTag).get_global_bounds().height));
	buttonList.at(ButtonTag::bookStandTag).set_position(Vector2f(Helper::GetScreenSize().x * 2 / 5 - buttonList.at(ButtonTag::bookStandTag).get_global_bounds().width, Helper::GetScreenSize().y * 14 / 15 - buttonList.at(ButtonTag::bookButtonTag).get_global_bounds().height));
	buttonList.at(ButtonTag::bookGlowTag).set_position(Vector2f(Helper::GetScreenSize().x * 2 / 5 - buttonList.at(ButtonTag::bookStandTag).get_global_bounds().width, Helper::GetScreenSize().y * 14 / 15 - buttonList.at(ButtonTag::bookButtonTag).get_global_bounds().height));
	buttonList.at(ButtonTag::bookLightningTag).set_position(Vector2f(Helper::GetScreenSize().x * 2 / 5 - buttonList.at(ButtonTag::bookStandTag).get_global_bounds().width, Helper::GetScreenSize().y * 14 / 15 - buttonList.at(ButtonTag::bookButtonTag).get_global_bounds().height));
	//-------------------------------
}

void hero_book::initContent()
{
	somePage.initObjectInfo();
}

void hero_book::getAllOuterInfo(std::vector<HeroBag>* bags, std::string name, WorldObject* object, Cell* worldHeldItem, bool nearTheTable)
{
	boundBags = bags;
	somePage.boundBags = bags;
	this->worldMouseName = std::move(name);
	this->worldSelectedObject = object;
	somePage.nearTheTable = nearTheTable;
}

void hero_book::setPage(int page)
{
	currentPage = page;
	somePage.setPage(page);
}

std::vector<sprite_chain_element*> hero_book::prepareHpLine(float hpRatio)
{
	std::vector<sprite_chain_element*> result = {};
	buttonList.at(ButtonTag::hpFrameTag).set_position(getHpLinePosition());
	buttonList.at(ButtonTag::hpLineTag).set_size(Vector2f(hpRatio * buttonList.at(ButtonTag::hpFrameTag).get_global_bounds().width, buttonList.at(ButtonTag::hpFrameTag).get_global_bounds().height));
	buttonList.at(ButtonTag::hpLineTag).set_position(getHpLinePosition());

	result.push_back(buttonList.at(ButtonTag::hpFrameTag).prepare_sprite());
	result.push_back(buttonList.at(ButtonTag::hpLineTag).prepare_sprite());

	return result;
}

std::vector<sprite_chain_element*> hero_book::prepareWreathMatrix()
{	
	if (currentPage != 5 || somePage.getOriginalSetups()[currentDraft].id != currentDraft)
		return {};

	std::vector<sprite_chain_element*> result = {};

	const auto upperLeftCorner = Vector2f(
		buttonList.at(ButtonTag::sketching).get_global_bounds().left + buttonList.at(ButtonTag::sketching).get_global_bounds().width / 1.875f,
		buttonList.at(ButtonTag::sketching).get_global_bounds().top + buttonList.at(ButtonTag::sketching).get_global_bounds().height / 8.2f);
	Vector2f distance;
	float offset;
	ButtonTag currentType;

	for (auto raw = 0u; raw < somePage.wreathMatrix.size(); raw++)
	{
		for (auto column = 0u; column < somePage.wreathMatrix[raw].size(); column++)
		{
			if (somePage.wreathMatrix[raw][column] == Tag::selectedCell)
				currentType = ButtonTag::cellSelected;
			else
				currentType = ButtonTag::cell;

			if (raw % 2 != 0)
				offset = buttonList.at(currentType).get_global_bounds().width * 3 / 4;
			else
				offset = 0;

			distance.x = buttonList.at(currentType).get_global_bounds().width * 6 / 4;
			distance.y = buttonList.at(currentType).get_global_bounds().height / 2;
			somePage.wreathMatrixPositions[raw][column] = Vector2f(upperLeftCorner.x + column * distance.x + offset, upperLeftCorner.y + raw * distance.y);

			if (hero_book_page::checkWreathCellFit(raw, column, somePage.getOriginalSetups().at(currentDraft).rings))
			{
				// draw cell
				buttonList.at(currentType).set_position(somePage.wreathMatrixPositions[raw][column]);
				result.push_back(buttonList.at(currentType).prepare_sprite());
			}
		}
	}
	auto lineMatrixElements = prepareLineMatrix();
	result.insert(result.end(), lineMatrixElements.begin(), lineMatrixElements.end());

	return result;
}

std::vector<sprite_chain_element*> hero_book::prepareLineMatrix()
{
	std::vector<sprite_chain_element*> result = {};

	Vector2f distance = Vector2f(buttonList.at(ButtonTag::cell).get_global_bounds().width * 6 / 4,
		distance.y = buttonList.at(ButtonTag::cell).get_global_bounds().height / 2);

	for (auto raw = 0u; raw < somePage.wreathMatrix.size(); raw++)
	{
		for (auto column = 0u; column < somePage.wreathMatrix[raw].size(); column++)
		{
			if (somePage.wreathMatrix[raw][column] == Tag::emptyCell)
				continue;

			if (somePage.wreathMatrix[raw][column] != Tag::emptyCell)
			{
				const Vector2f contentOffset = Vector2f(buttonList.at(ButtonTag(somePage.wreathMatrix[raw][column])).get_global_bounds().width * 0.03f,
					buttonList.at(ButtonTag(somePage.wreathMatrix[raw][column])).get_global_bounds().height * 0.05f);

				for (const auto cell : somePage.getBorderCells(raw, column))
					for (auto& connection : somePage.plantsConnections.at(somePage.wreathMatrix[raw][column]))
						if (somePage.wreathMatrix[cell.first][cell.second] == connection)
						{
							if (hero_book_page::checkWreathCellFit(raw, column, somePage.getOriginalSetups().at(currentDraft).rings) && 
								hero_book_page::checkWreathCellFit(cell.first, cell.second, somePage.getOriginalSetups().at(currentDraft).rings))
							{
								const Vector2f firstLnDot = Vector2f(somePage.wreathMatrixPositions[raw][column].x + distance.y + contentOffset.x, somePage.wreathMatrixPositions[raw][column].y + distance.y + contentOffset.y);
								const Vector2f secondLnDot = Vector2f(somePage.wreathMatrixPositions[cell.first][cell.second].x + distance.y + contentOffset.x, somePage.wreathMatrixPositions[cell.first][cell.second].y + distance.y + contentOffset.y);
								auto line = Helper::makeLine(firstLnDot, secondLnDot);
								//result.push_back(line);
							}
						}
			}
		}
	}

	auto plantsMatrixElements = preparePlantsMatrix();
	result.insert(result.end(), plantsMatrixElements.begin(), plantsMatrixElements.end());

	return result;
}

std::vector<sprite_chain_element*> hero_book::preparePlantsMatrix()
{
	std::vector<sprite_chain_element*> result = {};

	for (auto raw = 0u; raw < somePage.wreathMatrix.size(); raw++)
	{
		for (auto column = 0u; column < somePage.wreathMatrix[raw].size(); column++)
		{
			if (somePage.wreathMatrix[raw][column] != Tag::emptyCell && somePage.wreathMatrix[raw][column] != Tag::selectedCell)
			{
				const Vector2f contentOffset = Vector2f(buttonList.at(ButtonTag(somePage.wreathMatrix[raw][column])).get_global_bounds().width * 0.195f,
					buttonList.at(ButtonTag(somePage.wreathMatrix[raw][column])).get_global_bounds().height * 0.195f);
				buttonList.at(ButtonTag(somePage.wreathMatrix[raw][column])).set_position(Vector2f(somePage.wreathMatrixPositions[raw][column].x - contentOffset.x,
					somePage.wreathMatrixPositions[raw][column].y - contentOffset.y));
				buttonList.at(ButtonTag(somePage.wreathMatrix[raw][column])).become_gray();

				for (const auto cell : somePage.getBorderCells(raw, column))
					for (auto& connection : somePage.plantsConnections.at(somePage.wreathMatrix[raw][column]))
						if (somePage.wreathMatrix[cell.first][cell.second] == connection)
						{
							buttonList.at(ButtonTag(somePage.wreathMatrix[raw][column])).stop_being_gray();
							break;
						}
				if (hero_book_page::checkWreathCellFit(raw, column, somePage.getOriginalSetups().at(currentDraft).rings))
				{
					buttonList.at(ButtonTag(somePage.wreathMatrix[raw][column])).stop_being_gray();
					result.push_back(buttonList.at(ButtonTag(somePage.wreathMatrix[raw][column])).prepare_sprite());					
				}
			}
		}
	}

	return result;
}

std::vector<sprite_chain_element*> hero_book::preparePlantsList()
{
	if (currentPage != 5)
		return {};

	std::vector<sprite_chain_element*> result = {};

	const auto upperLeftCorner = Vector2f(
		buttonList.at(ButtonTag::sketching).get_global_bounds().left + buttonList.at(ButtonTag::sketching).get_global_bounds().width * 0.066f,
		buttonList.at(ButtonTag::sketching).get_global_bounds().top + buttonList.at(ButtonTag::sketching).get_global_bounds().height * 0.55f);

	for (auto raw = 0u; raw < somePage.plantsMatrix.size(); raw++)
	{
		for (auto column = 0u; column < somePage.plantsMatrix[raw].size(); column++)
		{
			if (somePage.plantsMatrix[raw][column].first == Tag::emptyCell)
				continue;
			auto curFlower = ButtonTag(somePage.plantsMatrix[raw][column].first);
			const auto size = Vector2f(buttonList.at(curFlower).get_global_bounds().width, buttonList.at(curFlower).get_global_bounds().height);			

			buttonList.at(curFlower).set_position(Vector2f(upperLeftCorner.x + column * size.x, upperLeftCorner.y + raw * size.y));
			if (buttonList.at(curFlower).is_selected(Vector2f(Mouse::getPosition())) && currentFlower == Tag::emptyCell)
				this->currentFlower = Tag(int(curFlower));

			buttonList.at(curFlower).stop_being_gray();
			result.push_back(somePage.prepareIconFrame(curFlower, 1));
			result.push_back(buttonList.at(curFlower).prepare_sprite());
		}
	}

	return result;
}

std::vector<drawable_chain_element*> hero_book::prepareWreathCost(Vector2f pos)
{
	const Tag currentObject = hero_book_page::pageToObjectId(currentPage);
	if (!somePage.nearTheTable || !somePage.getObjectInfo()->at(currentObject).isUnlocked || somePage.doneRecipes.count(currentObject) == 0)
		return {};

	std::vector<drawable_chain_element*> result = {};

	Vector2f drawPos = pos;

	for (auto item : somePage.doneRecipes.at(currentObject))
	{
		auto flowerButton = ButtonTag(item.first);
		if (buttonList.count(flowerButton) > 0)
		{
			buttonList.at(flowerButton).set_position(drawPos);
			result.push_back(buttonList.at(flowerButton).prepare_sprite());
			result.push_back(new text_chain_element(drawPos, { 0, 0 }, sf::Color::Black, std::to_string(item.second)));
			drawPos.x += buttonList.at(flowerButton).get_global_bounds().width;
		}
	}

	return result;
}

std::vector<drawable_chain_element*> hero_book::prepareSprites(float hpRatio, long long elapsedTime)
{
	std::vector<drawable_chain_element*> result = {};

	auto hpLineElements = prepareHpLine(hpRatio);
	result.insert(result.end(), hpLineElements.begin(), hpLineElements.end());

	result.push_back(buttonList.at(ButtonTag::bookStandTag).prepare_sprite());
	result.push_back(buttonList.at(ButtonTag::bookButtonTag).prepare_sprite());
	result.push_back(buttonList.at(ButtonTag::bookGlowTag).prepare_sprite());
	if (buttonList.at(ButtonTag::bookButtonTag).is_selected(Vector2f(Mouse::getPosition())))
		result.push_back(buttonList.at(ButtonTag::bookLightningTag).prepare_sprite());

	// turn off all book buttons
	for (auto& button : buttonList)
		if (button.first != ButtonTag::bookButtonTag)
			button.second.is_active = false;
	if (heldItem.first == Tag::emptyCell)
		currentFlower = Tag::emptyCell;
	//-----------------------------

	if (!visibility)
		return result;

	if (currentPage != 0)
	{
		result.push_back(buttonList.at(ButtonTag::pageBackground).prepare_sprite());
	}

	const auto pageContent = somePage.getPreparedContent(currentPage, currentDraft);

	// main page content	
	if (currentPage >= 101 && currentPage <= 499)
	{
		auto lineElements = somePage.prepareLines();
		result.insert(result.end(), lineElements.begin(), lineElements.end());
	}

	auto iconElements = somePage.prepareAllIcons(pageContent);
	result.insert(result.end(), iconElements.begin(), iconElements.end());

	auto wreathElements = prepareWreathMatrix();
	result.insert(result.end(), wreathElements.begin(), wreathElements.end());

	auto plantsElements = preparePlantsList();
	result.insert(result.end(), plantsElements.begin(), plantsElements.end());

	if (currentFlower != Tag::emptyCell)
	{
		auto connectableFlowers = somePage.prepareConnectableFlowers(currentFlower);
		result.insert(result.end(), connectableFlowers.begin(), connectableFlowers.end());
	}

	/*TextSystem::drawTextBox(pageContent.blockDescription, FontName::NormalFont, 25, pageGlobalBounds.left + pageGlobalBounds.width * blockDescriptionPoint.x,
		pageGlobalBounds.top + pageGlobalBounds.height * blockDescriptionPoint.y, pageGlobalBounds.width * 0.4, pageGlobalBounds.height * 0.24, window, Color(100, 68, 34, 180));
	TextSystem::drawTextBox(pageContent.pageDescription, FontName::NormalFont, 25, pageGlobalBounds.left + pageGlobalBounds.width * pageDescriptionPoint.x,
		pageGlobalBounds.top + pageGlobalBounds.height * pageDescriptionPoint.y, pageGlobalBounds.width * 0.4, pageGlobalBounds.height * 0.24, window, Color(100, 68, 34, 180));*/

	auto headingText = somePage.prepareHeadingText();
	result.insert(result.end(), headingText.begin(), headingText.end());

	if (buttonList.at(ButtonTag::makeWreath).is_selected(Vector2f(Mouse::getPosition())))
	{
		auto wreathCostElements = prepareWreathCost(buttonList.at(ButtonTag::makeWreath).get_position());
		result.insert(result.end(), wreathCostElements.begin(), wreathCostElements.end());
	}
	//------------------

	//draw arrows
	if (currentPage >= 999)
		buttonList.at(ButtonTag::nextPage).is_active = false;
	else
	{
		buttonList.at(ButtonTag::previousPage).is_active = true;
		result.push_back(buttonList.at(ButtonTag::nextPage).prepare_sprite());
	}
	if (currentPage <= 0)
		buttonList.at(ButtonTag::previousPage).is_active = false;
	else
	{
		buttonList.at(ButtonTag::previousPage).is_active = true;
		result.push_back(buttonList.at(ButtonTag::previousPage).prepare_sprite());
	}
	//------------------------------

	//draw held item
	debugInfo = std::to_string((Mouse::getPosition().x - buttonList.at(ButtonTag::bookmarksList).get_global_bounds().left) / buttonList.at(ButtonTag::bookmarksList).get_global_bounds().width) + " " 
	+ std::to_string((Mouse::getPosition().y - buttonList.at(ButtonTag::bookmarksList).get_global_bounds().top) / buttonList.at(ButtonTag::bookmarksList).get_global_bounds().height);

	if (heldItem.first != Tag::emptyCell)
	{
		const float offset = buttonList.at(ButtonTag(heldItem.first)).get_global_bounds().width / 2;
		buttonList.at(ButtonTag(heldItem.first)).set_position(Vector2f(Mouse::getPosition().x - offset, Mouse::getPosition().y - offset));
		result.push_back(buttonList.at(ButtonTag(heldItem.first)).prepare_sprite());
	}
	//--------------

	return result;
}

void hero_book::interact()
{
	if (Mouse::isButtonPressed(Mouse::Left))
		WhileMouseDown();
}

void hero_book::onMouseUp()
{
	// plant dragging to draft
	if (currentPage == 5)
	{
		const auto selectedCell = somePage.getSelectedWreathCell();
		if (selectedCell != std::make_pair(-1, -1) && currentDraft != Tag::emptyDraft)
		{
			auto canBePlaced = true;
			auto plants = somePage.getOriginalSetups().at(currentDraft).plants;
			for (auto& plant : plants)
			{
				if (selectedCell.first == int(plant.second.first) && selectedCell.second == (plant.second.second) ||
					!hero_book_page::checkWreathCellFit(selectedCell.first, selectedCell.second, somePage.getOriginalSetups().at(currentDraft).rings))
				{
					canBePlaced = false;
					break;
				}
			}
			if (canBePlaced)
				somePage.wreathMatrix[selectedCell.first][selectedCell.second] = heldItem.first;
		}
		somePage.readyToFinishDraft = somePage.isCenterSurrounded();
		if (heldItem.first != Tag::emptyCell)
		{
			heldItem = { Tag::emptyCell, 0 };
			return;
		}
	}
	//------------------------

	// change page
	if (buttonList.at(ButtonTag::bookButtonTag).is_selected(Vector2f(Mouse::getPosition())))
		changeVisibility();
	
	for (auto& button : buttonList)
		if (somePage.buttonToPage(button.first) != -1 && button.second.is_selected(Vector2f(Mouse::getPosition())) && button.second.is_active)
			setPage(somePage.buttonToPage(button.first));
	//------------	

	// craft a wreath
	if (buttonList.at(ButtonTag::makeWreath).is_active && somePage.nearTheTable)
		if (buttonList.at(ButtonTag::makeWreath).is_selected(Vector2f(Mouse::getPosition())))		
			if (boundBags)
				if (hero_book_page::tagToWreath(Tag(currentPage)) != Tag::emptyCell)
				HeroBag::putItemIn(new std::pair<Tag, int>(hero_book_page::tagToWreath(Tag(currentPage)), 1), boundBags);					
	//---------------

	// clicking the plus in the draft center
	if (buttonList.at(ButtonTag::plus).is_active)
	{
		if (buttonList.at(ButtonTag::plus).is_selected(Vector2f(Mouse::getPosition())))
		{
			for (auto& raw : somePage.wreathMatrix)
				for (auto& column : raw)
					if (column != Tag::emptyCell)
						somePage.doneRecipes[currentDraft][column] += 1;

			somePage.unlockObject(currentDraft);
			setPage(hero_book_page::getHeadingPage(currentDraft));
			somePage.clearWreathMatrix();
			currentDraft = Tag::emptyDraft;
			somePage.readyToFinishDraft = false;
		}
	}
	//--------------------------------------

	// set draft into scheme center
	if (worldSelectedObject != nullptr)
	{
		const auto worldObjectTag = worldSelectedObject->tag;
		if (somePage.getOriginalSetups()[currentDraft].id == currentDraft)
		{
			if (worldMouseName == "Sketch")
			{
				visibility = true;
				setPage(5);
				currentDraft = worldObjectTag;
				somePage.setPlantsOnMatrix(somePage.getOriginalSetups().at(worldObjectTag).plants);
			}
		}
	}
	//-----------------------------

}

void hero_book::WhileMouseDown()
{
	// flower to matrix transfer
	if (currentPage == 5) 
	{
		if (heldItem.first == Tag::emptyCell)
		{
			const auto selectedCell = somePage.getSelectedPlantsCell();
			if (selectedCell != std::make_pair(-1, -1))
			{
				heldItem.first = Tag(int(somePage.plantsMatrix[selectedCell.first][selectedCell.second].first));
				heldItem.second = somePage.plantsMatrix[selectedCell.first][selectedCell.second].second;
			}
		}
		else
		{
			if (int(heldItem.first) >= 401 && int(heldItem.first) <= 499)
				this->currentFlower = Tag(int(heldItem.first));
		}
	}
}

