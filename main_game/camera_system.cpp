#include "camera_system.h"
#include "scale_system.h"
#include "world_object.h"
#include "world_metrics.h"

#include <cstdlib>

const sf::Vector2f camera_system::max_camera_distance = sf::Vector2f(250, 250), camera_system::cam_offset = { 0, -0.04f };
const float camera_system::shake_default_speed = 0.0005f;
const long long camera_system::shake_default_duration = 30000;

camera_system::camera_system() : scale_system_(std::make_shared<scale_system>())
{
}

void camera_system::interact(const long long elapsed_time)
{
	//position = focused_object_.lock()->get_position();
	shake_interact(elapsed_time);
}

void camera_system::make_shake(const int count, const float speed)
{
	if (count == 0)
		return;

	shake_count_ = count;

	if (speed == 0)
		shake_speed_ = shake_default_speed;
	else
		shake_speed_ = speed;

	shake_vector_ = sf::Vector2f(float(rand() % 60 - 30), float(rand() % 60 - 30));

	if (shake_vector_ == sf::Vector2f(0, 0))
		shake_vector_ = { 1, 1 };

	shake_duration_ = shake_default_duration;
}

void camera_system::shake_interact(const long long elapsed_time)
{
	if (shake_duration_ > 0)
	{
		const auto k = shake_speed_ * float(elapsed_time) / float(sqrt(pow(shake_vector_.x, 2) + pow(shake_vector_.y, 2)));
		//position.x += shake_vector_.x * k; position.y += shake_vector_.y * k;
		shake_duration_ -= elapsed_time;
	}
	else
	{
		shake_duration_ = 0;
		if (shake_count_ > 0)
			make_shake(--shake_count_, shake_speed_);
	}
}

void camera_system::set_focus(std::weak_ptr<world_object> focus)
{
	focused_object_ = std::move(focus);
	scale_system_->set_focus(focused_object_);
	//position = focused_object_.lock()->get_position();
}

sf::Vector2f camera_system::focused_object_screen_position_normalized() const
{
	const auto size = world_metrics::window_size;
	const auto position = world_metrics::screen_to_world_position(focused_object_.lock()->get_position());
	const auto result = sf::Vector2f{ position.x / size.x, position.y / size.y };

	return result;
}
