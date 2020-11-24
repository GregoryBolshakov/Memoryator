#include <thread>

#include "console.h"
#include "draw_system.h"
#include "hero_book.h"
#include "menu_system.h"
#include "shader_system.h"


using namespace sf;

int main() {	
	srand(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
	
	const auto screen_size = helper::GetScreenSize();
	RenderWindow main_window(VideoMode(static_cast<unsigned int>(screen_size.x), static_cast<unsigned int>(screen_size.y), 32), "game");//, Style::Fullscreen);

	auto scale_system = make_shared<::scale_system>();
	auto camera_system = make_shared<::camera_system>(scale_system);
	auto time_system = make_shared<::time_system>();
	
	auto shader_system = make_shared<::shader_system>(camera_system, time_system);
	shader_system->initialize();
	
	auto draw_system = make_shared<::draw_system>(shader_system, screen_size);
	auto menu_system = make_shared<::menu_system>();
	
	auto world = make_shared<::world_handler>(40000, 40000, camera_system, draw_system->get_packs_map());
	auto window_focus = true;
	
	auto console = make_shared<::console>(FloatRect(
		helper::GetScreenSize().x * 0.2f,
		helper::GetScreenSize().y * 0.8f,
		helper::GetScreenSize().x * 0.6f,
		helper::GetScreenSize().y * 0.03f),
		time_system,
		world);

	Clock clock;
	long long time_micro_sec = 0;
	auto current_mouse_button = 0;

	//hero_book main_book;
	//main_book.init(&draw_system->packs_map);

	text_system text_writer;
	
	while (main_window.isOpen())
	{
		Event event{};

		while (main_window.pollEvent(event))
		{
			world->handle_events(event);
			if (Mouse::isButtonPressed(Mouse::Left))
				current_mouse_button = 1;
			else
				if (Mouse::isButtonPressed(Mouse::Right))
					current_mouse_button = 2;

			if (event.type == sf::Event::MouseWheelMoved)
			{
				if (menu_system->get_state() == closed)
					scale_system->set_scale_factor(event.mouseWheel.delta);
			}	

			if (event.type == Event::MouseButtonReleased)
			{			
				/*if (menu_system->get_state() == closed && world.getBuildSystem().get_success_init())
				{
					world.onMouseUp(current_mouse_button);
					main_book.onMouseUp();
				}*/
					
				if (current_mouse_button == 1)
					menu_system->interact(world, main_window);
			}

			if (event.type == Event::KeyReleased)
			{
				menu_system->on_key_down(event, world);
				if (event.key.code == Keyboard::Escape)
					world->pedestal_controller.stop();
			}

			if (event.type == Event::GainedFocus)
				window_focus = true;

			if (event.type == Event::LostFocus)
				window_focus = false;

			if (event.type == Event::Closed)
			{
				world->save();
				world->~world_handler();
				main_window.close();
				break;
			}
			console->handle_events(event);
		}

		if (menu_system->get_state() == main_menu)
		{
			main_window.clear(sf::Color::White);
			draw_system->draw(main_window, menu_system->prepare_sprites());
			main_window.display();

			clock.restart();
			continue;
		}

		if (window_focus && menu_system->get_state() != game_menu)
		{
			time_micro_sec = clock.getElapsedTime().asMicroseconds();
			clock.restart();

			if (!console->get_state())
			{
				scale_system->interact();
				time_system->interact(time_micro_sec);
				world->interact(screen_size, time_micro_sec, event);
				world->focused_object->handle_input(world->get_inventory_system()->get_used_mouse(), time_micro_sec);

				shader_system->update();
			}

			auto hero = dynamic_pointer_cast<deerchant>(world->focused_object);
			//main_book.getAllOuterInfo(&hero->bags, world.getMouseDisplayName(), world.getSelectedObject(), &world.getInventorySystem().get_held_item(), hero->near_the_table);
			//main_book.interact();

			main_window.clear(sf::Color::White);
			const auto scale = scale_system->get_scale_factor();
			
			draw_system->draw(main_window, world->prepare_sprites(time_micro_sec, true), scale, camera_system->position);
			draw_system->draw(main_window, world->prepare_sprites(time_micro_sec, false), scale, camera_system->position);

			//draw_system->draw(main_window, shader_kind::dynamic_light);

			//draw_system->draw(main_window, draw_system::upcast_chain(world.getBuildSystem().prepare_sprites(world.getStaticGrid(), world.getLocalTerrain(), &draw_system->packs_map)), scale, camera_system->position);
			//text_system::draw_string(world.getMouseDisplayName(), font_name::normal_font, 30, float(Mouse::getPosition().x), float(Mouse::getPosition().y), main_window, sf::Color(255, 255, 255, 180));
			world->pedestal_controller.draw(&main_window, camera_system->position, scale);
			//draw_system->draw(main_window, main_book.prepare_sprites(world.focusedObject->get_health_point() / world.focusedObject->get_max_health_point_value(), time_micro_sec));
			//draw_system->draw(main_window, world.getInventorySystem().prepare_sprites(time_micro_sec, world.packsMap));
			text_system::draw_string(std::to_string(1000000 / time_micro_sec), font_name::bebas_font, 40, 200, 200, main_window, sf::Color::Black);
		}
		else
			clock.restart();

		draw_system->draw(main_window, menu_system->prepare_sprites());

		world->focused_object->move_system.set_move_offset(time_micro_sec);

		console->interact(time_micro_sec);
		console->draw(main_window);

		main_window.display();
	}
}