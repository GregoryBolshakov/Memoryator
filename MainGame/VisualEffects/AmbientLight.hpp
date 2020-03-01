#pragma once

#include "VisualEffect.hpp"

class AmbientLight final : public VisualEffect
{
public:
	explicit AmbientLight(sf::Vector2f render_target_size);

private:
	const float light_radius_ = 0.5f; // render target percent

	bool on_load() override;
	void on_update(float x_norm, float y_norm, long long) override;

	sf::Vector2f render_target_size_;
	sf::Glsl::Vec3 light_position_;
};