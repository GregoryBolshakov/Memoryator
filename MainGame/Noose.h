/*#pragma once
#ifndef NOOSE_H
#define NOOSE_H

#include "DynamicObject.h"
#include "Helper.h"

using namespace sf;

class Noose : public DynamicObject
{
private:
    void stopping(bool doStand = false, bool forgetBoundTarget = false);
    void endingPreviousAction();
	void rotateAndExtend(SpriteChainElement* rope, SpriteChainElement* loop);

	float timeForNewSprite, timeAfterHit = 0, timeForNewHit = 100000;
	float ropeElongation = 0, maximumLength = 1500;
	int animationLength, strikingSprite = 4;
	bool wasHit = false;
	SpriteChainElement stillRope, stillLoop;
	WorldObject* owner;
	Vector2f ownerPos = { 0, 0 };
	Vector2i ownerGlobalBounds = { 0, 0 };
public:
	Noose(std::string objectName, Vector2f centerPosition, WorldObject* owner);
	~Noose();
	Vector2i calculateTextureOffset() override;
	std::vector<SpriteChainElement> prepareSprites(long long elapsedTime) override;
    void behaviorWithDynamic(DynamicObject* target, long long elapsedTime) override;
	void behaviorWithStatic(WorldObject* target, long long elapsedTime) override;
	void behavior(long long elapsedTime) override;
	Vector2f getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	int getBuildType(Vector2f ounPos, Vector2f otherPos) override;	
	void setOwner(WorldObject* owner) { this->owner = owner; };
	void setTarget(DynamicObject& object) override;
	void jerkInteract(long long elapsedTime);
	void jerk(float power, float deceleration, Vector2f destinationPoint = Vector2f(-1, -1)) override;
	void fightInteract(long long elapsedTime, DynamicObject* target = nullptr) override;
};

#endif*/