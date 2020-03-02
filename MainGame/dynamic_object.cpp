#include "dynamic_object.h"

#include "helper.h"

dynamic_object::dynamic_object(std::string objectName, const Vector2f centerPosition) : world_object(std::move(objectName), centerPosition), currentAction()
{
	moveSystem.init(&radius_, &position_, &color);
	directionSystem.init(&position_, &movePosition);
}

dynamic_object::~dynamic_object()
= default;

void dynamic_object::handleInput(bool usedMouse)
{
}

void dynamic_object::init_micro_blocks()
{
	locked_micro_blocks_ = { Vector2i(int(ceil(position_.x / micro_block_size.x)), int(ceil(position_.y / micro_block_size.y))) };
}

bool dynamic_object::isIntersectDynamic(Vector2f newPosition, dynamic_object& otherDynamic) const
{
	//Vector2f position1 = dynamic1.getPosition();
	const auto position1 = newPosition;
	const auto position2 = otherDynamic.get_position();
	//return (abs(position1.x - position2.x) <= (this->getRadius() + otherDynamic.getRadius()) && abs(position1.y - position2.y) <= (this->getRadius() + otherDynamic.getRadius()));
	return sqrt(pow(position1.x - position2.x, 2) + pow(position1.y - position2.y, 2)) <= /*this->radius + */otherDynamic.radius_;
}

