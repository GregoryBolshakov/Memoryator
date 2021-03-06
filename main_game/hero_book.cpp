//#include "hero_book.h"
//
//#include <fstream>
//
//#include "helper.h"
//
//hero_book::hero_book()
//= default;
//
//hero_book::~hero_book()
//= default;
//
//void hero_book::init(std::map<pack_tag, sprite_pack>* packsMap)
//{
//	initButtons(packsMap);
//	//initContent();
//	//somePage.initAuxiliarySpriteMap();
//}
//
//void hero_book::initButtons(std::map<pack_tag, sprite_pack>* packsMap)
//{
//	const auto screenSize = world_metrics::window_size;
//
//	std::string packTag = "empty";
//	std::string packPart = "full";
//	int number = 1;
//	int selectedNumber = 1;
//	int pressedNumber = 1;
//	sf::Vector2f buttonPosition;
//	sf::Vector2f buttonSize;
//	sf::Vector2f offset = {0, 0};
//	int tag;
//	bool isSelectable;
//
//	std::ifstream fin(buttonsInfoFileDirectory);
//
//	while (fin >> isSelectable)
//	{
//		if (!isSelectable)
//			fin >> packTag >> packPart >> number;		
//		else
//			fin >> packTag >> packPart >> number >> selectedNumber >> pressedNumber;
//
//		fin >> buttonPosition.x >> buttonPosition.y >> buttonSize.y >> offset.x >> offset.y >> tag;
//
//		buttonPosition.x = buttonPosition.x * screenSize.x / 100;
//		buttonPosition.y = buttonPosition.y * screenSize.y / 100;
//		const auto textureSize = packsMap->at(sprite_pack::mapped_pack_tag.at(packTag)).get_original_info(sprite_pack::mapped_pack_part.at(packPart), direction::DOWN, number).source_size;
//		buttonSize.y = buttonSize.y * screenSize.y / 100;
//		buttonSize.x = float(textureSize.w) * buttonSize.y / float(textureSize.h);
//
//		buttonList[button_tag(tag)].initialize(sprite_pack::mapped_pack_tag.at(packTag), sprite_pack::mapped_pack_part.at(packPart), number, selectedNumber, pressedNumber, buttonPosition, buttonSize, isSelectable, button_tag(tag), offset);
//	}
//
//	fin.close();
//
//	// bind all the buttons to page
//	somePage.buttonListBounding(&buttonList);
//	//-----------------------------
//
//	// positioning interface elements
//	buttonList.at(button_tag::bookButtonTag).set_position(sf::Vector2f(world_metrics::window_size.x * 2 / 5 - buttonList.at(button_tag::bookButtonTag).get_global_bounds().width, world_metrics::window_size.y * 14 / 15 - buttonList.at(button_tag::bookButtonTag).get_global_bounds().height));
//	buttonList.at(button_tag::bookStandTag).set_position(sf::Vector2f(world_metrics::window_size.x * 2 / 5 - buttonList.at(button_tag::bookStandTag).get_global_bounds().width, world_metrics::window_size.y * 14 / 15 - buttonList.at(button_tag::bookButtonTag).get_global_bounds().height));
//	buttonList.at(button_tag::bookGlowTag).set_position(sf::Vector2f(world_metrics::window_size.x * 2 / 5 - buttonList.at(button_tag::bookStandTag).get_global_bounds().width, world_metrics::window_size.y * 14 / 15 - buttonList.at(button_tag::bookButtonTag).get_global_bounds().height));
//	buttonList.at(button_tag::bookLightningTag).set_position(sf::Vector2f(world_metrics::window_size.x * 2 / 5 - buttonList.at(button_tag::bookStandTag).get_global_bounds().width, world_metrics::window_size.y * 14 / 15 - buttonList.at(button_tag::bookButtonTag).get_global_bounds().height));
//	//-------------------------------
//}
//
//void hero_book::initContent()
//{
//	somePage.initObjectInfo();
//}
//
//void hero_book::getAllOuterInfo(std::vector<hero_bag>* bags, std::string name, world_object* object, cell* worldHeldItem, bool nearTheTable)
//{
//	boundBags = bags;
//	somePage.boundBags = bags;
//	this->worldMouseName = std::move(name);
//	this->worldSelectedObject = object;
//	somePage.nearTheTable = nearTheTable;
//}
//
//void hero_book::setPage(int page)
//{
//	currentPage = page;
//	somePage.setPage(page);
//}
//
//std::vector<sprite_chain_element*> hero_book::prepareHpLine(float hpRatio)
//{
//	std::vector<sprite_chain_element*> result = {};
//	buttonList.at(button_tag::hpFrameTag).set_position(getHpLinePosition());
//	buttonList.at(button_tag::hpLineTag).set_size(sf::Vector2f(hpRatio * buttonList.at(button_tag::hpFrameTag).get_global_bounds().width, buttonList.at(button_tag::hpFrameTag).get_global_bounds().height));
//	buttonList.at(button_tag::hpLineTag).set_position(getHpLinePosition());
//
//	result.push_back(buttonList.at(button_tag::hpFrameTag).prepare_sprite());
//	result.push_back(buttonList.at(button_tag::hpLineTag).prepare_sprite());
//
//	return result;
//}
//
//std::vector<sprite_chain_element*> hero_book::prepareWreathMatrix()
//{	
//	if (currentPage != 5 || somePage.getOriginalSetups()[currentDraft].id != currentDraft)
//		return {};
//
//	std::vector<sprite_chain_element*> result = {};
//
//	const auto upperLeftCorner = sf::Vector2f(
//		buttonList.at(button_tag::sketching).get_global_bounds().left + buttonList.at(button_tag::sketching).get_global_bounds().width / 1.875f,
//		buttonList.at(button_tag::sketching).get_global_bounds().top + buttonList.at(button_tag::sketching).get_global_bounds().height / 8.2f);
//	sf::Vector2f distance;
//	float offset;
//	button_tag currentType;
//
//	for (auto raw = 0u; raw < somePage.wreathMatrix.size(); raw++)
//	{
//		for (auto column = 0u; column < somePage.wreathMatrix[raw].size(); column++)
//		{
//			if (somePage.wreathMatrix[raw][column] == entity_tag::selectedCell)
//				currentType = button_tag::cellSelected;
//			else
//				currentType = button_tag::cell;
//
//			if (raw % 2 != 0)
//				offset = buttonList.at(currentType).get_global_bounds().width * 3 / 4;
//			else
//				offset = 0;
//
//			distance.x = buttonList.at(currentType).get_global_bounds().width * 6 / 4;
//			distance.y = buttonList.at(currentType).get_global_bounds().height / 2;
//			somePage.wreathMatrixPositions[raw][column] = sf::Vector2f(upperLeftCorner.x + column * distance.x + offset, upperLeftCorner.y + raw * distance.y);
//
//			if (hero_book_page::checkWreathCellFit(raw, column, somePage.getOriginalSetups().at(currentDraft).rings))
//			{
//				// draw cell
//				buttonList.at(currentType).set_position(somePage.wreathMatrixPositions[raw][column]);
//				result.push_back(buttonList.at(currentType).prepare_sprite());
//			}
//		}
//	}
//	auto lineMatrixElements = prepareLineMatrix();
//	result.insert(result.end(), lineMatrixElements.begin(), lineMatrixElements.end());
//
//	return result;
//}
//
//std::vector<sprite_chain_element*> hero_book::prepareLineMatrix()
//{
//	std::vector<sprite_chain_element*> result = {};
//
//	sf::Vector2f distance = sf::Vector2f(buttonList.at(button_tag::cell).get_global_bounds().width * 6 / 4,
//		distance.y = buttonList.at(button_tag::cell).get_global_bounds().height / 2);
//
//	for (auto raw = 0u; raw < somePage.wreathMatrix.size(); raw++)
//	{
//		for (auto column = 0u; column < somePage.wreathMatrix[raw].size(); column++)
//		{
//			if (somePage.wreathMatrix[raw][column] == entity_tag::emptyCell)
//				continue;
//
//			if (somePage.wreathMatrix[raw][column] != entity_tag::emptyCell)
//			{
//				const sf::Vector2f contentOffset = sf::Vector2f(buttonList.at(button_tag(somePage.wreathMatrix[raw][column])).get_global_bounds().width * 0.03f,
//					buttonList.at(button_tag(somePage.wreathMatrix[raw][column])).get_global_bounds().height * 0.05f);
//
//				for (const auto cell : somePage.getBorderCells(raw, column))
//					for (auto& connection : somePage.plantsConnections.at(somePage.wreathMatrix[raw][column]))
//						if (somePage.wreathMatrix[cell.first][cell.second] == connection)
//						{
//							if (hero_book_page::checkWreathCellFit(raw, column, somePage.getOriginalSetups().at(currentDraft).rings) && 
//								hero_book_page::checkWreathCellFit(cell.first, cell.second, somePage.getOriginalSetups().at(currentDraft).rings))
//							{
//								const sf::Vector2f firstLnDot = sf::Vector2f(somePage.wreathMatrixPositions[raw][column].x + distance.y + contentOffset.x, somePage.wreathMatrixPositions[raw][column].y + distance.y + contentOffset.y);
//								const sf::Vector2f secondLnDot = sf::Vector2f(somePage.wreathMatrixPositions[cell.first][cell.second].x + distance.y + contentOffset.x, somePage.wreathMatrixPositions[cell.first][cell.second].y + distance.y + contentOffset.y);
//								auto line = helper::makeLine(firstLnDot, secondLnDot);
//								//result.push_back(line);
//							}
//						}
//			}
//		}
//	}
//
//	auto plantsMatrixElements = preparePlantsMatrix();
//	result.insert(result.end(), plantsMatrixElements.begin(), plantsMatrixElements.end());
//
//	return result;
//}
//
//std::vector<sprite_chain_element*> hero_book::preparePlantsMatrix()
//{
//	std::vector<sprite_chain_element*> result = {};
//
//	for (auto raw = 0u; raw < somePage.wreathMatrix.size(); raw++)
//	{
//		for (auto column = 0u; column < somePage.wreathMatrix[raw].size(); column++)
//		{
//			if (somePage.wreathMatrix[raw][column] != entity_tag::emptyCell && somePage.wreathMatrix[raw][column] != entity_tag::selectedCell)
//			{
//				const sf::Vector2f contentOffset = sf::Vector2f(buttonList.at(button_tag(somePage.wreathMatrix[raw][column])).get_global_bounds().width * 0.195f,
//					buttonList.at(button_tag(somePage.wreathMatrix[raw][column])).get_global_bounds().height * 0.195f);
//				buttonList.at(button_tag(somePage.wreathMatrix[raw][column])).set_position(sf::Vector2f(somePage.wreathMatrixPositions[raw][column].x - contentOffset.x,
//					somePage.wreathMatrixPositions[raw][column].y - contentOffset.y));
//				buttonList.at(button_tag(somePage.wreathMatrix[raw][column])).become_gray();
//
//				for (const auto cell : somePage.getBorderCells(raw, column))
//					for (auto& connection : somePage.plantsConnections.at(somePage.wreathMatrix[raw][column]))
//						if (somePage.wreathMatrix[cell.first][cell.second] == connection)
//						{
//							buttonList.at(button_tag(somePage.wreathMatrix[raw][column])).stop_being_gray();
//							break;
//						}
//				if (hero_book_page::checkWreathCellFit(raw, column, somePage.getOriginalSetups().at(currentDraft).rings))
//				{
//					buttonList.at(button_tag(somePage.wreathMatrix[raw][column])).stop_being_gray();
//					result.push_back(buttonList.at(button_tag(somePage.wreathMatrix[raw][column])).prepare_sprite());					
//				}
//			}
//		}
//	}
//
//	return result;
//}
//
//std::vector<sprite_chain_element*> hero_book::preparePlantsList()
//{
//	if (currentPage != 5)
//		return {};
//
//	std::vector<sprite_chain_element*> result = {};
//
//	const auto upperLeftCorner = sf::Vector2f(
//		buttonList.at(button_tag::sketching).get_global_bounds().left + buttonList.at(button_tag::sketching).get_global_bounds().width * 0.066f,
//		buttonList.at(button_tag::sketching).get_global_bounds().top + buttonList.at(button_tag::sketching).get_global_bounds().height * 0.55f);
//
//	for (auto raw = 0u; raw < somePage.plantsMatrix.size(); raw++)
//	{
//		for (auto column = 0u; column < somePage.plantsMatrix[raw].size(); column++)
//		{
//			if (somePage.plantsMatrix[raw][column].first == entity_tag::emptyCell)
//				continue;
//			auto curFlower = button_tag(somePage.plantsMatrix[raw][column].first);
//			const auto size = sf::Vector2f(buttonList.at(curFlower).get_global_bounds().width, buttonList.at(curFlower).get_global_bounds().height);			
//
//			buttonList.at(curFlower).set_position(sf::Vector2f(upperLeftCorner.x + column * size.x, upperLeftCorner.y + raw * size.y));
//			if (buttonList.at(curFlower).is_selected(sf::Vector2f(sf::Mouse::getPosition())) && currentFlower == entity_tag::emptyCell)
//				this->currentFlower = entity_tag(int(curFlower));
//
//			buttonList.at(curFlower).stop_being_gray();
//			result.push_back(somePage.prepareIconFrame(curFlower, 1));
//			result.push_back(buttonList.at(curFlower).prepare_sprite());
//		}
//	}
//
//	return result;
//}
//
//std::vector<drawable_chain_element*> hero_book::prepareWreathCost(sf::Vector2f pos)
//{
//	const entity_tag currentObject = hero_book_page::pageToObjectId(currentPage);
//	if (!somePage.nearTheTable || !somePage.getObjectInfo()->at(currentObject).isUnlocked || somePage.doneRecipes.count(currentObject) == 0)
//		return {};
//
//	std::vector<drawable_chain_element*> result = {};
//
//	sf::Vector2f drawPos = pos;
//
//	for (auto item : somePage.doneRecipes.at(currentObject))
//	{
//		auto flowerButton = button_tag(item.first);
//		if (buttonList.count(flowerButton) > 0)
//		{
//			buttonList.at(flowerButton).set_position(drawPos);
//			result.push_back(buttonList.at(flowerButton).prepare_sprite());
//			result.push_back(new text_chain_element(drawPos, { 0, 0 }, sf::Color::Black, std::to_string(item.second)));
//			drawPos.x += buttonList.at(flowerButton).get_global_bounds().width;
//		}
//	}
//
//	return result;
//}
//
//std::vector<drawable_chain_element*> hero_book::prepareSprites(float hpRatio, long long elapsedTime)
//{
//	std::vector<drawable_chain_element*> result = {};
//
//	auto hpLineElements = prepareHpLine(hpRatio);
//	result.insert(result.end(), hpLineElements.begin(), hpLineElements.end());
//
//	result.push_back(buttonList.at(button_tag::bookStandTag).prepare_sprite());
//	result.push_back(buttonList.at(button_tag::bookButtonTag).prepare_sprite());
//	result.push_back(buttonList.at(button_tag::bookGlowTag).prepare_sprite());
//	if (buttonList.at(button_tag::bookButtonTag).is_selected(sf::Vector2f(sf::Mouse::getPosition())))
//		result.push_back(buttonList.at(button_tag::bookLightningTag).prepare_sprite());
//
//	// turn off all book buttons
//	for (auto& button : buttonList)
//		if (button.first != button_tag::bookButtonTag)
//			button.second.is_active = false;
//	if (heldItem.first == entity_tag::emptyCell)
//		currentFlower = entity_tag::emptyCell;
//	//-----------------------------
//
//	if (!visibility)
//		return result;
//
//	if (currentPage != 0)
//	{
//		result.push_back(buttonList.at(button_tag::pageBackground).prepare_sprite());
//	}
//
//	const auto pageContent = somePage.getPreparedContent(currentPage, currentDraft);
//
//	// main page content	
//	if (currentPage >= 101 && currentPage <= 499)
//	{
//		auto lineElements = somePage.prepareLines();
//		result.insert(result.end(), lineElements.begin(), lineElements.end());
//	}
//
//	auto iconElements = somePage.prepareAllIcons(pageContent);
//	result.insert(result.end(), iconElements.begin(), iconElements.end());
//
//	auto wreathElements = prepareWreathMatrix();
//	result.insert(result.end(), wreathElements.begin(), wreathElements.end());
//
//	auto plantsElements = preparePlantsList();
//	result.insert(result.end(), plantsElements.begin(), plantsElements.end());
//
//	if (currentFlower != entity_tag::emptyCell)
//	{
//		auto connectableFlowers = somePage.prepareConnectableFlowers(currentFlower);
//		result.insert(result.end(), connectableFlowers.begin(), connectableFlowers.end());
//	}
//
//	/*TextSystem::drawTextBox(pageContent.blockDescription, FontName::NormalFont, 25, pageGlobalBounds.left + pageGlobalBounds.width * blockDescriptionPoint.x,
//		pageGlobalBounds.top + pageGlobalBounds.height * blockDescriptionPoint.y, pageGlobalBounds.width * 0.4, pageGlobalBounds.height * 0.24, window, sf::Color(100, 68, 34, 180));
//	TextSystem::drawTextBox(pageContent.pageDescription, FontName::NormalFont, 25, pageGlobalBounds.left + pageGlobalBounds.width * pageDescriptionPoint.x,
//		pageGlobalBounds.top + pageGlobalBounds.height * pageDescriptionPoint.y, pageGlobalBounds.width * 0.4, pageGlobalBounds.height * 0.24, window, sf::Color(100, 68, 34, 180));*/
//
//	auto headingText = somePage.prepareHeadingText();
//	result.insert(result.end(), headingText.begin(), headingText.end());
//
//	if (buttonList.at(button_tag::makeWreath).is_selected(sf::Vector2f(sf::Mouse::getPosition())))
//	{
//		auto wreathCostElements = prepareWreathCost(buttonList.at(button_tag::makeWreath).get_position());
//		result.insert(result.end(), wreathCostElements.begin(), wreathCostElements.end());
//	}
//	//------------------
//
//	//draw arrows
//	if (currentPage >= 999)
//		buttonList.at(button_tag::nextPage).is_active = false;
//	else
//	{
//		buttonList.at(button_tag::previousPage).is_active = true;
//		result.push_back(buttonList.at(button_tag::nextPage).prepare_sprite());
//	}
//	if (currentPage <= 0)
//		buttonList.at(button_tag::previousPage).is_active = false;
//	else
//	{
//		buttonList.at(button_tag::previousPage).is_active = true;
//		result.push_back(buttonList.at(button_tag::previousPage).prepare_sprite());
//	}
//	//------------------------------
//
//	//draw held item
//	debugInfo = std::to_string((sf::Mouse::getPosition().x - buttonList.at(button_tag::bookmarksList).get_global_bounds().left) / buttonList.at(button_tag::bookmarksList).get_global_bounds().width) + " " 
//	+ std::to_string((sf::Mouse::getPosition().y - buttonList.at(button_tag::bookmarksList).get_global_bounds().top) / buttonList.at(button_tag::bookmarksList).get_global_bounds().height);
//
//	if (heldItem.first != entity_tag::emptyCell)
//	{
//		const float offset = buttonList.at(button_tag(heldItem.first)).get_global_bounds().width / 2;
//		buttonList.at(button_tag(heldItem.first)).set_position(sf::Vector2f(sf::Mouse::getPosition().x - offset, sf::Mouse::getPosition().y - offset));
//		result.push_back(buttonList.at(button_tag(heldItem.first)).prepare_sprite());
//	}
//	//--------------
//
//	return result;
//}
//
//void hero_book::interact()
//{
//	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
//		WhileMouseDown();
//}
//
//void hero_book::onMouseUp()
//{
//	// plant dragging to draft
//	if (currentPage == 5)
//	{
//		const auto selectedCell = somePage.getSelectedWreathCell();
//		if (selectedCell != std::make_pair(-1, -1) && currentDraft != entity_tag::emptyDraft)
//		{
//			auto canBePlaced = true;
//			auto plants = somePage.getOriginalSetups().at(currentDraft).plants;
//			for (auto& plant : plants)
//			{
//				if (selectedCell.first == int(plant.second.first) && selectedCell.second == (plant.second.second) ||
//					!hero_book_page::checkWreathCellFit(selectedCell.first, selectedCell.second, somePage.getOriginalSetups().at(currentDraft).rings))
//				{
//					canBePlaced = false;
//					break;
//				}
//			}
//			if (canBePlaced)
//				somePage.wreathMatrix[selectedCell.first][selectedCell.second] = heldItem.first;
//		}
//		somePage.readyToFinishDraft = somePage.isCenterSurrounded();
//		if (heldItem.first != entity_tag::emptyCell)
//		{
//			heldItem = { entity_tag::emptyCell, 0 };
//			return;
//		}
//	}
//	//------------------------
//
//	// change page
//	if (buttonList.at(button_tag::bookButtonTag).is_selected(sf::Vector2f(sf::Mouse::getPosition())))
//		changeVisibility();
//	
//	for (auto& button : buttonList)
//		if (somePage.buttonToPage(button.first) != -1 && button.second.is_selected(sf::Vector2f(sf::Mouse::getPosition())) && button.second.is_active)
//			setPage(somePage.buttonToPage(button.first));
//	//------------	
//
//	// craft a wreath
//	if (buttonList.at(button_tag::makeWreath).is_active && somePage.nearTheTable)
//		if (buttonList.at(button_tag::makeWreath).is_selected(sf::Vector2f(sf::Mouse::getPosition())))		
//			if (boundBags)
//				if (hero_book_page::tagToWreath(entity_tag(currentPage)) != entity_tag::emptyCell)
//				hero_bag::put_item_in(new std::pair<entity_tag, int>(hero_book_page::tagToWreath(entity_tag(currentPage)), 1), boundBags);					
//	//---------------
//
//	// clicking the plus in the draft center
//	if (buttonList.at(button_tag::plus).is_active)
//	{
//		if (buttonList.at(button_tag::plus).is_selected(sf::Vector2f(sf::Mouse::getPosition())))
//		{
//			for (auto& raw : somePage.wreathMatrix)
//				for (auto& column : raw)
//					if (column != entity_tag::emptyCell)
//						somePage.doneRecipes[currentDraft][column] += 1;
//
//			somePage.unlockObject(currentDraft);
//			setPage(hero_book_page::getHeadingPage(currentDraft));
//			somePage.clearWreathMatrix();
//			currentDraft = entity_tag::emptyDraft;
//			somePage.readyToFinishDraft = false;
//		}
//	}
//	//--------------------------------------
//
//	// set draft into scheme center
//	if (worldSelectedObject != nullptr)
//	{
//		const auto worldObjectTag = worldSelectedObject->tag;
//		if (somePage.getOriginalSetups()[currentDraft].id == currentDraft)
//		{
//			if (worldMouseName == "Sketch")
//			{
//				visibility = true;
//				setPage(5);
//				currentDraft = worldObjectTag;
//				somePage.setPlantsOnMatrix(somePage.getOriginalSetups().at(worldObjectTag).plants);
//			}
//		}
//	}
//	//-----------------------------
//
//}
//
//void hero_book::WhileMouseDown()
//{
//	// flower to matrix transfer
//	if (currentPage == 5) 
//	{
//		if (heldItem.first == entity_tag::emptyCell)
//		{
//			const auto selectedCell = somePage.getSelectedPlantsCell();
//			if (selectedCell != std::make_pair(-1, -1))
//			{
//				heldItem.first = entity_tag(int(somePage.plantsMatrix[selectedCell.first][selectedCell.second].first));
//				heldItem.second = somePage.plantsMatrix[selectedCell.first][selectedCell.second].second;
//			}
//		}
//		else
//		{
//			if (int(heldItem.first) >= 401 && int(heldItem.first) <= 499)
//				this->currentFlower = entity_tag(int(heldItem.first));
//		}
//	}
//}
//
