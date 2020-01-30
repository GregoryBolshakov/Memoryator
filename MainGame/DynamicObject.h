#pragma once
#ifndef DYNAMICOBJECT_H
#define DYNAMICOBJECT_H

#include "WorldObject.h"
#include "TerrainObject.h"

const float pi = 3.14159265358979323846f;

enum class Direction { RIGHT = 0, UPRIGHT = 45, UP = 90, UPLEFT = 135, LEFT = 180, DOWNLEFT = 225, DOWN = 270, DOWNRIGHT = 315, STAND = 360 };
enum Actions { directHit = 2, relax = 3, combatState = 4, move = 5, dead = 6, commonHit = 9, moveHit = 10, dropping = 11, transitionToEnotherWorld = 12,
	open = 13, absorbs = 14, grab = 15, builds = 16, jerking = 17, upFlap = 30, leftFlap = 31, rightFlap = 32, startFlap = 33, stopFlap = 34, throwNoose = 35 };
enum Side { up = 1, right = 2, down = 3, left = 4 };
enum class SpeedLineDirection
{
	stand = 0,
	up_upright = 1, upright_right = 2, right_downright = 3, downright_down = 4, down_downleft = 5, downleft_left = 6, left_upleft = 7, upleft_up = 8,
	upright_up = 9, right_upright = 10, downright_right = 11, down_downright = 12, downleft_down = 13, left_downleft = 14, upleft_left = 15, up_upleft = 16
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
	Side calculateSide(Vector2f otherObjectPosition, float elapsedTime);
	Direction calculateDirection() const;
	static Side invertSide(Side side);	
	Side side = down;	
	Actions currentAction, lastAction = relax;
	Direction direction = Direction::STAND, lastDirection = Direction::DOWN;
	Vector2f moveOffset = Vector2f (-1, -1);
	WorldObject *boundTarget = nullptr;
	bool routeGenerationAbility = true;

	//fight logic
	virtual void fightLogic(float elapsedTime, DynamicObject* target = nullptr) = 0;
	void pushAway(float elapsedTime);
	virtual void pushByBumping(DynamicObject* object);
	std::vector<Vector2f> bumpedPositions = {};
	float bumpDistance = 0;
	float pushDamage = 0, pushDistance = 0;
	Vector2f pushDirection = { 0, 0 }, pushVector = {0, 0};
	float defaultPushDuration = 2e5, pushDuration = 0, pushRestDuration = 0, pushShift = 0.0004f;
	//-----------

	//jerk logic
	float jerkPower, jerkDeceleration = 0, jerkDistance = 0;
	float jerkDuration = 1000000, jerkTime = 0;
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
	static std::string sideToString(Side side);
	static std::string directionToString(Direction direction);
	Direction DynamicObject::sideToDirection(Side side);
	Direction invertDirection(Direction dir);
	static SpeedLineDirection getSpeedLineDirection(Direction from, Direction to);

	void setCurrentAction(Actions action) { this->currentAction = action; }	
	void changeMovePositionToRoute(Vector2f newPosition) { movePosition = newPosition; }
	void takeDamage(float damage, Vector2f attackerPos = {-1, -1}) override;
	void setMoveOffset(float elapsedTime);
	void setRoute(std::vector<std::pair<int, int>> route) { this->route = route; }
	void initMicroBlocks() override;
	virtual void changeAction(Actions newAction, bool resetSpriteNumber = false, bool rememberLastAction = false);
	virtual void handleInput(bool usedMouse = false);
	virtual void behaviorWithDynamic(DynamicObject* target, float elapsedTime) = 0;
	virtual void behaviorWithStatic(WorldObject* target, float elapsedTime) = 0;
	virtual void behavior(float elapsedTime) = 0;
	virtual void setTarget(DynamicObject &object) = 0;
	virtual void jerk(float power, float deceleration, Vector2f destinationPoint) = 0;

	float timeForNewHitself = 6e5;
	float timeAfterHit = 0, timeForNewHit = 100000, sightRange = 0;
	bool isIntersectDynamic(Vector2f newPosition, DynamicObject& otherDynamic);
	bool canCrashIntoDynamic = true, canCrashIntoStatic = false, doShake = false;
	Vector2f EllipceSlip(DynamicObject *dynamic, Vector2f newPos, Vector2f destination, Vector2f f1, Vector2f f2, float ellipseSize, float height, float elapsedTime);
	Vector2f doMove(long long elapsedTime);
	Vector2f doSlip(Vector2f newPosition, std::vector<StaticObject*> localStaticItems, float height, float elapsedTime);
	Vector2f doSlipOffDynamic(Vector2f newPosition, std::vector<DynamicObject*> localDynamicItems, float height, float elapsedTime);
	Vector2f laxMovePosition = { -1, -1 };
	std::string lastIntersected = "";
	std::vector<std::pair<int, int>> route = { {} };

	std::string debugInfo;
};

#endif