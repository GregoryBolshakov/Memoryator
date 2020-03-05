#pragma once

#include <SFML/Graphics.hpp>



class visual_effect: public sf::RenderStates, public sf::NonCopyable
{
public:
	void load();
	void update();

	sf::Shader* shader;

protected:
	~visual_effect();
	explicit visual_effect(std::string name);
	sf::Shader shader_;

private:
	virtual bool on_load() = 0;
	virtual void on_update() = 0;

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
	is_loaded_ = sf::Shader::isAvailable() && on_load();
}

inline void visual_effect::update()
{
	if (is_loaded_)
		on_update();
}
