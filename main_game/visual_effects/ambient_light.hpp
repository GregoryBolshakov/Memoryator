#pragma once

#include "../time_system.h"
#include "visual_effect.hpp"

class ambient_light final : public visual_effect
{
public:
	explicit ambient_light(time_system& time_system, sf::Vector2f render_target_size);

private:
	bool on_load() override;
	void on_update() override;

	time_system& time_system_;
	sf::Vector2f render_target_size_;
	sf::Texture light_texture_;
};
