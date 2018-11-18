/*
  ==============================================================================

    TimerBasedAnimator.cpp
    Created: 1 May 2011 11:12:26am
    Author:  Haydxn Gemson

  ==============================================================================
*/

#include "TimerBasedAnimator.h"

TimerBasedAnimator::TimerBasedAnimator (int interval)
:	timerInterval(interval),
	lastTime(0)
{
}

TimerBasedAnimator::~TimerBasedAnimator()
{
	if (isActive())
		stop();
}

void TimerBasedAnimator::start ()
{
	if (!isTimerRunning())
	{
		lastTime = Time::getMillisecondCounter();
		
		startTimer (timerInterval);
	}
}

void TimerBasedAnimator::stop ()
{
	if (isTimerRunning())
	{
		stopTimer ();
	}
}

bool TimerBasedAnimator::isActive ()
{
	return isTimerRunning();
}

void TimerBasedAnimator::timerCallback()
{
	uint32 t = Time::getMillisecondCounter();
	uint32 elapsed = t - lastTime;
	getTimingNode().setRunningTimeRelative (elapsed * 0.001);
	lastTime = t;

	// This is currently calculating a relative time, which isn't going to be
	// as accurate as just using the direct time... it'll do for now though
	// (i want to be able to have the time not advance when it's not running,
	// and that's a bit more complex than I can be arsed with at the moment
	// when using direct time, as you have to store some offset + elapsed time).
}












