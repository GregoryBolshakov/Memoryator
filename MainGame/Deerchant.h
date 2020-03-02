#pragma once

#include "dynamic_object.h"
#include "hero_bag.h"

using namespace sf;

class deerchant : public dynamic_object
{
public:	
	deerchant(std::string objectName, Vector2f centerPosition);
	~deerchant();
	//BuildSystem
	bool isBuildSystem = false;
	//draw
	std::string nameOfFile;
	Vector2f calculateTextureOffset() override;
	std::vector<sprite_chain_element*> prepareSprites(long long elapsedTime) override;
	//control
	void handleInput(bool usedMouse = false) override;
	void moveEnd(bool animate = false, bool invertDirection = false);
	[[nodiscard]] int calculateNextMoveEndSprite(int currentSprite = 1) const;
	void behaviorWithDynamic(dynamic_object* target, long long elapsedTime) override;
	void behaviorWithStatic(world_object* target, long long elapsedTime) override;
	void behavior(long long elapsedTime) override;
	void onMouseUp(int currentMouseButton, world_object *mouseSelectedObject, Vector2f mouseWorldPos, bool isBuilding = false);
	void setTarget(dynamic_object& object) override;
	void endingPreviousAction();
	void stopping(bool doStand = false, bool forgetBoundTarget = false, bool offUnsealInventory = false);
	void calculateSpeedLineDirection(Direction lastDirection = Direction::STAND, Direction direction = Direction::STAND);
	void changeAction(Actions newAction, bool resetSpriteNumber, bool rememberLastAction) override;
	void changeMovePositionToRoute(Vector2f newPosition) override { if (currentAction != jerking) movePosition = newPosition; };
	[[nodiscard]] Vector2f getBeltPosition() const;

	void jerk(float power, float deceleration, Vector2f destinationPoint = Vector2f(-1, -1)) override;
	void jerkInteract(long long elapsedTime);
	void speedInteract(long long elapsedTime);
	void fightInteract(long long elapsedTime, dynamic_object* target = nullptr) override;
	void animationSmoothInteract(long long elapsedTime);
	void animationSmooth();

	cell* heldItem = nullptr;
	Vector2f getBuildPosition(std::vector<world_object*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	int getBuildType(Vector2f ounPos, Vector2f otherPos) override;
	std::vector<hero_bag> bags;
	bool nearTheTable = false;
private:
	void setHitDirection();
	sprite_chain_element* prepareSpeedLine();

	float hitDistance = 0;
	float moveEndDistance = 0;
	long long moveTime = 0;
	long long smoothMoveTime = 0;
	Direction smoothDirection = Direction::STAND;
	world_object* owner = nullptr, *unsealInventoryOwner = nullptr;
	int strikingSprite = 0;
	Vector2f lastPosition = { 0, 0 };
	Direction speedLineDirection = Direction::STAND;
	bool wasPushedAfterMovement = false, mirroredSpeedLine = false, reverseSpeedLine = false;
	std::vector<Direction> smoothDirections = {};
};
