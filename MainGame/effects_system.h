#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <unordered_set>

using namespace  sf;

enum class effects { transparency_removal = 1 };

struct effect
{
	effect() : body(nullptr), duration(0) {}
	effect(Drawable* body, const long long duration) : body(body), duration(duration) {}
	Drawable* body;
	long long duration;
	long long time_passed = 0;
	//std::string name;
};

class effects_system
{
public:
	effects_system();
	~effects_system();
	static void init();
	void reset_effects(const std::vector<std::string>& remove_list);
	void add_effect(effects effect_kind, Drawable* elem, const std::string& name, long long duration = default_duration);
	void interact(long long elapsed_time);

	static long long default_duration;
private:
	std::unordered_map<std::string, effect> transparency_removal_;
	std::unordered_set<std::string> names_;

};
