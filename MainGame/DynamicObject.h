#pragma once

#include "MoveSystem.h"
#include "TerrainObject.h"
#include "WorldObject.h"

class DynamicObject : public WorldObject
{
protected:
	int ellipseSize{};
	Vector2f movePosition = { -1, -1 };
	Vector2f focus1, focus2;
	float strength = 0;
	long long timeAfterHitSelf = 0;
	
	Actions currentAction, lastAction = relax;
	WorldObject *boundTarget = nullptr;
	bool routeGenerationAbility = true;

	//fight logic
	virtual void fightInteract(long long elapsedTime, DynamicObject* target = nullptr) = 0;
	//-----------

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
	[[nodiscard]] MoveSystem getMoveSystem() const { return moveSystem; }
	[[nodiscard]] DirectionSystem getDirectionSystem() const { return directionSystem; }
	[[nodiscard]] float getStrength() const { return strength; }
	[[nodiscard]] long long getTimeAfterHitSelf() const { return timeAfterHitSelf; }
	[[nodiscard]] bool getRouteGenerationAbility() const { return routeGenerationAbility; }
	[[nodiscard]] Vector2f getFocus1() const { return focus1; }
	[[nodiscard]] Vector2f getFocus2() const { return focus2; }
	[[nodiscard]] Vector2f getMovePosition() const { return movePosition; }
	[[nodiscard]] Actions getCurrentAction() const { return currentAction; }
	[[nodiscard]] WorldObject *getBoundTarget() const { return boundTarget; }

	void setCurrentAction(Actions action) { this->currentAction = action; }	
	virtual void changeMovePositionToRoute(Vector2f newPosition) { movePosition = newPosition; }
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

	MoveSystem moveSystem;
	DirectionSystem directionSystem;
	long long timeForNewHitSelf = long(6e5);
	long long timeAfterHit = 0;
	long long timeForNewHit = long(100000);
	float sightRange = 0;
	float shakeSpeed = -1;
	bool isIntersectDynamic(Vector2f newPosition, DynamicObject& otherDynamic) const;
	
	Vector2f ellipseSlip(DynamicObject *dynamic, Vector2f newPos, Vector2f destination, Vector2f f1, Vector2f f2, float ellipseSize, float height, long long elapsedTime) const;
	Vector2f doMove(long long elapsedTime);
	Vector2f doSlip(Vector2f newPosition, const std::vector<StaticObject*>& localStaticItems, float height, long long elapsedTime);
	Vector2f doSlipOffDynamic(Vector2f newPosition, const std::vector<DynamicObject*>& localDynamicItems, float height, long long elapsedTime);
	Vector2f laxMovePosition = { -1, -1 };
	std::string lastIntersected = "";
	std::vector<std::pair<int, int>> route = { {} };

	std::string debugInfo;
};
