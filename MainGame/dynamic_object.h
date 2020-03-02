#pragma once

#include "move_system.h"
#include "terrain_object.h"
#include "world_object.h"

class dynamic_object : public world_object
{
protected:
	int ellipseSize{};
	Vector2f movePosition = { -1, -1 };
	Vector2f focus1, focus2;
	float strength = 0;
	long long timeAfterHitSelf = 0;
	
	actions currentAction, lastAction = relax;
	world_object *boundTarget = nullptr;
	bool routeGenerationAbility = true;

	//fight logic
	virtual void fightInteract(long long elapsedTime, dynamic_object* target = nullptr) = 0;
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
	dynamic_object(std::string objectName, Vector2f centerPosition);
	virtual ~dynamic_object();
	int get_sprite_number() override { return current_sprite_[0]; }
	[[nodiscard]] int getEllipseSize() const { return ellipseSize; }
	[[nodiscard]] move_system getMoveSystem() const { return moveSystem; }
	[[nodiscard]] direction_system getDirectionSystem() const { return directionSystem; }
	[[nodiscard]] float getStrength() const { return strength; }
	[[nodiscard]] long long getTimeAfterHitSelf() const { return timeAfterHitSelf; }
	[[nodiscard]] bool getRouteGenerationAbility() const { return routeGenerationAbility; }
	[[nodiscard]] Vector2f getFocus1() const { return focus1; }
	[[nodiscard]] Vector2f getFocus2() const { return focus2; }
	[[nodiscard]] Vector2f getMovePosition() const { return movePosition; }
	[[nodiscard]] actions getCurrentAction() const { return currentAction; }
	[[nodiscard]] world_object *getBoundTarget() const { return boundTarget; }

	void setCurrentAction(actions action) { this->currentAction = action; }	
	virtual void changeMovePositionToRoute(Vector2f newPosition) { movePosition = newPosition; }
	void take_damage(float damage, Vector2f attackerPos = {-1, -1}) override;
	void setMoveOffset(long long elapsedTime);
	void setRoute(std::vector<std::pair<int, int>> route) { this->route = std::move(route); }
	void init_micro_blocks() override;
	virtual void changeAction(actions newAction, bool resetSpriteNumber = false, bool rememberLastAction = false);
	virtual void handleInput(bool usedMouse = false);
	virtual void behaviorWithDynamic(dynamic_object* target, long long elapsedTime) = 0;
	virtual void behaviorWithStatic(world_object* target, long long elapsedTime) = 0;
	virtual void behavior(long long elapsedTime) = 0;
	virtual void setTarget(dynamic_object &object) = 0;
	virtual void jerk(float power, float deceleration, Vector2f destinationPoint) = 0;

	move_system moveSystem;
	direction_system directionSystem;
	long long timeForNewHitSelf = long(6e5);
	long long timeAfterHit = 0;
	long long timeForNewHit = long(100000);
	float sightRange = 0;
	float shakeSpeed = -1;
	bool isIntersectDynamic(Vector2f newPosition, dynamic_object& otherDynamic) const;
	
	Vector2f ellipseSlip(dynamic_object *dynamic, Vector2f newPos, Vector2f destination, Vector2f f1, Vector2f f2, float ellipseSize, float height, long long elapsedTime) const;
	Vector2f doMove(long long elapsedTime);
	Vector2f doSlip(Vector2f newPosition, const std::vector<static_object*>& localStaticItems, float height, long long elapsedTime);
	Vector2f doSlipOffDynamic(Vector2f newPosition, const std::vector<dynamic_object*>& localDynamicItems, float height, long long elapsedTime);
	Vector2f laxMovePosition = { -1, -1 };
	std::string lastIntersected = "";
	std::vector<std::pair<int, int>> route = { {} };

	std::string debugInfo;
};
