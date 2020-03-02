#pragma once

#include <SFML/Graphics.hpp>
#include <stack>

#include "sprite_chain_element.h"

class world_object;
using namespace sf;

enum state { common = 1, absorbed = 2, constructed = 3 };

struct birth_static_info
{
	Tag tag;
	Vector2f position = { 0,0 };
	int typeOfObject = 1;
	int count = 1;
	std::vector<std::pair<Tag, int>> inventory;
};

struct  birth_dynamic_info
{
	Tag tag;
	Vector2f position = { 0,0 };
	world_object* owner = nullptr;
};

class world_object
{
public:
	world_object(std::string objectName, Vector2f centerPosition);
	virtual ~world_object();

	[[nodiscard]] int getZCoords() const	{ return zCoord; }
	[[nodiscard]] int getType() const	{ return typeOfObject; }
	[[nodiscard]] int getVarietyOfTypes() const	{ return varietyOfTypes; }
	[[nodiscard]] float getHealthPoint() const	{ return healthPoint; }
	[[nodiscard]] float getMaxHealthPointValue() const;
	[[nodiscard]] bool getDeletePromise() const { return deletePromise; }
	[[nodiscard]] bool getMirroredState() const { return mirrored; }
	[[nodiscard]] float getRadius() const { return radius; }
	[[nodiscard]] float getPermissibleDistance() const { return permissibleDistance; }
	[[nodiscard]] std::string getToSaveName() const { return toSaveName; }
	[[nodiscard]] std::string getName() const { return name; }
	virtual std::vector<sprite_chain_element*> prepareSprites(long long elapsedTime) = 0;
	virtual void onSpriteChange();
	virtual int getSpriteNumber() = 0;
	[[nodiscard]] Vector2f getPosition() const { return position; }
	Vector2f *getPtrPosition() { return &position; }
	[[nodiscard]] Vector2f getTextureSize() const { return { textureBox.width, textureBox.height }; }
	[[nodiscard]] Vector2f getTextureOffset() const { return { textureBoxOffset.x, textureBoxOffset.y }; }
	[[nodiscard]] Vector2f getScaleRatio() const;
	[[nodiscard]] Vector2f getConditionalSizeUnits() const { return conditionalSizeUnits; }
	[[nodiscard]] Vector2f getMicroBlockCheckAreaBounds() const { return microBlockCheckAreaBounds; }
	[[nodiscard]] std::vector<Vector2i> getLockedMicroBlocks() const { return lockedMicroBlocks; }
	virtual Vector2f getBuildPosition(std::vector<world_object*> visibleItems, float scaleFactor, Vector2f cameraPosition) = 0;
	virtual int getBuildType(Vector2f ounPos, Vector2f otherPos) = 0;
	[[nodiscard]] FloatRect getOriginalTextureBox() const { return originalTextureBox; }
	[[nodiscard]] state getState() const { return state; }	
	std::pair<std::stack<birth_static_info>, std::stack<birth_dynamic_info>> getBirthObjects() { return std::make_pair(birthStatics, birthDynamics); }	

	void manuallyDisableMirroring() { mirrored = false; }
	void cancelMirroring();
	void clearBirthStack() { birthStatics = std::stack<birth_static_info>(); birthDynamics = std::stack<birth_dynamic_info>(); }
	void setPosition(Vector2f newPosition);
	void setHealthPoint(float healthPoint) { this->healthPoint = healthPoint; }
	void setName(std::string name) { this->name = std::move(name); }
	void deletePromiseOn() { deletePromise = true; }
	static void setUnscaled(const std::vector<sprite_chain_element*>& items) { for (auto& item : items) item->unscaled = true; }
	virtual void setTextureSize(Vector2f textureSize);
	void setState(state state) { this->state = state; }
	virtual void takeDamage(float damage, Vector2f attackerPos = { -1, -1 });
	virtual void initMicroBlocks();
	virtual bool isLockedPlace(const std::map<std::pair<int, int>, bool>& checkBlocks);

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
	float radius = 0, permissibleDistance = 0;
	state state = common;
	std::stack<birth_static_info> birthStatics;
	std::stack<birth_dynamic_info> birthDynamics;
	std::vector<Vector2i> lockedMicroBlocks = {};
};
