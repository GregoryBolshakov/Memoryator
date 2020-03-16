#pragma once

#include "../camera_system.h"
#include "../time_system.h"
#include "visual_effect.hpp"


class dynamic_light final : public visual_effect
{
public:
	explicit dynamic_light(camera_system& camera_system, time_system& time_system);

private:
	bool on_load() override;
	void on_update() override;

	camera_system& camera_system_;
	time_system& time_system_;

	Texture overlay_texture_;
	Texture multiply_texture_;
	Texture field_of_view_texture_;
};
