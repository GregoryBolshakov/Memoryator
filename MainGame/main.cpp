#include "DrawSystem.h"
#include "MenuSystem.h"
#include "HeroBook.h"
#include "Deerchant.h"
#include "Console.h"

#include <thread>

using namespace sf;
using namespace std;

int main() {	
	srand(time(0));
	const auto screenSize = Helper::GetScreenSize();
	RenderWindow mainWindow(VideoMode(static_cast<unsigned int>(screenSize.x), static_cast<unsigned int>(screenSize.y), 32), "game", Style::Fullscreen);

    DrawSystem drawSystem;
	MenuSystem menuSystem;
	WorldHandler world(40000, 40000, &drawSystem.packsMap);
	bool windowFocus = true;
	Console console(IntRect(Helper::GetScreenSize().x * 0.2, Helper::GetScreenSize().y * 0.8, Helper::GetScreenSize().x * 0.6, Helper::GetScreenSize().y * 0.03), &world);

	Clock clock;
	float time = 0;
	int currentMouseButton = 0;

	HeroBook mainBook;

	TextSystem textWriter;

	while (mainWindow.isOpen())
	{
		Event event;

		while (mainWindow.pollEvent(event))
		{			
			world.handleEvents(event);
			if (Mouse::isButtonPressed(Mouse::Left))
				currentMouseButton = 1;
			else
				if (Mouse::isButtonPressed(Mouse::Right))
					currentMouseButton = 2;

			if (event.type == sf::Event::MouseWheelMoved)
			{
				if (menuSystem.getState() == closed)
					world.setScaleFactor(event.mouseWheel.delta);
			}	

			if (event.type == Event::MouseButtonReleased)
			{			
				if (menuSystem.getState() == closed && world.getBuildSystem().getSuccessInit())
				{
					world.onMouseUp(currentMouseButton);					
					//mainBook.onMouseUp();
				}
					
				if (currentMouseButton == 1)
					menuSystem.interact(world, mainWindow);
			}

			if (event.type == Event::KeyReleased)
			{
				menuSystem.onKeyDown(event, world);
				if (event.key.code == Keyboard::Escape)
					world.pedestalController.stop();
			}

			if (event.type == Event::GainedFocus)			
				windowFocus = true;	

			if (event.type == Event::LostFocus)			
				windowFocus = false;			

			if (event.type == Event::Closed)
			{
				world.Save();
				world.~WorldHandler();
				mainWindow.close();
				break;
			}
			console.handleEvents(event);
		}

		if (menuSystem.getState() == mainMenu)
		{		
			mainWindow.clear(sf::Color::White);
			drawSystem.draw(mainWindow, DrawSystem::UpcastChain(menuSystem.prepareSprites()));
			mainWindow.display();

			clock.restart();
			continue;
		}

		if (windowFocus && menuSystem.getState() != gameMenu)
		{
			time = clock.getElapsedTime().asMicroseconds();
			clock.restart();

			if (!console.getState())
			{				
				world.interact(mainWindow, time, event);
				world.focusedObject->handleInput(world.getInventorySystem().getUsedMouse());
			}

			//mainBook.getAllOuterInfo(&hero->bags, world.getMouseDisplayName(), world.getSelectedObject(), &world.getInventorySystem().getHeldItem(), hero->nearTheTable);
			//mainBook.interact(time);

			mainWindow.clear(sf::Color::White);

            drawSystem.draw(mainWindow, DrawSystem::UpcastChain(world.prepareSprites(time, true)), world.getWorldGenerator().scaleFactor, world.getCameraPosition());
            drawSystem.draw(mainWindow, DrawSystem::UpcastChain(world.prepareSprites(time, false)), world.getWorldGenerator().scaleFactor, world.getCameraPosition());
			drawSystem.draw(mainWindow, DrawSystem::UpcastChain(world.getBuildSystem().prepareSprites(world.getStaticGrid(), world.getLocalTerrain(), &drawSystem.packsMap)), world.getWorldGenerator().scaleFactor, world.getCameraPosition());
			TextSystem::drawString(world.getMouseDisplayName(), FontName::NormalFont, 30, Mouse::getPosition().x, Mouse::getPosition().y, mainWindow, sf::Color(255, 255, 255, 180));
			world.pedestalController.draw(&mainWindow, world.getCameraPosition(), world.getWorldGenerator().scaleFactor);
			drawSystem.draw(mainWindow, world.getInventorySystem().prepareSprites(time, world.packsMap));
			//mainBook.draw(&mainWindow, world.focusedObject->getHealthPoint() / world.focusedObject->getMaxHealthPointValue(), time);
			//world.runInventorySystemDrawing(mainWindow, time);
		}
		else
			clock.restart();

		drawSystem.draw(mainWindow, DrawSystem::UpcastChain(menuSystem.prepareSprites()));

		textWriter.drawString(std::to_string(world.getWorldGenerator().scaleFactor), FontName::NormalFont, TextChainElement::defaultCharacterSize, 200, 200, mainWindow);

		console.interact(time);
		console.draw(mainWindow);

		mainWindow.display();
	}
}