#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <unordered_set>

using namespace  sf;

enum class Effects { transparencyRemoval = 1 };

struct Effect
{
	Effect() : body(nullptr), duration(0) {}
	Effect(Drawable* body, const long long duration) : body(body), duration(duration) {}
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
	void addEffect(Effects effect, Drawable* elem, const std::string& name, long long duration = defaultDuration);
	void interact(long long elapsedTime);

	static long long defaultDuration;
private:
	std::unordered_map<std::string, Effect> transparencyRemoval;
	std::unordered_set<std::string> names;

};
