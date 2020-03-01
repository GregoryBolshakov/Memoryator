#include "AmbientLight.hpp"


AmbientLight::AmbientLight(const sf::Vector2f render_target_size)
	: VisualEffect("AmbientLight"), render_target_size_{ render_target_size }
{
	light_position_.z = light_radius_;
}

bool AmbientLight::on_load()
{
	if (!shader_.loadFromFile("Game/shaders/dynamic_light.frag", sf::Shader::Fragment))
		return false;

	shader_.setUniform("screen_size", render_target_size_);

	return true;
}

void AmbientLight::on_update(const float x_norm, const float y_norm, long long)
{
	light_position_.x = x_norm;
	light_position_.y = y_norm;
	
	shader_.setUniform("light", light_position_);
}
