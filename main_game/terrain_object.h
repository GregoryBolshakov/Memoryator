#pragma once
#ifndef TERRAIN_OBJECT_H
#define TERRAIN_OBJECT_H

#include "static_object.h"

class terrain_object : public static_object
{
public:
	terrain_object(std::string object_name, Vector2f center_position);
	virtual ~terrain_object();
	void init_micro_blocks() override;
	[[nodiscard]] Vector2f get_focus1() const { return focus1_; }
	[[nodiscard]] Vector2f get_focus2() const { return focus2_; }
	void setFocuses(std::vector<Vector2f> focuses);
	[[nodiscard]] Vector2f get_dot1() const { return dot1_; }
	[[nodiscard]] Vector2f get_dot2() const { return dot2_; }
	[[nodiscard]] Vector2f get_current_dot() const { return current_dot_; }
	std::vector<float> ellipse_size_multipliers = { 1.4f };
	float getEllipseSize(int i = 0);
	bool isIntersected(Vector2f cur_position, Vector2f new_position); //const;
	[[nodiscard]] std::vector<int> get_multi_ellipse_intersect(Vector2f position) const;
	[[nodiscard]] Vector2f new_slipping_position_for_dots_adjusted(Vector2f motion_vector, float speed, long long elapsed_time) const;	

	std::vector<std::pair<Vector2f, Vector2f>> internal_ellipses; // focus1 - focus2
protected:
	Vector2f focus1_, focus2_;
	Vector2f dot1_, dot2_, current_dot_ = Vector2f(-1, -1);
};

#endif