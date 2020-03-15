#pragma once
#ifndef NEUTRAL_MOB_H
#define NEUTRAL_MOB_H

#include "dynamic_object.h"
#include "helper.h"

using namespace sf;

class neutral_mob : public dynamic_object
{
protected:
	int morality_{};
	int fear_{};
	float distance_to_nearest_ = 10e6, run_away_range_ = 0;
public:
	neutral_mob(std::string objectName, Vector2f centerPosition);
	~neutral_mob();
	void behavior_with_dynamic(dynamic_object* target, long long elapsedTime) override;
	void behavior_with_static(world_object* target, long long elapsedTime) override;
	void behavior(long long elapsedTime) override;
	void set_target(dynamic_object& object) override;
protected:
	void fight_interact(long long elapsedTime, dynamic_object* target = nullptr) override;
};

#endif