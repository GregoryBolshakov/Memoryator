#include "camera_system.h"

#include <cstdlib>

const Vector2f camera_system::maxCameraDistance = Vector2f(250, 250), camera_system::camOffset = { 0, -0.04f };
const float camera_system::shakeDefaultSpeed = 0.0005f;
const long long camera_system::shakeDefaultDuration = long(3e4);

camera_system::camera_system()
= default;

camera_system::~camera_system()
= default;

void camera_system::makeShake(const int count, const float speed)
{
	if (count == 0)
		return;

	shakeCount = count;

	if (speed == 0)
		shakeSpeed = shakeDefaultSpeed;
	else
		shakeSpeed = speed;

	shakeVector = Vector2f(float(rand() % 60 - 30), float(rand() % 60 - 30));

	if (shakeVector == Vector2f(0, 0))
		shakeVector = { 1, 1 };
	
	shakeDuration = shakeDefaultDuration;
}

void camera_system::shakeInteract(long long elapsedTime)
{
	if (shakeDuration > 0)
	{
		const float k = shakeSpeed * float(elapsedTime) / float(sqrt(pow(shakeVector.x, 2) + pow(shakeVector.y, 2)));
		position.x += shakeVector.x * k; position.y += shakeVector.y * k;
		shakeDuration -= elapsedTime;
	}
	else
	{
		shakeDuration = 0;
		if (shakeCount > 0)
			makeShake(--shakeCount, shakeSpeed);
	}
}

