#pragma once

class time_system
{
public:
	time_system();
	~time_system();
	void init(long long minutes_per_day = 10, long long start_time = 0);
	void interact(long long elapsed_time); // microseconds
	void set_time(long long new_time);
	void set_minutes_per_day(long long minutes);
	[[nodiscard]] long long get_time() const;
	[[nodiscard]] float get_day_part() const;

	static const long long minute = long(20e6);
private:
	long long minutes_per_day_ = 0;
	long long time_ = 0;
	bool success_init_ = false;
};
