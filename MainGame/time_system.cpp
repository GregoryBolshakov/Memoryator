#include "time_system.h"

time_system::time_system()
= default;

time_system::~time_system()
= default;

void time_system::init(const long long minutesPerDay, const long long startTime)
{
	this->minutesPerDay = minutesPerDay;
	this->time = startTime;
	successInit = true;
}

void time_system::interact(const long long elapsedTime)
{
	this->time += elapsedTime;
	time = time % (minutesPerDay * minute);
}

void time_system::setTime(const long long newTime)
{
	this->time = newTime;
}

void time_system::setMinutesPerDay(const long long minutes)
{
	this->minutesPerDay = minutes;
}

long long time_system::getTime() const
{
	return time;
}

float time_system::getDayPart() const
{
	return float(time) / float(minutesPerDay * minute);
}

