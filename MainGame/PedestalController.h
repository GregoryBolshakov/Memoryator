#pragma once
#ifndef PEDESTALCONTROLLER_H
#define PEDESTALCONTROLLER_H

#include "TerrainObject.h"

class PedestalController
{
public:
	PedestalController();
	~PedestalController();
	void start(TerrainObject* object);
	void stop();
	void draw(RenderWindow* window, Vector2f cameraPosition, float scaleFactor);
	void interact(float elapsedTime);
	bool isRunning() { return running; }
	bool readyToStart = false;
private:
	bool running = false;
	TerrainObject* boundObject = nullptr;
	Vector2f size = Vector2f(WorldObject::microBlockSize.x / 2, WorldObject::microBlockSize.y / 2);
	RectangleShape filedFigure;
	std::vector<CircleShape> focusFigures;
	std::vector<Vector2f> focuses;
	Vector2f cameraPosition = { 0, 0 }, lastMousePos = {0, 0};
	float scaleFactor = 0;
	void writeToFile();
};

#endif


