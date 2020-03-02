#pragma once

#include <SFML/Graphics.hpp>

#include "button.h"
#include "helper.h"
#include "world_handler.h"

using namespace sf;

enum menu_states { mainMenu = 1, gameMenu = 2, closed = 3 };

class menu_system
{
public:
	menu_system();
	~menu_system();
	void interact(world_handler &world, RenderWindow &window);
	std::vector<sprite_chain_element*> prepareSprites();
	menu_states getState() { return menuState; }
	void setState(menu_states state) { menuState = state; }
	void onKeyDown(Event event, world_handler &world);
	bool getActivity() { return wasActive; }
private:
	menu_states menuState = mainMenu;
	std::unordered_map<ButtonTag, button> buttonList;
	std::string buttonsInfoFileDirectory = "Game/MenuSystem/buttonsInfo.txt";
	void initButtons();
	bool wasActive;
	Vector2f screenSize;
};
