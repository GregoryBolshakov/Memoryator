#pragma once

#include "dynamic_object.h"
#include "helper.h"

using namespace sf;

class owl_boss : public dynamic_object
{
private:
	float time_for_new_sprite_ = 0;
	float jerk_distance_ = 0;
	int animation_length_;
	int flaps_before_jerk_count_ = 4;
	bool was_hit_ = false;
public:
	owl_boss(std::string objectName, Vector2f centerPosition);
	~owl_boss();
	Vector2f calculate_texture_offset() override;
	std::vector<sprite_chain_element*> prepare_sprites(long long elapsedTime) override;
	void behavior_with_dynamic(dynamic_object* target, long long elapsedTime) override;
	void behavior_with_static(world_object* target, long long elapsedTime) override;
	void behavior(long long elapsedTime) override;
	Vector2f get_build_position(std::vector<world_object*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	int get_build_type(Vector2f ounPos, Vector2f otherPos) override;
	void set_target(dynamic_object& object) override;
	void jerk(float power, float deceleration, Vector2f destinationPoint) override;
	void fight_interact(long long elapsedTime, dynamic_object* target = nullptr) override;
};
