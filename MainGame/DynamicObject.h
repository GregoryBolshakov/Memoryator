#pragma once
#ifndef DYNAMICOBJECT_H
#define DYNAMICOBJECT_H

#include "WorldObject.h"
#include "TerrainObject.h"
#include "MoveSystem.h"

class DynamicObject : public WorldObject
{
protected:
	int ellipseSize;
	Vector2f movePosition = { -1, -1 };
	Vector2f focus1, focus2;
	float strength = 0;
	float timeAfterHitself = 0;
	
	Actions currentAction, lastAction = relax;	
	WorldObject *boundTarget = nullptr;
	bool routeGenerationAbility = true;

	//fight logic
	virtual void fightInteract(long long elapsedTime, DynamicObject* target = nullptr) = 0;	
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
	MoveSystem getMoveSystem() const { return moveSystem; }
	DirectionSystem getDirectionSystem() const { return directionSystem; }
	float getStrength() const { return strength; }
	float getTimeAfterHitself() const { return timeAfterHitself; }
	bool getRouteGenerationAbility() const { return routeGenerationAbility; }
	Vector2f getFocus1() const { return focus1; }
	Vector2f getFocus2() const { return focus2; }
	Vector2f getMovePosition() const { return movePosition; }
	Actions getCurrentAction() const { return currentAction; }
	WorldObject *getBoundTarget() const { return boundTarget; }

	void setCurrentAction(Actions action) { this->currentAction = action; }	
	virtual void changeMovePositionToRoute(Vector2f newPosition) { movePosition = newPosition; }
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

	MoveSystem moveSystem;
	DirectionSystem directionSystem;
	float timeForNewHitself = 6e5;
	float timeAfterHit = 0, timeForNewHit = 100000, sightRange = 0, shakeSpeed = -1;
	bool isIntersectDynamic(Vector2f newPosition, DynamicObject& otherDynamic) const;
	
	Vector2f ellipseSlip(DynamicObject *dynamic, Vector2f newPos, Vector2f destination, Vector2f f1, Vector2f f2, float ellipseSize, float height, long long elapsedTime) const;
	Vector2f doMove(long long elapsedTime);
	Vector2f doSlip(Vector2f newPosition, std::vector<StaticObject*> localStaticItems, float height, long long elapsedTime);
	Vector2f doSlipOffDynamic(Vector2f newPosition, std::vector<DynamicObject*> localDynamicItems, float height, long long elapsedTime);
	Vector2f laxMovePosition = { -1, -1 };
	std::string lastIntersected = "";
	std::vector<std::pair<int, int>> route = { {} };

	std::string debugInfo;
};

#endif