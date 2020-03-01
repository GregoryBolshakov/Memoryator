#pragma once

class TimeSystem
{
public:
	TimeSystem();
	~TimeSystem();
	void init(long long minutesPerDay = 10, long long startTime = 0);
	void interact(long long elapsedTime); // microseconds
	void setTime(long long newTime);
	void setMinutesPerDay(long long minutes);
	[[nodiscard]] long long getTime() const;
	[[nodiscard]] float getDayPart() const;

	static const long long minute = long(20e6);
private:
	long long minutesPerDay = 0, time = 0;
	bool successInit = false;
};
