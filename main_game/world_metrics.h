#pragma once

#include <memory>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>

inline sf::Vector2f operator - (const sf::Vector2f v1, const sf::Vector2f v2) { return sf::Vector2f(v1.x - v2.x, v1.y - v2.y); }
inline sf::Vector2f operator + (const sf::Vector2f v1, const sf::Vector2f v2) { return sf::Vector2f(v1.x + v2.x, v1.y + v2.y); }
inline sf::Vector2f operator * (const sf::Vector2f v1, const sf::Vector2f v2) { return sf::Vector2f(v1.x * v2.x, v1.y * v2.y); }
inline sf::Vector2f operator * (const sf::Vector2f v, const float f) { return sf::Vector2f(v.x * f, v.y * f); }
inline sf::Vector2f operator / (const sf::Vector2f v1, const sf::Vector2f v2) { return sf::Vector2f(v1.x / v2.x, v1.y / v2.y); }
inline sf::Vector2f operator / (const sf::Vector2f v, const float f) { return sf::Vector2f(v.x / f, v.y / f); }

const float smallest_block_to_window_scale = 76;

namespace world_metrics
{
	inline std::shared_ptr<sf::RenderTarget> window;
	inline sf::Vector2f window_size;

	// all sizes are proportional to the smallest
	inline sf::Vector2f route_block_size; // the smallest block
	inline sf::Vector2f terrain_cell_block_size;
	inline sf::Vector2f block_size;

	inline sf::Vector2u matrix_size;

	inline sf::FloatRect constant_zone;
	inline sf::FloatRect visible_zone;
	inline sf::Vector2f center;

	inline void set_world_metrics(std::shared_ptr<sf::RenderTarget> window)
	{
		world_metrics::window = std::move(window);
		world_metrics::window_size = sf::Vector2f(world_metrics::window->getSize());
		
		world_metrics::route_block_size = window_size / smallest_block_to_window_scale;
		world_metrics::terrain_cell_block_size = route_block_size * 2.0f;
		world_metrics::block_size = route_block_size * 6.0f;

		world_metrics::constant_zone = sf::FloatRect(
			0
			, 0
			, (static_cast<long>(window_size.x) % static_cast<long>(block_size.x) ? window_size.x + 2 * block_size.x : window_size.x + block_size.x)
			, (static_cast<long>(window_size.y) % static_cast<long>(block_size.y) ? window_size.y + 2 * block_size.y : window_size.y + block_size.y));
		world_metrics::visible_zone = sf::FloatRect(block_size.x, block_size.y, window_size.x, window_size.y);
		world_metrics::center = sf::Vector2f(visible_zone.left + visible_zone.width / 2, visible_zone.top + visible_zone.height / 2);

		world_metrics::matrix_size = sf::Vector2u(world_metrics::constant_zone.width / world_metrics::block_size.x + 1,
			world_metrics::constant_zone.height / world_metrics::block_size.y + 1);
	}
};
