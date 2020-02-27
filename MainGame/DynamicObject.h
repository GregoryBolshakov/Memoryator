#pragma once
#ifndef DYNAMICOBJECT_H
#define DYNAMICOBJECT_H

#include "WorldObject.h"
#include "TerrainObject.h"
#include "DirectionsSystem.h"

const float pi = 3.14159265358979323846f;

struct BumpChainElement
{
	BumpChainElement(Vector2f position, bool cancelable) : position{ position } , cancelable{ cancelable } {};
	Vector2f position;
	bool cancelable = true;
};

class DynamicObject : public WorldObject
{
protected:
	int ellipseSize;
	Vector2f movePosition = { -1, -1 };
	Vector2f focus1, focus2;
	float strength = 0;
	float defaultSpeed, speed;
	float timeAfterHitself = 0;
	Side calculateSide(Vector2f otherObjectPosition, long long elapsedTime) const;
	void calculateDirection();
	static Side invertSide(Side side);	
	Side side = down;	
	Actions currentAction, lastAction = relax;
	Direction direction = Direction::STAND, lastDirection = Direction::DOWN;
	Vector2f moveOffset = Vector2f (-1, -1);
	WorldObject *boundTarget = nullptr;
	bool routeGenerationAbility = true;

	//fight logic
	virtual void fightInteract(long long elapsedTime, DynamicObject* target = nullptr) = 0;
	void pushAway(long long elapsedTime, float pushSpeed = 0);
	void endPush(long long elapsedTime);
	virtual void pushByBumping(DynamicObject* object);
	std::vector<BumpChainElement> bumpedPositions = {};
	float bumpDistance = 0;
	float pushDamage = 0, pushDistance = 0;
	Vector2f pushDirection = { 0, 0 }, pushVector = {0, 0};
	float pushDuration = 0, pushRestDuration = 0, redDuration = 0, redRestDuration = 0;
	const float DEFAULT_PUSH_DURATION = 2e5, DEFAULT_PUSH_SPEED = 0.00025f;
	//-----------

	//jerk logic
	float jerkPower, jerkDeceleration = 0, jerkDistance = 0;
	float jerkDuration = 1e6, jerkTime = 0;
	bool isJerking = false;
	//----------
public:
	DynamicObject(std::string objectName, Vector2f centerPosition);
	virtual ~DynamicObject();
	int getSpriteNumber() override { return currentSprite[0]; }
	int getEllipseSize() const { return ellipseSize; }	
	float getSpeed() { return speed; }
	float getStrength() { return strength; }
	float getTimeAfterHitself() { return timeAfterHitself; }
	bool getRouteGenerationAbility() { return routeGenerationAbility; }
	Vector2f getFocus1() const { return focus1; }
	Vector2f getFocus2() const { return focus2; }
	Vector2f getMoveOffset() { return moveOffset; }
	Vector2f getMovePosition() { return movePosition; }	
	Actions getCurrentAction() { return currentAction; }
	Direction getDirection() { return direction; }
	Direction getLastDirection() { return lastDirection; }
	Side getSide() { return side; }
	WorldObject *getBoundTarget() { return boundTarget; }

	void setCurrentAction(Actions action) { this->currentAction = action; }	
	void changeMovePositionToRoute(Vector2f newPosition) { movePosition = newPosition; }
	void takeDamage(float damage, Vector2f attackerPos = {-1, -1}) override;
	void setMoveOffset(long long elapsedTime);
	void setRoute(std::vector<std::pair<int, int>> route) { this->route = route; }
	void initMicroBlocks() override;
	virtual void changeAction(Actions newAction, bool resetSpriteNumber = false, bool rememberLastAction = false);
	virtual void handleInput(bool usedMouse = false);
	virtual void behaviorWithDynamic(DynamicObject* target, long long elapsedTime) = 0;
	virtual void behaviorWithStatic(WorldObject* target, long long elapsedTime) = 0;
	virtual void behavior(long long elapsedTime) = 0;
	virtual void setTarget(DynamicObject &object) = 0;
	virtual void jerk(float power, float deceleration, Vector2f destinationPoint) = 0;

	float timeForNewHitself = 6e5;
	float timeAfterHit = 0, timeForNewHit = 100000, sightRange = 0;
	bool isIntersectDynamic(Vector2f newPosition, DynamicObject& otherDynamic);
	bool canCrashIntoDynamic = true, canCrashIntoStatic = false, doShake = false;
	Vector2f EllipceSlip(DynamicObject *dynamic, Vector2f newPos, Vector2f destination, Vector2f f1, Vector2f f2, float ellipseSize, float height, long long elapsedTime);
	Vector2f doMove(long long elapsedTime);
	Vector2f doSlip(Vector2f newPosition, std::vector<StaticObject*> localStaticItems, float height, long long elapsedTime);
	Vector2f doSlipOffDynamic(Vector2f newPosition, std::vector<DynamicObject*> localDynamicItems, float height, long long elapsedTime);
	Vector2f laxMovePosition = { -1, -1 };
	std::string lastIntersected = "";
	std::vector<std::pair<int, int>> route = { {} };

	std::string debugInfo;
};

#endif