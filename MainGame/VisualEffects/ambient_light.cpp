#include "ambient_light.hpp"


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

	if (!light_texture_.loadFromFile("Game/shaders/circadian_light.png"))
		return false;

	shader_.setUniform("target_tex", sf::Shader::CurrentTexture);
	shader_.setUniform("screen_size", render_target_size_);
	shader_.setUniform("light_tex", light_texture_);
	shader_.setUniform("light_tex_size", sf::Glsl::Vec2(light_texture_.getSize()));

	return true;
}

void ambient_light::on_update()
{
	const auto a = time_system_.get_time_normalized();
	shader_.setUniform("norm_time", time_system_.get_time_normalized());
}
