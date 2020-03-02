#pragma once

#include "dynamic_object.h"
#include "Helper.h"

using namespace sf;

class noose : public dynamic_object
{
public:
	noose(const std::string& objectName, Vector2f centerPosition, world_object* owner);
	~noose();
	Vector2f calculateTextureOffset() override;
	std::vector<sprite_chain_element*> prepareSprites(long long elapsedTime) override;
    void behaviorWithDynamic(dynamic_object* target, long long elapsedTime) override;
	void behaviorWithStatic(world_object* target, long long elapsedTime) override;
	void behavior(long long elapsedTime) override;
	Vector2f getBuildPosition(std::vector<world_object*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	int getBuildType(Vector2f ounPos, Vector2f otherPos) override;	
	void setOwner(world_object* owner) { this->owner = owner; };
	void setTarget(dynamic_object& object) override;
	void jerkInteract(long long elapsedTime);
	void jerk(float power, float deceleration, Vector2f destinationPoint = Vector2f(-1, -1)) override;
	void fightInteract(long long elapsedTime, dynamic_object* target = nullptr) override;

private:
	void stopping(bool doStand = false, bool forgetBoundTarget = false);
    void endingPreviousAction();
	void rotateAndExtend(sprite_chain_element* rope, sprite_chain_element* loop) const;

    long long timeForNewSprite;
    float ropeElongation = 0;
    float maximumLength = 1500;
	int animationLength;
	world_object* owner;
	Vector2f ownerPos = { 0, 0 };
	Vector2f ownerGlobalBounds = { 0, 0 };
};