Vector2f dynamic_object::ellipseSlip(dynamic_object *dynamic, Vector2f newPos, Vector2f destination, Vector2f f1, Vector2f f2, float ellipseSize, float height, long long elapsedTime) const
{
	const auto dynamicPos = Vector2f(dynamic->get_position().x, height - dynamic->get_position().y);
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
		dist1 = helper::getDist(newPosition, Vector2f(x1, yl)), dist2 = helper::getDist(newPosition, Vector2f(x2, y2));
	else
		dist1 = helper::getDist(destinationPos, Vector2f(x1, yl)), dist2 = helper::getDist(destinationPos, Vector2f(x2, y2));

	if (dist1 <= dist2 && d >= 0 && sqrt(pow(x1 - dynamic->get_position().x, 2) + pow(height - yl - dynamic->get_position().y, 2)) != 0)
	{
		const auto k = ((dynamic->getMoveSystem().speed * time) / sqrt(pow(x1 - dynamic->get_position().x, 2) + pow(height - yl - dynamic->get_position().y, 2)));
		return { (x1 - dynamic->get_position().x) * k, (height - yl - dynamic->get_position().y) * k };
	}
	if (dist2 <= dist1 && d >= 0 && sqrt(pow(x2 - dynamic->get_position().x, 2) + pow(height - y2 - dynamic->get_position().y, 2)) != 0)
	{
		const auto k = ((dynamic->getMoveSystem().speed * time) / sqrt(pow(x2 - dynamic->get_position().x, 2) + pow(height - y2 - dynamic->get_position().y, 2)));
		return { (x2 - dynamic->get_position().x) * k, (height - y2 - dynamic->get_position().y) * k };
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

void dynamic_object::setMoveOffset(long long elapsedTime)
{
	if (!(currentAction == move ||
		currentAction == moveHit ||
		currentAction == moveEnd ||
		currentAction == jerking ||
		currentAction == throwNoose || 
		currentAction == moveSlowly))
	{
		moveSystem.moveOffset = { -1, -1 };
		return;
	}

	if (movePosition == Vector2f(-1, -1))
	{
		if (directionSystem.direction != direction::STAND)
		{
			const auto angle = float(directionSystem.direction) * pi / 180;
			const float k = moveSystem.speed * float(elapsedTime) / sqrt(pow(cos(angle), 2) + pow(sin(angle), 2));
			moveSystem.moveOffset.x = float(k * cos(angle));
			moveSystem.moveOffset.y = float(k * -sin(angle));
			return;
		}
		moveSystem.moveOffset = Vector2f(-1, -1);
		return;
	}

	const auto distanceToTarget = float(sqrt(pow(movePosition.x - position_.x, 2) + pow(movePosition.y - position_.y, 2)));
	if (distanceToTarget == 0)
	{
		moveSystem.moveOffset = Vector2f(-1, -1);
		return;
	}

	const auto k = moveSystem.speed * float(elapsedTime) / sqrt(pow(movePosition.x - position_.x, 2) + pow(movePosition.y - position_.y, 2));
	/*if (distanceToTarget <= k)
	{
		moveOffset = Vector2f(-1, -1);
		return;
	}*/
	moveSystem.moveOffset = { (movePosition.x - position_.x) * k, (movePosition.y - position_.y) * k };
}

Vector2f dynamic_object::doMove(long long elapsedTime)
{
	setMoveOffset(elapsedTime);
	auto position = this->position_;
	position.x += moveSystem.pushVector.x; position.y += moveSystem.pushVector.y;

	//if (this->direction == Direction::STAND)
		//return position;

	if (moveSystem.moveOffset != Vector2f(-1, -1))
	{
		position.x += moveSystem.moveOffset.x;
		position.y += moveSystem.moveOffset.y;
	}

	return position;
}

Vector2f dynamic_object::doSlip(Vector2f newPosition, const std::vector<static_object*> & localStaticItems, const float height, const long long elapsedTime)
{
	bool crashed = false;
	if (!moveSystem.canCrashIntoStatic)
		return newPosition;

	for (auto& staticItem : localStaticItems)
	{
		auto terrain = dynamic_cast<terrain_object*>(staticItem);
		if (!terrain || staticItem->is_background || staticItem->get_radius() == 0)
			continue;

		if (tag != entity_tag::hero && staticItem->is_multi_ellipse)
			continue;

		if (terrain->is_multi_ellipse)
		{
			auto curEllipses = terrain->getMultiellipseIntersect(newPosition);
			Vector2f motionAfterSlipping;

			if (!curEllipses.empty())
				for (auto& curEllipse : curEllipses)
				{
					if (crashed)
						return Vector2f(-1, -1);

					crashed = true;
					motionAfterSlipping = this->ellipseSlip(this, newPosition, this->movePosition, terrain->internalEllipses[curEllipse].first, terrain->internalEllipses[curEllipse].second, terrain->getEllipseSize(curEllipse), height, elapsedTime);

					if (motionAfterSlipping != Vector2f(-1, -1))
					{
						newPosition = Vector2f(this->get_position().x + motionAfterSlipping.x, this->get_position().y + motionAfterSlipping.y);
						continue;
					}

					return Vector2f(-1, -1);
				}
		}
		else
			if (terrain->isIntersected(this->get_position(), newPosition))
			{
				if (crashed)
					return Vector2f(-1, -1);

				crashed = true;
				Vector2f motionAfterSlipping;

				if (staticItem->is_dots_adjusted)
					motionAfterSlipping = terrain->newSlippingPositionForDotsAdjusted(this->get_position(), moveSystem.speed, elapsedTime);
				else
					motionAfterSlipping = this->ellipseSlip(this, newPosition, this->movePosition, terrain->getFocus1(), terrain->getFocus2(), terrain->getEllipseSize(), height, elapsedTime);

				if (motionAfterSlipping != Vector2f(-1, -1))
				{
					newPosition = Vector2f(this->get_position().x + motionAfterSlipping.x, this->get_position().y + motionAfterSlipping.y);
					continue;
				}

				return Vector2f(-1, -1);
			}
	}

	return newPosition;
}

Vector2f dynamic_object::doSlipOffDynamic(Vector2f newPosition, const std::vector<dynamic_object*> & localDynamicItems, const float height, const long long elapsedTime)
{
	if (!moveSystem.canCrashIntoDynamic)
		return newPosition;

	lastIntersected = "";

	for (auto& otherDynamicItem : localDynamicItems)
	{
		if (otherDynamicItem == this)
			continue;

		if (this->isIntersectDynamic(newPosition, *otherDynamicItem) && otherDynamicItem->lastIntersected != name_)
		{
			this->lastIntersected = otherDynamicItem->get_name();
			auto motionAfterSlipping = ellipseSlip(this, newPosition, movePosition, otherDynamicItem->get_position(), otherDynamicItem->get_position(), otherDynamicItem->get_radius() * 2, height, elapsedTime);

			if (motionAfterSlipping != Vector2f(-1, -1))
			{
				newPosition = Vector2f(position_.x + motionAfterSlipping.x, position_.y + motionAfterSlipping.y);
				continue;
			}

			return Vector2f(-1, -1);
		}
	}

	return newPosition;
}

void dynamic_object::changeAction(const actions newAction, const bool resetSpriteNumber, const bool rememberLastAction)
{
	if (rememberLastAction)
		lastAction = currentAction;

	currentAction = newAction;

	if (resetSpriteNumber)
		for (auto& number : current_sprite_)
			number = 1;
}

void dynamic_object::take_damage(const float damage, const Vector2f attackerPos)
{
	if (timeAfterHitSelf < timeForNewHitSelf)
		return;
	
	this->timeAfterHitSelf = 0;
	this->health_point_ -= damage / this->armor_;

	moveSystem.pushDamage = damage;
	moveSystem.pushDuration = moveSystem.DEFAULT_PUSH_DURATION;
	moveSystem.pushRestDuration = moveSystem.pushDuration;
	moveSystem.redDuration = 2 * moveSystem.pushDuration;
	moveSystem.redRestDuration = moveSystem.redDuration;

	moveSystem.pushDistance = helper::getDist(this->get_position(), attackerPos);
	if (attackerPos != Vector2f(-1, -1))
		moveSystem.pushDirection = Vector2f(this->position_.x - attackerPos.x, this->position_.y - attackerPos.y);
}