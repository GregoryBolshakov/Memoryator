#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "TextSystem.h"

using namespace sf;

class InputBox
{
public:
	InputBox();
	InputBox(FloatRect rect);
	~InputBox();
	void init(FloatRect rect);
	void draw(RenderWindow& window) const;
	void onMouseRelease();
	void handleEvents(Event event);
	void interact(long long elapsedTime);
	void resetCursor(bool toEnd = false);
	std::string line;
	
private:
	void setCursorToPos();
	void animateCursor();
	FloatRect rect;
	size_t cursorPos = 0;
	sf::Color outFill = sf::Color(85, 47, 8, 220);
	sf::Color inFill = sf::Color(210, 163, 116, 60);
	sf::Color inFillSelected = sf::Color(235, 200, 166, 100);
	sf::Color textColor = sf::Color(235, 200, 166, 180);
	RectangleShape body, innerPart, cursor;
	bool mousePressed = false;
	std::pair<float, float> mouseSelectVector;
	float characterSize = 30.0f;
	float microOffset = 6.0f;
	Text text;

	long long timeForCursorAnimation = long(5e5);
	long long timeAfterCursorAnimation = 0;
};
