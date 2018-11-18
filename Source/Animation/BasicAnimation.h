/*
  ==============================================================================

    BasicAnimation.h
    Created: 1 May 2011 11:42:44am
    Author:  Haydxn Gemson

  ==============================================================================
*/

#ifndef __BASICANIMATION_H_E9AF4E95__
#define __BASICANIMATION_H_E9AF4E95__

#include "Animation.h"

/** Base for animations which can be expressed as a simple 0-1 progress. */

class BasicAnimation	:	public Animation
{
public:
	
	BasicAnimation ();
	virtual ~BasicAnimation ();
	
	void update (double t);
	virtual void updateProgress (double progress) = 0;
	
	//TODO: add progress-warping via 0-1 timing shapers (e.g. ease in/out)
	
private:
	
};



#endif  // __BASICANIMATION_H_E9AF4E95__
