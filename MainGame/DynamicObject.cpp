#include "DynamicObject.h"
#include "Helper.h"

DynamicObject::DynamicObject(std::string objectName, const Vector2f centerPosition) : WorldObject(std::move(objectName), centerPosition), currentAction()
{
	speed = 0;
	direction = Direction::STAND;
}

DynamicObject::~DynamicObject()
= default;

void DynamicObject::handleInput(bool usedMouse)
{
}

void DynamicObject::initMicroBlocks()
{
	lockedMicroBlocks = { Vector2i(int(ceil(position.x / microBlockSize.x)), int(ceil(position.y / microBlockSize.y))) };
}

Side DynamicObject::calculateSide(const Vector2f otherObjectPosition) const
{
	auto answer = down;

	const auto alpha = atan((float(otherObjectPosition.y) - this->position.y) / (float(otherObjectPosition.x) - this->position.x)) * 180 / pi;
	if (this->position.y >= otherObjectPosition.y && abs(alpha) >= 45 && abs(alpha) <= 90)
		answer = up;
	else
		if (this->position.x <= otherObjectPosition.x && abs(alpha) >= 0 && abs(alpha) <= 45)
			answer = right;
		else
			if (this->position.y <= otherObjectPosition.y && abs(alpha) >= 45 && abs(alpha) <= 90)
				answer = down;
			else
				if (this->position.x >= otherObjectPosition.x && abs(alpha) >= 0 && abs(alpha) <= 45)
					answer = left;
	return answer;
}

void DynamicObject::calculateDirection()
{
	const Vector2f curPos = this->getPosition();
	const Vector2f tarPos = movePosition;

	if (tarPos == Vector2f(-1, -1))
		direction = Direction::STAND;

	const float directionCorridor = 50;

	if (abs(curPos.x - tarPos.x) <= directionCorridor && abs(curPos.y - tarPos.y) <= directionCorridor)
	{
		if (abs(curPos.x - tarPos.x) > abs(curPos.y - tarPos.y))
		{
			if (curPos.x >= tarPos.x)
				direction = Direction::LEFT;
			else
				direction = Direction::RIGHT;
		}
		else
		{
			if (curPos.y >= tarPos.y)
				direction = Direction::UP;
			else
				direction = Direction::DOWN;
		}
		lastDirection = direction;
		return;
	}

	if (abs(curPos.x - tarPos.x) <= directionCorridor)
	{
		if (curPos.y < tarPos.y)
			direction = Direction::DOWN;
		else
			direction = Direction::UP;
	}
	else
		if (abs(curPos.y - tarPos.y) <= directionCorridor)
		{
			if (curPos.x < tarPos.x)
				direction = Direction::RIGHT;
			else
				direction = Direction::LEFT;
		}
		else
			if (curPos.x < tarPos.x)
			{
				if (curPos.y < tarPos.y)
					direction = Direction::DOWNRIGHT;
				else
					if (curPos.y > tarPos.y)
						direction = Direction::UPRIGHT;
			}
			else
				if (curPos.x > tarPos.x)
				{
					if (curPos.y < tarPos.y)
						direction = Direction::DOWNLEFT;
					else
						if (curPos.y > tarPos.y)
							direction = Direction::UPLEFT;
				}

	if (direction != Direction::STAND)
		lastDirection = direction;
}

Side DynamicObject::invertSide(const Side side)
{
	switch (side)
	{
	case up:
		return down;
	case down:
		return up;
	case left:
		return right;
	case right:
		return left;
	default:
		return down;
	}
}

bool DynamicObject::isIntersectDynamic(const Vector2f newPosition, DynamicObject & otherDynamic) const
{
	//Vector2f position1 = dynamic1.getPosition();
	const auto position1 = newPosition;
	const auto position2 = otherDynamic.getPosition();
	//return (abs(position1.x - position2.x) <= (this->getRadius() + otherDynamic.getRadius()) && abs(position1.y - position2.y) <= (this->getRadius() + otherDynamic.getRadius()));
	return sqrt(pow(position1.x - position2.x, 2) + pow(position1.y - position2.y, 2)) <= /*this->radius + */otherDynamic.radius;
}

