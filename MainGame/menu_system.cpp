#include "menu_system.h"

const Vector2f NEW_RUN_BUTTON_POSITION(Helper::GetScreenSize().x * 0.06f, Helper::GetScreenSize().y * 0.06f);
const Vector2f CONTINUE_BUTTON_POSITION(Helper::GetScreenSize().x * 0.06f, Helper::GetScreenSize().y * 0.21f);
const Vector2f EXIT_BUTTON_POSITION(Helper::GetScreenSize().x * 0.06f, Helper::GetScreenSize().y * 0.36f);
const Vector2f BUTTON_SIZE(Helper::GetScreenSize().x * 0.2f, Helper::GetScreenSize().y * 0.14f);

menu_system::menu_system()
{
	initButtons();
}

void menu_system::initButtons()
{
	Vector2f screenSize = Helper::GetScreenSize();

    buttonList[ButtonTag::newRunTag].initialize(PackTag::interfaceElements, PackPart::menu, 1, 1, 1, NEW_RUN_BUTTON_POSITION, BUTTON_SIZE, true, ButtonTag::newRunTag);
    buttonList[ButtonTag::continueTag].initialize(PackTag::interfaceElements, PackPart::menu, 2, 2, 2, CONTINUE_BUTTON_POSITION, BUTTON_SIZE, true, ButtonTag::continueTag);
    buttonList[ButtonTag::exitTag].initialize(PackTag::interfaceElements, PackPart::menu, 4, 4, 4, EXIT_BUTTON_POSITION, BUTTON_SIZE, true, ButtonTag::exitTag);
}

menu_system::~menu_system()
= default;

void menu_system::onKeyDown(Event event, world_handler &world)
{
	if (event.key.code == Keyboard::Escape)
	{
		if (menuState == closed)
		{
			menuState = gameMenu;
		}
		else
			if (menuState == gameMenu)
			{
				menuState = closed;
			}
	}
}

void menu_system::interact(world_handler &world, RenderWindow &window)
{
	wasActive = false;

	Vector2f mousePos = (Vector2f )Mouse::getPosition();

	if (menuState == mainMenu)
	{
		if (buttonList[ButtonTag::newRunTag].is_selected(mousePos))
		{
			world.runWorldGenerator();
			menuState = closed;
			wasActive = true;
			return;
		}

		if (buttonList[ButtonTag::continueTag].is_selected(mousePos))
		{
			//world.Load();
            world.runWorldGenerator();
			menuState = closed;
			wasActive = true;
			return;
		}

		if (buttonList[ButtonTag::exitTag].is_selected(mousePos))
		{
			menuState = closed;
			window.close();
			wasActive = true;
			return;
		}

		return;
	}

	if (menuState == gameMenu)
	{
		if (buttonList[ButtonTag::newRunTag].is_selected(mousePos))
		{
			world.runWorldGenerator();
			menuState = closed;
			wasActive = true;
			return;
		}

		if (buttonList[ButtonTag::continueTag].is_selected(mousePos))
		{
			menuState = closed;
			wasActive = true;
			return;
		}

		if (buttonList[ButtonTag::exitTag].is_selected(mousePos))
		{
			world.Save();
			menuState = mainMenu;
			wasActive = true;
			return;
		}

		return;
	}

	if (menuState == closed)
	{
		/*if (buttonList.at(ButtonTag::openMenu).isSelected(mousePos))
		{
			menuState = gameMenu;
			wasActive = true;
			return;
		}*/
	}
}

std::vector<sprite_chain_element*> menu_system::prepareSprites()
{
    std::vector<sprite_chain_element*> result = {};
	if (menuState == mainMenu)
	{
        result.push_back(buttonList.at(ButtonTag::newRunTag).prepare_sprite());
		result.push_back(buttonList.at(ButtonTag::continueTag).prepare_sprite());
		result.push_back(buttonList.at(ButtonTag::exitTag).prepare_sprite());		
	}

	if (menuState == gameMenu)
	{
		result.push_back(buttonList.at(ButtonTag::newRunTag).prepare_sprite());
		result.push_back(buttonList.at(ButtonTag::continueTag).prepare_sprite());
		result.push_back(buttonList.at(ButtonTag::exitTag).prepare_sprite());
	}

	if (menuState == closed)
	{
		
	}

    return result;
}
	
