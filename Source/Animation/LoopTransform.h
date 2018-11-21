/*
  ==============================================================================

    LoopTransform.h
    Created: 1 May 2011 4:46:36pm
    Author:  Haydxn Gemson

  ==============================================================================
*/

#ifndef __LOOPTRANSFORM_H_2609ED5D__
#define __LOOPTRANSFORM_H_2609ED5D__

#include "JuceHeader.h"

class LoopTransform
{
public:
	
	struct Status
	{
		Status ();
		void reset ();
		
		int currentLoopIndex;
		bool inPingPongReverse;
	};
	
	enum LoopType
	{
		noLoopType,
		cycleCountLoopType,
		durationLoopType,
		endlessLoopType,
	};
	
	LoopTransform ();
	virtual ~LoopTransform ();
	
	void setDuration (double newDuration);
	double getDuration () const;
	
	bool isEndless () const;
	
	void setPingPong (bool shouldPingPong);
	bool isPingPong () const;
	
	void setNumCycles (int numCycles);
	void setNumCycles (double numCycles);
	double getNumCycles () const;
	
	void setLoopDuration (double newLoopDuration);
	double getLoopDuration () const;
	
	LoopType getLoopType () const;
	bool isLooping () const;
	
	double getTotalDuration () const;
	
	double getLoopedTime (double sourceTime, Status& status);
	
	virtual void loopConfigChanged ();
	
private:
	
	void loopConfigChangedInternal ();
	double calculateTotalDuration ();
	
	double duration;
	double loopParameter;
	double totalDuration;
	LoopType loopType;
	bool pingPong;
};





#endif  // __LOOPTRANSFORM_H_2609ED5D__
