#pragma once
/*

#include "DynamicObject.h"
#include "Helper.h"

class ClapWhirl : public DynamicObject
{
public:
	ClapWhirl(std::string objectName, sf::Vector2f centerPosition, WorldObject* owner);
	~ClapWhirl();
	sf::Vector2f calculateTextureOffset() override;
	std::vector<SpriteChainElement*> prepare_sprites(long long elapsedTime) override;
	void behaviorWithDynamic(DynamicObject* target, long long elapsedTime) override;
	void behaviorWithStatic(WorldObject* target, long long elapsedTime) override;
	void behavior(long long elapsedTime) override;
	sf::Vector2f getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, sf::Vector2f cameraPosition) override;
	int getBuildType(sf::Vector2f ounPos, sf::Vector2f otherPos) override;
	void setOwner(WorldObject* owner) { this->owner = owner; };
	void setTarget(DynamicObject& object) override;
	void jerkInteract(long long elapsedTime);
	void jerk(float power, float deceleration, sf::Vector2f destinationPoint = sf::Vector2f(-1, -1)) override;
	void fightInteract(long long elapsedTime, DynamicObject* target = nullptr) override;
private:
	void stopping(bool doStand = false, bool forgetBoundTarget = false);
	void endingPreviousAction();
	void rotateAndExtend(SpriteChainElement* whirl);

	float timeForNewSprite, timeAfterHit = 0, timeForNewHit = 100000;
	int animationLength, strikingSprite = 4;
	std::map<Tag, bool> cantStopMe = { {Tag::chamomile, true},  {Tag::yarrow, true}, {Tag::mugwort, true}, {Tag::poppy, true}, {Tag::fern, true}, {Tag::stump, true}, {Tag::rock, true} };
	bool wasHit = false;
	SpriteChainElement stillRope, stillLoop;
	WorldObject* owner;
	sf::Vector2f ownerPos = { 0, 0 }, startPos = { 0, 0 };
	Vector2i ownerGlobalBounds = { 0, 0 };
};
*/