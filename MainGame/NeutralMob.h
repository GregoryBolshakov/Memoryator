#pragma once
#ifndef NEUTRALMOB_H
#define NEUTRALMOB_H

#include "DynamicObject.h"
#include "Helper.h"

using namespace sf;

class NeutralMob : public DynamicObject
{
protected:
	int morality;
	int fear;
	float distanceToNearest = 10e6;
public:
	NeutralMob(std::string objectName, Vector2f centerPosition);
	~NeutralMob();
	void behaviorWithDynamic(DynamicObject* target, long long elapsedTime) override;
	void behaviorWithStatic(WorldObject* target, long long elapsedTime) override;
	void behavior(long long elapsedTime) override;
	void setTarget(DynamicObject& object) override;
protected:
	void fightLogic(long long elapsedTime, DynamicObject* target = nullptr) override;
};

#endif