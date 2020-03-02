#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>

#include "hero_book_page.h"
#include "button.h"
#include "HeroBag.h"
#include "TextSystem.h"
#include "WorldObject.h"

using namespace sf;

class hero_book
{
public:
	hero_book();
	~hero_book();
	void init(std::map<PackTag, SpritePack>* packsMap);
	std::vector<drawable_chain_element*> prepareSprites(float hpRatio, long long elapsedTime);
	void interact();
	void onMouseUp();
	void getAllOuterInfo(
		std::vector<HeroBag>* bags,
		std::string name,
		WorldObject* object,
		Cell* worldHeldItem,
		bool nearTheTable = false);
	void WhileMouseDown();
	void setPage(int page);
	void changeVisibility() { this->visibility = !this->visibility; }
	[[nodiscard]] int getCurrentPage() const { return currentPage; }
	
	void boundHeldItem() { this->worldHeldItem = worldHeldItem; }
	std::string debugInfo = "";
	Vector2f getHpLinePosition() { return {	buttonList.at(ButtonTag::bookStandTag).get_position().x + buttonList.at(ButtonTag::bookStandTag).get_global_bounds().width - 40,
		buttonList.at(ButtonTag::bookStandTag).get_position().y + buttonList.at(ButtonTag::bookStandTag).get_global_bounds().height / 2 - 20 }; }
private:
	hero_book_page somePage;
	std::vector<sprite_chain_element*> prepareHpLine(float hpRatio);
	std::vector<sprite_chain_element*> prepareWreathMatrix();
	std::vector<sprite_chain_element*> preparePlantsMatrix();
	std::vector <sprite_chain_element*> prepareLineMatrix();
	std::vector<sprite_chain_element*> preparePlantsList();
	std::vector<drawable_chain_element*> prepareWreathCost(Vector2f pos);
	void initButtons(std::map<PackTag, SpritePack>* packsMap);
	void initContent();

	std::string buttonsInfoFileDirectory = "Game/bookButtons.txt";
	std::unordered_map<ButtonTag, button> buttonList;
	int currentPage = 0;
	/////////////////////////
	// creatures 101 - 199 //
	// craft 201 - 299     //
	// plants 301 - 399    //
	// wreath 401 - 499    //
	/////////////////////////
	bool visibility = false;
	std::pair<Tag, int> heldItem = { Tag::emptyCell, 0 };
	Cell* worldHeldItem = nullptr;
	WorldObject* worldSelectedObject = nullptr;
	Tag currentDraft = Tag::emptyDraft, currentFlower = Tag::emptyCell;
	std::string worldMouseName = "";
	Vector2f blockDescriptionPoint = Vector2f(0.53f, 0.104f); // as a percentage of the size of the page markup
	Vector2f pageDescriptionPoint = Vector2f(0.53f, 0.395f); // as a percentage of the size of the page markup
	std::vector<HeroBag>* boundBags{};
};

