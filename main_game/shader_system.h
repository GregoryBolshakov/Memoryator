#pragma once

#include "sprite_chain_element.h"
#include "visual_effects/visual_effect.hpp"

using std::shared_ptr, std::make_shared;

class camera_system;
class scale_system;
class time_system;
class world_handler;

enum class shader_kind { dynamic_light = 0, wave_wiggle = 1 };

class shader_system
{
public:
	shader_system(shared_ptr<camera_system>& camera_system, const shared_ptr<time_system>& time_system);
	~shader_system() = default;
	void initialize();
	void update();

	const sf::Shader& get_shader(shader_kind kind);
	const sf::Shader& get_shader(sf::RenderTarget& target, sprite_chain_element&, sf::Sprite& sprite);
private:
	std::map<shader_kind, std::unique_ptr<visual_effect>> shaders_;
	shared_ptr<camera_system> camera_system_;
	shared_ptr<time_system> time_system_;
	sf::Shader empty_shader_;
};
