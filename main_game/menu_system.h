#pragma once

#include <SFML/Graphics.hpp>

#include "button.h"
#include "helper.h"
#include "world_handler.h"

using namespace sf;

enum menu_states { main_menu = 1, game_menu = 2, closed = 3 };

class menu_system
{
public:
	menu_system();
	~menu_system();
	void interact(const shared_ptr<world_handler>& world, RenderWindow &window);
	std::vector<unique_ptr<drawable_chain_element>> prepare_sprites();

	[[nodiscard]] menu_states get_state() const
	{ return menu_state_; }
	void set_state(menu_states state) { menu_state_ = state; }
	void on_key_down(Event event, const shared_ptr<world_handler>& world);

	[[nodiscard]] bool get_activity() const
	{ return was_active_; }
private:
	menu_states menu_state_ = main_menu;
	std::unordered_map<button_tag, button> button_list_;
	std::string buttons_info_file_directory_ = "Game/MenuSystem/buttonsInfo.txt";
	void init_buttons();
	bool was_active_{};
	Vector2f screen_size_;
};
