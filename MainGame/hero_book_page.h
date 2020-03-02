#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>

#include "button.h"
#include "hero_bag.h"
#include "sprite_structures.h"
#include "text_system.h"
#include "wreath_draft.h"

enum class auxiliary_sprite_tag { line1 = 1, line2 = 2, line3 = 3, line4 = 4, line5 = 5, line6 = 6, line7 = 7 };

struct connection
{
	connection (int id, int pedestal, std::string description)
	{
		this->id = id;
		this->pedestal = pedestal;
		this->description = std::move(description);
	}
	int id;
	int pedestal;
	std::string description;
};

struct book_object_info
{
	entity_tag tag;
	bool isUnlocked = false;
	std::vector<connection> connections;
	std::string textDescription;
};

struct page_content
{
	std::vector<button_tag> buttons;
	std::string blockDescription;
	std::string pageDescription;
	std::vector<RectangleShape> connections;
};

class hero_book_page
{
public:
	hero_book_page();
	~hero_book_page();
	void initAuxiliarySpriteMap();
	void buttonListBounding(std::unordered_map<button_tag, button>* buttonList);
	void setPage(int page) { this->currentPage = page; }

	// object information (creatures, craft, plants, wreaths, nightmare)
	void initObjectInfo();
	static entity_tag pageToObjectId(int page);
	int buttonToPage(button_tag button);
	void setButtonLock(button_tag button, button_tag changedButton);
	sprite_chain_element* prepareIconFrame(button_tag button, int type = 1);
	std::vector<sprite_chain_element*> prepareAllIcons(const page_content& content);
	std::vector<sprite_chain_element*> prepareLines();
	std::vector<text_chain_element*> prepareHeadingText();
	void unlockObject(entity_tag object);
	static int getHeadingPage(entity_tag object);
	std::unordered_map<entity_tag, book_object_info>* getObjectInfo() { return &objectInfo; }
	static std::string buttonToString(button_tag button);
	static entity_tag tagToWreath(entity_tag item);
	//------------------------------------------------------------------

	//wreaths
	void clearWreathMatrix();
	std::pair<int, int> getSelectedWreathCell();
	std::pair<int, int> getSelectedPlantsCell();
	std::vector<std::pair<int, int>> getBorderCells(int raw, int column);
	static bool checkWreathCellFit(int i, int j, const std::vector<int>& rings = std::vector<int>());
	void setPlantsOnMatrix(const std::vector<std::pair<entity_tag, std::pair<int, int>>>& plants);
	bool isCenterSurrounded();
	std::vector<drawable_chain_element*> prepareConnectableFlowers(entity_tag currentFlower);

	std::vector<std::vector<Vector2f>> wreathMatrixPositions;
	std::vector<std::vector<entity_tag>> wreathMatrix;
	std::vector<std::vector<std::pair<entity_tag, int>>> plantsMatrix;
	std::map<entity_tag, std::map<entity_tag, int>> doneRecipes;
	std::unordered_map<entity_tag, std::vector<entity_tag>> plantsConnections;
	std::unordered_map<entity_tag, draft_info> getOriginalSetups() { return originalSetups; };
	bool readyToFinishDraft = false; 
	bool nearTheTable = false;
	//-------

	std::string debugInfo = "";
	page_content getPreparedContent(int pageNumber, entity_tag currentDraft = entity_tag::emptyDraft);
	std::vector<hero_bag>* boundBags{};
private:
	static int div_up(int n, int d) { return n / d + (((n < 0) ^ (d > 0)) && (n % d)); }
	std::unordered_map<button_tag, button>* buttonList{};
	void setBookmarkPosition() const;
	void preparePageBase();
	void putHeadingsToPositions(std::vector<button_tag> buttons);
	void setArrowsPosition() const;
	
	int currentPage = 0;
	FloatRect pageGlobalBounds;
	FloatRect blockGlobalBounds, arrowToPageGlobalBounds;
	Vector2f leftPagePosition, rightPagePosition;

	std::unordered_map<auxiliary_sprite_tag, board_sprite> auxiliarySpriteMap;
	text_system textWriter;

	// object information (creatures, craft, plants, wreaths, nightmare)
	Vector2f getConnectionPosition(int numberInOrder);	

	std::unordered_map<entity_tag, book_object_info> objectInfo;
	std::vector<Vector2f> connectionPedestals = { {0.264f, 0.278f}, {0.113f, 0.390f}, {0.145f, 0.606f}, {0.126f, 0.789f}, {0.242f, 0.479f}, {0.269f, 0.718f}, {0.412f, 0.789f}, {0.363f, 0.566f} };
	std::vector<Vector2f> headingPedestals = { {0.061f, 0.315f}, {0.061f, 0.441f}, {0.061f, 0.568f}, {0.061f, 0.696f}, {0.53f, 0.097f}, {0.53f, 0.222f}, {0.53f, 0.348f}, {0.53f, 0.478f}, {0.53f, 0.614f}, {0.53f, 0.740f} };
	std::vector<Vector2f> headingTextPedestals = { {0.170f, 0.315f}, {0.170f, 0.441f}, {0.170f, 0.568f}, {0.170f, 0.696f}, {0.637f, 0.097f}, {0.637f, 0.222f}, {0.637f, 0.348f}, {0.637f, 0.478f}, {0.637f, 0.614f}, {0.637f, 0.740f} };
	std::vector<Vector2f> flowerConnectionsPedestals = { {0.072f, 0.342f}, {0.160f, 0.342f}, {0.246f, 0.342f} };
	//------------------------------------------------------------------

	// wreaths
	void initDrafts();
	void coloredDfs(int raw, int column, int color, bool flowerPatency = false);	

	std::unordered_map<entity_tag, draft_info> originalSetups;
	std::vector<std::vector<int>> coloredMatrix;	
	//--------
};

