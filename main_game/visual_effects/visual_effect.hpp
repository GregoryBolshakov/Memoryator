#pragma once

#include <SFML/Graphics.hpp>

class visual_effect
{
public:
	void load();
	void update();
	void update(sf::RenderTarget& target, sf::Sprite& sprite);
	virtual ~visual_effect() = default;

	sf::Shader* shader;
protected:
	explicit visual_effect(std::string name);
	sf::Shader shader_;

private:
	virtual bool on_load() = 0;
	virtual void on_update();
	virtual void on_update(sf::RenderTarget& target, sf::Sprite& sprite);

	std::string name_;
	bool is_loaded_;
};

inline visual_effect::visual_effect(std::string name) :
	name_(std::move(name)),
	is_loaded_(false)
{
	shader = &shader_;
}

inline void visual_effect::on_update()
{
}

inline void visual_effect::on_update(sf::RenderTarget& target, sf::Sprite& sprite)
{
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

inline void visual_effect::update(sf::RenderTarget& target, sf::Sprite& sprite)
{
	if (is_loaded_)
		on_update(target, sprite);
}
