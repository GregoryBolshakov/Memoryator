#include "HeroBookPage.h"
#include "Helper.h"
#include "GridList.h"

HeroBookPage::HeroBookPage()
{
	initDrafts();

	wreathMatrix.resize(17, std::vector<Tag>(5));
	wreathMatrixPositions.resize(17, std::vector<Vector2f>(5));
	coloredMatrix.resize(17, std::vector<int>(5));
	plantsMatrix.resize(2, std::vector<std::pair<Tag, int>>(3));
	for (auto& raw : wreathMatrix)
		for (auto& cell : raw)
			cell = Tag::emptyCell;

	plantsMatrix[0][0] = { Tag::chamomile, 1 };
	plantsMatrix[0][1] = { Tag::yarrow, 1 };
	plantsMatrix[0][2] = { Tag::fern, 1 };
	plantsMatrix[1][0] = { Tag::mugwort, 1 };
	plantsMatrix[1][1] = { Tag::poppy, 1 };
	plantsMatrix[1][2] = { Tag::emptyCell, 0 };

	plantsConnections[Tag::chamomile] = { Tag::poppy, Tag::yarrow };
	plantsConnections[Tag::yarrow] = { Tag::chamomile, Tag::fern };
	plantsConnections[Tag::fern] = { Tag::yarrow, Tag::mugwort };
	plantsConnections[Tag::mugwort] = { Tag::fern, Tag::poppy };
	plantsConnections[Tag::poppy] = { Tag::mugwort, Tag::chamomile };
}

HeroBookPage::~HeroBookPage()
= default;

void HeroBookPage::setBookmarkPosition() const
{
	FloatRect pageGlobalBounds, bookmarkGlobalBounds = buttonList->at(ButtonTag::bookmarkMobs).getGlobalBounds();
	if (currentPage == 0)
		pageGlobalBounds = buttonList->at(ButtonTag::bookCover).getGlobalBounds();
	else
		pageGlobalBounds = buttonList->at(ButtonTag::pageBackground).getGlobalBounds();

	float bookmarkApproximateX;
	if (currentPage == 0)
		bookmarkApproximateX = pageGlobalBounds.left + pageGlobalBounds.width * 8.9 / 10;
	else
		bookmarkApproximateX = pageGlobalBounds.left + pageGlobalBounds.width * 9.38 / 10;
	buttonList->at(ButtonTag::bookmarkMobs).setPosition(Vector2f(bookmarkApproximateX, pageGlobalBounds.top + pageGlobalBounds.height * 1.2 / 10));
	buttonList->at(ButtonTag::bookmarkItems).setPosition(Vector2f(bookmarkApproximateX, pageGlobalBounds.top + pageGlobalBounds.height * 2.2 / 10));
	buttonList->at(ButtonTag::bookmarkWorld).setPosition(Vector2f(bookmarkApproximateX, pageGlobalBounds.top + pageGlobalBounds.height * 3.2 / 10));
	buttonList->at(ButtonTag::bookmarkFlowers).setPosition(Vector2f(bookmarkApproximateX, pageGlobalBounds.top + pageGlobalBounds.height * 4.2 / 10));
	buttonList->at(ButtonTag::bookmarkWreathes).setPosition(Vector2f(bookmarkApproximateX, pageGlobalBounds.top + pageGlobalBounds.height * 5.2 / 10));
	buttonList->at(ButtonTag::bookmarkNightmare).setPosition(Vector2f(bookmarkApproximateX, pageGlobalBounds.top + pageGlobalBounds.height * 7.2 / 10));
	buttonList->at(ButtonTag::plus).setPosition(Vector2f(bookmarkApproximateX, pageGlobalBounds.top + pageGlobalBounds.height * 8.2 / 10));
	buttonList->at(ButtonTag::makeWreath).setPosition(Vector2f(pageGlobalBounds.left + pageGlobalBounds.width * 1.35 / 10, pageGlobalBounds.top + pageGlobalBounds.height * 6.6 / 10));
}

void HeroBookPage::setArrowsPosition() const
{
	const FloatRect pagePatternGlobalBounds = buttonList->at(ButtonTag::pagePattern).getGlobalBounds();
	Vector2f nextPagePos = Vector2f(pagePatternGlobalBounds.left + pagePatternGlobalBounds.width - arrowToPageGlobalBounds.width,
		pagePatternGlobalBounds.top + pagePatternGlobalBounds.height - arrowToPageGlobalBounds.height);
	const Vector2f previousPagePos = Vector2f(Vector2f(pagePatternGlobalBounds.left,
		pagePatternGlobalBounds.top + pagePatternGlobalBounds.height - arrowToPageGlobalBounds.height));
	
	if (currentPage == 0)
		nextPagePos.x -= buttonList->at(ButtonTag::bookmarksList).getGlobalBounds().width / 2;

	buttonList->at(ButtonTag::nextPage).setPosition(nextPagePos);
	buttonList->at(ButtonTag::previousPage).setPosition(previousPagePos);	
}

