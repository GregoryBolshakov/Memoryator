#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>

#include "hero_book_page.h"
#include "button.h"
#include "hero_bag.h"
#include "text_system.h"
#include "world_object.h"

using namespace sf;

class hero_book
{
public:
	hero_book();
	~hero_book();
	void init(std::map<pack_tag, sprite_pack>* packsMap);
	std::vector<drawable_chain_element*> prepareSprites(float hpRatio, long long elapsedTime);
	void interact();
	void onMouseUp();
	void getAllOuterInfo(
		std::vector<hero_bag>* bags,
		std::string name,
		world_object* object,
		cell* worldHeldItem,
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
	void initButtons(std::map<pack_tag, sprite_pack>* packsMap);
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
	std::pair<entity_tag, int> heldItem = { entity_tag::emptyCell, 0 };
	cell* worldHeldItem = nullptr;
	world_object* worldSelectedObject = nullptr;
	entity_tag currentDraft = entity_tag::emptyDraft, currentFlower = entity_tag::emptyCell;
	std::string worldMouseName = "";
	Vector2f blockDescriptionPoint = Vector2f(0.53f, 0.104f); // as a percentage of the size of the page markup
	Vector2f pageDescriptionPoint = Vector2f(0.53f, 0.395f); // as a percentage of the size of the page markup
	std::vector<hero_bag>* boundBags{};
};

