#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <unordered_set>

using namespace  sf;

enum class effects { transparencyRemoval = 1 };

struct effect
{
	effect() : body(nullptr), duration(0) {}
	effect(Drawable* body, const long long duration) : body(body), duration(duration) {}
	Drawable* body;
	long long duration;
	long long timePassed = 0;
	//std::string name;
};

class effects_system
{
public:
	effects_system();
	~effects_system();
	static void init();
	void resetEffects(const std::vector<std::string>& removeList);
	void add_effect(effects effect_kind, Drawable* elem, const std::string& name, long long duration = defaultDuration);
	void interact(long long elapsedTime);

	static long long defaultDuration;
private:
	std::unordered_map<std::string, effect> transparencyRemoval;
	std::unordered_set<std::string> names;

};
