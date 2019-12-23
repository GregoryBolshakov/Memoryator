#include "Console.h"

Console::Console(IntRect rect, WorldHandler* world)
{
	this->world = world;
	body.init(rect);
}

Console::~Console()
{
}

void Console::draw(sf::RenderWindow& window)
{
	if (!state)
		return;

	body.draw(window);
}

void Console::interact(long long elapsedTime)
{
	if (!state)
		return;
	//body.line = commandStack[commandStackIterator];
	body.interact(elapsedTime);	
}

void Console::handleEvents(Event event)
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
bool findByValue(std::vector<K> & vec, std::map<K, V> mapOfElemen, V value)
{
	bool bResult = false;
	auto it = mapOfElemen.begin();
	// Iterate through the map
	while (it != mapOfElemen.end())
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
		it++;
	}
	return bResult;
}

void Console::doCommand()
{
	resetCommandStackIterator();

	if (body.line.empty())
		return;

	std::vector<std::string> commands;
	std::string temp = body.line;
	std::string delimiter = " ";
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
	commandStack.push_back("");
	body.line.clear();

	if (!world)
		return;

	for (auto& command : commands)
		std::transform(command.begin(), command.end(), command.begin(), ::tolower);
	if (commands.size() >= 2)
	{	
		if (commands[0] == "spawn" && ObjectInitializer::mappedStrings.count(commands[1]) > 0)
		{
			auto object = ObjectInitializer::mappedStrings.at(commands[1]);
			if (int(object) >= 102 && int(object) <= 112)
				world->getWorldGenerator().initializeDynamicItem(object, { world->focusedObject->getPosition().x + 50, world->focusedObject->getPosition().y + 50 }, "");
		}
		if (commands[0] == "set" && commands[1] == "ellipse")		
			world->pedestalController.readyToStart = true;		
	}

	state = false;
}