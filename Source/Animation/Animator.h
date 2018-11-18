/*
  ==============================================================================

    Animator.h
    Created: 24 Apr 2011 12:52:57pm
    Author:  Haydxn

  ==============================================================================
*/

#ifndef __ANIMATOR_H_332DB77B__
#define __ANIMATOR_H_332DB77B__

#include "Animation.h"

class Animator	:	public TimingNode::Listener
{
public:
	
	enum OptionFlags
	{
		autoStart = 1,
		autoStop = 2,
		autoRemove = 4,
		autoStartAndStop = (autoStart | autoStop),
		
		fullyAutomatic = (autoStartAndStop | autoRemove)
	};
	
	Animator ();
	virtual ~Animator ();

	void playAnimation (Animation* anim, double delayStart = 0.0);
	void stopAnimation (Animation* anim);
	int getNumPlayingAnims () const;
	Animation* getAnimation (int index);

	virtual void start () = 0;
	virtual void stop () = 0;
	virtual bool isActive () = 0;
	
	void setOptionFlags (OptionFlags optionFlags);
	OptionFlags getOptionFlags () const;
	
	virtual void animationAdded (Animation* anim);
	virtual void animationRemoved (Animation* anim);
	
	TimingNode& getTimingNode ();

	void timingNodeTimeChanged (TimingNode* node);
	void timingNodeChildAdded (TimingNode* node, TimingNode* child);
	void timingNodeChildRemoved (TimingNode* node, TimingNode* child);
	void timingNodeParentChanged (TimingNode* source, TimingNode* parent);
	void timingNodeStarted (TimingNode* source);
	void timingNodeStopped (TimingNode* source);

	void removeFinishedAnimations ();
		
private:
	
	ReferenceCountedArray<Animation> animations;
	TimingNode timingNode;
	OptionFlags options;
};



#endif  // __ANIMATOR_H_332DB77B__