Vector2f DynamicObject::EllipseSlip(
	DynamicObject * dynamic,
	const Vector2f newPos,
	const Vector2f destination,
	const Vector2f f1,
	const Vector2f f2,
	const float ellipseSize,
	const float height,
	const long long elapsedTime) const
{
	const auto dynamicPos = Vector2f(dynamic->getPosition().x, height - dynamic->getPosition().y);
	const auto focus1 = Vector2f(f1.x, height - f1.y);
	const auto focus2 = Vector2f(f2.x, height - f2.y);
	const auto newPosition = Vector2f(newPos.x, height - newPos.y);
	const auto destinationPos = Vector2f(destination.x, height - destination.y);

	const auto a1 = newPosition.x - dynamicPos.x;
	const auto b1 = newPosition.y - dynamicPos.y;
	const auto c1 = -a1 * newPosition.x - b1 * newPosition.y;

	const auto l = (ellipseSize - abs(focus2.x - focus1.x)) / 2;
	const auto a2 = ellipseSize / 2;
	const auto b2 = sqrt(pow((2 * l + abs(focus2.x - focus1.x)) / 2, 2) - pow(abs(focus2.x - focus1.x) / 2, 2));
	const auto x0 = (focus1.x + focus2.x) / 2;
	const auto y0 = (focus1.y + focus2.y) / 2;

	float x1, yl, x2, y2, d;

	if (a1 == 0 && b1 != 0)
	{
		d = pow(2 * x0 * b1 * b1 * b2 * b2, 2) - 4 * (b1 * b1 * b2 * b2) * (x0 * x0 * b1 * b1 * b2 * b2 + c1 * c1 * a2 * a2 + y0 * y0 * a2 * a2 * b1 * b1 + 2 * c1 * y0 * a2 * a2 * b1 - a2 * a2 * b1 * b1 * b2 * b2);

		if (b1 * b1 * b2 * b2 == 0 || d < 0)
			return { -1, -1 };

		x1 = -((-(2 * x0 * b1 * b1 * b2 * b2) + sqrt(d)) / (2 * (b1 * b1 * b2 * b2)));
		x2 = -((-(2 * x0 * b1 * b1 * b2 * b2) - sqrt(d)) / (2 * (b1 * b1 * b2 * b2)));
		yl = -c1 / b1;
		y2 = -c1 / b1;
	}
	else
		if (a1 != 0 && b1 == 0)
		{
			d = pow(-2 * y0 * a1 * a1 * a2 * a2, 2) - 4 * (a1 * a1 * a2 * a2) * (c1 * c1 * b2 * b2 + x0 * x0 * a1 * a1 * b2 * b2 + 2 * c1 * x0 * a1 * b2 * b2 + y0 * y0 * a1 * a1 * a2 * a2 - a1 * a1 * a2 * a2 * b2 * b2);

			if (a1 * a1 * a2 * a2 == 0 || d < 0)
				return { -1, -1 };

			yl = (2 * y0 * a1 * a1 * a2 * a2 + sqrt(d)) / (2 * a1 * a1 * a2 * a2);
			y2 = (2 * y0 * a1 * a1 * a2 * a2 - sqrt(d)) / (2 * a1 * a1 * a2 * a2);
			x1 = -c1 / a1;
			x2 = -c1 / a1;
		}
		else
			if (a1 != 0 && b1 != 0)
			{
				d = pow(2 * b1 * b2 * b2 * c1 + 2 * a1 * b1 * b2 * b2 * x0 - 2 * a1 * a1 * a2 * a2 * y0, 2) - 4 * (b1 * b1 * b2 * b2 + a1 * a1 * a2 * a2) * (b2 * b2 * c1 * c1 + a1 * a1 * b2 * b2 * x0 * x0 + 2 * a1 * b2 * b2 * c1 * x0 + a1 * a1 * a2 * a2 * y0 * y0 - a1 * a1 * a2 * a2 * b2 * b2);

				if (b1 * b1 * b2 * b2 + a1 * a1 * a2 * a2 == 0 || d < 0)
					return { -1, -1 };

				yl = (-(2 * b1 * b2 * b2 * c1 + 2 * a1 * b1 * b2 * b2 * x0 - 2 * a1 * a1 * a2 * a2 * y0) + sqrt(d)) / (2 * (b1 * b1 * b2 * b2 + a1 * a1 * a2 * a2));
				y2 = (-(2 * b1 * b2 * b2 * c1 + 2 * a1 * b1 * b2 * b2 * x0 - 2 * a1 * a1 * a2 * a2 * y0) - sqrt(d)) / (2 * (b1 * b1 * b2 * b2 + a1 * a1 * a2 * a2));
				x1 = -1 * (b1 * yl + c1) / a1;
				x2 = -1 * (b1 * y2 + c1) / a1;
			}
			else
				return { -1, -1 };

	float dist1, dist2;
	const auto time = float(elapsedTime);

	if (destination == Vector2f(-1, -1))
		dist1 = Helper::getDist(newPosition, Vector2f(x1, yl)), dist2 = Helper::getDist(newPosition, Vector2f(x2, y2));
	else
		dist1 = Helper::getDist(destinationPos, Vector2f(x1, yl)), dist2 = Helper::getDist(destinationPos, Vector2f(x2, y2));

	if (dist1 <= dist2 && d >= 0 && sqrt(pow(x1 - dynamic->getPosition().x, 2) + pow(height - yl - dynamic->getPosition().y, 2)) != 0)
	{
		const auto k = dynamic->getSpeed() * time / sqrt(pow(x1 - dynamic->getPosition().x, 2) + pow(height - yl - dynamic->getPosition().y, 2));
		return { (x1 - dynamic->getPosition().x) * k, (height - yl - dynamic->getPosition().y) * k };
	}
	if (dist2 <= dist1 && d >= 0 && sqrt(pow(x2 - dynamic->getPosition().x, 2) + pow(height - y2 - dynamic->getPosition().y, 2)) != 0)
	{
		const auto k = dynamic->getSpeed() * time / sqrt(pow(x2 - dynamic->getPosition().x, 2) + pow(height - y2 - dynamic->getPosition().y, 2));
		return { (x2 - dynamic->getPosition().x) * k, (height - y2 - dynamic->getPosition().y) * k };
	}

	return { -1, -1 };
}

