#pragma once

#include "DynamicObject.h"
#include "Helper.h"

using namespace sf;

class Monster : public DynamicObject
{
public:
	Monster(std::string objectName, Vector2f centerPosition);
	~Monster();
	void behaviorWithDynamic(DynamicObject* target, long long elapsedTime) override;
	void behaviorWithStatic(WorldObject* target, long long elapsedTime) override;
	void behavior(long long elapsedTime) override;
	virtual void endingPreviousAction();
	void stopping(bool doStand, bool forgetBoundTarget);
	Vector2f getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	int getBuildType(Vector2f ounPos, Vector2f otherPos) override;
	[[nodiscard]] WorldObject* getVictim() const { return victim; }
	void setTarget(DynamicObject& object) override;
	void jerk(float power, float deceleration, Vector2f destinationPoint) override;
	void fightInteract(long long elapsedTime, DynamicObject* target = nullptr) override;
	virtual void doAttack(WorldObject* target) = 0;
protected:
	WorldObject* victim = nullptr;
	std::map<Actions, bool> isAttack = { {commonHit, true}, {directHit, true} };
	float hitDistance = 30;
	int animationLength{};
	bool wasHit = false;

	int strikingSprite = 0;
};
