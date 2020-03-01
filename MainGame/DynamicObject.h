#pragma once

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
	int ellipseSize{};
	Vector2f movePosition = { -1, -1 };
	Vector2f focus1, focus2;
	float strength = 0;
	float defaultSpeed{}, speed;
	long long timeAfterHitSelf = 0;
	[[nodiscard]] Side calculateSide(Vector2f otherObjectPosition) const;
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
	void endPush();
	virtual void pushByBumping(DynamicObject* object);
	std::vector<BumpChainElement> bumpedPositions = {};
	float bumpDistance = 0;
	float pushDamage = 0;
	float pushDistance = 0;
	Vector2f pushDirection = { 0, 0 };
	Vector2f pushVector = {0, 0};
	long long pushDuration = 0;
	long long pushRestDuration = 0;
	long long redDuration = 0;
	long long redRestDuration = 0;

	const long long DEFAULT_PUSH_DURATION = long(2e5);
	const float DEFAULT_PUSH_SPEED = 0.00025f;

	float jerkPower{};
	float jerkDeceleration = 0;
	float jerkDistance = 0;
	long long jerkDuration = long(1e6);
	long long jerkTime = 0;
	bool isJerking = false;
public:
	DynamicObject(std::string objectName, Vector2f centerPosition);
	virtual ~DynamicObject();
	int getSpriteNumber() override { return currentSprite[0]; }
	[[nodiscard]] int getEllipseSize() const { return ellipseSize; }
	[[nodiscard]] float getSpeed() const	{ return speed; }
	[[nodiscard]] float getStrength() const	{ return strength; }
	[[nodiscard]] long long getTimeAfterHitSelf() const	{ return timeAfterHitSelf; }
	[[nodiscard]] bool getRouteGenerationAbility() const	{ return routeGenerationAbility; }
	[[nodiscard]] Vector2f getFocus1() const { return focus1; }
	[[nodiscard]] Vector2f getFocus2() const { return focus2; }
	[[nodiscard]] Vector2f getMoveOffset() const	{ return moveOffset; }
	[[nodiscard]] Vector2f getMovePosition() const	{ return movePosition; }
	[[nodiscard]] Actions getCurrentAction() const	{ return currentAction; }
	[[nodiscard]] Direction getDirection() const	{ return direction; }
	[[nodiscard]] Direction getLastDirection() const	{ return lastDirection; }
	[[nodiscard]] Side getSide() const	{ return side; }
	[[nodiscard]] WorldObject* getBoundTarget() const	{ return boundTarget; }
	void setCurrentAction(Actions action) { this->currentAction = action; }	
	void changeMovePositionToRoute(Vector2f newPosition) { movePosition = newPosition; }
	void takeDamage(float damage, Vector2f attackerPos = {-1, -1}) override;
	void setMoveOffset(long long elapsedTime);
	void setRoute(std::vector<std::pair<int, int>> route) { this->route = std::move(route); }
	void initMicroBlocks() override;
	virtual void changeAction(Actions newAction, bool resetSpriteNumber = false, bool rememberLastAction = false);
	virtual void handleInput(bool usedMouse = false);
	virtual void behaviorWithDynamic(DynamicObject* target, long long elapsedTime) = 0;
	virtual void behaviorWithStatic(WorldObject* target, long long elapsedTime) = 0;
	virtual void behavior(long long elapsedTime) = 0;
	virtual void setTarget(DynamicObject &object) = 0;
	virtual void jerk(float power, float deceleration, Vector2f destinationPoint) = 0;

	long long timeForNewHitSelf = long(6e5);
	long long timeAfterHit = 0;
	long long timeForNewHit = 100000;
	float sightRange = 0;
	bool isIntersectDynamic(Vector2f newPosition, DynamicObject& otherDynamic) const;
	bool canCrashIntoDynamic = true;
	bool canCrashIntoStatic = false;
	bool doShake = false;
	Vector2f EllipseSlip(DynamicObject *dynamic, Vector2f newPos, Vector2f destination, Vector2f f1, Vector2f f2, float ellipseSize, float height, long long elapsedTime) const;
	Vector2f doMove(long long elapsedTime);
	Vector2f doSlip(Vector2f newPosition, const std::vector<StaticObject*>& localStaticItems, float height, long long elapsedTime);
	Vector2f doSlipOffDynamic(Vector2f newPosition, const std::vector<DynamicObject*>& localDynamicItems, float height, long long elapsedTime);
	Vector2f laxMovePosition = { -1, -1 };
	std::string lastIntersected = "";
	std::vector<std::pair<int, int>> route = { {} };

	std::string debugInfo;
};
