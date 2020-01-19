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
	void handleEvents(Event& event);
	void interact(float elapsedTime, Event event);
	bool isRunning() { return running; }
	bool readyToStart = false;
private:
	bool running = false;
	TerrainObject* boundObject = nullptr;
	Vector2f size = Vector2f(WorldObject::microBlockSize.x / 2, WorldObject::microBlockSize.y / 2);
	RectangleShape filedFigure;
	CircleShape focusFigure, centerFigure;
	std::vector<Vector2f> focuses;
	Vector2f cameraPosition = { 0, 0 }, lastMousePos = { 0, 0 }, centerPosition = { 0, 0 };
	float scaleFactor = 0, doubleClickTimer = 1e6, elapsedTime = 0;
	void writeToFile();
	int selectedFocus = -1, selectedEllipse = -1;
	bool selectedCenter = false;
};

#endif


