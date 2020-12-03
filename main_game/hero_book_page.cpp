//#include <fstream>
//
//#include "hero_book_page.h"
//
//hero_book_page::hero_book_page()
//{
//	initDrafts();
//
//	wreathMatrix.resize(17, std::vector<entity_tag>(5));
//	wreathMatrixPositions.resize(17, std::vector<sf::Vector2f>(5));
//	coloredMatrix.resize(17, std::vector<int>(5));
//	plantsMatrix.resize(2, std::vector<std::pair<entity_tag, int>>(3));
//	for (auto& raw : wreathMatrix)
//		for (auto& cell : raw)
//			cell = entity_tag::emptyCell;
//
//	plantsMatrix[0][0] = { entity_tag::chamomile, 1 };
//	plantsMatrix[0][1] = { entity_tag::yarrow, 1 };
//	plantsMatrix[0][2] = { entity_tag::fern, 1 };
//	plantsMatrix[1][0] = { entity_tag::mugwort, 1 };
//	plantsMatrix[1][1] = { entity_tag::poppy, 1 };
//	plantsMatrix[1][2] = { entity_tag::emptyCell, 0 };
//
//	plantsConnections[entity_tag::chamomile] = { entity_tag::poppy, entity_tag::yarrow };
//	plantsConnections[entity_tag::yarrow] = { entity_tag::chamomile, entity_tag::fern };
//	plantsConnections[entity_tag::fern] = { entity_tag::yarrow, entity_tag::mugwort };
//	plantsConnections[entity_tag::mugwort] = { entity_tag::fern, entity_tag::poppy };
//	plantsConnections[entity_tag::poppy] = { entity_tag::mugwort, entity_tag::chamomile };
//}
//
//hero_book_page::~hero_book_page()
//= default;
//
//void hero_book_page::setBookmarkPosition() const
//{
//	FloatRect pageGlobalBounds;
//	
//	if (currentPage == 0)
//		pageGlobalBounds = buttonList->at(button_tag::bookCover).get_global_bounds();
//	else
//		pageGlobalBounds = buttonList->at(button_tag::pageBackground).get_global_bounds();
//
//	float bookmarkApproximateX;
//	if (currentPage == 0)
//		bookmarkApproximateX = pageGlobalBounds.left + pageGlobalBounds.width * 8.9f / 10.0f;
//	else
//		bookmarkApproximateX = pageGlobalBounds.left + pageGlobalBounds.width * 9.38f / 10.0f;
//	buttonList->at(button_tag::bookmarkMobs).set_position(sf::Vector2f(bookmarkApproximateX, pageGlobalBounds.top + pageGlobalBounds.height * 1.2f / 10.0f));
//	buttonList->at(button_tag::bookmarkItems).set_position(sf::Vector2f(bookmarkApproximateX, pageGlobalBounds.top + pageGlobalBounds.height * 2.2f / 10.0f));
//	buttonList->at(button_tag::bookmarkWorld).set_position(sf::Vector2f(bookmarkApproximateX, pageGlobalBounds.top + pageGlobalBounds.height * 3.2f / 10.0f));
//	buttonList->at(button_tag::bookmarkFlowers).set_position(sf::Vector2f(bookmarkApproximateX, pageGlobalBounds.top + pageGlobalBounds.height * 4.2f / 10.0f));
//	buttonList->at(button_tag::bookmarkWreathes).set_position(sf::Vector2f(bookmarkApproximateX, pageGlobalBounds.top + pageGlobalBounds.height * 5.2f / 10.0f));
//	buttonList->at(button_tag::bookmarkNightmare).set_position(sf::Vector2f(bookmarkApproximateX, pageGlobalBounds.top + pageGlobalBounds.height * 7.2f / 10.0f));
//	buttonList->at(button_tag::plus).set_position(sf::Vector2f(bookmarkApproximateX, pageGlobalBounds.top + pageGlobalBounds.height * 8.2f / 10.0f));
//	buttonList->at(button_tag::makeWreath).set_position(sf::Vector2f(pageGlobalBounds.left + pageGlobalBounds.width * 1.35f / 10.0f, pageGlobalBounds.top + pageGlobalBounds.height * 6.6f / 10.0f));
//}
//
//void hero_book_page::setArrowsPosition() const
//{
//	const auto pagePatternGlobalBounds = buttonList->at(button_tag::pagePattern).get_global_bounds();
//	auto nextPagePos = sf::Vector2f(pagePatternGlobalBounds.left + pagePatternGlobalBounds.width - arrowToPageGlobalBounds.width,
//	                            pagePatternGlobalBounds.top + pagePatternGlobalBounds.height - arrowToPageGlobalBounds.height);
//	const auto previousPagePos = sf::Vector2f(sf::Vector2f(pagePatternGlobalBounds.left,
//	                                               pagePatternGlobalBounds.top + pagePatternGlobalBounds.height - arrowToPageGlobalBounds.height));
//	
//	if (currentPage == 0)
//		nextPagePos.x -= buttonList->at(button_tag::bookmarksList).get_global_bounds().width / 2;
//
//	buttonList->at(button_tag::nextPage).set_position(nextPagePos);
//	buttonList->at(button_tag::previousPage).set_position(previousPagePos);	
//}
//
//void hero_book_page::preparePageBase()
//{
//	buttonList->at(button_tag::pageBackground).set_position(sf::Vector2f(buttonList->at(button_tag::bookCover).get_position().x - buttonList->at(button_tag::bookCover).get_global_bounds().width,
//		buttonList->at(button_tag::bookCover).get_position().y));
//
//	if (currentPage == 0)
//		pageGlobalBounds = buttonList->at(button_tag::bookCover).get_global_bounds();
//	else
//		pageGlobalBounds = buttonList->at(button_tag::pageBackground).get_global_bounds();
//
//	buttonList->at(button_tag::pagePattern).set_position(sf::Vector2f(pageGlobalBounds.left + pageGlobalBounds.width * 1.0f / 13.0f, pageGlobalBounds.top + pageGlobalBounds.height * 1.0f / 21.0f));	
//	buttonList->at(button_tag::bookmarksList).set_position(sf::Vector2f(pageGlobalBounds.left + pageGlobalBounds.width * 1.0f / 13.0f, pageGlobalBounds.top + pageGlobalBounds.height * 1.0f / 21.0f));	
//	buttonList->at(button_tag::sketching).set_position(sf::Vector2f(pageGlobalBounds.left + pageGlobalBounds.width * 1.0f / 13.0f, pageGlobalBounds.top + pageGlobalBounds.height * 1.0f / 21.0f));	
//
//	blockGlobalBounds = buttonList->at(button_tag::yarrow).get_global_bounds();
//	arrowToPageGlobalBounds = buttonList->at(button_tag::nextPage).get_global_bounds();
//	leftPagePosition = sf::Vector2f(pageGlobalBounds.left, pageGlobalBounds.top);
//	rightPagePosition = sf::Vector2f(pageGlobalBounds.left + pageGlobalBounds.width / 2.0f, pageGlobalBounds.top);
//
//	setArrowsPosition();
//}
//
//void hero_book_page::buttonListBounding(std::unordered_map<button_tag, button>* buttonList)
//{
//	this->buttonList = buttonList;
//}
//
//void hero_book_page::putHeadingsToPositions(std::vector<button_tag> buttons)
//{
//	const auto upperLeftCorner = buttonList->at(button_tag::bookmarksList).get_position();
//	const auto x = buttonList->at(button_tag::bookmarksList).get_global_bounds().width;
//	const auto y = buttonList->at(button_tag::bookmarksList).get_global_bounds().height;
//
//	for (auto i = 0u; i < buttons.size(); i++)
//	{
//		if (i >= headingPedestals.size())
//			return;
//		buttonList->at(buttons[i]).set_position(sf::Vector2f(headingPedestals[i].x * x + upperLeftCorner.x, headingPedestals[i].y * y + upperLeftCorner.y));
//	}
//}
//
//void hero_book_page::initAuxiliarySpriteMap()
//{
//	const auto screenSize = world_metrics::window_size;
//	std::string buttonImagePathDefault;
//	sf::Vector2f buttonPosition, buttonSize; // in percents
//	sf::Vector2f offset = { 0, 0 };
//	int tag;
//
//	std::ifstream fin("Game/heroBook/auxiliarySprites.txt");
//
//	while (fin >> buttonImagePathDefault)
//	{		
//		fin >> buttonPosition.x >> buttonPosition.y >> buttonSize.y >> offset.x >> offset.y >> tag;
//
//		Texture buttonTextureDefault;
//		buttonTextureDefault.loadFromFile(buttonImagePathDefault);
//
//		buttonPosition.x = buttonPosition.x * screenSize.x / 100;
//		buttonPosition.y = buttonPosition.y * screenSize.y / 100;
//		buttonSize.y = buttonSize.y * screenSize.y / 100;
//		buttonSize.x = buttonTextureDefault.getSize().x * buttonSize.y / buttonTextureDefault.getSize().y;		
//
//		auxiliarySpriteMap[auxiliary_sprite_tag(tag)].texture.loadFromFile(buttonImagePathDefault);
//		const auto textureSize = sf::Vector2f(auxiliarySpriteMap[auxiliary_sprite_tag(tag)].texture.getSize());
//
//		auxiliarySpriteMap[auxiliary_sprite_tag(tag)].sprite.setTexture(auxiliarySpriteMap[auxiliary_sprite_tag(tag)].texture);
//		auxiliarySpriteMap[auxiliary_sprite_tag(tag)].sprite.setScale(buttonSize.x / textureSize.x, buttonSize.y / textureSize.y);		
//	}
//
//	fin.close();
//}
//
//// object information (creatures, craft, plants, wreaths, nightmare)
//void hero_book_page::initObjectInfo()
//{
//	int pageNumber;
//	int blocksCount;
//	int curConnection;
//	int pedestal;
//	auto isUnlocked = false;
//	char blockDescription[1000], objectDescription[1000];
//	
//	std::ifstream fin("Game/bookContent.txt");
//
//	while (fin >> pageNumber >> isUnlocked >> blocksCount)
//	{
//		for (auto cnt = 0; cnt < blocksCount; cnt++)
//		{
//			fin >> curConnection >> pedestal;
//			fin.getline(blockDescription, sizeof blockDescription);
//			objectInfo[pageToObjectId(pageNumber)].connections.emplace_back(curConnection, pedestal, blockDescription);
//		}
//		fin.getline(objectDescription, sizeof objectDescription);
//		objectInfo[pageToObjectId(pageNumber)].textDescription = objectDescription;
//		objectInfo[pageToObjectId(pageNumber)].tag = pageToObjectId(pageNumber);
//		objectInfo[pageToObjectId(pageNumber)].isUnlocked = isUnlocked;
//	}
//
//	fin.close();
//}
//
//Vector2f hero_book_page::getConnectionPosition(int numberInOrder)
//{
//	const auto upperLeft = sf::Vector2f(buttonList->at(button_tag::pagePattern).get_global_bounds().left, buttonList->at(button_tag::pagePattern).get_global_bounds().top);
//	const auto pageSize = sf::Vector2f(buttonList->at(button_tag::pagePattern).get_global_bounds().width, buttonList->at(button_tag::pagePattern).get_global_bounds().height);
//	const auto frameOffset = buttonList->at(button_tag::iconFrame2).get_global_bounds().height / 2;
//	return { upperLeft.x + connectionPedestals[numberInOrder].x * pageSize.x - frameOffset, upperLeft.y + connectionPedestals[numberInOrder].y * pageSize.y - frameOffset };
//}
//
//entity_tag hero_book_page::pageToObjectId(int page)
//{
//	if (page < 101 || page > 499)
//		return entity_tag::emptyPage;
//	return entity_tag(page);
//}
//
//int hero_book_page::buttonToPage(button_tag button)
//{
//	if (int(button) >= 101 && int(button) <= 499)
//	{
//		if (objectInfo.find(entity_tag(int(button))) != objectInfo.end())		
//			if (objectInfo.at(entity_tag(int(button))).isUnlocked)
//				return int(button);
//		return -1;
//	}
//	if (button == button_tag::nextPage && currentPage < 999)
//		return currentPage + 1;
//	if (button == button_tag::previousPage && currentPage > 0)
//		return currentPage - 1;
//	if (button == button_tag::bookmarkMobs)
//		return 1;
//	if (button == button_tag::bookmarkItems)
//		return 2;
//	if (button == button_tag::bookmarkWorld)
//		return 3;
//	if (button == button_tag::bookmarkFlowers)
//		return 4;
//	if (button == button_tag::bookmarkWreathes)
//		return 5;
//	return -1;
//}
//
//std::vector<text_chain_element*> hero_book_page::prepareHeadingText()
//{
//	std::vector<text_chain_element*> result = {};
//	const auto upperLeftCorner = buttonList->at(button_tag::bookmarksList).get_position();
//	const auto x = buttonList->at(button_tag::bookmarksList).get_global_bounds().width;
//	const auto y = buttonList->at(button_tag::bookmarksList).get_global_bounds().height;
//
//	const auto textColor = sf::Color(100, 68, 34, 180);
//
//	switch (currentPage)
//	{
//	case 1:
//	{
//		result.push_back(new text_chain_element({ 0.2f * x + upperLeftCorner.x, 0.125f * y + upperLeftCorner.y }, { 0, 0 }, textColor, "Creatures", text_chain_element::default_character_size * 1.2f));
//		result.push_back(new text_chain_element({ headingTextPedestals[0].x * x + upperLeftCorner.x, headingTextPedestals[0].y * y + upperLeftCorner.y }, { 0, 0 }, textColor, "Hare"));
//		result.push_back(new text_chain_element({ headingTextPedestals[1].x * x + upperLeftCorner.x, headingTextPedestals[1].y * y + upperLeftCorner.y }, { 0, 0 }, textColor, "Owl"));
//		result.push_back(new text_chain_element({ headingTextPedestals[2].x * x + upperLeftCorner.x, headingTextPedestals[2].y * y + upperLeftCorner.y }, { 0, 0 }, textColor, "Deer"));
//		result.push_back(new text_chain_element({ headingTextPedestals[3].x * x + upperLeftCorner.x, headingTextPedestals[3].y * y + upperLeftCorner.y }, { 0, 0 }, textColor, "Bear"));
//	}
//	break;
//	case 2:
//	{
//		result.push_back(new text_chain_element({ 0.2f * x + upperLeftCorner.x, 0.125f * y + upperLeftCorner.y }, { 0, 0 }, textColor, "Items", text_chain_element::default_character_size * 1.2f));
//		result.push_back(new text_chain_element({ headingTextPedestals[0].x * x + upperLeftCorner.x, headingTextPedestals[0].y * y + upperLeftCorner.y }, { 0, 0 }, textColor, "Bag"));
//	}
//	break;
//	case 3:
//	{
//		result.push_back(new text_chain_element({ 0.2f * x + upperLeftCorner.x, 0.125f * y + upperLeftCorner.y }, { 0, 0 }, textColor, "World", text_chain_element::default_character_size * 1.2f));
//	}
//	break;
//	case 4:
//	{
//		result.push_back(new text_chain_element({ 0.2f * x + upperLeftCorner.x, 0.125f * y + upperLeftCorner.y }, { 0, 0 }, textColor, "Flowers", text_chain_element::default_character_size * 1.2f));
//		result.push_back(new text_chain_element({ headingTextPedestals[0].x * x + upperLeftCorner.x, headingTextPedestals[0].y * y + upperLeftCorner.y }, { 0, 0 }, textColor, "Chamomile"));
//		result.push_back(new text_chain_element({ headingTextPedestals[1].x * x + upperLeftCorner.x, headingTextPedestals[1].y * y + upperLeftCorner.y }, { 0, 0 }, textColor, "Yarrow"));
//		result.push_back(new text_chain_element({ headingTextPedestals[2].x * x + upperLeftCorner.x, headingTextPedestals[2].y * y + upperLeftCorner.y }, { 0, 0 }, textColor, "Fern"));
//		result.push_back(new text_chain_element({ headingTextPedestals[3].x * x + upperLeftCorner.x, headingTextPedestals[3].y * y + upperLeftCorner.y }, { 0, 0 }, textColor, "Mugwort"));
//		result.push_back(new text_chain_element({ headingTextPedestals[4].x * x + upperLeftCorner.x, headingTextPedestals[4].y * y + upperLeftCorner.y }, { 0, 0 }, textColor, "Poppy"));
//	}
//	break;
//	case 5:
//	{
//		result.push_back(new text_chain_element({ 0.2f * x + upperLeftCorner.x, 0.125f * y + upperLeftCorner.y }, { 0, 0 }, textColor, "Sketching", text_chain_element::default_character_size * 1.2f));
//	}
//	break;
//	case 102:
//	case 103:
//	case 104:
//	case 201:
//	case 401:
//	case 402:
//	case 403:
//	case 404:
//	case 405:
//	{
//		result.push_back(new text_chain_element({ 0.2f * x + upperLeftCorner.x, 0.085f * y + upperLeftCorner.y }, { 0, 0 }, textColor, buttonToString(button_tag(currentPage)), text_chain_element::default_character_size * 1.2f));
//	}
//	break;
//	default:
//		break;
//	}
//
//	return result;
//}
//
//void hero_book_page::setButtonLock(button_tag button, const button_tag changedButton)
//{
//	if (!(int(button) >= 101 && int(button) <= 499 || (button == button_tag::iconFrame1 || button == button_tag::iconFrame2)))
//		return;
//
//	if (currentPage == 5)
//	{
//		buttonList->at(changedButton).stop_being_gray();
//		return;
//	}
//
//	if (objectInfo.find(entity_tag(int(button))) != objectInfo.end())
//		if (objectInfo.at(entity_tag(int(button))).isUnlocked)
//		{
//			buttonList->at(changedButton).stop_being_gray();
//			return;
//		}	
//
//	buttonList->at(changedButton).become_gray();
//}
//
//sprite_chain_element* hero_book_page::prepareIconFrame(const button_tag button, const int type)
//{	
//	button_tag currentIcon;
//	if (type == 1)
//		currentIcon = button_tag::iconFrame1;
//	else
//		currentIcon = button_tag::iconFrame2;
//
//	buttonList->at(currentIcon).set_position(buttonList->at(button).get_position());
//	setButtonLock(button, currentIcon);
//	return buttonList->at(currentIcon).prepare_sprite();
//}
//
//std::vector<sprite_chain_element*> hero_book_page::prepareAllIcons(const page_content& content)
//{
//	std::vector<sprite_chain_element*> result = {};
//
//	for (auto& item : content.buttons)
//	{
//		setButtonLock(item, item);
//		result.push_back(buttonList->at(item).prepare_sprite());
//
//		if (int(item) >= 101 && int(item) <= 499) // items with description page
//		{
//			if (currentPage >= 1 && currentPage <= 4) // pages with headings
//				result.push_back(prepareIconFrame(item, 1));
//			else
//				result.push_back(prepareIconFrame(item, 2));
//		}
//	}
//
//	return result;
//}
//
//std::vector<sprite_chain_element*> hero_book_page::prepareLines()
//{
//	if (!(currentPage >= 101 && currentPage <= 499))
//		return {};
//
//	if (objectInfo.find(pageToObjectId(currentPage)) == objectInfo.end())
//		return {};
//
//	std::vector<sprite_chain_element*> result = {};
//
//	/*auto upperFramePos = getConnectionPosition(0);
//	
//	const auto frameOffset = buttonList->at(ButtonTag::iconFrame2).getGlobalBounds().height / 2;
//	upperFramePos.x += frameOffset; upperFramePos.y += frameOffset;
//
//	for (auto& connection : objectInfo.at(pageToObjectId(currentPage)).connections)
//	{
//		if (connection.pedestal == 0)
//			continue;
//
//		if (connection.pedestal <= 4) // left-oriented lines
//			auxiliarySpriteMap.at(AuxiliarySpriteTag(connection.pedestal)).sprite.setPosition(upperFramePos.x - auxiliarySpriteMap.at(AuxiliarySpriteTag(connection.pedestal)).sprite.getGlobalBounds().width, upperFramePos.y);
//		else
//			auxiliarySpriteMap.at(AuxiliarySpriteTag(connection.pedestal)).sprite.setPosition(upperFramePos);
//
//		if (!objectInfo[pageToObjectId(connection.id)].isUnlocked)
//		{
//			auxiliarySpriteMap.at(AuxiliarySpriteTag(connection.pedestal)).sprite.setColor(sf::Color(255, 255, 255, 100));
//			result.push_back(auxiliarySpriteMap.at(AuxiliarySpriteTag(connection.pedestal)).sprite);
//			auxiliarySpriteMap.at(AuxiliarySpriteTag(connection.pedestal)).sprite.setColor(sf::Color(255, 255, 255, 255));
//		}
//		else
//			window->draw(auxiliarySpriteMap.at(AuxiliarySpriteTag(connection.pedestal)).sprite);
//	}*/
//
//	return result;
//}
//
//void hero_book_page::unlockObject(entity_tag object)
//{
//	objectInfo[object].isUnlocked = true;
//}
//
//int hero_book_page::getHeadingPage(entity_tag object)
//{
//	if (!(int(object) >= 101 && int(object) <= 499))
//		return -1;
//
//	return int(object) / 100;
//}
//
//std::string hero_book_page::buttonToString(const button_tag button)
//{
//	switch (button)
//	{
//	case button_tag::hare:
//		return "Hare";
//	case button_tag::owl:
//		return "Owl";
//	case button_tag::deer:
//		return "Deer";
//	case button_tag::fox:
//		return "Fox";
//	case button_tag::bear:
//		return "Bear";
//	case button_tag::chamomile:
//		return "Chamomile";
//	case button_tag::yarrow:
//		return "Yarrow";
//	case button_tag::fern:
//		return "Fern";
//	case button_tag::mugwort:
//		return "Mugwort";
//	case button_tag::poppy:
//		return "Poppy";
//	case button_tag::heroBag:
//		return "Bag";
//	default: return "Info";
//	}	
//}
////------------------------------------------------------------------
//
//// wreaths
//void hero_book_page::initDrafts()
//{
//	int draftId;
//	int ringsCount;
//	int plantsCount;
//	
//	std::ifstream fin("Game/drafts.txt");
//	
//	while (fin >> draftId >> ringsCount >> plantsCount)
//	{
//		originalSetups[entity_tag(draftId)].id = entity_tag(draftId);
//
//		for (auto i = 0; i < ringsCount; i++)
//		{
//			int ring;
//			fin >> ring;
//			originalSetups[entity_tag(draftId)].rings.push_back(ring);
//		}
//
//		for (auto i = 0; i < plantsCount; i++)
//		{
//			int raw, column, plant;
//			fin >> raw >> column >> plant;
//			originalSetups[entity_tag(draftId)].plants.emplace_back(entity_tag(plant), std::make_pair(raw, column));
//		}
//	}
//
//	fin.close();
//}
//
//entity_tag hero_book_page::tagToWreath(entity_tag item)
//{
//	switch (item)
//	{
//	case entity_tag::hare:
//		return entity_tag::hareWreath;
//	case entity_tag::owl:
//		return entity_tag::owlWreath;
//	default:
//		return entity_tag::emptyCell;
//	}	
//}
//
//bool hero_book_page::checkWreathCellFit(int i, int j, const std::vector<int>& rings)
//{
//	//all rings
//	if ((i == 0 || i == 16) && (j == 0 || j == 1 || j == 3 || j == 4))
//		return false;
//	if ((i == 1 || i == 15) && (j == 0 || j == 3 || j == 4))
//		return false;
//	if ((i == 2 || i == 14) && (j == 0 || j == 4))
//		return false;
//	if ((i == 3 || i == 5 || i == 7 || i == 9 || i == 11 || i == 13) && j == 4)
//		return false;
//	if (i == 8 && j == 2)
//		return false;
//
//	//if (rings.empty())
//		//return true;
//
//	std::vector<int> invertedRings = { 1, 2, 3, 4 };
//	for (auto& ring : rings)
//	{
//		for (auto cnt = 0u; cnt < invertedRings.size(); cnt++)
//			if (ring == invertedRings[cnt])
//				invertedRings.erase(invertedRings.begin() + cnt);
//	}
//
//	std::sort(invertedRings.begin(), invertedRings.end());
//
//	for (auto invertedRing : invertedRings)
//	{
//		switch (invertedRing)
//		{
//		case 1:
//			if (i == 4 && j == 0 || i == 3 && j == 0 || i == 2 && j == 1 || i == 1 && j == 1 || i == 0 && j == 2 || i == 1 && j == 2 || i == 2 && j == 3 || i == 3 && j == 3 ||
//				i == 4 && j == 4 || i == 4 && j == 4 || i == 6 && j == 4 || i == 8 && j == 4 || i == 10 && j == 4 || i == 12 && j == 4 ||
//				i == 13 && j == 3 || i == 14 && j == 3 || i == 15 && j == 2 || i == 16 && j == 2 || i == 15 && j == 1 || i == 14 && j == 1 || i == 13 && j == 0 || i == 12 && j == 0 ||
//				i == 10 && j == 0 || i == 8 && j == 0 || i == 6 && j == 0)
//				return false;
//			break;
//		case 2:
//			if (i == 5 && j == 0 || i == 4 && j == 1 || i == 3 && j == 1 || i == 2 && j == 2 || i == 3 && j == 2 || i == 4 && j == 3 || i == 5 && j == 3 || i == 7 && j == 3 || i == 9 && j == 3 ||
//				i == 11 && j == 3 || i == 12 && j == 3 || i == 13 && j == 2 || i == 14 && j == 2 || i == 13 && j == 1 || i == 12 && j == 1 || i == 11 && j == 0 || i == 9 && j == 0 || i == 7 && j == 0)
//				return false;
//			break;
//		case 3:
//			if (i == 6 && j == 1 || i == 5 && j == 1 || i == 4 && j == 2 || i == 5 && j == 2 || i == 6 && j == 3 || i == 8 && j == 3 || i == 10 && j == 3 || i == 11 && j == 2 || i == 12 && j == 2 ||
//				i == 11 && j == 1 || i == 10 && j == 1 || i == 8 && j == 1)
//				return false;
//			break;
//		case 4:
//			if (i == 7 && j == 1 || i == 6 && j == 2 || i == 7 && j == 2 || i == 9 && j == 2 || i == 10 && j == 2 || i == 9 && j == 1)
//				return false;
//			break;
//		default:
//			return false;
//		}
//	}
//	return true;
//}
//
//std::pair<int, int> hero_book_page::getSelectedWreathCell()
//{
//	const auto mousePos = sf::Vector2f(sf::Mouse::getPosition());
//	auto minD = float(10e4);
//	auto ans = std::make_pair(-1, -1);
//	for (auto cnt1 = 0u; cnt1 < wreathMatrix.size(); cnt1++)
//		for (auto cnt2 = 0u; cnt2 < wreathMatrix[cnt1].size(); cnt2++)
//		{
//			if (wreathMatrix[cnt1][cnt2] == entity_tag::selectedCell)
//				wreathMatrix[cnt1][cnt2] = entity_tag::emptyCell;
//			const auto curPos = sf::Vector2f(wreathMatrixPositions[cnt1][cnt2].x + buttonList->at(button_tag::cell).get_global_bounds().width / 2.0f,
//			                             wreathMatrixPositions[cnt1][cnt2].y + buttonList->at(button_tag::cell).get_global_bounds().height / 2.0f);
//			if (helper::getDist(sf::Vector2f(mousePos), curPos) < minD &&
//				helper::getDist(sf::Vector2f(mousePos), curPos) <= buttonList->at(button_tag::cell).get_global_bounds().width / 2)
//			{
//				minD = helper::getDist(sf::Vector2f(mousePos), curPos);
//				ans = std::make_pair(cnt1, cnt2);
//			}
//		}
//
//	return ans;
//}
//
//std::pair<int, int> hero_book_page::getSelectedPlantsCell()
//{
//	const auto mousePos = sf::Vector2f(sf::Mouse::getPosition());
//	const auto upperLeftCorner = sf::Vector2f(
//		buttonList->at(button_tag::sketching).get_global_bounds().left + buttonList->at(button_tag::sketching).get_global_bounds().width * 0.066f,
//		buttonList->at(button_tag::sketching).get_global_bounds().top + buttonList->at(button_tag::sketching).get_global_bounds().height * 0.55f);
//
//	const auto size = sf::Vector2f(buttonList->at(button_tag::chamomile).get_global_bounds().width,
//	                           buttonList->at(button_tag::chamomile).get_global_bounds().height);
//
//	if (mousePos.x < upperLeftCorner.x || mousePos.y < upperLeftCorner.y ||
//		mousePos.x > upperLeftCorner.x + size.x * plantsMatrix[0].size() || mousePos.y > upperLeftCorner.y + size.y * plantsMatrix.size())
//		return { -1, -1 };
//
//	std::pair<int, int> ans = std::make_pair(int((mousePos.y - upperLeftCorner.y) / size.y),
//	                                         int((mousePos.x - upperLeftCorner.x) / size.x));
//
//	if (plantsMatrix[ans.first][ans.second].first == entity_tag::emptyCell || plantsMatrix[ans.first][ans.second].second == 0)
//		return { -1, -1 };
//
//	return ans;
//}
//
//std::vector<std::pair<int, int>> hero_book_page::getBorderCells(const int raw, int column)
//{
//	std::vector<std::pair<int, int>> ans;
//	
//	if (raw == -1 || column == -1)
//		return {};
//	
//	if (raw % 2 == 0)	
//		ans = { {raw - 2, column}, {raw - 1, column}, {raw + 1, column}, {raw + 2, column}, {raw + 1, column - 1}, {raw - 1, column - 1} };
//	else
//		ans = { {raw - 2, column}, {raw - 1, column + 1}, {raw + 1, column + 1}, {raw + 2, column}, {raw + 1, column}, {raw - 1, column} };
//
//	while (true)
//	{
//		auto isBreak = true;
//		for (auto i = 0u; i < ans.size(); i++)
//			if (ans[i].first < 0 || ans[i].second < 0 || unsigned(ans[i].first) >= wreathMatrix.size() || unsigned(ans[i].second) >= wreathMatrix[0].size())
//			{
//				ans.erase(ans.begin() + i);
//				isBreak = false;
//			}
//		if (isBreak)
//			break;
//	}
//
//	return ans;
//}
//
//void hero_book_page::clearWreathMatrix()
//{
//	for (auto& raw : wreathMatrix)
//		for (auto column = 0u; column < wreathMatrix[0].size(); column++)
//			raw[column] = entity_tag::emptyCell;	
//}
//
//void hero_book_page::setPlantsOnMatrix(const std::vector<std::pair<entity_tag, std::pair<int, int>>>& plants)
//{
//	clearWreathMatrix();
//	for (auto& plant : plants)
//		wreathMatrix[plant.second.first][plant.second.second] = plant.first;
//}
//
//void hero_book_page::coloredDfs(int raw, int column, int color, bool flowerPatency)
//{
//	coloredMatrix[raw][column] = color;
//	auto ways = getBorderCells(raw, column);
//	
//	for (const auto way : ways)
//	{
//		if (!flowerPatency)
//		{
//			if (coloredMatrix[way.first][way.second] == 0 && (wreathMatrix[raw][column] == entity_tag::emptyCell || wreathMatrix[way.first][way.second] == entity_tag::selectedCell))
//				coloredDfs(way.first, way.second, color, flowerPatency);
//		}
//		else
//		{
//			if (coloredMatrix[way.first][way.second] == 1 && (wreathMatrix[raw][column] == entity_tag::emptyCell || wreathMatrix[way.first][way.second] == entity_tag::selectedCell) ||
//				coloredMatrix[way.first][way.second] == 0)
//				coloredDfs(way.first, way.second, color, flowerPatency);
//		}
//	}
//}
//
//bool hero_book_page::isCenterSurrounded()
//{
//	for (auto& raw : coloredMatrix)
//		for (auto& column : raw)
//			column = 0;
//
//	const std::pair<int, int> center = { 8, 2 };
//	std::vector<std::pair<int, int>> corners = { {0, 0}, {0, 4}, {16, 4}, {16, 0} };
//
//	coloredDfs(center.first, center.second, 1);
//	for (const auto corner : corners)
//		coloredDfs(corner.first, corner.second, 2, true);
//
//	return coloredMatrix[center.first][center.second] == 1;
//}
//
//std::vector<drawable_chain_element*> hero_book_page::prepareConnectableFlowers(entity_tag currentFlower)
//{
//	if (plantsConnections.find(entity_tag(int(currentFlower))) == plantsConnections.end())
//		return {};	
//
//	std::vector<drawable_chain_element*> result = {};
//
//	const auto upperLeftCorner = sf::Vector2f(buttonList->at(button_tag::sketching).get_global_bounds().left, buttonList->at(button_tag::sketching).get_global_bounds().top);
//	const auto pageSize = sf::Vector2f(buttonList->at(button_tag::sketching).get_global_bounds().width, buttonList->at(button_tag::sketching).get_global_bounds().height);
//	const auto headingPos = sf::Vector2f(upperLeftCorner.x + 0.072f * pageSize.x, upperLeftCorner.y + 0.28f * pageSize.y);
//
//	result.push_back(new text_chain_element({ headingPos.x, headingPos.y }, { 0, 0 }, sf::Color(100, 68, 34, 180), "Connected with:"));
//
//	auto cnt = 0;
//	for (auto& connection : plantsConnections.at(entity_tag(int(currentFlower))))
//	{
//		if (buttonList->find(button_tag(int(connection))) == buttonList->end())
//			continue;
//		buttonList->at(button_tag(int(connection))).set_position(sf::Vector2f(upperLeftCorner.x + flowerConnectionsPedestals[cnt].x * pageSize.x,
//			upperLeftCorner.y + flowerConnectionsPedestals[cnt].y * pageSize.y));
//		result.push_back(buttonList->at(button_tag(int(connection))).prepare_sprite());
//		cnt++;
//	}
//
//	return result;
//}
//
////--------
//
//page_content hero_book_page::getPreparedContent(int pageNumber, entity_tag currentDraft)
//{
//	page_content result;
//
//	preparePageBase();
//	setBookmarkPosition();
//	setArrowsPosition();
//
//	const auto bookmarks = { button_tag::bookmarkMobs, button_tag::bookmarkItems, button_tag::bookmarkWorld, button_tag::bookmarkFlowers, button_tag::bookmarkWreathes, button_tag::bookmarkNightmare };
//	result.buttons.insert(result.buttons.end(), bookmarks.begin(), bookmarks.end());
//
//	switch (currentPage)
//	{
//	case 0:
//		result.buttons.push_back(button_tag::bookCover);
//		break;
//	// items lists
//	case 1:
//	case 2:
//	case 3:
//	case 4:
//	{
//		std::vector<button_tag> blocks;
//		if (currentPage == 1)
//			blocks = { button_tag::hare, button_tag::owl, button_tag::deer, button_tag::bear };
//		if (currentPage == 2)
//			blocks = { button_tag::heroBag };
//		if (currentPage == 4)
//			blocks = { button_tag::chamomile, button_tag::yarrow, button_tag::fern, button_tag::mugwort, button_tag::poppy };
//
//		putHeadingsToPositions(blocks);
//		result.buttons.insert(result.buttons.begin(), blocks.begin(), blocks.end());
//		result.buttons.push_back(button_tag::bookmarksList);
//	}
//	break;
//	//------------
//	// sketching page
//	case 5:
//	{		
//		result.buttons.push_back(button_tag::sketching);
//		if (readyToFinishDraft)
//		{
//			const auto plusPos = sf::Vector2f(wreathMatrixPositions[8][2].x - (buttonList->at(button_tag::plus).get_global_bounds().width - buttonList->at(button_tag::cell).get_global_bounds().width) / 2,
//			                              wreathMatrixPositions[8][2].y - (buttonList->at(button_tag::plus).get_global_bounds().height - buttonList->at(button_tag::cell).get_global_bounds().height) / 2);
//			buttonList->at(button_tag::plus).set_position(plusPos);
//			result.buttons.push_back(button_tag::plus);
//		}
//	}	
//	break;
//	//---------------
//	// description of each object
//	case 102:
//	case 103:
//	case 402:
//		result.buttons.push_back(button_tag::pagePattern);
//		//if (objectInfo.count(pageToObjectId(currentPage)) > 0 && objectInfo.at(pageToObjectId(currentPage)).isUnlocked)
//			result.buttons.push_back(button_tag::makeWreath);
//
//		// setting connections icons
//		for (auto& connection : objectInfo.at(pageToObjectId(currentPage)).connections)
//		{
//			result.buttons.push_back(button_tag(connection.id));
//			buttonList->at(button_tag(connection.id)).set_position(getConnectionPosition(connection.pedestal));
//			
//			if (buttonList->at(button_tag(connection.id)).is_selected(sf::Vector2f(sf::Mouse::getPosition())))
//				result.blockDescription = connection.description;
//		}
//		//--------------------------
//
//		if (objectInfo.find(pageToObjectId(currentPage)) != objectInfo.end())
//			result.pageDescription = objectInfo.at(pageToObjectId(currentPage)).textDescription;
//		break;
//	//---------------------------
//	default:
//		result.buttons.push_back(button_tag::pagePattern);		
//	}	
//
//	// turn on result buttons
//	for (auto& button : result.buttons)	
//		buttonList->at(button).is_active = true;	
//	//-----------------------------
//	return  result;
//}