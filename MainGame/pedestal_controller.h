#pragma once

#include "terrain_object.h"

class pedestal_controller
{
public:
	pedestal_controller();
	~pedestal_controller();
	void start(terrain_object* object);
	void stop();
	void draw(RenderWindow* window, Vector2f cameraPosition, float scaleFactor);
	void handleEvents(Event& event);
	void interact(long long elapsedTime, Event event);
	bool isRunning() const { return running; }
	bool readyToStart = false;
	
private:
	bool running = false;
	terrain_object* boundObject = nullptr;
	Vector2f size = Vector2f(world_object::microBlockSize.x / 2, world_object::microBlockSize.y / 2);
	RectangleShape filedFigure;
	CircleShape focusFigure, centerFigure;
	std::vector<Vector2f> focuses;
	Vector2f cameraPosition = { 0, 0 }, lastMousePos = { 0, 0 }, centerPosition = { 0, 0 };
	float scaleFactor = 1;
	long long doubleClickTimer = long(1e6);
	long long elapsedTime = 0;
	void writeToFile();
	int selectedFocus = -1, selectedEllipse = -1;
	bool selectedCenter = false;
};
