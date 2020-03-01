#pragma once

#include <SFML/Graphics.hpp>
#include <stack>

#include "SpriteChainElement.h"

class WorldObject;
using namespace sf;

enum State { common = 1, absorbed = 2, constructed = 3 };

struct birthStaticInfo
{
	Tag tag;
	Vector2f position = { 0,0 };
	int typeOfObject = 1;
	int count = 1;
	std::vector<std::pair<Tag, int>> inventory;
};

struct  birthDynamicInfo
{
	Tag tag;
	Vector2f position = { 0,0 };
	WorldObject* owner = nullptr;
};

class WorldObject
{
public:
	WorldObject(std::string objectName, Vector2f centerPosition);
	virtual ~WorldObject();

	[[nodiscard]] int getZCoords() const	{ return zCoord; }
	[[nodiscard]] int getType() const	{ return typeOfObject; }
	[[nodiscard]] int getVarietyOfTypes() const	{ return varietyOfTypes; }
	[[nodiscard]] float getHealthPoint() const	{ return healthPoint; }
	float getMaxHealthPointValue();
	[[nodiscard]] bool getDeletePromise() const { return deletePromise; }
	[[nodiscard]] bool getMirroredState() const { return mirrored; }
	void manuallyDisableMirroring() { mirrored = false; }
	void cancelMirroring();
	[[nodiscard]] float getRadius() const { return radius; }
	[[nodiscard]] float getPermissibleDistance() const { return permissibleDistance; }
	[[nodiscard]] std::string getToSaveName() const { return toSaveName; }
	[[nodiscard]] std::string getName() const { return name; }
	virtual std::vector<SpriteChainElement*> prepareSprites(long long elapsedTime) = 0;
	virtual void onSpriteChange();
	virtual int getSpriteNumber() = 0;
	[[nodiscard]] Vector2f getPosition() const { return position; }
	Vector2f *getPtrPosition() { return &position; }
	[[nodiscard]] Vector2f getTextureSize() const { return { textureBox.width, textureBox.height }; }
	[[nodiscard]] Vector2f getTextureOffset() const { return { textureBoxOffset.x, textureBoxOffset.y }; }
	Vector2f getScaleRatio();
	[[nodiscard]] Vector2f getConditionalSizeUnits() const { return conditionalSizeUnits; }
	[[nodiscard]] Vector2f getMicroBlockCheckAreaBounds() const { return microBlockCheckAreaBounds; }
	[[nodiscard]] std::vector<Vector2i> getLockedMicroBlocks() const { return lockedMicroBlocks; }
	virtual Vector2f getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition) = 0;
	virtual int getBuildType(Vector2f ounPos, Vector2f otherPos) = 0;
	[[nodiscard]] FloatRect getOriginalTextureBox() const { return originalTextureBox; }
	[[nodiscard]] State getState() const { return state; }	
	std::pair<std::stack<birthStaticInfo>, std::stack<birthDynamicInfo>> getBirthObjects() { return std::make_pair(birthStatics, birthDynamics); }	

	void clearBirthStack() { birthStatics = std::stack<birthStaticInfo>(); birthDynamics = std::stack<birthDynamicInfo>(); }
	void setPosition(Vector2f newPosition);
	void setHealthPoint(float healthPoint) { this->healthPoint = healthPoint; }
	void setName(std::string name) { this->name = std::move(name); }
	void deletePromiseOn() { deletePromise = true; }
	static void setUnscaled(const std::vector<SpriteChainElement*>& items) { for (auto& item : items) item->unscaled = true; }
	virtual void setTextureSize(Vector2f textureSize);
	void setState(State state) { this->state = state; }
	virtual void takeDamage(float damage, Vector2f attackerPos = { -1, -1 });
	virtual void initMicroBlocks();
	virtual bool isLockedPlace(std::map<std::pair<int, int>, bool> checkBlocks);

	bool isTransparent = false, isSelected = false, isVisibleInventory = false;
	bool isProcessed = false;
	bool isBackground = false, isTerrain = false, isDotsAdjusted = false, isMultiEllipse = false, intangible = false;

	virtual Vector2f calculateTextureOffset() = 0;
	virtual void initPedestal();

	sf::Color color = sf::Color(255, 255, 255);
	std::vector<std::pair<Tag, int>> inventory = {};
	static Vector2f microBlockSize;
	Tag tag = Tag::emptyObject;
protected:
	int typeOfObject{};
	int zCoord = 1;
	std::vector<int> currentSprite;
	int varietyOfTypes{};
	bool deletePromise = false;
	bool mirrored = false;
	long long timeForNewSprite{};
	float animationSpeed{};
	float healthPoint = 0, armor = 1, maxHealthPointValue = 0;
	std::string name, toSaveName = "";
	FloatRect textureBox, originalTextureBox;
	Vector2f textureBoxOffset;
	Vector2f conditionalSizeUnits;	
	Vector2f microBlockCheckAreaBounds = { 0, 0 };
	Vector2f position = { 0, 0 };
	float radius, permissibleDistance = 0;
	State state = common;
	std::stack<birthStaticInfo> birthStatics;
	std::stack<birthDynamicInfo> birthDynamics;
	std::vector<Vector2i> lockedMicroBlocks = {};
};
