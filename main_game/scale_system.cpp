#include "scale_system.h"
#include "world_object.h"
#include "world_metrics.h"

#include <cmath>
#include <cassert>

template <typename T> int sgn(T val) {
	return (T(0) < val) - (val < T(0));
}

scale_system::scale_system() : zoom_factor_(world_metrics::furthest_factor)
{
}

float scale_system::calculate_scale()
{
	const auto focused_object_height = focused_object_.lock()->get_size().y;
	focused_to_screen_factor_ = world_metrics::window_size.y / (focused_objects_in_height * focused_object_height);
	return zoom_factor_ * focused_to_screen_factor_;
}

void scale_system::update_zoom_factor(const int delta)
{
	if (delta == -1 && zoom_factor_ > world_metrics::furthest_factor)
	{
		scale_decrease_ = -0.15f;
		scale_decrease_step_ = -0.01f;
	}
	else
		if (delta == 1 && zoom_factor_ < world_metrics::closest_factor)
		{
			scale_decrease_ = 0.15f;
			scale_decrease_step_ = 0.01f;
		}

	if (scale_decrease_ < 0 && zoom_factor_ < world_metrics::furthest_factor)
		zoom_factor_ = world_metrics::furthest_factor;
	if (scale_decrease_ > 0 && zoom_factor_ > world_metrics::closest_factor)
		zoom_factor_ = world_metrics::closest_factor;
}

void scale_system::interact()
{
	scale_smoothing();
	time_after_scale_decrease_ += scale_decrease_clock_.getElapsedTime().asMicroseconds();
	scale_decrease_clock_.restart();
	world_metrics::update_scale(calculate_scale());
	world_metrics::update_further(focused_to_screen_factor_);
}

void scale_system::scale_smoothing()
{
	if (scale_decrease_ == 0)
	{
		scale_decrease_step_ = 0;
		return;
	}

	const auto time_for_scale_decrease = 20000;
	if (time_after_scale_decrease_ < time_for_scale_decrease)
		return;

	if (zoom_factor_ + scale_decrease_step_ < world_metrics::furthest_factor || zoom_factor_ + scale_decrease_ + scale_decrease_step_ > world_metrics::closest_factor)
	{
		scale_decrease_ = 0;
		scale_decrease_step_ = 0;
		return;
	}

	zoom_factor_ += scale_decrease_step_;
	scale_decrease_ -= scale_decrease_step_;

	if (sgn(scale_decrease_) != sgn(scale_decrease_step_))
		scale_decrease_ = 0;

	time_after_scale_decrease_ = 0;
}
