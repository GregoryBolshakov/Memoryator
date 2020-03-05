#include "camera_system.h"

#include <cstdlib>

const Vector2f camera_system::max_camera_distance = Vector2f(250, 250), camera_system::cam_offset = { 0, -0.04f };
const float camera_system::shake_default_speed = 0.0005f;
const long long camera_system::shake_default_duration = long(3e4);

camera_system::camera_system()
= default;

camera_system::~camera_system()
= default;

void camera_system::make_shake(const int count, const float speed)
{
	if (count == 0)
		return;

	shake_count = count;

	if (speed == 0)
		shake_speed = shake_default_speed;
	else
		shake_speed = speed;

	shake_vector = Vector2f(float(rand() % 60 - 30), float(rand() % 60 - 30));

	if (shake_vector == Vector2f(0, 0))
		shake_vector = { 1, 1 };
	
	shake_duration = shake_default_duration;
}

void camera_system::shake_interact(long long elapsed_time)
{
	if (shake_duration > 0)
	{
		const float k = shake_speed * float(elapsed_time) / float(sqrt(pow(shake_vector.x, 2) + pow(shake_vector.y, 2)));
		position.x += shake_vector.x * k; position.y += shake_vector.y * k;
		shake_duration -= elapsed_time;
	}
	else
	{
		shake_duration = 0;
		if (shake_count > 0)
			make_shake(--shake_count, shake_speed);
	}
}

