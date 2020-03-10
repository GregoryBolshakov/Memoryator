#pragma once

#include "../time_system.h"
#include "visual_effect.hpp"

class field_of_view final : public visual_effect
{
public:
	explicit field_of_view(time_system& time_system, sf::Vector2f render_target_size);

private:
	const float variable_view_distance_ = 0.55F;
	const float constant_view_distance_ = 0.3F;
	
	bool on_load() override;
	void on_update() override;

	time_system& time_system_;
	sf::Vector2f render_target_size_;
	sf::Glsl::Vec3 hero_view_;
};
