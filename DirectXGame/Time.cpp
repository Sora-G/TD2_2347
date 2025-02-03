#include "Time.h"

Time::Time()
{
}

Time::~Time()
{
}

void Time::Initialize()
{
	elapsedTime = 0;
}

void Time::Update()
{
	elapsedTime += 1 / 60;

}

void Time::Draw()
{
}

float Time::RestTime()
{
	return restTime - elapsedTime;

}

bool Time::IsFinish()
{
	return RestTime() <= 0;

}
