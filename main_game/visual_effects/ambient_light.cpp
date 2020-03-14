#include "ambient_light.hpp"

#include "../math_constants.h"


ambient_light::ambient_light(time_system& time_system, const sf::Vector2f render_target_size)
	: visual_effect("AmbientLight"),
	time_system_{ time_system },
	render_target_size_{ render_target_size }
{
}

bool ambient_light::on_load()
{
	if (!shader_.loadFromFile("Game/shaders/dynamic_light.frag", sf::Shader::Fragment))
		return false;

	if (!overlay_texture_.loadFromFile("Game/shaders/overlay.png"))
		return false;

	if (!multiply_texture_.loadFromFile("Game/shaders/mix.png"))
		return false;

	if (!field_of_view_texture_.loadFromFile("Game/shaders/vignette.png"))
		return false;
	
	shader_.setUniform("scene_tex", sf::Shader::CurrentTexture);
	shader_.setUniform("overlay_tex",overlay_texture_);
	shader_.setUniform("multiply_tex", multiply_texture_);
	shader_.setUniform("fov_tex", field_of_view_texture_);
	shader_.setUniform("screen_size", render_target_size_);

	return true;
}

void ambient_light::on_update()
{
	hero_view_.x = 0.5;
	hero_view_.y = 0.5;

	const auto t = time_system_.get_time_normalized();

	// produces 0.0 -> 1.0 -> 0.0 getting time values 0.0 -> 0.5 -> 1.0 
	const auto m = float((sin((t * 2.0 + 3.0 / 2.0) * pi) + 1.0) / 2.0); 

	hero_view_.z = variable_view_distance_ * m + constant_view_distance_;

	shader_.setUniform("hero", hero_view_);
	shader_.setUniform("norm_time", time_system_.get_time_normalized());
}
