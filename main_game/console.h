#pragma once

#include "input_box.h"
#include "time_system.h"
#include "world_handler.h"

using namespace sf;

class console
{
public:	
	console(FloatRect rect, shared_ptr<time_system> time_system, shared_ptr<world_handler> world);
	~console();
	input_box get_body() const;
	void draw(RenderWindow& window) const;
	void interact(long long elapsed_time);
	void handle_events(Event event);
	void reset_command_stack_iterator();;
	bool get_state() const;
private:
	void do_command();

	input_box body;
	bool state_ = false;
	std::vector<std::string> command_stack_ = { {""} };
	size_t command_stack_iterator_ = 0;
	shared_ptr<time_system> time_system_;
	shared_ptr<world_handler> world_;
};

