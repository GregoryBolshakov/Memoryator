#include "time_system.h"

time_system::time_system()
{
	init(10.0 / (hours_per_day * minutes_per_hour));
	set_day_part(0.5);
}

time_system::~time_system()
= default;

void time_system::init(const double real_to_game_time_ratio, const mr::time start_at)
{
	real_to_game_time_ratio_ = real_to_game_time_ratio;

	const auto start_seconds =
		start_at.hours * minutes_per_hour * seconds_per_minute +
		start_at.minutes * seconds_per_minute +
		start_at.seconds;

	time_ = long long(start_seconds) * micro_seconds_per_second;
}

void time_system::interact(const long long elapsed_time_microseconds)
{
	time_ = (time_ + long long(double(elapsed_time_microseconds) / real_to_game_time_ratio_)) % day_duration;
}

void time_system::set_day_part(const double day_part)
{
	this->time_ = long long(day_part * hours_per_day * minutes_per_hour * seconds_per_minute * micro_seconds_per_second);
}

long long time_system::get_time_total_micro_seconds() const
{
	return time_;
}

float time_system::get_time_normalized() const
{
	const auto result = double(time_) / double(day_duration);

	return float(result);
}
