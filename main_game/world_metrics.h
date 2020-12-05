#pragma once

#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Rect.hpp>
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
	inline std::weak_ptr<sf::RenderWindow> window;
	inline sf::Vector2f window_size;

	// all sizes are proportional to the smallest
	inline sf::Vector2f route_block_size; // the smallest block
	inline sf::Vector2f terrain_cell_block_size;
	inline sf::Vector2f block_size;

	inline sf::Vector2u matrix_size;

	inline sf::FloatRect constant_zone; // coords always (0,0)
	inline sf::FloatRect visible_zone;
	inline sf::FloatRect constant_zone_no_scale;
	inline sf::FloatRect visible_zone_no_scale;
	inline sf::Vector2f center; // in world position
	inline float scale, furthest_factor = 0.8f, closest_factor = 1.5f;
	inline float closest_scale, further_scale;
	inline int get_fps(const long long elapsed_time)
	{
		return 1000000 / elapsed_time;
	}
	
	inline void set_world_metrics(const std::shared_ptr<sf::RenderWindow>& window)
	{
		// IMPORTANT: the scale will be calculated only AT THE STEP OF WORLD GENERATION, because only then will the hero appear
		// therefore, the first call can and should be before creating scale_system. So the scale is 1.
		world_metrics::window = window;
		window_size = sf::Vector2f(window->getSize());
		world_metrics::scale = 1;
		
		route_block_size = window_size / smallest_block_to_window_scale;
		terrain_cell_block_size = route_block_size * 2.0f;
		block_size = route_block_size * 6.0f;

		constant_zone_no_scale = sf::FloatRect(
			0
			, 0
			, (static_cast<long>(window_size.x) % static_cast<long>(block_size.x) ? window_size.x + 2 * block_size.x : window_size.x + block_size.x)
			, (static_cast<long>(window_size.y) % static_cast<long>(block_size.y) ? window_size.y + 2 * block_size.y : window_size.y + block_size.y));
		constant_zone = sf::FloatRect(0, 0, constant_zone_no_scale.width / scale, constant_zone_no_scale.height / scale);
		visible_zone_no_scale = sf::FloatRect(block_size.x, block_size.y, window_size.x, window_size.y);
		visible_zone = sf::FloatRect(
			visible_zone_no_scale.left * scale, 
			visible_zone_no_scale.top * scale,
			constant_zone_no_scale.width / scale
			, constant_zone_no_scale.height / scale);
		center = sf::Vector2f(visible_zone.left + visible_zone.width / 2, visible_zone.height + visible_zone.height / 2);
	}

	inline void update_scale(const float scale)
	{
		world_metrics::scale = scale;
		window_size = sf::Vector2f(window.lock()->getSize());
		// division by scale instead of multiplication because it's window, not the object
		constant_zone = sf::FloatRect(constant_zone_no_scale.left, constant_zone_no_scale.top, constant_zone_no_scale.width / scale, constant_zone_no_scale.height / scale);
		visible_zone = sf::FloatRect(visible_zone_no_scale.left, visible_zone_no_scale.top, window_size.x / scale, window_size.y / scale);
		matrix_size = sf::Vector2u(constant_zone.width / block_size.x + 1, constant_zone.height / block_size.y + 1);
		center = sf::Vector2f(visible_zone.left + visible_zone.width / 2, visible_zone.top + visible_zone.height / 2);
		further_scale = furthest_factor * scale;
		closest_scale = closest_factor * scale;
	}

	inline void update_further(const float scale)
	{
		world_metrics::further_scale = scale * world_metrics::furthest_factor;
		world_metrics::closest_scale = scale * world_metrics::closest_factor;
	}

	inline sf::Vector2f screen_to_world_position(const sf::Vector2f position)
	{
		return position / scale + sf::Vector2f(visible_zone.left, visible_zone.top);
	}

	/* not used: inline sf::Vector2f screen_to_world_draw_position(const sf::Vector2f position, const sf::Vector2f offset)
	{
		return (position + offset) / scale + sf::Vector2f(visible_zone.left, visible_zone.top);
	}*/

	inline sf::Vector2f world_to_screen_position(const sf::Vector2f position)
	{
		return (position - sf::Vector2f(visible_zone.left, visible_zone.top)) * scale;
	}

	inline sf::Vector2f world_to_screen_draw_position(const sf::Vector2f position, const sf::Vector2f offset)
	{
		return (position - offset - sf::Vector2f(visible_zone.left, visible_zone.top)) * scale;
	}

	inline float get_dist(const sf::Vector2f a, const sf::Vector2f b)
	{
		return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
	}

	inline float triangle_area(const float x1, const float y1, const float x2, const float y2, const float x3, const float y3)
	{
		return (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1);
	}
};
