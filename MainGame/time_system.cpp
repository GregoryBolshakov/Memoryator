#include "time_system.h"

time_system::time_system()
= default;

time_system::~time_system()
= default;

void time_system::init(const long long minutes_per_day, const long long start_time)
{
	this->minutes_per_day_ = minutes_per_day;
	this->time_ = start_time;
	success_init_ = true;
}

void time_system::interact(const long long elapsed_time)
{
	this->time_ += elapsed_time;
	time_ = time_ % (minutes_per_day_ * minute);
}

void time_system::set_time(const long long new_time)
{
	this->time_ = new_time;
}

void time_system::set_minutes_per_day(const long long minutes)
{
	this->minutes_per_day_ = minutes;
}

long long time_system::get_time() const
{
	return time_;
}

float time_system::get_day_part() const
{
	return float(time_) / float(minutes_per_day_ * minute);
}

