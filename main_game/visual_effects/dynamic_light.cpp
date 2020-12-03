#include "dynamic_light.hpp"
#include "../world_metrics.h"
#include "../scale_system.h"


dynamic_light::dynamic_light(camera_system& camera_system, time_system& time_system) :
	  visual_effect("DynamicLight")
	, camera_system_{ camera_system }
	, time_system_{ time_system }
{
}

bool dynamic_light::on_load()
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
	shader_.setUniform("overlay_tex", overlay_texture_);
	shader_.setUniform("multiply_tex", multiply_texture_);
	shader_.setUniform("fov_tex", field_of_view_texture_);
	shader_.setUniform("scene_size", world_metrics::window_size);

	return true;
}

void dynamic_light::on_update()
{
	const auto position = camera_system_.focused_object_screen_position_normalized();
	const sf::Glsl::Vec3 view{
		position.x,
		position.y,
		camera_system_.get_scale_system()->calculate_scale()
	};
	shader_.setUniform("view", view);
	shader_.setUniform("norm_time", time_system_.get_time_normalized());
}
