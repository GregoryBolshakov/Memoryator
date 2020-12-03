#pragma once

#include <memory>
#include <unordered_map>
#include <SFML/System/Vector2.hpp>

namespace sf
{
	class Event;
	class RenderWindow;
}
class world_handler;
class drawable_chain_element;
class button;
enum class button_tag;

enum menu_states { main_menu = 1, game_menu = 2, closed = 3 };

class menu_system
{
public:
	menu_system();
	~menu_system();
	void interact(const std::shared_ptr<world_handler>& world, const std::shared_ptr<sf::RenderWindow>& window);
	std::vector<std::unique_ptr<drawable_chain_element>> prepare_sprites();

	[[nodiscard]] menu_states get_state() const
	{ return menu_state_; }
	void set_state(const menu_states state) { menu_state_ = state; }
	void on_key_down(sf::Event event, const std::shared_ptr<world_handler>& world);

	[[nodiscard]] bool get_activity() const
	{ return was_active_; }
private:
	menu_states menu_state_ = main_menu;
	std::unordered_map<button_tag, button> button_list_;
	bool was_active_{};
	sf::Vector2f screen_size_;
};
