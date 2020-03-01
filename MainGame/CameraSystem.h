#pragma once

#include <SFML/System/Vector2.hpp>

using Vector2f = sf::Vector2f;

class CameraSystem
{
public:
	CameraSystem();
	~CameraSystem();

	void makeShake(int count = 0, float speed = 0);
	void shakeInteract(long long elapsedTime);
	
	static const Vector2f maxCameraDistance, camOffset;
	static const float shakeDefaultSpeed;
	static const long long shakeDefaultDuration;
	Vector2f shakeVector = { 0, 0 };
	Vector2f position = { 0, 0 };
	long long shakeDuration = 0;
	float shakeSpeed = shakeDefaultSpeed;
	int shakeCount = 0;
};

