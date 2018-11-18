/*
  ==============================================================================

    LoopTransform.cpp
    Created: 1 May 2011 4:46:36pm
    Author:  Haydxn Gemson

  ==============================================================================
*/

#include "LoopTransform.h"



LoopTransform::Status::Status ()
:	currentLoopIndex(0),
	inPingPongReverse(false)
{
	
}

LoopTransform::LoopTransform ()
:	duration (1.0),
	loopParameter (1.0),
	totalDuration (1.0),
	loopType (noLoopType),
	pingPong (false)
{
}

LoopTransform::~LoopTransform()
{
	
}

bool LoopTransform::isEndless () const
{
	return (duration < 0.0 || loopType == endlessLoopType);
}

void LoopTransform::setDuration(double newDuration)
{
	duration = newDuration;
	loopConfigChangedInternal();
}

double LoopTransform::getDuration() const
{
	return duration;
}

void LoopTransform::setNumCycles(int numCycles)
{
	setNumCycles((double) numCycles);	
}

void LoopTransform::setNumCycles(double numCycles)
{
	if (numCycles == 1.0)
	{
		loopType = noLoopType;
	}
	else if (numCycles <= 0)
	{
		loopParameter = -1.0;
		loopType = endlessLoopType;
	}
	else
	{
		loopParameter = numCycles;
		loopType = cycleCountLoopType;
	}
	loopConfigChangedInternal();
}

double LoopTransform::getNumCycles() const
{
	switch (loopType)
	{
		case cycleCountLoopType:		
			return loopParameter;
		
		case durationLoopType:
			if (duration > 0)
				return (loopParameter / duration);
			else return 0.0;
			
		case noLoopType:
			return 1.0;
			
		case endlessLoopType:
			return -1;
	};
	return -1;
}

void LoopTransform::setPingPong(bool shouldPingPong)
{
	pingPong = shouldPingPong;
}

bool LoopTransform::isPingPong() const
{
	return pingPong;
}

void LoopTransform::setLoopDuration(double newLoopDuration)
{
	if (newLoopDuration < 0)
	{
		loopType = endlessLoopType;
		loopParameter = -1;
	}
	else
	{
		loopParameter = newLoopDuration;
		loopType = durationLoopType;
	}
	loopConfigChangedInternal();
}

double LoopTransform::getLoopDuration() const
{
	return getTotalDuration();
}

LoopTransform::LoopType LoopTransform::getLoopType() const
{
	return loopType;
}

double LoopTransform::getLoopedTime (double sourceTime, LoopTransform::Status &status)
{
	double time = sourceTime;

	status.currentLoopIndex = floor (time / duration);
	if (status.currentLoopIndex > 0)
	{
		bool loopBoundary = false;
		time = fmod (time, duration);
		
		if (time == 0 && sourceTime > 0) // on loop boundary...
		{
			// We're on a loop boundary. The time has been wrapped by the
			// duration, and we're already on the next cycle.
			loopBoundary = true;
			time = duration;
		}
		
		if (pingPong)
		{
			if (loopBoundary)
				status.inPingPongReverse = (status.currentLoopIndex-1) % 2;
			else status.inPingPongReverse = status.currentLoopIndex % 2;
			
			if (status.inPingPongReverse)
			{
				time = duration - time;
			}
		}
		else
		{
		}
	}
	return time;
}

double LoopTransform::getTotalDuration() const
{
	return totalDuration;
}

double LoopTransform::calculateTotalDuration()
{
	if (isEndless())
		return -1;
	
	if (loopType == cycleCountLoopType)
	{
		return loopParameter * duration;
	}
	else if (loopType == durationLoopType)
	{
		return loopParameter;
	}
	return duration;
}

void LoopTransform::loopConfigChangedInternal()
{
	totalDuration = calculateTotalDuration ();
	loopConfigChanged ();
}

void LoopTransform::loopConfigChanged()
{
	
}


