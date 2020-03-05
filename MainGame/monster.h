#pragma once

#include "dynamic_object.h"
#include "helper.h"

using namespace sf;

class monster : public dynamic_object
{
public:
	monster(std::string objectName, Vector2f centerPosition);
	~monster();
	void behaviorWithDynamic(dynamic_object* target, long long elapsedTime) override;
	void behaviorWithStatic(world_object* target, long long elapsedTime) override;
	void behavior(long long elapsedTime) override;
	virtual void endingPreviousAction();
	void stopping(bool doStand, bool forgetBoundTarget);
	Vector2f get_build_position(std::vector<world_object*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	int get_build_type(Vector2f ounPos, Vector2f otherPos) override;
	[[nodiscard]] world_object* getVictim() const { return victim; }
	void setTarget(dynamic_object& object) override;
	void jerk(float power, float deceleration, Vector2f destinationPoint) override;
	void fightInteract(long long elapsedTime, dynamic_object* target = nullptr) override;
	virtual void doAttack(world_object* target) = 0;
protected:
	world_object* victim = nullptr;
	std::map<actions, bool> isAttack = { {commonHit, true}, {directHit, true} };
	float hitDistance = 30;
	int animationLength{};
	bool wasHit = false;

	int strikingSprite = 0;
};
