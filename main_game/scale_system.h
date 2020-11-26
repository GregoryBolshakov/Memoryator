#pragma once

#include <SFML/System/Clock.hpp>

class scale_system
{
public:
	const float closest_scale = 1.7f;
	const float further_scale = 0.998f;
	
	scale_system();

	[[nodiscard]] float get_scale_factor() const;
	void set_scale_factor(int delta);

	[[nodiscard]] float get_main_scale() const;
	
	void interact();
	[[nodiscard]] float scale_delta_normalized() const;
private:
	float scale_factor_ = 1;
	float main_scale_ = 1;	
	float scale_decrease_{};
	float time_for_scale_decrease_ = 0;
	sf::Clock scale_decrease_clock_;

	void scale_smoothing();
};
