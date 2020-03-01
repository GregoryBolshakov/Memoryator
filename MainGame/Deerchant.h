#pragma once

#include "DynamicObject.h"
#include "HeroBag.h"

using namespace sf;

class Deerchant : public DynamicObject
{
public:	
	Deerchant(std::string objectName, Vector2f centerPosition);
	~Deerchant();
	//BuildSystem
	bool isBuildSystem = false;
	//draw
	std::string nameOfFile;
	Vector2f calculateTextureOffset() override;
	std::vector<SpriteChainElement*> prepareSprites(long long elapsedTime) override;
	//control
	void handleInput(bool usedMouse = false) override;
	void moveEnd(bool animate = false, bool invertDirection = false);
	[[nodiscard]] int calculateNextMoveEndSprite(int currentSprite = 1) const;
	void behaviorWithDynamic(DynamicObject* target, long long elapsedTime) override;
	void behaviorWithStatic(WorldObject* target, long long elapsedTime) override;
	void behavior(long long elapsedTime) override;
	void onMouseUp(int currentMouseButton, WorldObject *mouseSelectedObject, Vector2f mouseWorldPos, bool isBuilding = false);
	void setTarget(DynamicObject& object) override;
	void endingPreviousAction();
	void stopping(bool doStand = false, bool forgetBoundTarget = false, bool offUnsealInventory = false);
	void calculateSpeedLineDirection(Direction lastDirection = Direction::STAND, Direction direction = Direction::STAND);
	void changeAction(Actions newAction, bool resetSpriteNumber, bool rememberLastAction) override;
	void changeMovePositionToRoute(Vector2f newPosition) override { if (currentAction != jerking) movePosition = newPosition; };
	[[nodiscard]] Vector2f getBeltPosition() const;

	void jerk(float power, float deceleration, Vector2f destinationPoint = Vector2f(-1, -1)) override;
	void jerkInteract(long long elapsedTime);
	void speedInteract(long long elapsedTime);
	void fightInteract(long long elapsedTime, DynamicObject* target = nullptr) override;
	void animationSmoothInteract(long long elapsedTime);
	void animationSmooth();

	Cell* heldItem = nullptr;
	Vector2f getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	int getBuildType(Vector2f ounPos, Vector2f otherPos) override;
	std::vector<HeroBag> bags;
	bool nearTheTable = false;
private:
	void setHitDirection();
	SpriteChainElement* prepareSpeedLine();

	float hitDistance = 0;
	float moveEndDistance = 0;
	long long moveTime = 0;
	long long smoothMoveTime = 0;
	Direction smoothDirection = Direction::STAND;
	WorldObject* owner = nullptr, *unsealInventoryOwner = nullptr;
	int strikingSprite = 0;
	Vector2f lastPosition = { 0, 0 };
	Direction speedLineDirection = Direction::STAND;
	bool wasPushedAfterMovement = false, mirroredSpeedLine = false, reverseSpeedLine = false;
	std::vector<Direction> smoothDirections = {};
};
