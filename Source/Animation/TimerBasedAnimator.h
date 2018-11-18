/*
  ==============================================================================

    TimerBasedAnimator.h
    Created: 1 May 2011 11:12:26am
    Author:  Haydxn Gemson

  ==============================================================================
*/

#ifndef __TIMERBASEDANIMATOR_H_7FFC75D4__
#define __TIMERBASEDANIMATOR_H_7FFC75D4__

#include "Animator.h"

class TimerBasedAnimator	:	public Animator,
								public Timer
{
public:
	
	TimerBasedAnimator (int interval = 20);
	virtual ~TimerBasedAnimator ();
	
	void start ();
	void stop ();
	bool isActive ();
	
	void timerCallback ();
	
private:
	
	int timerInterval;
	uint32 lastTime;
};



#endif  // __TIMERBASEDANIMATOR_H_7FFC75D4__
