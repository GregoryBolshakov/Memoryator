#include "console.h"

console::console(const FloatRect rect, time_system& time_system, world_handler* world) :time_system_{ time_system }
{
	this->world_ = world;
	body.init(rect);
}

console::~console()
= default;

input_box console::get_body() const
{
	return body;
}

void console::reset_command_stack_iterator()
{
	command_stack_iterator_ = command_stack_.size();
}

bool console::get_state() const
{
	return state_;
}

void console::draw(RenderWindow & window) const
{
	if (!state_)
		return;

	body.draw(window);
}

void console::interact(long long elapsed_time)
{
	if (!state_)
		return;
	//body->line = commandStack[commandStackIterator];
	body.interact(elapsed_time);
}

void console::handle_events(const Event event)
{
	if (event.type == Event::KeyPressed && Keyboard::isKeyPressed(Keyboard::Tilde))
		state_ = !state_;
	if (event.type == Event::KeyPressed && Keyboard::isKeyPressed(Keyboard::Enter))
		do_command();
	if (event.type == Event::KeyPressed && Keyboard::isKeyPressed(Keyboard::Up) && command_stack_iterator_ > 0)
	{
		command_stack_iterator_--;
		body.line = command_stack_[command_stack_iterator_];
		body.reset_cursor(true);
	}
	if (event.type == Event::KeyPressed && Keyboard::isKeyPressed(Keyboard::Down) && command_stack_iterator_ < command_stack_.size() - 1)
	{
		command_stack_iterator_++;
		body.line = command_stack_[command_stack_iterator_];
		body.reset_cursor(true);
	}

	if (!state_)
		return;
	body.handle_events(event);
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

void console::do_command()
{
	reset_command_stack_iterator();

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

	body.reset_cursor();
	command_stack_[command_stack_.size() - 1] = body.line;
	command_stack_.emplace_back("");
	body.line.clear();

	if (!world_)
		return;

	for (auto& command : commands)
		std::transform(command.begin(), command.end(), command.begin(), tolower);

	if (commands.size() >= 3)
	{
		if (commands[0] == "set" && commands[1] == "time")
		{
			const auto time = std::stoll(commands[2]);
			if (time != -1)
				time_system_.set_time_total_micro_seconds(time);
		}
		if (commands[0] == "set" && commands[1] == "daypart")
		{
			const auto day_part = std::stod(commands[2]);
			if (day_part != -1)
				time_system_.set_day_part(day_part);
		}
	}
	if (commands.size() >= 2)
	{
		if (commands[0] == "spawn" && object_initializer::mapped_strings.count(commands[1]) > 0)
		{
			auto object = object_initializer::mapped_strings.at(commands[1]);
			if (int(object) >= 102 && int(object) <= 112)
				world_->getWorldGenerator().initialize_dynamic_item(object, { world_->focusedObject->get_position().x + 50, world_->focusedObject->get_position().y + 50 }, "");
		}
		if (commands[0] == "build" && object_initializer::mapped_strings.count(commands[1]) > 0)
		{
			auto typeOfObject = 1;
			if (commands.size() >= 3 && std::stoi(commands[2]) >= 1 && std::stoi(commands[2]) <= 1000)
				typeOfObject = std::stoi(commands[2]);
			auto object = object_initializer::mapped_strings.at(commands[1]);

			if (int(object) >= 211 || int(object) >= 301 && int(object) <= 405)
				world_->setObjectToBuild(object, typeOfObject, true);
		}
		if (commands[0] == "set" && commands[1] == "pedestal")
			world_->pedestalController.ready_to_start = true;
	}

	state_ = false;
}