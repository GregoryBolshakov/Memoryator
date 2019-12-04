#pragma once
#ifndef WORLDHANDLER_H
#define WORLDHANDLER_H

#include <unordered_map>
#include <ltbl/lighting/LightSystem.h>
#include <windows.h>

#include "Helper.h"
#include "EffectsSystemMaker.h"
#include "WorldGenerator.h"

#include "DynamicObject.h"
#include "EmptyObject.h"

#include "ForestTree.h"
#include "Brazier.h"

using namespace sf;

typedef void(*LPSEARCHFUNC)(LPCTSTR lpszFileName, std::unordered_map<std::string, BoardSprite> &spriteMap);
typedef bool(*func)(Vector2f &pos);

class WorldHandler
{
public:
	WorldHandler(int width, int height);
	~WorldHandler();

	//lightSystem
	void initLightSystem(RenderWindow &window);
	void renderLightSystem(View view, RenderWindow &window);

	//adding to the grid
	void birthObjects();

	//getters
	Vector2f getWorldSize() { return Vector2f (width, height); }
	GridList getStaticGrid() { return staticGrid; }
	GridList getDynamicGrid() { return dynamicGrid; }
	Vector2f getCameraPosition() { return cameraPosition; }
	InventoryMaker& getInventorySystem() { return inventorySystem; }
	BuildSystemMaker& getBuildSystem() { return buildSystem; }
	WorldGenerator& getWorldGenerator() { return worldGenerator; }
	ltbl::LightSystem& getLightSystem() { return ls; }
	std::string getMouseDisplayName() const { return mouseDisplayName; }
	WorldObject* getSelectedObject() const { return selectedObject; }

	//save-load logic
	void clearWorld();
	void Load();
	void Save();
	

	//base (draw, interact)	
	std::unordered_map<std::string, BoardSprite> spriteMap;
	void interact(RenderWindow& window, long long elapsedTime);
	void draw(RenderWindow& window, long long elapsedTime);
	void setItemFromBuildSystem();
	void drawVisibleItems(RenderWindow& window, long long elapsedTime, std::vector<spriteChainElement> sprites);
	Vector2f worldUpperLeft, worldBottomRight;
	void runBuildSystemDrawing(RenderWindow& window, float elapsedTime);
	void runInventorySystemDrawing(RenderWindow& window, float elapsedTime);
	void runWorldGenerator();

	//zoom
	float scaleFactor;
	float mainScale;
	void setScaleFactor(int delta);
	void scaleSmoothing();
	float scaleDecrease, timeForScaleDecrease = 0;
	Clock scaleDecreaseClock;
	std::map<Tag, bool> unscaledObjects = {{Tag::hero1, true}, {Tag::nightmare1, true}, {Tag::nightmare2, true}, {Tag::nightmare3, true}, {Tag::grass, true} };

	//hero
	DynamicObject* focusedObject = nullptr;
	Brazier* brazier;

	//events
	void onMouseUp(int currentMouseButton);
	bool getHeroBookVisability() { return isHeroBookVisible; }
	void changeBookVisability() { isHeroBookVisible = !isHeroBookVisible; }

	Vector2i currentTransparentPos = Vector2i(0, 0);
	std::string debugInfo = "";
private:
	//lightSystem
	const Color commonWorldColor = Color(0, 0, 0, 255),
		//commonWorldHandlerColorOutfill = Color(240, 200, 200, 255),
		commonWorldColorOutfill = Color(99, 161, 159, 205),
		spiritWorldColor = Color(73, 193, 214, 255),
		spiritWorldColorOutfill = Color(12, 78, 89, 255);
	ContextSettings contextSettings;
	sf::RenderStates lightRenderStates;
	sf::Sprite Lsprite;//Спрайт света.
	Texture pointLightTexture, directionLightTexture;// Текстура света.
	Texture penumbraTexture;// Текстура полутени.
	Shader unshadowShader, lightOverShapeShader;// Шейдеры для рендера света.
	ltbl::LightSystem ls;//Глобальная система света и тени.	
	sf::View view;
	std::shared_ptr<ltbl::LightPointEmission> brightner = nullptr;

	//hero
	const std::string heroTextureName = "Game/worldSprites/hero/move/body/down/1.png";
	bool isHeroBookVisible = false;

	//world base
	float width, height;
	Vector2i blockSize = { 1000, 1000 }, microblockSize = { 20, 20 };
	Vector2f cameraPosition, maxCameraDistance = Vector2f(100, 100), camOffset = { 0, -0.04f};
	void initSpriteMap();
	bool searchFiles(LPCTSTR lpszFileName, LPSEARCHFUNC lpSearchFunc, bool bInnerFolders = true);
	float WorldHandler::getScaleFactor();
	std::string spriteNameFileDirectory = "Game/objects.txt";
	const float heroToScreenRatio = 0.25f;
	bool fixedClimbingBeyond(Vector2f &pos);
	void makeCameraShake(float power = 0.0002f);
	void cameraShakeInteract(float elapsedTime);
	Vector2f cameraShakeVector = { 0, 0 };
	float cameraShakeDuration = 0, cameraShakePower = 0.0002f;
	WorldGenerator worldGenerator;

	//time logic
	Clock timer;
	int newNameId = 10;	
	float timeForNewSave = 0, timeAfterSave = 6e7;
	float timeForNewRoute = 3e5, timeAfterNewRoute = 3e5;

	//selection logic
	void setTransparent(std::vector<WorldObject*> visibleItems, float elapsedTime);
	std::string mouseDisplayName;
	WorldObject *selectedObject = focusedObject;

	//shaders
	sf::Shader spiritWorldShader;
	sf::Texture distortionMap;
	void initShaders();
	EffectsSystemMaker effectSystem;
	UIEffectsSystemMaker uiEffectSystem;

	//inventorySystem
	InventoryMaker inventorySystem;
	BuildSystemMaker buildSystem;

	//grids
	GridList staticGrid;
	GridList dynamicGrid;
	std::vector<spriteChainElement> visibleBackground, visibleTerrain;
	std::vector<WorldObject*> localTerrain;

	//test
	std::vector<std::pair<Tag, int>>* testInv = new std::vector<std::pair<Tag, int>>({ {Tag::chamomile, 2}, {Tag::chamomile, 2}, {Tag::chamomile, 2} });
};

#endif