#include "effects_system.h"

long long effects_system::default_duration = long(10e5);

effects_system::effects_system()
= default;


effects_system::~effects_system()
= default;

void effects_system::init()
{
	//for (auto& item : names)	
		//item.second = false;	
}

void effects_system::reset_effects(const std::vector<std::string>& remove_list)
{
	auto cnt = -1;
	for (const auto& name : remove_list)
	{
		if (names_.count(name) == 0)
			continue;
		cnt++;
		auto sprite = dynamic_cast<Sprite*>(transparency_removal_.at(name).body);
		if (sprite)
		{
			const auto spriteColor = sprite->getColor();
			sprite->setColor(Color(spriteColor.r, spriteColor.g, spriteColor.b, 255));
		}
		const auto text = dynamic_cast<Text*>(transparency_removal_.at(name).body);
		if (text)
		{
			const auto textColor = text->getFillColor();
			text->setFillColor(Color(textColor.r, textColor.g, textColor.b, 255));
		}
		transparency_removal_.erase(name);
		names_.erase(name);
	}
}


void effects_system::add_effect(const effects effect_kind, Drawable* elem, const std::string& name, const long long duration)
{
	switch (effect_kind)
	{
		case effects::transparency_removal:
			if (names_.count(name) == 0)
			{
				names_.insert(name);
				transparency_removal_[name] = effect(elem, duration);
				auto sprite = dynamic_cast<Sprite*>(transparency_removal_.at(name).body);
				if (sprite)
					sprite->setColor(Color(sprite->getColor().r, sprite->getColor().g, sprite->getColor().b, 0));
				auto text = dynamic_cast<Text*>(transparency_removal_.at(name).body);
				if (text)
					text->setFillColor(Color(text->getFillColor().r, text->getFillColor().g, text->getFillColor().b, 0));
			}
			break;
	}
}


void effects_system::interact(const long long elapsed_time)
{
	auto cnt = -1;
	for (const auto& name : names_)
	{
		cnt++;
		if (transparency_removal_.at(name).time_passed >= transparency_removal_.at(name).duration)
		{
			transparency_removal_.erase(name);
			names_.erase(name);				
			continue;
		}
		auto sprite = dynamic_cast<Sprite*>(transparency_removal_.at(name).body);
		if (sprite)
		{
			const auto spriteColor = sprite->getColor();
			sprite->setColor(Color(spriteColor.r, spriteColor.g, spriteColor.b, Uint8(255 * transparency_removal_.at(name).time_passed / transparency_removal_.at(name).duration)));			
			transparency_removal_.at(name).time_passed += elapsed_time;
		}
		const auto text = dynamic_cast<Text*>(transparency_removal_.at(name).body);
		if (text)
		{
			const auto textColor = text->getFillColor();
			text->setFillColor(Color(textColor.r, textColor.g, textColor.b, Uint8(255 * transparency_removal_.at(name).time_passed / transparency_removal_.at(name).duration)));
			transparency_removal_.at(name).time_passed += elapsed_time;
		}
	}
}