#include "MenuMaker.h"
#include "HeroBook.h"
#include "Deerchant.h"
#include "Console.h"

#include <thread>

using namespace sf;
using namespace std;

int main() {	
	srand(time(0));
	auto screenSize = Helper::GetScreenSize();
	RenderWindow mainWindow(VideoMode(static_cast<unsigned int>(screenSize.x), static_cast<unsigned int>(screenSize.y), 32), "game", Style::Fullscreen);
	
	MenuMaker menuSystem;
	WorldHandler world(40000, 40000);
	world.initLightSystem(mainWindow);
	bool windowFocus = true;
	Console console(IntRect(Helper::GetScreenSize().x * 0.2, Helper::GetScreenSize().y * 0.8, Helper::GetScreenSize().x * 0.6, Helper::GetScreenSize().y * 0.03), &world);

	Clock interactClock;
	Clock drawClock;
	Clock scaleDecreaseClock;	

	float interactTime = 0, drawTime = 0;
	int currentMouseButton = 0;

	HeroBook mainBook;

	TextWriter textWriter;

	while (mainWindow.isOpen())
	{
		Event event;

		while (mainWindow.pollEvent(event))
		{					
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
				if (menuSystem.getState() == closed && world.getBuildSystem().succesInit)
				{
					world.onMouseUp(currentMouseButton);					
					mainBook.onMouseUp();
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
			mainWindow.clear(Color::White);
			menuSystem.drawButtons(mainWindow);
			mainWindow.display();

			interactClock.restart();
			drawClock.restart();
			continue;
		}

		if (windowFocus && menuSystem.getState() != gameMenu)
		{
			interactTime = interactClock.getElapsedTime().asMicroseconds();
			interactClock.restart();
			
			drawTime = drawClock.getElapsedTime().asMicroseconds();
			drawClock.restart();

			if (!console.getState())
			{
				world.focusedObject->handleInput(world.getInventorySystem().getUsedMouse());
				world.interact(mainWindow, interactTime);
			}
			auto hero = dynamic_cast<Deerchant*>(world.focusedObject);
			mainBook.getAllOuterInfo(&hero->bags, world.getMouseDisplayName(), world.getSelectedObject(), &world.getInventorySystem().getHeldItem(), hero->nearTheTable);
			mainBook.interact(interactTime);

			mainWindow.clear(Color::White);

			world.draw(mainWindow, drawTime);
			world.runBuildSystemDrawing(mainWindow, drawTime);
			mainBook.draw(&mainWindow, world.focusedObject->getHealthPoint() / world.focusedObject->getMaxHealthPointValue(), drawTime);
			world.runInventorySystemDrawing(mainWindow, drawTime);
		}
		else
		{
			world.draw(mainWindow, 0);
			interactClock.restart();
			drawClock.restart();
		}

		menuSystem.drawButtons(mainWindow);

		auto hero = dynamic_cast<DynamicObject*>(world.focusedObject);
		console.interact(interactTime);
		console.draw(mainWindow);
		TextWriter::drawString(std::to_string(Helper::getFps()), NormalFont, 30, 200, 200, &mainWindow, Color::Black);	

		mainWindow.display();
	}
}