void HeroBookPage::preparePageBase()
{
	buttonList->at(ButtonTag::pageBackground).setPosition(Vector2f(buttonList->at(ButtonTag::bookCover).getPosition().x - buttonList->at(ButtonTag::bookCover).getGlobalBounds().width,
		buttonList->at(ButtonTag::bookCover).getPosition().y));

	if (currentPage == 0)
		pageGlobalBounds = buttonList->at(ButtonTag::bookCover).getGlobalBounds();
	else
		pageGlobalBounds = buttonList->at(ButtonTag::pageBackground).getGlobalBounds();

	buttonList->at(ButtonTag::pagePattern).setPosition(Vector2f(pageGlobalBounds.left + pageGlobalBounds.width * 1 / 13, pageGlobalBounds.top + pageGlobalBounds.height * 1 / 21));	
	buttonList->at(ButtonTag::bookmarksList).setPosition(Vector2f(pageGlobalBounds.left + pageGlobalBounds.width * 1 / 13, pageGlobalBounds.top + pageGlobalBounds.height * 1 / 21));	
	buttonList->at(ButtonTag::sketching).setPosition(Vector2f(pageGlobalBounds.left + pageGlobalBounds.width * 1 / 13, pageGlobalBounds.top + pageGlobalBounds.height * 1 / 21));	

	blockGlobalBounds = buttonList->at(ButtonTag::yarrow).getGlobalBounds();
	arrowToPageGlobalBounds = buttonList->at(ButtonTag::nextPage).getGlobalBounds();
	leftPagePosition = Vector2f(pageGlobalBounds.left, pageGlobalBounds.top);
	rightPagePosition = Vector2f(pageGlobalBounds.left + pageGlobalBounds.width / 2, pageGlobalBounds.top);

	setArrowsPosition();
}

void HeroBookPage::buttonListBounding(std::unordered_map<ButtonTag, Button>* buttonList)
{
	this->buttonList = buttonList;
}

void HeroBookPage::putHeadingsToPositions(std::vector<ButtonTag> buttons)
{
	const Vector2f upperLeftCorner = buttonList->at(ButtonTag::bookmarksList).getPosition();
	const float x = buttonList->at(ButtonTag::bookmarksList).getGlobalBounds().width;
	const float y = buttonList->at(ButtonTag::bookmarksList).getGlobalBounds().height;

	for (int i = 0; i < buttons.size(); i++)
	{
		if (i >= headingPedestals.size())
			return;
		buttonList->at(buttons[i]).setPosition(Vector2f(headingPedestals[i].x * x + upperLeftCorner.x, headingPedestals[i].y * y + upperLeftCorner.y));
	}
}

void HeroBookPage::initAuxiliarySpriteMap()
{
	const Vector2f screenSize = Helper::GetScreenSize();
	std::string buttonImagePathDefault;
	Vector2f buttonPosition, buttonSize; // in percents
	Vector2f offset = { 0, 0 };
	int tag;

	std::ifstream fin("Game/heroBook/auxiliarySprites.txt");

	while (fin >> buttonImagePathDefault)
	{		
		fin >> buttonPosition.x >> buttonPosition.y >> buttonSize.y >> offset.x >> offset.y >> tag;

		Texture buttonTextureDefault;
		buttonTextureDefault.loadFromFile(buttonImagePathDefault);

		buttonPosition.x = buttonPosition.x * screenSize.x / 100;
		buttonPosition.y = buttonPosition.y * screenSize.y / 100;
		buttonSize.y = buttonSize.y * screenSize.y / 100;
		buttonSize.x = buttonTextureDefault.getSize().x * buttonSize.y / buttonTextureDefault.getSize().y;		

		auxiliarySpriteMap[AuxiliarySpriteTag(tag)].texture.loadFromFile(buttonImagePathDefault);
		const Vector2f textureSize = Vector2f(auxiliarySpriteMap[AuxiliarySpriteTag(tag)].texture.getSize());

		auxiliarySpriteMap[AuxiliarySpriteTag(tag)].sprite.setTexture(auxiliarySpriteMap[AuxiliarySpriteTag(tag)].texture);
		auxiliarySpriteMap[AuxiliarySpriteTag(tag)].sprite.setScale(buttonSize.x / textureSize.x, buttonSize.y / textureSize.y);		
	}

	fin.close();
}

