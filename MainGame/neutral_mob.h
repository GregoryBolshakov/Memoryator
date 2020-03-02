#pragma once

#include "dynamic_object.h"
#include "Helper.h"

using namespace sf;

class neutral_mob : public dynamic_object
{
protected:
	int morality{};
	int fear{};
	float distanceToNearest = 10e6;
public:
	neutral_mob(std::string objectName, Vector2f centerPosition);
	~neutral_mob();
	void behaviorWithDynamic(dynamic_object* target, long long elapsedTime) override;
	void behaviorWithStatic(world_object* target, long long elapsedTime) override;
	void behavior(long long elapsedTime) override;
	void setTarget(dynamic_object& object) override;
protected:
	void fightInteract(long long elapsedTime, dynamic_object* target = nullptr) override;
};
