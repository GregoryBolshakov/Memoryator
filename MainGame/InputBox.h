#pragma once
#ifndef  INPUTBOX_H
#define INPUTBOX_H
#include <SFML/Graphics.hpp>
#include <string>
#include "TextWriter.h"

using namespace sf;

class InputBox
{
public:
	InputBox();
	InputBox(IntRect rect);
	~InputBox();
	void init(IntRect rect);
	void draw(RenderWindow& window);
	void onMouseRelease();
	void handleEvents(Event event);
	void interact(long long elapsedTime);
	void resetCursor(bool toEnd = false);
	std::string line;
private:
	void setCursorToPos();
	void animateCursor();
	IntRect rect;
	int cursorPos = 0;
	Color outFill = Color(85, 47, 8, 220), inFill = Color(210, 163, 116, 60), inFillSelected = Color(235, 200, 166, 100), textColor = Color(235, 200, 166, 180);
	RectangleShape body, innerPart, cursor;
	bool mousePressed = false;
	std::pair<float, float> mouseSelectVector;
	int characterSize = 30, microOffset = 6;
	Text text;

	float timeForCursorAnimation = 5e5, timeAfterCursorAnimation = 0;
};
#endif


