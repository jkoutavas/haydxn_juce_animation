/*
  ==============================================================================

    Animation.h
    Created: 24 Apr 2011 11:06:21am
    Author:  Haydxn

  ==============================================================================
*/

#ifndef __ANIMATION_H_32343764__
#define __ANIMATION_H_32343764__

#include "TimingNode.h"
#include "LoopTransform.h"

/** Reference counted Animation base, for defining some time-based change in
	some target property. */
class Animation	:	public ReferenceCountedObject,
					public TimingNode::Listener,
					public TimingNode::Transformer,
					public LoopTransform
{
public:
	
	typedef ReferenceCountedObjectPtr <Animation> Ptr;
	
	Animation ();
	virtual ~Animation ();
	
	virtual void update (double newTime) = 0;
	void started ();
	void stopped ();
	
	void reset (double startTimeOffset = 0.0);
	bool hasFinished ();
	
	TimingNode& getTimingNode ();
	
	void timingNodeTimeChanged (TimingNode* source);
	void timingNodeStarted (TimingNode* source);
	void timingNodeStopped (TimingNode* source);
	void timingNodeParentChanged (TimingNode* source, TimingNode* parent);
	void loopConfigChanged ();
	
	double getLocalProgress () const;
	
	double transformRunningTime (double time);
	
	class Listener
	{
	public:
		Listener() {}
		virtual ~Listener () {}
		
		virtual void animationChanged (Animation* source) = 0;
	};
	
	void addListener (Listener* listener);
	void removeListener (Listener* listener);
	
	void notifyChanged ();
	
private:

	TimingNode timingNode;
	ListenerList<Listener> listeners;
	LoopTransform::Status loopStatus;
	bool hasPlayed;
};



class ComplexAnimation	:	public Animation,
							public Animation::Listener
{
public:
	
	ComplexAnimation ();
	virtual ~ComplexAnimation ();
	
	void update (double newTime);
	
	void addChild (Animation* anim, double offset);
	void removeChild (int index);
	void removeChild (Animation* anim);
	void clear ();
	Animation* getChild (int index);
	int indexOfChild (Animation* anim);
	int getNumChildren ();
	
	void animationChanged (Animation* anim);
	
	void markAsChanged ();
	virtual void applyChanges ();

private:
	
	ReferenceCountedArray<Animation> children;
	bool changedFlag;
	
};


#endif  // __ANIMATION_H_32343764__
