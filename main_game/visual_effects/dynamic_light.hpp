#pragma once

#include "../camera_system.h"
#include "../time_system.h"
#include "../world_handler.h"
#include "visual_effect.hpp"


class dynamic_light final : public visual_effect
{
public:
	explicit dynamic_light(
		time_system& time_system,
		world_handler& world_handler,
		Vector2f render_target_size);

private:
	bool on_load() override;
	void on_update() override;

	time_system& time_system_;

	// TODO: Replace the god like world handler with a dedicated system
	world_handler& world_handler_;
	
	Vector2f render_target_size_;
	Texture overlay_texture_;
	Texture multiply_texture_;
	Texture field_of_view_texture_;
};