//void DynamicObject::setMoveOffset(const long long elapsedTime)
//{
//	if (!(currentAction == move || currentAction == moveHit || currentAction == moveEnd || currentAction == jerking || currentAction == throwNoose))
//	{
//		moveOffset = { -1, -1 };
//		return;
//	}
//
//	const auto time = float(elapsedTime);
//
//	if (movePosition == Vector2f(-1, -1))
//	{
//		if (direction != Direction::STAND)
//		{
//			const auto angle = float(this->getDirection()) * pi / 180;
//			const auto k = speed * time / sqrt(pow(cos(angle), 2) + pow(sin(angle), 2));
//			moveOffset.x = float(k * cos(angle));
//			moveOffset.y = float(k * -sin(angle));
//			return;
//		}
//		moveOffset = Vector2f(-1, -1);
//		return;
//	}
//
//	const auto distanceToTarget = float(sqrt(pow(movePosition.x - position.x, 2) + pow(movePosition.y - position.y, 2)));
//	if (distanceToTarget == 0)
//	{
//		moveOffset = Vector2f(-1, -1);
//		return;
//	}
//
//	const float k = speed * time / sqrt(pow(movePosition.x - position.x, 2) + pow(movePosition.y - position.y, 2));
//	/*if (distanceToTarget <= k)
//	{
//		moveOffset = Vector2f(-1, -1);
//		return;
//	}*/
//	moveOffset = { (movePosition.x - position.x) * k, (movePosition.y - position.y) * k };
//}

