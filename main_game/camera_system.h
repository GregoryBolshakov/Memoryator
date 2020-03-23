#pragma once

#include <SFML/System/Vector2.hpp>

#include "dynamic_object.h"
#include "scale_system.h"

using Vector2f = sf::Vector2f;

class camera_system
{
public:
	static const Vector2f max_camera_distance;
	static const Vector2f cam_offset;
	static const float shake_default_speed;
	static const long long shake_default_duration;

	explicit camera_system(scale_system& scale_system);

	Vector2f position = { 0, 0 };

	void set_focused_object(dynamic_object* object);
	scale_system& get_scale_system();
	static Vector2f get_screen_size();

	void make_shake(int count = 0, float speed = 0);
	void shake_interact(long long elapsed_time);
	[[nodiscard]] Vector2f focused_object_screen_position_normalized() const;
	Vector2f object_screen_position(dynamic_object* obj) const;

private:
	scale_system& scale_system_;
	dynamic_object* focused_object_{};
	Vector2f shake_vector_ = { 0, 0 };
	long long shake_duration_ = 0;
	float shake_speed_ = shake_default_speed;
	int shake_count_ = 0;
};