// object information (creatures, craft, plants, wreaths, nightmare)
void HeroBookPage::initObjectInfo()
{
	int pageNumber, blocksCount, curConnection, pedestal;
	bool isUnlocked = false;
	char blockDescription[1000], objectDescription[1000];
	std::ifstream fin("Game/bookContent.txt");
	while (fin >> pageNumber >> isUnlocked >> blocksCount)
	{
		for (int cnt = 0; cnt < blocksCount; cnt++)
		{
			fin >> curConnection >> pedestal;
			fin.getline(blockDescription, sizeof(blockDescription));
			objectInfo[pageToObjectId(pageNumber)].connections.emplace_back(curConnection, pedestal, blockDescription);
		}
		fin.getline(objectDescription, sizeof(objectDescription));
		objectInfo[pageToObjectId(pageNumber)].textDescription = objectDescription;
		objectInfo[pageToObjectId(pageNumber)].tag = pageToObjectId(pageNumber);
		objectInfo[pageToObjectId(pageNumber)].isUnlocked = isUnlocked;
	}

	fin.close();
}

Vector2f HeroBookPage::getConnectionPosition(int numberInOrder)
{
	const Vector2f upperLeft = Vector2f(buttonList->at(ButtonTag::pagePattern).getGlobalBounds().left, buttonList->at(ButtonTag::pagePattern).getGlobalBounds().top);
	const Vector2f pageSize = Vector2f(buttonList->at(ButtonTag::pagePattern).getGlobalBounds().width, buttonList->at(ButtonTag::pagePattern).getGlobalBounds().height);
	const float frameOffset = buttonList->at(ButtonTag::iconFrame2).getGlobalBounds().height / 2;
	return { upperLeft.x + connectionPedestals[numberInOrder].x * pageSize.x - frameOffset, upperLeft.y + connectionPedestals[numberInOrder].y * pageSize.y - frameOffset };
}

Tag HeroBookPage::pageToObjectId(int page)
{
	if (page < 101 || page > 499)
		return Tag::emptyPage;
	return Tag(page);
}

int HeroBookPage::buttonToPage(ButtonTag button)
{
	if (int(button) >= 101 && int(button) <= 499)
	{
		if (objectInfo.find(Tag(int(button))) != objectInfo.end())		
			if (objectInfo.at(Tag(int(button))).isUnlocked)
				return int(button);
		return -1;
	}
	if (button == ButtonTag::nextPage && currentPage < 999)
		return currentPage + 1;
	if (button == ButtonTag::previousPage && currentPage > 0)
		return currentPage - 1;
	if (button == ButtonTag::bookmarkMobs)
		return 1;
	if (button == ButtonTag::bookmarkItems)
		return 2;
	if (button == ButtonTag::bookmarkWorld)
		return 3;
	if (button == ButtonTag::bookmarkFlowers)
		return 4;
	if (button == ButtonTag::bookmarkWreathes)
		return 5;
	return -1;
}

