#include "scale_system.h"
#include "world_object.h"
#include "world_metrics.h"

#include <cmath>
#include <cassert>

float scale_system::calculate_scale()
{
	assert(focused_object_.lock()->get_conditional_size_units() != sf::Vector2f());
	focused_to_screen_factor_ = world_metrics::window_size.y / (focused_objects_in_height * focused_object_.lock()->get_conditional_size_units().y);
	return zoom_factor_ * focused_to_screen_factor_;
}

void scale_system::update_zoom_factor(const int delta)
{
	if (delta == -1 && zoom_factor_ > further_scale* focused_to_screen_factor_)
	{
		zoom_factor_ -= 0.01f;
		scale_decrease_ = -0.03f;
	}
	else
		if (delta == 1 && zoom_factor_ < closest_scale * focused_to_screen_factor_)
		{
			zoom_factor_ += 0.01f;
			scale_decrease_ = 0.03f;
		}

	if (scale_decrease_ < 0 && zoom_factor_ < further_scale * focused_to_screen_factor_)
		zoom_factor_ = further_scale * focused_to_screen_factor_;
	if (scale_decrease_ > 0 && zoom_factor_ > closest_scale* focused_to_screen_factor_)
		zoom_factor_ = closest_scale * focused_to_screen_factor_;
}

void scale_system::interact()
{
	scale_smoothing();
	time_for_scale_decrease_ += scale_decrease_clock_.getElapsedTime().asMicroseconds();
	scale_decrease_clock_.restart();
}

void scale_system::scale_smoothing()
{
	if (abs(scale_decrease_) >= 0.02 && time_for_scale_decrease_ >= 30000)
	{
		if (zoom_factor_ != further_scale * focused_to_screen_factor_ && zoom_factor_ != closest_scale * focused_to_screen_factor_)
			zoom_factor_ += scale_decrease_;
		if (scale_decrease_ < 0 && zoom_factor_ <= further_scale * focused_to_screen_factor_)
			zoom_factor_ = further_scale * focused_to_screen_factor_;
		if (scale_decrease_ > 0 && zoom_factor_ >= closest_scale * focused_to_screen_factor_)
			zoom_factor_ = closest_scale * focused_to_screen_factor_;

		if (scale_decrease_ < 0)
		{
			scale_decrease_ += 0.001f;
		}
		else
			if (scale_decrease_ > 0)
			{
				scale_decrease_ -= 0.001f;
			}


		time_for_scale_decrease_ = 0;
	}
}

float scale_system::scale_delta_normalized() const
{
	return (zoom_factor_ - focused_to_screen_factor_) / focused_to_screen_factor_;
}
