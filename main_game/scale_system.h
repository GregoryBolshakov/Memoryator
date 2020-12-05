#pragma once

#include <memory>
#include <SFML/System/Clock.hpp>

class world_object;

class scale_system
{
public:
	scale_system();
	~scale_system() = default;

	[[nodiscard]] float calculate_scale();
	void set_focus(std::weak_ptr<world_object> focused_object) { focused_object_ = std::move(focused_object); }
	void update_zoom_factor(int delta);
	
	void interact();
private:
	const float focused_objects_in_height = 4;
	std::weak_ptr<world_object> focused_object_;
	float zoom_factor_;
	float focused_to_screen_factor_ = 1;
	float scale_decrease_ = 0, scale_decrease_step_ = 0;
	float time_after_scale_decrease_ = 0;
	sf::Clock scale_decrease_clock_;

	void scale_smoothing();
};
