#pragma once
#ifndef CAMERASYSTEM_H
#define CAMERASYSTEM_H
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
	static const float shakeDefaultSpeed, shakeDefaultDuration;
	Vector2f shakeVector = { 0, 0 };
	Vector2f position = { 0, 0 };
	float shakeDuration = 0, shakeSpeed = shakeDefaultSpeed;
	int shakeCount = 0;
private:

};

#endif

