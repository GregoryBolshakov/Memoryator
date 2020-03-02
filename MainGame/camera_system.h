#pragma once

#include <SFML/System/Vector2.hpp>

using Vector2f = sf::Vector2f;

class camera_system
{
public:
	camera_system();
	~camera_system();

	void make_shake(int count = 0, float speed = 0);
	void shake_interact(long long elapsed_time);

	static const Vector2f max_camera_distance;
	static const Vector2f cam_offset;
	static const float shake_default_speed;
	static const long long shake_default_duration;
	Vector2f shake_vector = { 0, 0 };
	Vector2f position = { 0, 0 };
	long long shake_duration = 0;
	float shake_speed = shake_default_speed;
	int shake_count = 0;
};

