#include "menu_system.h"
#include "button.h"
#include "helper.h"
#include "world_handler.h"

menu_system::menu_system()
{
	const sf::Vector2f button_size(world_metrics::window_size.x * 0.2f, world_metrics::window_size.y * 0.14f);
	button_list_.insert({ 
		button_tag::newRunTag,
		button(pack_tag::interfaceElements, 
		pack_part::menu, 1,
		1,
		1,
		sf::Vector2f(world_metrics::window_size.x * 0.06f, world_metrics::window_size.y * 0.06f),
		button_size,
		true,
		button_tag::newRunTag) });
	button_list_.insert({ 
		button_tag::continueTag,
		button(pack_tag::interfaceElements,
		pack_part::menu,
		2, 
		2, 
		2, 
		sf::Vector2f(world_metrics::window_size.x * 0.06f, world_metrics::window_size.y * 0.21f),
		button_size,
		true,
		button_tag::continueTag) });
	button_list_.insert({ 
		button_tag::exitTag,
		button(pack_tag::interfaceElements,
		pack_part::menu,
		4,
		4,
		4,
		sf::Vector2f(world_metrics::window_size.x * 0.06f, world_metrics::window_size.y * 0.36f),
		button_size,
		true,
		button_tag::exitTag) });

}

menu_system::~menu_system()
= default;

void menu_system::on_key_down(const sf::Event event, const shared_ptr<world_handler>& world)
{
	if (event.key.code == sf::Keyboard::Escape)
	{
		if (menu_state_ == closed)
		{
			menu_state_ = game_menu;
		}
		else
			if (menu_state_ == game_menu)
			{
				menu_state_ = closed;
			}
	}
}

void menu_system::interact(const shared_ptr<world_handler>& world, const shared_ptr<sf::RenderWindow>& window)
{
	was_active_ = false;

	const sf::Vector2f mouse_pos = sf::Vector2f(sf::Mouse::getPosition());

	if (menu_state_ == main_menu)
	{
		if (button_list_.at(button_tag::newRunTag).is_selected(mouse_pos))
		{
			world->run_world_generator();
			menu_state_ = closed;
			was_active_ = true;
			return;
		}

		if (button_list_.at(button_tag::continueTag).is_selected(mouse_pos))
		{
			//world.Load();
			world->run_world_generator();
			menu_state_ = closed;
			was_active_ = true;
			return;
		}

		if (button_list_.at(button_tag::exitTag).is_selected(mouse_pos))
		{
			menu_state_ = closed;
			window->close();
			was_active_ = true;
			return;
		}

		return;
	}

	if (menu_state_ == game_menu)
	{
		if (button_list_.at(button_tag::newRunTag).is_selected(mouse_pos))
		{
			world->run_world_generator();
			menu_state_ = closed;
			was_active_ = true;
			return;
		}

		if (button_list_.at(button_tag::continueTag).is_selected(mouse_pos))
		{
			menu_state_ = closed;
			was_active_ = true;
			return;
		}

		if (button_list_.at(button_tag::exitTag).is_selected(mouse_pos))
		{
			world->save();
			menu_state_ = main_menu;
			was_active_ = true;
			return;
		}

		return;
	}

	if (menu_state_ == closed)
	{
		/*if (buttonList.at(ButtonTag::openMenu).isSelected(mousePos))
		{
			menuState = gameMenu;
			wasActive = true;
			return;
		}*/
	}
}

std::vector<unique_ptr<drawable_chain_element>> menu_system::prepare_sprites()
{
	std::vector<unique_ptr<drawable_chain_element>> result;
	if (menu_state_ == main_menu)
	{
        result.push_back(button_list_.at(button_tag::newRunTag).prepare_sprite());
		result.push_back(button_list_.at(button_tag::continueTag).prepare_sprite());
		result.push_back(button_list_.at(button_tag::exitTag).prepare_sprite());
	}

	if (menu_state_ == game_menu)
	{
		result.push_back(button_list_.at(button_tag::newRunTag).prepare_sprite());
		result.push_back(button_list_.at(button_tag::continueTag).prepare_sprite());
		result.push_back(button_list_.at(button_tag::exitTag).prepare_sprite());
	}

	if (menu_state_ == closed)
	{
		
	}

    return result;
}
	
