#include "console.h"
#include "draw_system.h"
#include "hero_book.h"
#include "menu_system.h"
#include "shader_system.h"
#include "world_handler.h"
#include "scale_system.h"
#include "camera_system.h"
#include "text_system.h"
#include "time_system.h"
#include "inventory_system.h"
#include "deerchant.h"
#include "world_generator.h"
#include "helper.h"
#include "world_metrics.h"
#include "sprite_pack.h"
#include "visual_effects/visual_effect.hpp"
#include "text_chain_element.h"
#include "shape_chain_element.h"

#include <thread>

int main()
{
	srand(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));

	auto window = make_shared<sf::RenderWindow>(sf::VideoMode::getDesktopMode(), "Memoryator", sf::Style::Fullscreen);
	world_metrics::set_world_metrics(window);
	const auto screen_size = world_metrics::window_size;

	auto camera_system = make_shared<::camera_system>();
	auto time_system = make_shared<::time_system>();

	auto shader_system = make_shared<::shader_system>(camera_system, time_system);
	shader_system->initialize();
	
	auto draw_system = make_shared<::draw_system>(shader_system, screen_size);
	auto menu_system = make_shared<::menu_system>();
	
	auto world = make_shared<::world_handler>(camera_system);
	auto window_focus = true;
	
	auto console = make_shared<::console>(sf::FloatRect(
		world_metrics::window_size.x * 0.2f,
		world_metrics::window_size.y * 0.8f,
		world_metrics::window_size.x * 0.6f,
		world_metrics::window_size.y * 0.03f),
		time_system,
		world);

	Clock clock;
	long long time_micro_sec = 0;
	auto current_mouse_button = 0;

	//hero_book main_book;
	//main_book.init(&draw_system->packs_map);

	text_system text_writer;
	
	while (window->isOpen())
	{
		Event event{};

		while (window->pollEvent(event))
		{
			world->handle_events(event);
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				current_mouse_button = 1;
			else
				if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
					current_mouse_button = 2;

			if (event.type == sf::Event::MouseWheelMoved && menu_system->get_state() == closed)
					camera_system->get_scale_system()->update_zoom_factor(event.mouseWheel.delta);

			if (event.type == sf::Event::MouseButtonReleased)
			{			
				if (menu_system->get_state() == closed)
				{
					world->on_mouse_up(current_mouse_button);
					//main_book.onMouseUp();
				}
					
				if (current_mouse_button == 1)
					menu_system->interact(world, window);
			}

			if (event.type == sf::Event::KeyReleased)
			{
				menu_system->on_key_down(event, world);
				if (event.key.code == sf::Keyboard::Escape)
					world->pedestal_controller.stop();
			}

			if (event.type == sf::Event::GainedFocus)
				window_focus = true;

			if (event.type == sf::Event::LostFocus)
				window_focus = false;

			if (event.type == sf::Event::Closed)
			{
				world->save();
				window->close();
				return 0;
			}
			console->handle_events(event);
		}

		if (menu_system->get_state() == main_menu)
		{
			window->clear(sf::Color::White);
			draw_system->draw(window, menu_system->prepare_sprites());
			window->display();

			clock.restart();
			continue;
		}

		if (window && window_focus && menu_system->get_state() != game_menu)
		{
			time_micro_sec = clock.getElapsedTime().asMicroseconds();
			clock.restart();

			if (!console->get_state())
			{
				camera_system->get_scale_system()->interact();
				time_system->interact(time_micro_sec);
				world->interact(time_micro_sec);
				world->get_player().lock()->handle_input(world->get_inventory_system()->get_used_mouse(), time_micro_sec);

				shader_system->update();
			}

			auto hero = dynamic_pointer_cast<deerchant>(world->get_player().lock());

			window->clear(sf::Color::White);
			const auto scale = camera_system->get_scale_system()->calculate_scale();
			
			draw_system->draw(window, world->prepare_sprites(time_micro_sec, true), scale);
			draw_system->draw(window, world->prepare_sprites(time_micro_sec, false), scale);

			//draw_system->draw(main_window, shader_kind::dynamic_light);
			text_system::draw_string("constant:   " + std::to_string(int(world_metrics::constant_zone.left)) + "   " + std::to_string(int(world_metrics::constant_zone.top)) + "   " +  std::to_string(int(world_metrics::constant_zone.width)) + "   " + std::to_string(int(world_metrics::constant_zone.height)), font_name::bebas_font, 40, 200, 100, window, sf::Color::Black);
			text_system::draw_string("visible:   " + std::to_string(int(world_metrics::visible_zone.left)) + "   " + std::to_string(int(world_metrics::visible_zone.top)) + "   " + std::to_string(int(world_metrics::visible_zone.width)) + "   " + std::to_string(int(world_metrics::visible_zone.height)), font_name::bebas_font, 40, 200, 200, window, sf::Color::Black);
			text_system::draw_string("block:   " + std::to_string(int(world_metrics::block_size.x)) + "   " + std::to_string(int(world_metrics::block_size.y)) + "   " + std::to_string(int(world_metrics::block_size.x * world_metrics::scale)) + "   " + std::to_string(int(world_metrics::block_size.y * world_metrics::scale)), font_name::bebas_font, 40, 200, 300, window, sf::Color::Black);
			text_system::draw_string("player:   " + std::to_string(int(world->get_player().lock()->get_position().x)) + "   " + std::to_string(int(world->get_player().lock()->get_position().y)), font_name::bebas_font, 40, 200, 400, window, sf::Color::Black);
			text_system::draw_string("center:   " + std::to_string(int(world_metrics::center.x)) + "   " + std::to_string(int(world_metrics::center.y)), font_name::bebas_font, 40, 200, 350, window, sf::Color::Black);
			text_system::draw_string("mouse:   " + std::to_string(int(world_metrics::screen_to_world_position(Vector2f(Mouse::getPosition(*window))).x)) + "   " + std::to_string(int(world_metrics::screen_to_world_position(Vector2f(Mouse::getPosition(*window))).y)), font_name::bebas_font, 40, 200, 500, window, sf::Color::Black);
			text_system::draw_string("scale:   " + std::to_string(world_metrics::scale), font_name::bebas_font, 40, 200, 600, window, sf::Color::Black);
		}
		else
			clock.restart();

		draw_system->draw(window, menu_system->prepare_sprites());

		world->get_player().lock()->move_system.set_move_offset(time_micro_sec);

		console->interact(time_micro_sec);
		console->draw(window);

		window->display();
	}
}