std::vector<TextChainElement*> HeroBookPage::prepareHeadingText()
{
	std::vector<TextChainElement*> result = {};
	const Vector2f upperLeftCorner = buttonList->at(ButtonTag::bookmarksList).getPosition();
	const float x = buttonList->at(ButtonTag::bookmarksList).getGlobalBounds().width;
	const float y = buttonList->at(ButtonTag::bookmarksList).getGlobalBounds().height;

	const sf::Color textColor = sf::Color(100, 68, 34, 180);

	switch (currentPage)
	{
	case 1:
	{
		result.push_back(new TextChainElement({ 0.2f * x + upperLeftCorner.x, 0.125f * y + upperLeftCorner.y }, { 0, 0 }, textColor, "Creatures", TextChainElement::defaultCharacterSize * 1.2f));
		result.push_back(new TextChainElement({ headingTextPedestals[0].x * x + upperLeftCorner.x, headingTextPedestals[0].y * y + upperLeftCorner.y }, { 0, 0 }, textColor, "Hare"));
		result.push_back(new TextChainElement({ headingTextPedestals[1].x * x + upperLeftCorner.x, headingTextPedestals[1].y * y + upperLeftCorner.y }, { 0, 0 }, textColor, "Owl"));
		result.push_back(new TextChainElement({ headingTextPedestals[2].x * x + upperLeftCorner.x, headingTextPedestals[2].y * y + upperLeftCorner.y }, { 0, 0 }, textColor, "Deer"));
		result.push_back(new TextChainElement({ headingTextPedestals[3].x * x + upperLeftCorner.x, headingTextPedestals[3].y * y + upperLeftCorner.y }, { 0, 0 }, textColor, "Bear"));
	}
	break;
	case 2:
	{
		result.push_back(new TextChainElement({ 0.2f * x + upperLeftCorner.x, 0.125f * y + upperLeftCorner.y }, { 0, 0 }, textColor, "Items", TextChainElement::defaultCharacterSize * 1.2f));
		result.push_back(new TextChainElement({ headingTextPedestals[0].x * x + upperLeftCorner.x, headingTextPedestals[0].y * y + upperLeftCorner.y }, { 0, 0 }, textColor, "Bag"));
	}
	break;
	case 3:
	{
		result.push_back(new TextChainElement({ 0.2f * x + upperLeftCorner.x, 0.125f * y + upperLeftCorner.y }, { 0, 0 }, textColor, "World", TextChainElement::defaultCharacterSize * 1.2f));
	}
	break;
	case 4:
	{
		result.push_back(new TextChainElement({ 0.2f * x + upperLeftCorner.x, 0.125f * y + upperLeftCorner.y }, { 0, 0 }, textColor, "Flowers", TextChainElement::defaultCharacterSize * 1.2f));
		result.push_back(new TextChainElement({ headingTextPedestals[0].x * x + upperLeftCorner.x, headingTextPedestals[0].y * y + upperLeftCorner.y }, { 0, 0 }, textColor, "Chamomile"));
		result.push_back(new TextChainElement({ headingTextPedestals[1].x * x + upperLeftCorner.x, headingTextPedestals[1].y * y + upperLeftCorner.y }, { 0, 0 }, textColor, "Yarrow"));
		result.push_back(new TextChainElement({ headingTextPedestals[2].x * x + upperLeftCorner.x, headingTextPedestals[2].y * y + upperLeftCorner.y }, { 0, 0 }, textColor, "Fern"));
		result.push_back(new TextChainElement({ headingTextPedestals[3].x * x + upperLeftCorner.x, headingTextPedestals[3].y * y + upperLeftCorner.y }, { 0, 0 }, textColor, "Mugwort"));
		result.push_back(new TextChainElement({ headingTextPedestals[4].x * x + upperLeftCorner.x, headingTextPedestals[4].y * y + upperLeftCorner.y }, { 0, 0 }, textColor, "Poppy"));
	}
	break;
	case 5:
	{
		result.push_back(new TextChainElement({ 0.2f * x + upperLeftCorner.x, 0.125f * y + upperLeftCorner.y }, { 0, 0 }, textColor, "Sketching", TextChainElement::defaultCharacterSize * 1.2f));
	}
	break;
	case 102:
	case 103:
	case 104:
	case 201:
	case 401:
	case 402:
	case 403:
	case 404:
	case 405:
	{
		result.push_back(new TextChainElement({ 0.2f * x + upperLeftCorner.x, 0.085f * y + upperLeftCorner.y }, { 0, 0 }, textColor, buttonToString(ButtonTag(currentPage)), TextChainElement::defaultCharacterSize * 1.2f));
	}
	break;
	default:
		break;
	}

	return result;
}

void HeroBookPage::setButtonLock(ButtonTag button, ButtonTag changedButton)
{
	if (!((int(button) >= 101 && int(button) <= 499) || (button == ButtonTag::iconFrame1 || button == ButtonTag::iconFrame2)))
		return;

	if (currentPage == 5)
	{
		buttonList->at(changedButton).stopBeingGray();
		return;
	}

	if (objectInfo.find(Tag(int(button))) != objectInfo.end())
		if (objectInfo.at(Tag(int(button))).isUnlocked)
		{
			buttonList->at(changedButton).stopBeingGray();
			return;
		}	

	buttonList->at(changedButton).bekomeGray();
}

SpriteChainElement* HeroBookPage::prepareIconFrame(ButtonTag button, int type)
{	
	ButtonTag currentIcon;
	if (type == 1)
		currentIcon = ButtonTag::iconFrame1;
	else
		currentIcon = ButtonTag::iconFrame2;

	buttonList->at(currentIcon).setPosition(buttonList->at(button).getPosition());
	setButtonLock(button, currentIcon);
	return buttonList->at(currentIcon).prepareSprite();
}

std::vector<SpriteChainElement*> HeroBookPage::prepareAllIcons(pageContent content)
{
	std::vector<SpriteChainElement*> result = {};

	for (auto& item : content.buttons)
	{
		setButtonLock(item, item);
		result.push_back(buttonList->at(item).prepareSprite());

		if (int(item) >= 101 && int(item) <= 499) // items with description page
		{
			if (currentPage >= 1 && currentPage <= 4) // pages with headings
				result.push_back(prepareIconFrame(item, 1));
			else
				result.push_back(prepareIconFrame(item, 2));
		}
	}

	return result;
}