void DynamicObject::setMoveOffset(long long elapsedTime)
{
	if (!(currentAction == move || currentAction == moveHit || currentAction == moveEnd || currentAction == jerking || currentAction == throwNoose))
	{
		moveOffset = { -1, -1 };
		return;
	}

	if (movePosition == Vector2f(-1, -1))
	{
		if (direction != Direction::STAND)
		{
			auto angle = int(this->getDirection()) * pi / 180;
			const float k = speed * elapsedTime / sqrt(pow(cos(angle), 2) + pow(sin(angle), 2));
			moveOffset.x = float(k * cos(angle));
			moveOffset.y = float(k * -sin(angle));
			return;
		}
		moveOffset = Vector2f(-1, -1);
		return;
	}

	const float distanceToTarget = float(sqrt(pow(movePosition.x - position.x, 2) + pow(movePosition.y - position.y, 2)));
	if (distanceToTarget == 0)
	{
		moveOffset = Vector2f(-1, -1);
		return;
	}

	const float k = speed * elapsedTime / sqrt(pow(movePosition.x - position.x, 2) + pow(movePosition.y - position.y, 2));
	/*if (distanceToTarget <= k)
	{
		moveOffset = Vector2f(-1, -1);
		return;
	}*/
	moveOffset = { (movePosition.x - position.x) * k, (movePosition.y - position.y) * k };
}

Vector2f DynamicObject::doMove(long long elapsedTime)
{
	setMoveOffset(elapsedTime);
	auto position = this->position;
	position.x += pushVector.x; position.y += pushVector.y;

	//if (this->direction == Direction::STAND)
		//return position;

	if (moveOffset != Vector2f(-1, -1))
	{
		position.x += moveOffset.x;
		position.y += moveOffset.y;
	}

	return Vector2f(position);
}

Vector2f DynamicObject::doSlip(Vector2f newPosition, const std::vector<StaticObject*> & localStaticItems, const float height, const long long elapsedTime)
{
	bool crashed = false;
	if (!canCrashIntoStatic)
		return newPosition;

	for (auto& staticItem : localStaticItems)
	{
		auto terrain = dynamic_cast<TerrainObject*>(staticItem);
		if (!terrain || staticItem->isBackground || staticItem->getRadius() == 0)
			continue;

		if (tag != Tag::hero && staticItem->isMultiEllipse)
			continue;

		if (terrain->isMultiEllipse)
		{
			auto curEllipses = terrain->getMultiellipseIntersect(newPosition);
			Vector2f motionAfterSlipping;

			if (!curEllipses.empty())
				for (auto& curEllipse : curEllipses)
				{
					if (crashed)
						return Vector2f(-1, -1);

					crashed = true;
					motionAfterSlipping = this->EllipseSlip(this, newPosition, this->movePosition, terrain->internalEllipses[curEllipse].first, terrain->internalEllipses[curEllipse].second, terrain->getEllipseSize(curEllipse), height, elapsedTime);

					if (motionAfterSlipping != Vector2f(-1, -1))
					{
						newPosition = Vector2f(this->getPosition().x + motionAfterSlipping.x, this->getPosition().y + motionAfterSlipping.y);
						continue;
					}

					return Vector2f(-1, -1);
				}
		}
		else
			if (terrain->isIntersected(this->getPosition(), newPosition))
			{
				if (crashed)
					return Vector2f(-1, -1);

				crashed = true;
				Vector2f motionAfterSlipping;

				if (staticItem->isDotsAdjusted)
					motionAfterSlipping = terrain->newSlippingPositionForDotsAdjusted(moveOffset, this->getSpeed(), elapsedTime);
				else
					motionAfterSlipping = this->EllipseSlip(this, newPosition, this->movePosition, terrain->getFocus1(), terrain->getFocus2(), terrain->getEllipseSize(), height, elapsedTime);

				if (motionAfterSlipping != Vector2f(-1, -1))
				{
					newPosition = Vector2f(this->getPosition().x + motionAfterSlipping.x, this->getPosition().y + motionAfterSlipping.y);
					continue;
				}

				return Vector2f(-1, -1);
			}
	}

	return newPosition;
}

