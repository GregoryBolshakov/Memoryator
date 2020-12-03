#pragma once

#include "world_metrics.h"
#include "shape_chain_element.h"

#include <memory>
#include <vector>
#include <functional>

namespace sf
{
	class RenderWindow;
	class Event;
}
class world_object;
class terrain_object;

class pedestal_controller
{
public:
	pedestal_controller();
	~pedestal_controller();
	void start(std::weak_ptr<terrain_object> object);
	void stop();
	std::vector<std::unique_ptr<drawable_chain_element>> prepare_sprites();
	void handle_events(sf::Event& event);
	void interact(long long elapsed_time);
	bool is_running() const { return running_; }
	bool ready_to_start = false;
	
private:
	bool running_ = false;
	std::weak_ptr<terrain_object> bound_object_;
	sf::Vector2f size_ = world_metrics::route_block_size;
	shape_chain_element filed_figure_, focus_figure_, center_figure_;
	std::vector<sf::Vector2f> focuses_;
	sf::Vector2f last_mouse_pos_;
	sf::Vector2f center_position_;
	long long double_click_timer_ = 1000000;
	long long elapsed_time_ = 0;
	void write_to_file();
	int selected_focus_ = -1;
	int selected_ellipse_ = -1;
	bool selected_center_ = false;
};