std::vector<SpriteChainElement*> HeroBookPage::prepareLines()
{
	if (!(currentPage >= 101 && currentPage <= 499))
		return {};

	if (objectInfo.find(pageToObjectId(currentPage)) == objectInfo.end())
		return {};

	std::vector<SpriteChainElement*> result = {};

	Vector2f upperFramePos = getConnectionPosition(0);
	
	const float frameOffset = buttonList->at(ButtonTag::iconFrame2).getGlobalBounds().height / 2;
	upperFramePos.x += frameOffset; upperFramePos.y += frameOffset;

	/*for (auto& connection : objectInfo.at(pageToObjectId(currentPage)).connections)
	{
		if (connection.pedestal == 0)
			continue;

		if (connection.pedestal <= 4) // left-oriented lines
			auxiliarySpriteMap.at(AuxiliarySpriteTag(connection.pedestal)).sprite.setPosition(upperFramePos.x - auxiliarySpriteMap.at(AuxiliarySpriteTag(connection.pedestal)).sprite.getGlobalBounds().width, upperFramePos.y);
		else
			auxiliarySpriteMap.at(AuxiliarySpriteTag(connection.pedestal)).sprite.setPosition(upperFramePos);

		if (!objectInfo[pageToObjectId(connection.id)].isUnlocked)
		{
			auxiliarySpriteMap.at(AuxiliarySpriteTag(connection.pedestal)).sprite.setColor(sf::Color(255, 255, 255, 100));
			result.push_back(auxiliarySpriteMap.at(AuxiliarySpriteTag(connection.pedestal)).sprite);
			auxiliarySpriteMap.at(AuxiliarySpriteTag(connection.pedestal)).sprite.setColor(sf::Color(255, 255, 255, 255));
		}
		else
			window->draw(auxiliarySpriteMap.at(AuxiliarySpriteTag(connection.pedestal)).sprite);
	}*/

	return result;
}

void HeroBookPage::unlockObject(Tag object)
{
	objectInfo[object].isUnlocked = true;
}

int HeroBookPage::getHeadingPage(Tag object)
{
	if (!(int(object) >= 101 && int(object) <= 499))
		return -1;

	return (int(object) / 100);
}

std::string HeroBookPage::buttonToString(ButtonTag button)
{
	switch (button)
	{
	case ButtonTag::hare:
		return "Hare";
	case ButtonTag::owl:
		return "Owl";
	case ButtonTag::deer:
		return "Deer";
	case ButtonTag::fox:
		return "Fox";
	case ButtonTag::bear:
		return "Bear";
	case ButtonTag::chamomile:
		return "Chamomile";
	case ButtonTag::yarrow:
		return "Yarrow";
	case ButtonTag::fern:
		return "Fern";
	case ButtonTag::mugwort:
		return "Mugwort";
	case ButtonTag::poppy:
		return "Poppy";
	case ButtonTag::heroBag:
		return "Bag";
	default: return "Info";
	}	
}
//------------------------------------------------------------------

// wreaths
void HeroBookPage::initDrafts()
{
	int draftId, ringsCount, plantsCount;
	std::ifstream fin("Game/drafts.txt");
	while (fin >> draftId >> ringsCount >> plantsCount)
	{
		originalSetups[Tag(draftId)].id = Tag(draftId);

		for (int i = 0; i < ringsCount; i++)
		{
			int ring;
			fin >> ring;
			originalSetups[Tag(draftId)].rings.push_back(ring);
		}

		for (int i = 0; i < plantsCount; i++)
		{
			int raw, column, plant;
			fin >> raw >> column >> plant;
			originalSetups[Tag(draftId)].plants.emplace_back(Tag(plant), std::make_pair(raw, column));
		}
	}

	fin.close();
}

Tag HeroBookPage::tagToWreath(Tag item)
{
	switch (item)
	{
	case Tag::hare:
		return Tag::hareWreath;
	case Tag::owl:
		return Tag::owlWreath;
	default:
		return Tag::emptyCell;
	}	
}

