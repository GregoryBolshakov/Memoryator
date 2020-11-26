#pragma once

#include "input_box.h"

#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>

class time_system;
class world_handler;

class console
{
public:	
	console(sf::FloatRect rect, std::shared_ptr<time_system> time_system, std::shared_ptr<world_handler> world);
	~console();
	input_box get_body() const;
	void draw(const std::shared_ptr<sf::RenderWindow>& target) const;
	void interact(long long elapsed_time);
	void handle_events(sf::Event event);
	void reset_command_stack_iterator();;
	bool get_state() const;
private:
	void do_command();

	input_box body;
	bool state_ = false;
	std::vector<std::string> command_stack_ = { {""} };
	size_t command_stack_iterator_ = 0;
	std::shared_ptr<time_system> time_system_;
	std::shared_ptr<world_handler> world_;
};

