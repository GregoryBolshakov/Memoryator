#pragma once

#include "../time_system.h"
#include "visual_effect.hpp"

class wave_wiggle final : public visual_effect
{
public:
	explicit wave_wiggle();

private:
	bool on_load() override;
	void on_update() override;
	void on_update(sf::RenderTarget& target, sf::Sprite& sprite) override;

	sf::Clock clock_;
};
