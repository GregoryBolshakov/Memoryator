#include "MenuSystem.h"

const Vector2f NEW_RUN_BUTTON_POSITION(Helper::GetScreenSize().x * 0.06f, Helper::GetScreenSize().y * 0.06f);
const Vector2f CONTINUE_BUTTON_POSITION(Helper::GetScreenSize().x * 0.06f, Helper::GetScreenSize().y * 0.21f);
const Vector2f EXIT_BUTTON_POSITION(Helper::GetScreenSize().x * 0.06f, Helper::GetScreenSize().y * 0.36f);
const Vector2f BUTTON_SIZE(Helper::GetScreenSize().x * 0.2f, Helper::GetScreenSize().y * 0.14f);

MenuSystem::MenuSystem()
{
	initButtons();
}

void MenuSystem::initButtons()
{
	Vector2f screenSize = Helper::GetScreenSize();

    buttonList[ButtonTag::newRunTag].initialize(PackTag::interfaceElements, PackPart::menu, 1, 1, 1, NEW_RUN_BUTTON_POSITION, BUTTON_SIZE, true, ButtonTag::newRunTag);
    buttonList[ButtonTag::continueTag].initialize(PackTag::interfaceElements, PackPart::menu, 2, 2, 2, CONTINUE_BUTTON_POSITION, BUTTON_SIZE, true, ButtonTag::continueTag);
    buttonList[ButtonTag::exitTag].initialize(PackTag::interfaceElements, PackPart::menu, 4, 4, 4, EXIT_BUTTON_POSITION, BUTTON_SIZE, true, ButtonTag::exitTag);
}

MenuSystem::~MenuSystem()
{

}

void MenuSystem::onKeyDown(Event event, WorldHandler &world)
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

void MenuSystem::interact(WorldHandler &world, RenderWindow &window)
{
	wasActive = false;

	Vector2f mousePos = (Vector2f )Mouse::getPosition();

	if (menuState == mainMenu)
	{
		if (buttonList[ButtonTag::newRunTag].isSelected(mousePos))
		{
			world.runWorldGenerator();
			menuState = closed;
			wasActive = true;
			return;
		}

		if (buttonList[ButtonTag::continueTag].isSelected(mousePos))
		{
			//world.Load();
            world.runWorldGenerator();
			menuState = closed;
			wasActive = true;
			return;
		}

		if (buttonList[ButtonTag::exitTag].isSelected(mousePos))
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
		if (buttonList[ButtonTag::newRunTag].isSelected(mousePos))
		{
			world.runWorldGenerator();
			menuState = closed;
			wasActive = true;
			return;
		}

		if (buttonList[ButtonTag::continueTag].isSelected(mousePos))
		{
			menuState = closed;
			wasActive = true;
			return;
		}

		if (buttonList[ButtonTag::exitTag].isSelected(mousePos))
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

std::vector<SpriteChainElement*> MenuSystem::prepareSprites()
{
    std::vector<SpriteChainElement*> result = {};
	if (menuState == mainMenu)
	{
        result.push_back(buttonList.at(ButtonTag::newRunTag).prepareSprite());
		result.push_back(buttonList.at(ButtonTag::continueTag).prepareSprite());
		result.push_back(buttonList.at(ButtonTag::exitTag).prepareSprite());		
	}

	if (menuState == gameMenu)
	{
		result.push_back(buttonList.at(ButtonTag::newRunTag).prepareSprite());
		result.push_back(buttonList.at(ButtonTag::continueTag).prepareSprite());
		result.push_back(buttonList.at(ButtonTag::exitTag).prepareSprite());
	}

	if (menuState == closed)
	{
		
	}

    return result;
}
	
