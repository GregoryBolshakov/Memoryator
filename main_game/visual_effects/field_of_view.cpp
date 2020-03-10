#include "field_of_view.hpp"

#include "../math_constants.h"

field_of_view::field_of_view(time_system& time_system, const sf::Vector2f render_target_size)
	: visual_effect("FieldOfView"),
	time_system_{ time_system },
	render_target_size_{ render_target_size }
{
}

bool field_of_view::on_load()
{
	if (!shader_.loadFromFile("Game/shaders/field_of_view.frag", sf::Shader::Fragment))
		return false;

	shader_.setUniform("tex", sf::Shader::CurrentTexture);
	shader_.setUniform("screen_size", render_target_size_);

	return true;
}

void field_of_view::on_update()
{
	hero_view_.x = render_target_size_.x / 2;
	hero_view_.y = render_target_size_.y / 2;

	const auto t = time_system_.get_time_normalized();

	const auto m = float((sin((t * 2.0 + 3.0 / 2.0) * pi) + 1.0) / 2.0); // 0.0 -> 1.0 -> 0.0
	hero_view_.z = variable_view_distance_ * m + constant_view_distance_;

	shader_.setUniform("hero", hero_view_);
}
