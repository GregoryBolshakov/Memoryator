#pragma once

#include <memory>
#include <SFML/System/Clock.hpp>

class world_object;

class scale_system
{
public:
	const float closest_scale = 1.7f;
	const float further_scale = 0.998f;
	
	scale_system() = default;
	~scale_system() = default;

	[[nodiscard]] float calculate_scale();
	void set_focus(std::weak_ptr<world_object> focused_object) { focused_object_ = std::move(focused_object); }
	void update_zoom_factor(int delta);
	
	void interact();
	[[nodiscard]] float scale_delta_normalized() const;
private:
	const float focused_objects_in_height = 5;
	std::weak_ptr<world_object> focused_object_;
	float zoom_factor_ = 1;
	float focused_to_screen_factor_{};
	float scale_decrease_ = 0;
	float time_for_scale_decrease_ = 0;
	sf::Clock scale_decrease_clock_;

	void scale_smoothing();
};
