#pragma once

namespace mr
{
	struct time
	{
		int hours;
		int minutes;
		int seconds;
	};
}

class time_system
{
public:
	static const long long hours_per_day = 24;
	static const long minutes_per_hour = 60;
	static const long seconds_per_minute = 60;
	static const long micro_seconds_per_second = int(1e6);
	static const long long day_duration = static_cast<long long>(
		hours_per_day * minutes_per_hour * seconds_per_minute
		) * micro_seconds_per_second;

	time_system();
	~time_system();

	void init(double real_to_game_time_ratio, mr::time start_at = { 0, 0, 0 });
	void interact(long long elapsed_time_microseconds);
	void set_time_total_micro_seconds(long long time) { this->time_ = time; }
	void set_day_part(const double day_part);
	[[nodiscard]] long long get_time_total_micro_seconds() const;
	[[nodiscard]] float get_time_normalized() const;
private:
	double real_to_game_time_ratio_ = 0;
	long long time_ = 0;
};