bool HeroBookPage::checkWreathCellFit(int i, int j, std::vector<int> rings)
{
	//all rings
	if ((i == 0 || i == 16) && (j == 0 || j == 1 || j == 3 || j == 4))
		return false;
	if ((i == 1 || i == 15) && (j == 0 || j == 3 || j == 4))
		return false;
	if ((i == 2 || i == 14) && (j == 0 || j == 4))
		return false;
	if ((i == 3 || i == 5 || i == 7 || i == 9 || i == 11 || i == 13) && (j == 4))
		return false;
	if (i == 8 && j == 2)
		return false;
	//---------

	//if (rings.empty())
		//return true;

	std::vector<int> invertedRings = { 1, 2, 3, 4 };
	for (auto& ring : rings)
	{
		for (int cnt = 0; cnt < invertedRings.size(); cnt++)
			if (ring == invertedRings[cnt])
				invertedRings.erase(invertedRings.begin() + cnt);
	}

	std::sort(invertedRings.begin(), invertedRings.end());

	for (int invertedRing : invertedRings)
	{
		switch (invertedRing)
		{
		case 1:
			if ((i == 4 && j == 0) || (i == 3 && j == 0) || (i == 2 && j == 1) || (i == 1 && j == 1) || (i == 0 && j == 2) || (i == 1 && j == 2) || (i == 2 && j == 3) || (i == 3 && j == 3) ||
				(i == 4 && j == 4) || (i == 4 && j == 4) || (i == 6 && j == 4) || (i == 8 && j == 4) || (i == 10 && j == 4) || (i == 12 && j == 4) ||
				(i == 13 && j == 3) || (i == 14 && j == 3) || (i == 15 && j == 2) || (i == 16 && j == 2) || (i == 15 && j == 1) || (i == 14 && j == 1) || (i == 13 && j == 0) || (i == 12 && j == 0) ||
				(i == 10 && j == 0) || (i == 8 && j == 0) || (i == 6 && j == 0))
				return false;
			break;
		case 2:
			if ((i == 5 && j == 0) || (i == 4 && j == 1) || (i == 3 && j == 1) || (i == 2 && j == 2) || (i == 3 && j == 2) || (i == 4 && j == 3) || (i == 5 && j == 3) || (i == 7 && j == 3) || (i == 9 && j == 3) ||
				(i == 11 && j == 3) || (i == 12 && j == 3) || (i == 13 && j == 2) || (i == 14 && j == 2) || (i == 13 && j == 1) || (i == 12 && j == 1) || (i == 11 && j == 0) || (i == 9 && j == 0) || (i == 7 && j == 0))
				return false;
			break;
		case 3:
			if ((i == 6 && j == 1) || (i == 5 && j == 1) || (i == 4 && j == 2) || (i == 5 && j == 2) || (i == 6 && j == 3) || (i == 8 && j == 3) || (i == 10 && j == 3) || (i == 11 && j == 2) || (i == 12 && j == 2) ||
				(i == 11 && j == 1) || (i == 10 && j == 1) || (i == 8 && j == 1))
				return false;
			break;
		case 4:
			if ((i == 7 && j == 1) || (i == 6 && j == 2) || (i == 7 && j == 2) || (i == 9 && j == 2) || (i == 10 && j == 2) || (i == 9 && j == 1))
				return false;
			break;
		}
	}
	return true;
}

std::pair<int, int> HeroBookPage::getSelectedWreathCell()
{
	Vector2f mousePos = Vector2f(Mouse::getPosition());
	float minD = 10e4;
	std::pair<int, int> ans = std::make_pair(-1, -1);
	for (int cnt1 = 0; cnt1 < wreathMatrix.size(); cnt1++)
		for (int cnt2 = 0; cnt2 < wreathMatrix[cnt1].size(); cnt2++)
		{
			if (wreathMatrix[cnt1][cnt2] == Tag::selectedCell)
				wreathMatrix[cnt1][cnt2] = Tag::emptyCell;
			Vector2f curPos = Vector2f(wreathMatrixPositions[cnt1][cnt2].x + buttonList->at(ButtonTag::cell).getGlobalBounds().width / 2,
				wreathMatrixPositions[cnt1][cnt2].y + buttonList->at(ButtonTag::cell).getGlobalBounds().height / 2);
			if (Helper::getDist(Vector2f(mousePos), curPos) < minD &&
				Helper::getDist(Vector2f(mousePos), curPos) <= buttonList->at(ButtonTag::cell).getGlobalBounds().width / 2)
			{
				minD = Helper::getDist(Vector2f(mousePos), curPos);
				ans = std::make_pair(cnt1, cnt2);
			}
		}

	return ans;
}