Vector2f DynamicObject::doSlipOffDynamic(Vector2f newPosition, const std::vector<DynamicObject*> & localDynamicItems, const float height, const long long elapsedTime)
{
	if (!canCrashIntoDynamic)
		return newPosition;

	lastIntersected = "";

	for (auto& otherDynamicItem : localDynamicItems)
	{
		if (otherDynamicItem == this)
			continue;

		if (this->isIntersectDynamic(newPosition, *otherDynamicItem) && otherDynamicItem->lastIntersected != name)
		{
			this->lastIntersected = otherDynamicItem->getName();
			auto motionAfterSlipping = EllipseSlip(this, newPosition, movePosition, otherDynamicItem->getPosition(), otherDynamicItem->getPosition(), otherDynamicItem->getRadius() * 2, height, elapsedTime);

			if (motionAfterSlipping != Vector2f(-1, -1))
			{
				newPosition = Vector2f(position.x + motionAfterSlipping.x, position.y + motionAfterSlipping.y);
				continue;
			}

			return Vector2f(-1, -1);
		}
	}

	return newPosition;
}

void DynamicObject::changeAction(const Actions newAction, const bool resetSpriteNumber, const bool rememberLastAction)
{
	if (rememberLastAction)
		lastAction = currentAction;

	currentAction = newAction;

	if (resetSpriteNumber)
		for (auto& number : currentSprite)
			number = 1;
}

void DynamicObject::pushByBumping(DynamicObject* object)
{
	if (!this->canCrashIntoDynamic || !object->canCrashIntoDynamic || pushDamage != 0)
		return;
	bumpedPositions.emplace_back(object->getPosition(), false);
	bumpDistance += object->getRadius();
}

void DynamicObject::endPush()
{
	pushDirection = { 0, 0 };
	pushDuration = 0;
	pushVector = { 0, 0 };
	pushDamage = 0;
	bumpedPositions.clear();
	bumpDistance = 0;
}

void DynamicObject::pushAway(const long long elapsedTime, float pushSpeed)
{
	if (pushSpeed == 0)
		pushSpeed = DEFAULT_PUSH_SPEED;
	
	timeAfterHitSelf += elapsedTime;
	const sf::Color pushColor = sf::Color(255, 100, 100, 255);

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
		
		bumpCenter.x /= bumpCount;
		bumpCenter.y /= bumpCount;
		pushDistance = this->radius + bumpDistance - Helper::getDist(this->getPosition(), bumpCenter);
		pushDirection = Vector2f(this->position.x - bumpCenter.x, this->position.y - bumpCenter.y);
		pushDuration = long(pushDistance / pushSpeed);
		pushRestDuration = pushDuration;
		bumpDistance = 0;
		bumpedPositions.clear();
		color = pushColor;
	}

	if (redRestDuration > 0)
		color = sf::Color(
			255,
			pushColor.g + (255 - pushColor.g) * (1 - Uint8(redRestDuration / redDuration)),
			pushColor.b + (255 - pushColor.b) * (1 - Uint8(redRestDuration / redDuration)),
			255);
	else
	{
		color = sf::Color::White;
		redRestDuration = 0;
	}

	redRestDuration -= elapsedTime;

	if (pushRestDuration <= 0 || currentAction == dead)
	{
		endPush();
		return;
	}

	pushRestDuration -= elapsedTime;

	const float elongationCoefficient = pushSpeed * float(elapsedTime) / sqrt(pow(pushDirection.x, 2) + pow(pushDirection.y, 2));
	if (pushDirection != Vector2f(0, 0))
		pushVector = { elongationCoefficient * pushDirection.x, elongationCoefficient * pushDirection.y };
	else
		pushVector = { 0, 0 };
}

void DynamicObject::takeDamage(const float damage, const Vector2f attackerPos)
{
	if (timeAfterHitSelf < timeForNewHitSelf)
		return;
	this->timeAfterHitSelf = 0;
	this->healthPoint -= damage / this->armor;

	pushDamage = damage;
	pushDuration = DEFAULT_PUSH_DURATION;
	pushRestDuration = pushDuration;
	redDuration = 2 * pushDuration;
	redRestDuration = redDuration;

	pushDistance = Helper::getDist(this->getPosition(), attackerPos);
	if (attackerPos != Vector2f(-1, -1))
		pushDirection = Vector2f(this->position.x - attackerPos.x, this->position.y - attackerPos.y);
}