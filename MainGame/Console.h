#pragma once

#include "input_box.h"
#include "world_handler.h"

using namespace sf;

class console
{
public:	
	console(FloatRect rect, world_handler* world = nullptr);
	~console();
	input_box getBody() const;
	void draw(RenderWindow& window);
	void interact(long long elapsedTime);
	void handleEvents(Event event);
	void resetCommandStackIterator();;
	bool getState() const;
private:
	void doCommand();

	input_box body;
	bool state = false;
	std::vector<std::string> commandStack = { {""} };
	size_t commandStackIterator = 0;
	world_handler* world = nullptr;
};