std::pair<int, int> HeroBookPage::getSelectedPlantsCell()
{
	const Vector2f mousePos = Vector2f(Mouse::getPosition());
	const Vector2f upperLeftCorner = Vector2f(
		buttonList->at(ButtonTag::sketching).getGlobalBounds().left + buttonList->at(ButtonTag::sketching).getGlobalBounds().width * 0.066,
		buttonList->at(ButtonTag::sketching).getGlobalBounds().top + buttonList->at(ButtonTag::sketching).getGlobalBounds().height * 0.55);

	const Vector2f size = Vector2f(buttonList->at(ButtonTag::chamomile).getGlobalBounds().width,
		buttonList->at(ButtonTag::chamomile).getGlobalBounds().height);

	if (mousePos.x < upperLeftCorner.x || mousePos.y < upperLeftCorner.y ||
		mousePos.x > upperLeftCorner.x + size.x * plantsMatrix[0].size() || mousePos.y > upperLeftCorner.y + size.y * plantsMatrix.size())
		return { -1, -1 };

	std::pair<int, int> ans = std::make_pair((mousePos.y - upperLeftCorner.y) / size.y,
	                                         (mousePos.x - upperLeftCorner.x) / size.x);

	if (plantsMatrix[ans.first][ans.second].first == Tag::emptyCell || plantsMatrix[ans.first][ans.second].second == 0)
		return { -1, -1 };

	return ans;
}

std::vector<std::pair<int, int>> HeroBookPage::getBorderCells(int raw, int column)
{
	std::vector<std::pair<int, int>> ans;
	if (raw == -1 || column == -1)
		return {};
	if (raw % 2 == 0)	
		ans = { {raw - 2, column}, {raw - 1, column}, {raw + 1, column}, {raw + 2, column}, {raw + 1, column - 1}, {raw - 1, column - 1} };
	else
		ans = { {raw - 2, column}, {raw - 1, column + 1}, {raw + 1, column + 1}, {raw + 2, column}, {raw + 1, column}, {raw - 1, column} };

	while (true)
	{
		bool isBreak = true;
		for (int i = 0; i < ans.size(); i++)
			if (ans[i].first < 0 || ans[i].second < 0 || ans[i].first >= wreathMatrix.size() || ans[i].second >= wreathMatrix[0].size())
			{
				ans.erase(ans.begin() + i);
				isBreak = false;
			}
		if (isBreak)
			break;
	}

	return ans;
}

void HeroBookPage::clearWreathMatrix()
{
	for (auto& raw : wreathMatrix)
		for (int column = 0; column < wreathMatrix[0].size(); column++)
			raw[column] = Tag::emptyCell;	
}

void HeroBookPage::setPlantsOnMatrix(std::vector<std::pair<Tag, std::pair<int, int>>> plants)
{
	clearWreathMatrix();
	for (auto& plant : plants)
		wreathMatrix[plant.second.first][plant.second.second] = plant.first;
}

void HeroBookPage::coloredDfs(int raw, int column, int color, bool flowerPatency)
{
	coloredMatrix[raw][column] = color;
	std::vector<std::pair<int, int>> ways = getBorderCells(raw, column);
	if (raw == 8 && column == 2)
	{
		int aTest = 12345;
	}
	for (const auto way : ways)
	{
		if (!flowerPatency)
		{
			if (coloredMatrix[way.first][way.second] == 0 && (wreathMatrix[raw][column] == Tag::emptyCell || wreathMatrix[way.first][way.second] == Tag::selectedCell))
				coloredDfs(way.first, way.second, color, flowerPatency);
		}
		else
		{
			if (coloredMatrix[way.first][way.second] == 1 && (wreathMatrix[raw][column] == Tag::emptyCell || wreathMatrix[way.first][way.second] == Tag::selectedCell) ||
				coloredMatrix[way.first][way.second] == 0)
				coloredDfs(way.first, way.second, color, flowerPatency);
		}
	}
}

bool HeroBookPage::isCenterSurrounded()
{
	for (auto& raw : coloredMatrix)
		for (auto& column : raw)
			column = 0;

	const std::pair<int, int> center = { 8, 2 };
	std::vector<std::pair<int, int>> corners = { {0, 0}, {0, 4}, {16, 4}, {16, 0} };

	coloredDfs(center.first, center.second, 1);
	for (const auto corner : corners)
		coloredDfs(corner.first, corner.second, 2, true);

	return (coloredMatrix[center.first][center.second] == 1);
}

