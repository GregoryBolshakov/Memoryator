#pragma once

#include <SFML/Graphics.hpp>

#include "Button.h"
#include "Helper.h"
#include "WorldHandler.h"

using namespace sf;

enum MenuStates { mainMenu = 1, gameMenu = 2, closed = 3 };

class MenuSystem
{
public:
	MenuSystem();
	~MenuSystem();
	void interact(WorldHandler &world, RenderWindow &window);
	std::vector<SpriteChainElement*> prepareSprites();
	MenuStates getState() { return menuState; }
	void setState(MenuStates state) { menuState = state; }
	void onKeyDown(Event event, WorldHandler &world);
	bool getActivity() { return wasActive; }
private:
	MenuStates menuState = mainMenu;
	std::unordered_map<ButtonTag, Button> buttonList;
	std::string buttonsInfoFileDirectory = "Game/MenuSystem/buttonsInfo.txt";
	void initButtons();
	bool wasActive;
	Vector2f screenSize;
};
