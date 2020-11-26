#pragma once
#ifndef TERRAIN_OBJECT_H
#define TERRAIN_OBJECT_H

#include "static_object.h"

class terrain_object : public static_object
{
public:
	terrain_object(std::string object_name, sf::Vector2f center_position);
	virtual ~terrain_object();
	void init_route_blocks() override;
	void set_position(sf::Vector2f new_position) override;
	[[nodiscard]] sf::Vector2f get_focus1() const { return focus1_; }
	[[nodiscard]] sf::Vector2f get_focus2() const { return focus2_; }
	void set_focuses(std::vector<sf::Vector2f> focuses);
	std::vector<float> ellipse_size_multipliers = { 1.4f };
	float get_ellipse_size(int i = 0);
	bool is_intersected(sf::Vector2f position) const;
	[[nodiscard]] std::vector<int> get_multi_ellipse_intersect(sf::Vector2f position) const;

	std::vector<std::pair<sf::Vector2f, sf::Vector2f>> internal_ellipses; // focus1 - focus2
protected:
	sf::Vector2f focus1_, focus2_;
};

#endif