#include "dynamic_light.hpp"


dynamic_light::dynamic_light(camera_system& camera_system, time_system& time_system)
	:
	visual_effect("DynamicLight"),
	camera_system_{ camera_system },
	time_system_{ time_system }
{
}

bool dynamic_light::on_load()
{
	if (!shader_.loadFromFile("Game/shaders/dynamic_light.frag", Shader::Fragment))
		return false;

	if (!overlay_texture_.loadFromFile("Game/shaders/overlay.png"))
		return false;

	if (!multiply_texture_.loadFromFile("Game/shaders/mix.png"))
		return false;

	if (!field_of_view_texture_.loadFromFile("Game/shaders/vignette.png"))
		return false;

	shader_.setUniform("scene_tex", Shader::CurrentTexture);
	shader_.setUniform("overlay_tex", overlay_texture_);
	shader_.setUniform("multiply_tex", multiply_texture_);
	shader_.setUniform("fov_tex", field_of_view_texture_);
	shader_.setUniform("scene_size", camera_system::get_screen_size());

	return true;
}

void dynamic_light::on_update()
{
	const auto position = camera_system_.focused_object_screen_position_normalized();
	const Glsl::Vec3 view{
		position.x,
		position.y,
		camera_system_.get_scale_system()->scale_delta_normalized()
	};
	shader_.setUniform("view", view);
	shader_.setUniform("norm_time", time_system_.get_time_normalized());
}
