#pragma once
#ifndef EFFECTSSYSTEM_H
#define EFFECTSSYSTEM_H

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <unordered_set>

using namespace  sf;

enum class Effects { transparencyRemoval = 1 };

struct Effect
{
	Effect() : body(nullptr), duration(0) {}
	Effect(Drawable* body, float duration) : body(body), duration(duration) {}
	Drawable* body;
	float duration;
	float timePassed = 0;
	//std::string name;
};

class EffectsSystem
{
public:
	EffectsSystem();
	~EffectsSystem();
	void init();
	void resetEffects(std::vector<std::string> removeList);
	void addEffect(Effects effect, Drawable* elem, std::string name, float duration = defaultDuration);
	void interact(long long elapsedTime);

	static float defaultDuration;
private:
	std::unordered_map<std::string, Effect> transparencyRemoval;
	std::unordered_set<std::string> names;

};

#endif