std::vector<DrawableChainElement*> HeroBookPage::prepareConnectableFlowers(Tag currentFlower)
{
	if (plantsConnections.find(Tag(int(currentFlower))) == plantsConnections.end())
		return {};	

	std::vector<DrawableChainElement*> result = {};

	const Vector2f upperLeftCorner = Vector2f(buttonList->at(ButtonTag::sketching).getGlobalBounds().left, buttonList->at(ButtonTag::sketching).getGlobalBounds().top);
	const Vector2f pageSize = Vector2f(buttonList->at(ButtonTag::sketching).getGlobalBounds().width, buttonList->at(ButtonTag::sketching).getGlobalBounds().height);
	const Vector2f headingPos = Vector2f(upperLeftCorner.x + 0.072 * pageSize.x, upperLeftCorner.y + 0.28 * pageSize.y);

	result.push_back(new TextChainElement({ headingPos.x, headingPos.y }, { 0, 0 }, sf::Color(100, 68, 34, 180), "Connected with:"));	

	int cnt = 0;
	for (auto& connection : plantsConnections.at(Tag(int(currentFlower))))
	{
		if (buttonList->find(ButtonTag(int(connection))) == buttonList->end())
			continue;
		buttonList->at(ButtonTag(int(connection))).setPosition(Vector2f(upperLeftCorner.x + flowerConnectionsPedestals[cnt].x * pageSize.x,
			upperLeftCorner.y + flowerConnectionsPedestals[cnt].y * pageSize.y));
		result.push_back(buttonList->at(ButtonTag(int(connection))).prepareSprite());
		cnt++;
	}

	return result;
}

//--------

pageContent HeroBookPage::getPreparedContent(int pageNumber, Tag currentDraft)
{
	pageContent result;

	preparePageBase();
	setBookmarkPosition();
	setArrowsPosition();

	auto bookmarks = { ButtonTag::bookmarkMobs, ButtonTag::bookmarkItems, ButtonTag::bookmarkWorld, ButtonTag::bookmarkFlowers, ButtonTag::bookmarkWreathes, ButtonTag::bookmarkNightmare };
	result.buttons.insert(result.buttons.end(), bookmarks.begin(), bookmarks.end());

	switch (currentPage)
	{
	case 0:
		result.buttons.push_back(ButtonTag::bookCover);
		break;
	// items lists
	case 1:
	case 2:
	case 3:
	case 4:
	{
		std::vector<ButtonTag> blocks;
		if (currentPage == 1)
			blocks = { ButtonTag::hare, ButtonTag::owl, ButtonTag::deer, ButtonTag::bear };
		if (currentPage == 2)
			blocks = { ButtonTag::heroBag };
		if (currentPage == 4)
			blocks = { ButtonTag::chamomile, ButtonTag::yarrow, ButtonTag::fern, ButtonTag::mugwort, ButtonTag::poppy };

		putHeadingsToPositions(blocks);
		result.buttons.insert(result.buttons.begin(), blocks.begin(), blocks.end());
		result.buttons.push_back(ButtonTag::bookmarksList);
	}
	break;
	//------------
	// sketching page
	case 5:
	{		
		result.buttons.push_back(ButtonTag::sketching);
		if (readyToFinishDraft)
		{
			const Vector2f plusPos = Vector2f(wreathMatrixPositions[8][2].x - ((buttonList->at(ButtonTag::plus).getGlobalBounds().width - (buttonList->at(ButtonTag::cell).getGlobalBounds().width))) / 2,
				wreathMatrixPositions[8][2].y - ((buttonList->at(ButtonTag::plus).getGlobalBounds().height - (buttonList->at(ButtonTag::cell).getGlobalBounds().height))) / 2);
			buttonList->at(ButtonTag::plus).setPosition(plusPos);
			result.buttons.push_back(ButtonTag::plus);
		}
	}	
	break;
	//---------------
	// description of each object
	case 102:
	case 103:
	case 402:
		result.buttons.push_back(ButtonTag::pagePattern);
		//if (objectInfo.count(pageToObjectId(currentPage)) > 0 && objectInfo.at(pageToObjectId(currentPage)).isUnlocked)
			result.buttons.push_back(ButtonTag::makeWreath);

		// setting connections icons
		for (auto& connection : objectInfo.at(pageToObjectId(currentPage)).connections)
		{
			result.buttons.push_back(ButtonTag(connection.id));
			buttonList->at(ButtonTag(connection.id)).setPosition(getConnectionPosition(connection.pedestal));
			
			if (buttonList->at(ButtonTag(connection.id)).isSelected(Vector2f(Mouse::getPosition())))
				result.blockDescription = connection.description;
		}
		//--------------------------

		if (objectInfo.find(pageToObjectId(currentPage)) != objectInfo.end())
			result.pageDescription = objectInfo.at(pageToObjectId(currentPage)).textDescription;
		break;
	//---------------------------
	default:
		result.buttons.push_back(ButtonTag::pagePattern);		
	}	

	// turn on result buttons
	for (auto& button : result.buttons)	
		buttonList->at(button).isActive = true;	
	//-----------------------------
	return  result;
}