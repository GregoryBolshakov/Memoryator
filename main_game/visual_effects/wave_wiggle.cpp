#include "wave_wiggle.hpp"

#include "../helper.h"
#include "../math_constants.h"


wave_wiggle::wave_wiggle()
	: visual_effect("WaveWiggle")
{
}

bool wave_wiggle::on_load()
{
	if (!shader_.loadFromFile("Game/shaders/wave_wiggle.vert", "Game/shaders/wave_wiggle.frag"))
		return false;

	shader_.setUniform("pack", sf::Shader::CurrentTexture);

	return true;
}

void wave_wiggle::on_update()
{
	const auto t = clock_.getElapsedTime().asSeconds() / 20;
	const auto func = float((cos(t * pi) * cos(t * pi) * cos(t * 3 * pi) * cos(t * 5 * pi) * 0.5 + sin(t * 25 * pi) * 0.02) / 10.0 );
	//const auto func = sin(clock_.getElapsedTime().asSeconds()) / 10;
	shader_.setUniform("func", func);
}

void wave_wiggle::on_update(sf::RenderTarget& target, sf::Sprite& sprite)
{
	const auto texture_rect = sf::FloatRect(sprite.getTextureRect());
	const auto target_size = sf::Vector2f(target.getSize());
	const auto sprite_bounds = sf::FloatRect(sprite.getGlobalBounds());	
	const auto sprite_pos = target.mapCoordsToPixel(sf::Vector2f(sprite_bounds.left, sprite_bounds.top));
	
	shader_.setUniform("angle", float(sprite.getRotation() * pi / 180.0));
	shader_.setUniform("tex_pos", sf::Vector2f(texture_rect.left, texture_rect.top));
	shader_.setUniform("tex_size", sf::Vector2f(texture_rect.width, texture_rect.height));
	shader_.setUniform("sprite_pos", sf::Vector2f(float(sprite_pos.x), float(sprite_pos.y)));
	shader_.setUniform("sprite_size", sf::Vector2f(float(sprite_bounds.width), float(sprite_bounds.height)));
	shader_.setUniform("screen_size", sf::Vector2f(target_size.x, target_size.y));
}
