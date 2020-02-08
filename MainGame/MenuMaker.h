#pragma once
#ifndef MENUMAKER_H
#define MENUMAKER_H

#include <SFML/Graphics.hpp>
#include "WorldHandler.h"
#include "Helper.h"
#include "ButtonMaker.h"

using namespace sf;

enum MenuStates { mainMenu = 1, gameMenu = 2, closed = 3 };

class MenuMaker
{
protected:

public:
	MenuMaker();
	~MenuMaker();
	void interact(WorldHandler &world, RenderWindow &window);
	std::vector<SpriteChainElement> prepareSprites();
	MenuStates getState() { return menuState; }
	void setState(MenuStates state) { menuState = state; }
	void onKeyDown(Event event, WorldHandler &world);
	bool getActivity() { return wasActive; }
private:
	MenuStates menuState = mainMenu;
	std::unordered_map<ButtonTag, ButtonMaker> buttonList;
	std::string buttonsInfoFileDirectory = "Game/MenuMaker/buttonsInfo.txt";
	void initButtons();
	bool wasActive;
	Vector2f screenSize;
};
#endif