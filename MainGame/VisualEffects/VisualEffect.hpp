#pragma once

#include <SFML/Graphics.hpp>

using namespace sf;


class VisualEffect: public RenderStates, public NonCopyable
{
public:
	void load();
	void update(float x, float y, long long time);

	Shader* shader;

protected:
	~VisualEffect();
	explicit VisualEffect(std::string name);
	Shader shader_;

private:
	virtual bool on_load() = 0;
	virtual void on_update(float x, float y, long long time = 0) = 0;

	std::string name_;
	bool is_loaded_;
};

inline VisualEffect::~VisualEffect() = default;

inline VisualEffect::VisualEffect(std::string name) :
	name_(std::move(name)),
	is_loaded_(false)
{
	shader = &shader_;
}

inline void VisualEffect::load()
{
	is_loaded_ = Shader::isAvailable() && on_load();
}

inline void VisualEffect::update(const float x, const float y, const long long time = 0)
{
	if (is_loaded_)
		on_update(x, y, time);
}
