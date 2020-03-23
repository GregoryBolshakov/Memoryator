#include "scale_system.h"

#include <cmath>


#include "deerchant.h"
#include "helper.h"

scale_system::scale_system()
{
	main_scale_ = helper::GetScreenSize().y / (5 * deerchant::conditional_size.y);
	main_scale_ = round(main_scale_ * 100) / 100;
	scale_factor_ = main_scale_;
}

float scale_system::get_scale_factor() const
{
	return scale_factor_;
}

void scale_system::set_scale_factor(const int delta)
{
	if (delta == -1 && scale_factor_ > further_scale* main_scale_)
	{
		scale_factor_ -= 0.01f;
		scale_decrease_ = -0.03f;
	}
	else
		if (delta == 1 && scale_factor_ < closest_scale * main_scale_)
		{
			scale_factor_ += 0.01f;
			scale_decrease_ = 0.03f;
		}

	if (scale_decrease_ < 0 && scale_factor_ < further_scale * main_scale_)
		scale_factor_ = further_scale * main_scale_;
	if (scale_decrease_ > 0 && scale_factor_ > closest_scale* main_scale_)
		scale_factor_ = closest_scale * main_scale_;
}

float scale_system::get_main_scale() const
{
	return main_scale_;
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
		if (scale_factor_ != further_scale * main_scale_ && scale_factor_ != closest_scale * main_scale_)
			scale_factor_ += scale_decrease_;
		if (scale_decrease_ < 0 && scale_factor_ <= further_scale * main_scale_)
			scale_factor_ = further_scale * main_scale_;
		if (scale_decrease_ > 0 && scale_factor_ >= closest_scale * main_scale_)
			scale_factor_ = closest_scale * main_scale_;

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
	return (scale_factor_ - main_scale_) / main_scale_;
}
