#pragma once

#include <memory>
#include <SFML/System/Vector2.hpp>

class world_object;
class scale_system;

class camera_system
{
public:
	static const sf::Vector2f max_camera_distance;
	static const sf::Vector2f cam_offset;
	static const float shake_default_speed;
	static const long long shake_default_duration;

	explicit camera_system(const std::shared_ptr<scale_system>& scale_system);

	sf::Vector2f position = { 0, 0 };

	static sf::Vector2f get_screen_size();

	void make_shake(int count = 0, float speed = 0);
	void shake_interact(long long elapsed_time);
	void set_focused_object(const std::shared_ptr<world_object>& focused_object) { focused_object_ = focused_object; }
	[[nodiscard]] const std::shared_ptr<scale_system>& get_scale_system() const { return scale_system_; };
	[[nodiscard]] sf::Vector2f focused_object_screen_position_normalized() const;
	[[nodiscard]] sf::Vector2f object_screen_position(sf::Vector2f pos) const;

private:
	std::shared_ptr<scale_system> scale_system_;
	std::shared_ptr<world_object> focused_object_;
	sf::Vector2f shake_vector_ = { 0, 0 };
	long long shake_duration_ = 0;
	float shake_speed_ = shake_default_speed;
	int shake_count_ = 0;
};
