#pragma once

#include "InputBox.h"
#include "WorldHandler.h"

using namespace sf;

class console
{
public:	
	console(FloatRect rect, WorldHandler* world = nullptr);
	~console();
	InputBox getBody() const;
	void draw(RenderWindow& window);
	void interact(long long elapsedTime);
	void handleEvents(Event event);
	void resetCommandStackIterator();;
	bool getState() const;
private:
	void doCommand();

	InputBox body;
	bool state = false;
	std::vector<std::string> commandStack = { {""} };
	size_t commandStackIterator = 0;
	WorldHandler* world = nullptr;
};

