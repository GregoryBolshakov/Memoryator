#include "effects_system.h"

long long effects_system::defaultDuration = long(10e5);

effects_system::effects_system()
= default;


effects_system::~effects_system()
= default;

void effects_system::init()
{
	//for (auto& item : names)	
		//item.second = false;	
}

void effects_system::resetEffects(const std::vector<std::string>& removeList)
{
	auto cnt = -1;
	for (const auto& name : removeList)
	{
		if (names.count(name) == 0)
			continue;
		cnt++;
		auto sprite = dynamic_cast<Sprite*>(transparencyRemoval.at(name).body);
		if (sprite)
		{
			const auto spriteColor = sprite->getColor();
			sprite->setColor(Color(spriteColor.r, spriteColor.g, spriteColor.b, 255));
		}
		const auto text = dynamic_cast<Text*>(transparencyRemoval.at(name).body);
		if (text)
		{
			const auto textColor = text->getFillColor();
			text->setFillColor(Color(textColor.r, textColor.g, textColor.b, 255));
		}
		transparencyRemoval.erase(name);
		names.erase(name);
	}
}


void effects_system::addEffect(Effects effect, Drawable* elem, const std::string& name, const long long duration)
{
	switch (effect)
	{
		case Effects::transparencyRemoval:
			if (names.count(name) == 0)
			{
				names.insert(name);
				transparencyRemoval[name] = Effect(elem, duration);
				auto sprite = dynamic_cast<Sprite*>(transparencyRemoval.at(name).body);
				if (sprite)
					sprite->setColor(Color(sprite->getColor().r, sprite->getColor().g, sprite->getColor().b, 0));
				auto text = dynamic_cast<Text*>(transparencyRemoval.at(name).body);
				if (text)
					text->setFillColor(Color(text->getFillColor().r, text->getFillColor().g, text->getFillColor().b, 0));
			}
			break;
	}
}


void effects_system::interact(const long long elapsedTime)
{
	auto cnt = -1;
	for (const auto& name : names)
	{
		cnt++;
		if (transparencyRemoval.at(name).timePassed >= transparencyRemoval.at(name).duration)
		{
			transparencyRemoval.erase(name);
			names.erase(name);				
			continue;
		}
		auto sprite = dynamic_cast<Sprite*>(transparencyRemoval.at(name).body);
		if (sprite)
		{
			const auto spriteColor = sprite->getColor();
			sprite->setColor(Color(spriteColor.r, spriteColor.g, spriteColor.b, Uint8(255 * transparencyRemoval.at(name).timePassed / transparencyRemoval.at(name).duration)));			
			transparencyRemoval.at(name).timePassed += elapsedTime;
		}
		const auto text = dynamic_cast<Text*>(transparencyRemoval.at(name).body);
		if (text)
		{
			const auto textColor = text->getFillColor();
			text->setFillColor(Color(textColor.r, textColor.g, textColor.b, Uint8(255 * transparencyRemoval.at(name).timePassed / transparencyRemoval.at(name).duration)));
			transparencyRemoval.at(name).timePassed += elapsedTime;
		}
	}
}