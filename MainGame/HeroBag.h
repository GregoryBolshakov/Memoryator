#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>

#include "SpriteStructures.h"

using namespace sf;

enum BagState { bagOpen = 1, bagClosed = 2, bagOpening = 3, bagClosing = 4, ejected = 5 };

struct BagSpriteChain
{
	SpriteChainElement* ClosedBag = new SpriteChainElement();
	SpriteChainElement* ClosedBagSelected = new SpriteChainElement();
	SpriteChainElement* ClosedBagBig = new SpriteChainElement();
	SpriteChainElement* OpenedBag = new SpriteChainElement();
	SpriteChainElement* OpenedBagSelected = new SpriteChainElement();
};

struct Cell
{
	Vector2f position;
	std::pair<Tag, int> content;
};

class HeroBag
{
public:
	HeroBag();
	~HeroBag();
	void initialize(Vector2f position, bool isSelectable, std::vector<std::pair<Tag, int>> inventory = testInventory);
	static Cell createCell(Vector2f position, std::pair<Tag, int> content);
	[[nodiscard]] Vector2f getPosition() const { return position; }
	[[nodiscard]] Vector2f getSizeClosed() const { return sizeClosed; }
	[[nodiscard]] Vector2f getSizeOpen() const { return sizeOpen; }
	[[nodiscard]] Vector2f getLastMousePos() const { return lastMousePos; }
	[[nodiscard]] float getRadius() const;
	int getSelectedCell(Vector2f position);
	bool static canAfford(std::vector<std::pair<Tag, int>> recipe, std::vector<HeroBag>* bags, Cell* heldItem = nullptr);
	void static takeItems(std::vector<std::pair<Tag, int>> recipe, std::vector<HeroBag>* bags, Cell* heldItem = nullptr);
	bool static putItemsIn(std::vector<std::pair<Tag, int>>* loot, std::vector<HeroBag>* bags);
	bool static putItemIn(std::pair<Tag, int>* loot, std::vector<HeroBag>* bags);
	static std::vector<std::pair<Tag, int>> cellsToInventory(const std::vector<Cell>& cells);
	void mouseMove();
	void setPosition(Vector2f position) { this->position = position; }
	void fixCells();
	void fixPos();

	static std::unordered_map<Tag, int> itemsMaxCount;
	static std::vector<std::pair<Tag, int>> testInventory;
	static std::vector<std::pair<Tag, int>> emptyInventory;

	Vector2f textureOpenOffset, textureClosedOffset;
	std::vector<Cell> cells;

	//animation
	SpriteChainElement* prepareSprite(long long elapsedTime, std::map<PackTag, SpritePack>* packsMap);
	void drawCircuit(RenderWindow* window);

	long long stateChangeTime = 100000;
	long long stateChangingTime = 0;
	float speed = 0.001f;
	BagState currentState = bagClosed;
	float minDistToBorder = 0;
	float closedRadius = 0;
	float openedRadius = 0;
	Vector2f selectionZoneClosedOffset;
	Vector2f selectionZoneOpenedOffset;
	Vector2f shiftVector = { 0, 0 };
	Vector2f movePosition = { -1, -1 };
	bool readyToChangeState = false;
	bool readyToEject = false;
	bool wasMoved = false;
private:
	BagSpriteChain bagSpriteChain;
	Vector2f position;
	Vector2f textureSizeClosed;
	Vector2f textureSizeOpen;
	Vector2f sizeClosed;
	Vector2f sizeOpen;
	Vector2f lastMousePos = { 0, 0 };
	bool isSelectable{};
	std::vector<Vector2f> cellsPos = { {-0.1f, -0.15f}, {0.135f, -0.15f}, {-0.2f, 0.014f}, {0.018f, 0.0f}, {0.241f, 0.004f}, {-0.08f, 0.150f}, {0.131f, 0.150f} };
};
