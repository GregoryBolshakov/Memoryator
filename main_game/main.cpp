#include <thread>

#include "console.h"
#include "draw_system.h"
#include "hero_book.h"
#include "menu_system.h"
#include "VisualEffects/ambient_light.hpp"


using namespace sf;

int main() {	
	srand(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
	
	const auto screen_size = helper::GetScreenSize();
	RenderWindow main_window(VideoMode(static_cast<unsigned int>(screen_size.x), static_cast<unsigned int>(screen_size.y), 32), "game", Style::Fullscreen);

    draw_system draw_system;
	menu_system menu_system;
	world_handler world(40000, 40000, &draw_system.packs_map);
	auto window_focus = true;
	console console(FloatRect(helper::GetScreenSize().x * 0.2f, helper::GetScreenSize().y * 0.8f, helper::GetScreenSize().x * 0.6f, helper::GetScreenSize().y * 0.03f), &world);

	Clock clock;
	long long time_micro_sec = 0;
	auto current_mouse_button = 0;

	hero_book main_book;
	main_book.init(&draw_system.packs_map);

	text_system text_writer;
	
	RenderTexture surface;

	if (!surface.create(static_cast<unsigned int>(screen_size.x), static_cast<unsigned int>(screen_size.y))) {
		err() << "Error creating renderer texture." << std::endl;
		return EXIT_FAILURE;
	}
	auto visual_effect_texture = surface.getTexture();
	auto visual_effect_sprite = Sprite(visual_effect_texture);

	ambient_light ambient_light(world.getTimeSystem(), screen_size);
	ambient_light.load();
	
	while (main_window.isOpen())
	{
		Event event{};

		while (main_window.pollEvent(event))
		{			
			world.handleEvents(event);
			if (Mouse::isButtonPressed(Mouse::Left))
				current_mouse_button = 1;
			else
				if (Mouse::isButtonPressed(Mouse::Right))
					current_mouse_button = 2;

			if (event.type == sf::Event::MouseWheelMoved)
			{
				if (menu_system.get_state() == closed)
					world.setScaleFactor(event.mouseWheel.delta);
			}	

			if (event.type == Event::MouseButtonReleased)
			{			
				if (menu_system.get_state() == closed && world.getBuildSystem().get_success_init())
				{
					world.onMouseUp(current_mouse_button);					
					main_book.onMouseUp();
				}
					
				if (current_mouse_button == 1)
					menu_system.interact(world, main_window);
			}

			if (event.type == Event::KeyReleased)
			{
				menu_system.on_key_down(event, world);
				if (event.key.code == Keyboard::Escape)
					world.pedestalController.stop();
			}

			if (event.type == Event::GainedFocus)			
				window_focus = true;	

			if (event.type == Event::LostFocus)			
				window_focus = false;			

			if (event.type == Event::Closed)
			{
				world.Save();
				world.~world_handler();
				main_window.close();
				break;
			}
			console.handle_events(event);
		}

		if (menu_system.get_state() == main_menu)
		{		
			main_window.clear(sf::Color::White);
			draw_system.draw(main_window, draw_system::upcast_chain(menu_system.prepare_sprites()));
			main_window.display();

			clock.restart();
			continue;
		}

		if (window_focus && menu_system.get_state() != game_menu)
		{
			time_micro_sec = clock.getElapsedTime().asMicroseconds();
			clock.restart();

			if (!console.get_state())
			{				
				world.interact(screen_size, time_micro_sec, event);
				world.focusedObject->handle_input(world.getInventorySystem().get_used_mouse(), time_micro_sec);
				ambient_light.update();
			}

			auto hero = dynamic_cast<deerchant*>(world.focusedObject);
			main_book.getAllOuterInfo(&hero->bags, world.getMouseDisplayName(), world.getSelectedObject(), &world.getInventorySystem().get_held_item(), hero->nearTheTable);
			main_book.interact();

			main_window.clear(sf::Color::White);
			
            draw_system.draw(main_window, world.prepareSprites(time_micro_sec, true), world.getWorldGenerator().scaleFactor, world.getCameraPosition());
            draw_system.draw(main_window, world.prepareSprites(time_micro_sec, false), world.getWorldGenerator().scaleFactor, world.getCameraPosition());
            
			visual_effect_texture.update(main_window);
			//main_window.draw(visual_effect_sprite, ambient_light.shader);

			draw_system.draw(main_window, draw_system::upcast_chain(world.getBuildSystem().prepare_sprites(world.getStaticGrid(), world.getLocalTerrain(), &draw_system.packs_map)), world.getWorldGenerator().scaleFactor, world.getCameraPosition());
			text_system::draw_string(world.getMouseDisplayName(), font_name::normal_font, 30, float(Mouse::getPosition().x), float(Mouse::getPosition().y), main_window, sf::Color(255, 255, 255, 180));
			world.pedestalController.draw(&main_window, world.getCameraPosition(), world.getWorldGenerator().scaleFactor);
			draw_system.draw(main_window, main_book.prepareSprites(world.focusedObject->get_health_point() / world.focusedObject->get_max_health_point_value(), time_micro_sec));
			draw_system.draw(main_window, world.getInventorySystem().prepare_sprites(time_micro_sec, world.packsMap));			
		}
		else
			clock.restart();

		draw_system.draw(main_window, draw_system::upcast_chain(menu_system.prepare_sprites()));

		world.focusedObject->move_system.set_move_offset(time_micro_sec);
		draw_system.draw(main_window, { new text_chain_element({500, 500}, {0, 0}, sf::Color::White, std::to_string(direction_system::calculate_angle(world.focusedObject->move_system.move_offset))) });
		
		console.interact(time_micro_sec);
		console.draw(main_window);

		main_window.display();
	}
}