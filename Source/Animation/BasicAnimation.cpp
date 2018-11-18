/*
  ==============================================================================

    BasicAnimation.cpp
    Created: 1 May 2011 11:42:44am
    Author:  Haydxn Gemson

  ==============================================================================
*/

#include "BasicAnimation.h"

BasicAnimation::BasicAnimation ()
{
	setDuration (1.0);
}

BasicAnimation::~BasicAnimation()
{
	
}

void BasicAnimation::update(double t)
{
	double progress = getLocalProgress();
	updateProgress (progress);
}







