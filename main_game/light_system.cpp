#include "light_system.h"


light_system::light_system()
= default;


light_system::~light_system()
= default;

void light_system::init(FloatRect visibleArea)
{
	this->visibleArea = visibleArea;
	circleCenter = { visibleArea.left + visibleArea.width / 2, visibleArea.top + visibleArea.height / 2 };
	calculationsRadius = sqrt(pow(visibleArea.width / 2, 2) + pow(visibleArea.height / 2, 2));
	calculationsCenter = circleCenter; calculationsCenter.y += calculationsRadius;
}

float light_system::calculateHorizontalOffset(float dayPart)
{
	if (dayPart <= 0.5f) // high noon
		return -calculationsRadius * (0.5f - dayPart) / 0.5f;
	return calculationsRadius * (dayPart - 0.5f) / 0.5f;
}

Vector2f light_system::calculateSunPosition(float dayPart)
{
	Vector2f result = calculationsCenter;
	const float horizontalOffset = calculateHorizontalOffset(dayPart);
	result.x += horizontalOffset;
	const float discriminant = 4 * pow(calculationsRadius, 2) - 4 * pow(horizontalOffset, 2);
	result.y -= (2 * calculationsRadius + sqrt(discriminant)) / 2;
	return result;
}
