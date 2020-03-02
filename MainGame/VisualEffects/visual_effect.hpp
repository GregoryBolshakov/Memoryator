#pragma once

#include <SFML/Graphics.hpp>

using namespace sf;


class visual_effect: public RenderStates, public NonCopyable
{
public:
	void load();
	void update(float x, float y, long long time);

	Shader* shader;

protected:
	~visual_effect();
	explicit visual_effect(std::string name);
	Shader shader_;

private:
	virtual bool on_load() = 0;
	virtual void on_update(float x, float y, long long time = 0) = 0;

	std::string name_;
	bool is_loaded_;
};

inline visual_effect::~visual_effect() = default;

inline visual_effect::visual_effect(std::string name) :
	name_(std::move(name)),
	is_loaded_(false)
{
	shader = &shader_;
}

inline void visual_effect::load()
{
	is_loaded_ = Shader::isAvailable() && on_load();
}

inline void visual_effect::update(const float x, const float y, const long long time = 0)
{
	if (is_loaded_)
		on_update(x, y, time);
}
