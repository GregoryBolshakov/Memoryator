#include "menu_system.h"

const Vector2f new_run_button_position(helper::GetScreenSize().x * 0.06f, helper::GetScreenSize().y * 0.06f);
const Vector2f continue_button_position(helper::GetScreenSize().x * 0.06f, helper::GetScreenSize().y * 0.21f);
const Vector2f exit_button_position(helper::GetScreenSize().x * 0.06f, helper::GetScreenSize().y * 0.36f);
const Vector2f button_size(helper::GetScreenSize().x * 0.2f, helper::GetScreenSize().y * 0.14f);

menu_system::menu_system()
{
	init_buttons();
}

void menu_system::init_buttons()
{
    button_list_[button_tag::newRunTag].initialize(pack_tag::interfaceElements, pack_part::menu, 1, 1, 1, new_run_button_position, button_size, true, button_tag::newRunTag);
    button_list_[button_tag::continueTag].initialize(pack_tag::interfaceElements, pack_part::menu, 2, 2, 2, continue_button_position, button_size, true, button_tag::continueTag);
    button_list_[button_tag::exitTag].initialize(pack_tag::interfaceElements, pack_part::menu, 4, 4, 4, exit_button_position, button_size, true, button_tag::exitTag);
}

menu_system::~menu_system()
= default;

void menu_system::on_key_down(const Event event, world_handler &world)
{
	if (event.key.code == Keyboard::Escape)
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

void menu_system::interact(world_handler &world, RenderWindow &window)
{
	was_active_ = false;

	const Vector2f mouse_pos = Vector2f(Mouse::getPosition());

	if (menu_state_ == main_menu)
	{
		if (button_list_[button_tag::newRunTag].is_selected(mouse_pos))
		{
			world.runWorldGenerator();
			menu_state_ = closed;
			was_active_ = true;
			return;
		}

		if (button_list_[button_tag::continueTag].is_selected(mouse_pos))
		{
			//world.Load();
            world.runWorldGenerator();
			menu_state_ = closed;
			was_active_ = true;
			return;
		}

		if (button_list_[button_tag::exitTag].is_selected(mouse_pos))
		{
			menu_state_ = closed;
			window.close();
			was_active_ = true;
			return;
		}

		return;
	}

	if (menu_state_ == game_menu)
	{
		if (button_list_[button_tag::newRunTag].is_selected(mouse_pos))
		{
			world.runWorldGenerator();
			menu_state_ = closed;
			was_active_ = true;
			return;
		}

		if (button_list_[button_tag::continueTag].is_selected(mouse_pos))
		{
			menu_state_ = closed;
			was_active_ = true;
			return;
		}

		if (button_list_[button_tag::exitTag].is_selected(mouse_pos))
		{
			world.Save();
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
	
