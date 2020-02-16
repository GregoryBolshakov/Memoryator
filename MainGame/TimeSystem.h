#pragma once
class TimeSystem
{
public:
	TimeSystem();
	~TimeSystem();
	void init(float minutesPerDay = 10, long long startTime = 0);
	void interact(long long elapsedTime); // microseconds
	void setTime(long long newTime) { this->time = newTime; }
	void setMinutesPerDay(long long minutes) { this->minutesPerDay = minutes; }
	long long getTime() const { return time; }
	float getDayPart() const { return float(time) / (minutesPerDay * minute); }

	static const long long minute = 20e6;
private:
	long long minutesPerDay = 0, time = 0;
	bool successInit = false;
};

