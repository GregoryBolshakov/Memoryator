#include <thread>

#include "console.h"
#include "deerchant.h"
#include "draw_system.h"
#include "hero_book.h"
#include "menu_system.h"
#include "VisualEffects/AmbientLight.hpp"


using namespace sf;
using namespace std;


int main() {	
	srand(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
	
	const auto screenSize = Helper::GetScreenSize();
	RenderWindow mainWindow(VideoMode(static_cast<unsigned int>(screenSize.x), static_cast<unsigned int>(screenSize.y), 32), "game");//, Style::Fullscreen);

    draw_system draw_system;
	menu_system menuSystem;
	WorldHandler world(40000, 40000, &draw_system.packsMap);
	bool windowFocus = true;
	console console(FloatRect(Helper::GetScreenSize().x * 0.2f, Helper::GetScreenSize().y * 0.8f, Helper::GetScreenSize().x * 0.6f, Helper::GetScreenSize().y * 0.03f), &world);

	Clock clock;
	long long time_micro_sec = 0;
	int currentMouseButton = 0;

	hero_book mainBook;
	mainBook.init(&draw_system.packsMap);

	text_system textWriter;
	
	RenderTexture surface;

	if (!surface.create(static_cast<unsigned int>(screenSize.x), static_cast<unsigned int>(screenSize.y))) {
		err() << "Error creating renderer texture." << std::endl;
		return EXIT_FAILURE;
	}
	auto visual_effect_texture = surface.getTexture();
	auto visual_effect_sprite = Sprite(visual_effect_texture);

	AmbientLight ambient_light(screenSize);
	ambient_light.load();
	
	while (mainWindow.isOpen())
	{
		Event event{};

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
			mainWindow.clear(sf::Color::White);
			draw_system.draw(mainWindow, draw_system::UpcastChain(menuSystem.prepareSprites()));
			mainWindow.display();

			clock.restart();
			continue;
		}

		if (windowFocus && menuSystem.getState() != gameMenu)
		{
			time_micro_sec = clock.getElapsedTime().asMicroseconds();
			clock.restart();

			if (!console.getState())
			{				
				world.interact(screenSize, time_micro_sec, event);
				world.focusedObject->handleInput(world.getInventorySystem().getUsedMouse());
				ambient_light.update(screenSize.x / 2, screenSize.y / 2);
			}

			auto hero = dynamic_cast<deerchant*>(world.focusedObject);
			mainBook.getAllOuterInfo(&hero->bags, world.getMouseDisplayName(), world.getSelectedObject(), &world.getInventorySystem().getHeldItem(), hero->nearTheTable);
			mainBook.interact();

			mainWindow.clear(sf::Color::White);
			
            draw_system.draw(mainWindow, draw_system::UpcastChain(world.prepareSprites(time_micro_sec, true)), world.getWorldGenerator().scaleFactor, world.getCameraPosition());
            draw_system.draw(mainWindow, draw_system::UpcastChain(world.prepareSprites(time_micro_sec, false)), world.getWorldGenerator().scaleFactor, world.getCameraPosition());
            
			//visual_effect_texture.update(mainWindow);
			//mainWindow.draw(visual_effect_sprite, ambient_light.shader);

			draw_system.draw(mainWindow, draw_system::UpcastChain(world.getBuildSystem().prepareSprites(world.getStaticGrid(), world.getLocalTerrain(), &draw_system.packsMap)), world.getWorldGenerator().scaleFactor, world.getCameraPosition());
			text_system::drawString(world.getMouseDisplayName(), FontName::NormalFont, 30, float(Mouse::getPosition().x), float(Mouse::getPosition().y), mainWindow, sf::Color(255, 255, 255, 180));
			world.pedestalController.draw(&mainWindow, world.getCameraPosition(), world.getWorldGenerator().scaleFactor);
			draw_system.draw(mainWindow, mainBook.prepareSprites(world.focusedObject->getHealthPoint() / world.focusedObject->getMaxHealthPointValue(), time_micro_sec));
			draw_system.draw(mainWindow, world.getInventorySystem().prepareSprites(time_micro_sec, world.packsMap));			
			//world.runInventorySystemDrawing(mainWindow, time);
		}
		else
			clock.restart();

		draw_system.draw(mainWindow, draw_system::UpcastChain(menuSystem.prepareSprites()));

		//textWriter.drawString(std::to_string(world.getWorldGenerator().scaleFactor), FontName::NormalFont, TextChainElement::defaultCharacterSize, 200, 200, mainWindow);

		console.interact(time_micro_sec);
		console.draw(mainWindow);

		mainWindow.display();
	}
}