#pragma once
#ifndef  CONSOLE_H
#define CONSOLE_H
#include "InputBox.h"
#include "WorldHandler.h"
#include "PedestalController.h"

using namespace sf;

class Console
{
public:	
	Console(IntRect rect, WorldHandler* world = nullptr);
	~Console();
	InputBox getBody() { return body; }
	void draw(RenderWindow& window);
	void interact(long long elapsedTime);
	void handleEvents(Event event);
	void resetCommandStackIterator() { commandStackIterator = commandStack.size(); };
	bool getState() { return state; }
private:
	void doCommand();

	InputBox body;
	bool state = false;
	std::vector<std::string> commandStack = { {""} };
	int commandStackIterator = 0;
	WorldHandler* world = nullptr;
};
#endif
