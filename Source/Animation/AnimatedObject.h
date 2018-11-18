/*
  ==============================================================================

    AnimatedObject.h
    Created: 2 May 2011 11:37:19am
    Author:  Haydxn

  ==============================================================================
*/

#ifndef __ANIMATEDOBJECT_H_A4152697__
#define __ANIMATEDOBJECT_H_A4152697__

#include "Animation.h"
#include "Animator.h"

class AnimatedObject	:	public Animator,
							public Animation::Listener
{
public:

	AnimatedObject ();
	virtual ~AnimatedObject ();

	void setAnimator (Animator* animatorToUse);
	Animator* getAnimator () const;

	void start ();
	void stop ();
	bool isActive ();
	
	virtual void applyChanges () = 0;

	void animationAdded (Animation* animation);
	void animationRemoved (Animation* animation);

	void animationChanged (Animation* source);
	void setChangedFlag (bool changed = true);

	void timingNodeTimeChanged (TimingNode* node);

private:

	void connectToAnimator ();
	void disconnectFromAnimator ();
	bool isConnected ();
	
	Animator* animator;
	bool changedFlag;
};



#endif  // __ANIMATEDOBJECT_H_A4152697__
