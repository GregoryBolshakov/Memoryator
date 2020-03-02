#pragma once

#include "DynamicObject.h"
#include "Helper.h"

using namespace sf;

class neutral_mob : public DynamicObject
{
protected:
	int morality{};
	int fear{};
	float distanceToNearest = 10e6;
public:
	neutral_mob(std::string objectName, Vector2f centerPosition);
	~neutral_mob();
	void behaviorWithDynamic(DynamicObject* target, long long elapsedTime) override;
	void behaviorWithStatic(WorldObject* target, long long elapsedTime) override;
	void behavior(long long elapsedTime) override;
	void setTarget(DynamicObject& object) override;
protected:
	void fightInteract(long long elapsedTime, DynamicObject* target = nullptr) override;
};
