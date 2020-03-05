#pragma once

#include "terrain_object.h"

class pedestal_controller
{
public:
	pedestal_controller();
	~pedestal_controller();
	void start(terrain_object* object);
	void stop();
	void draw(RenderWindow* window, Vector2f camera_position, float scale_factor);
	void handle_events(Event& event);
	void interact(long long elapsed_time, Event event);
	bool is_running() const { return running_; }
	bool ready_to_start = false;
	
private:
	bool running_ = false;
	terrain_object* bound_object_ = nullptr;
	Vector2f size_ = Vector2f(world_object::micro_block_size.x / 2, world_object::micro_block_size.y / 2);
	RectangleShape filed_figure_;
	CircleShape focus_figure_;
	CircleShape center_figure_;
	std::vector<Vector2f> focuses_;
	Vector2f camera_position_ = { 0, 0 };
	Vector2f last_mouse_pos_ = { 0, 0 };
	Vector2f center_position_ = { 0, 0 };
	float scale_factor_ = 1;
	long long double_click_timer_ = long(1e6);
	long long elapsed_time_ = 0;
	void write_to_file();
	int selected_focus_ = -1;
	int selected_ellipse_ = -1;
	bool selected_center_ = false;
};
