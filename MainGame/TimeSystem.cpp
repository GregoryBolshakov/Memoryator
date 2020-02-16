#include "TimeSystem.h"



TimeSystem::TimeSystem()
= default;

TimeSystem::~TimeSystem()
= default;

void TimeSystem::init(float minutesPerDay, long long startTime)
{
	this->minutesPerDay = minutesPerDay;
	this->time = startTime;
	successInit = true;
}

void TimeSystem::interact(long long elapsedTime)
{
	this->time += elapsedTime;
	time = time % (minutesPerDay * minute);
}

