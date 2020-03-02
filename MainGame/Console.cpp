#include "console.h"

console::console(const FloatRect rect, WorldHandler* world)
{
	this->world = world;
	body.init(rect);
}

console::~console()
= default;

input_box console::getBody() const
{
	return body;
}

void console::resetCommandStackIterator()
{
	commandStackIterator = commandStack.size();
}

bool console::getState() const
{
	return state;
}

void console::draw(RenderWindow& window)
{
	if (!state)
		return;

	body.draw(window);
}

void console::interact(long long elapsedTime)
{
	if (!state)
		return;
	//body->line = commandStack[commandStackIterator];
	body.interact(elapsedTime);	
}

void console::handleEvents(const Event event)
{
	if (event.type == Event::KeyPressed && Keyboard::isKeyPressed(Keyboard::Tilde))
		state = !state;
	if (event.type == Event::KeyPressed && Keyboard::isKeyPressed(Keyboard::Enter))
		doCommand();
	if (event.type == Event::KeyPressed && Keyboard::isKeyPressed(Keyboard::Up) && commandStackIterator > 0)
	{
		commandStackIterator--;
		body.line = commandStack[commandStackIterator];
		body.resetCursor(true);
	}
	if (event.type == Event::KeyPressed && Keyboard::isKeyPressed(Keyboard::Down) && commandStackIterator < commandStack.size() - 1)
	{
		commandStackIterator++;
		body.line = commandStack[commandStackIterator];
		body.resetCursor(true);
	}

	if (!state)
		return;
	body.handleEvents(event);
}

template<typename K, typename V>
bool findByValue(std::vector<K> & vec, std::map<K, V> mapOfElements, V value)
{
	auto bResult = false;
	auto it = mapOfElements.begin();
	// Iterate through the map
	while (it != mapOfElements.end())
	{
		// Check if value of this entry matches with given value
		if (it->second == value)
		{
			// Yes found
			bResult = true;
			// Push the key in given map
			vec.push_back(it->first);
		}
		// Go to next entry in map
		++it;
	}
	return bResult;
}

void console::doCommand()
{
	resetCommandStackIterator();

	if (body.line.empty())
		return;

	std::vector<std::string> commands;
	std::string temp = body.line;
	const std::string delimiter = " ";
	if (temp[temp.size() - 1] != ' ')
		temp.push_back(' ');
	while (!temp.empty())
	{
		auto token = temp.substr(0, temp.find(delimiter));
		commands.push_back(token);
		temp.erase(0, token.size() + 1);
	}

	body.resetCursor();
	commandStack[commandStack.size() - 1] = body.line;
	commandStack.emplace_back("");
	body.line.clear();

	if (!world)
		return;

	for (auto& command : commands)
		std::transform(command.begin(), command.end(), command.begin(), tolower);
	if (commands.size() >= 2)
	{
		if (commands[0] == "spawn" && object_initializer::mappedStrings.count(commands[1]) > 0)
		{
			auto object = object_initializer::mappedStrings.at(commands[1]);
			if (int(object) >= 102 && int(object) <= 112)
				world->getWorldGenerator().initializeDynamicItem(object, { world->focusedObject->getPosition().x + 50, world->focusedObject->getPosition().y + 50 }, "");
		}
		if (commands[0] == "build" && object_initializer::mappedStrings.count(commands[1]) > 0)
		{
			auto typeOfObject = 1;
			if (commands.size() >= 3 && std::stoi(commands[2]) >= 1 && std::stoi(commands[2]) <= 1000)
				typeOfObject = std::stoi(commands[2]);
			auto object = object_initializer::mappedStrings.at(commands[1]);

			if (int(object) >= 211 || int(object) >= 301 && int(object) <= 405)
				world->setObjectToBuild(object, typeOfObject, true);	
		}
		if (commands[0] == "set" && commands[1] == "pedestal")
			world->pedestalController.readyToStart = true;		
	}

	state = false;
}