#include "move_system.h"

#include <SFML/Graphics/Color.hpp>

#include "helper.h"

move_system::move_system()
= default;

move_system::~move_system()
= default;

void move_system::init(float* radius, Vector2f* position, sf::Color* color)
{
	this->radius = radius;
	this->position = position;
	this->color = color;
	turnedOn = true;
}

void move_system::pushByBumping(Vector2f position, float radius, bool canCrash)
{
	if (canCrashIntoDynamic || canCrash || pushDamage != 0)
		return;
	bumpedPositions.emplace_back(position, false);
	bumpDistance += radius;
}

void move_system::endPush(long long elapsedTime)
{
	pushDirection = { 0, 0 };
	pushDuration = 0;
	pushVector = { 0, 0 };
	pushDamage = 0;
	bumpedPositions.clear();
	bumpDistance = 0;
}

void move_system::pushAway(long long elapsedTime, float pushSpeed)
{
	if (pushSpeed == 0)
		pushSpeed = DEFAULT_PUSH_SPEED;

	const auto pushColor = sf::Color(255, 100, 100, 255);

	if (!bumpedPositions.empty() && this->canCrashIntoDynamic && pushDamage == 0)
	{
		Vector2f bumpCenter = { 0, 0 };
		auto bumpCount = 0;
		for (const auto bump : bumpedPositions)
		{
			bumpCount++;
			bumpCenter.x += bump.position.x;
			bumpCenter.y += bump.position.y;
		}
		if (bumpCount == 0 || pushSpeed == 0)
			return;
		bumpCenter.x /= float(bumpCount);
		bumpCenter.y /= float(bumpCount);
		pushDistance = (*radius + bumpDistance) - helper::getDist(*position, bumpCenter);
		pushDirection = Vector2f(position->x - bumpCenter.x, position->y - bumpCenter.y);
		pushDuration = long(pushDistance / pushSpeed);
		pushRestDuration = pushDuration;
		bumpDistance = 0;
		bumpedPositions.clear();
		*color = pushColor;
	}

	if (redRestDuration > 0)
		*color = sf::Color(
			255,
			Uint8(pushColor.g + (255 - pushColor.g) * (1 - redRestDuration / redDuration)),
			Uint8(pushColor.b + (255 - pushColor.b) * (1 - redRestDuration / redDuration)),
			255);
	else
	{
		*color = sf::Color::White;
		redRestDuration = 0;
	}

	redRestDuration -= elapsedTime;

	if (pushRestDuration <= 0 || !turnedOn)
	{
		endPush(elapsedTime);
		return;
	}

	pushRestDuration -= elapsedTime;

	const float elongationCoefficient = pushSpeed * float(elapsedTime) / sqrt(pow(pushDirection.x, 2) + pow(pushDirection.y, 2));
	if (pushDirection != Vector2f(0, 0))
		pushVector = { elongationCoefficient * pushDirection.x, elongationCoefficient * pushDirection.y };
	else
		pushVector = { 0, 0 };
}
