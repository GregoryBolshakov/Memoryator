#include "TimeSystem.h"

TimeSystem::TimeSystem()
= default;

TimeSystem::~TimeSystem()
= default;

void TimeSystem::init(const long long minutesPerDay, const long long startTime)
{
	this->minutesPerDay = minutesPerDay;
	this->time = startTime;
	successInit = true;
}

void TimeSystem::interact(const long long elapsedTime)
{
	this->time += elapsedTime;
	time = time % (minutesPerDay * minute);
}

void TimeSystem::setTime(const long long newTime)
{
	this->time = newTime;
}

void TimeSystem::setMinutesPerDay(const long long minutes)
{
	this->minutesPerDay = minutes;
}

long long TimeSystem::getTime() const
{
	return time;
}

float TimeSystem::getDayPart() const
{
	return float(time) / float(minutesPerDay * minute);
}

