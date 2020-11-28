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

	camera_system();
	~camera_system() = default;

	sf::Vector2f position = { 0, 0 };

	static sf::Vector2f get_screen_size();

	void make_shake(int count = 0, float speed = 0);
	void shake_interact(long long elapsed_time);
	void set_focus(std::weak_ptr<world_object> focus);
	[[nodiscard]] const std::shared_ptr<scale_system>& get_scale_system() const { return scale_system_; }
	[[nodiscard]] sf::Vector2f focused_object_screen_position_normalized() const;
	[[nodiscard]] sf::Vector2f object_screen_position(sf::Vector2f pos) const;
	[[nodiscard]] const std::weak_ptr<world_object>& focused_object() const { return focused_object_; }

private:
	std::shared_ptr<scale_system> scale_system_;
	std::weak_ptr<world_object> focused_object_;
	sf::Vector2f shake_vector_;
	long long shake_duration_ = 0;
	float shake_speed_ = shake_default_speed;
	int shake_count_ = 0;
};
