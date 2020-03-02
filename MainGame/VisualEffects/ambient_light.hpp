#pragma once

#include "visual_effect.hpp"

class ambient_light final : public visual_effect
{
public:
	explicit ambient_light(sf::Vector2f render_target_size);

private:
	const float light_radius_ = 0.5f; // render target percent

	bool on_load() override;
	void on_update(float x_norm, float y_norm, long long) override;

	sf::Vector2f render_target_size_;
	sf::Glsl::Vec3 light_position_;
};