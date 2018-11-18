/*
  ==============================================================================

    AnimatedObject.cpp
    Created: 2 May 2011 11:37:19am
    Author:  Haydxn

  ==============================================================================
*/

#include "AnimatedObject.h"

AnimatedObject::AnimatedObject ()
:	animator(0),
	changedFlag(false)
{
	
}

AnimatedObject::~AnimatedObject()
{
	
}

void AnimatedObject::setAnimator(Animator *animatorToUse)
{
	if (isConnected())
	{
		disconnectFromAnimator();
		animator = animatorToUse;
		connectToAnimator();
	}
	else
	{
		animator = animatorToUse;
	}
}

Animator* AnimatedObject::getAnimator() const
{
	return animator;
}

void AnimatedObject::start()
{
	connectToAnimator();
}

void AnimatedObject::stop()
{
	disconnectFromAnimator();
}

bool AnimatedObject::isActive()
{
	return isConnected();
}

void AnimatedObject::animationAdded(Animation *animation)
{
	animation->addListener(this);
}

void AnimatedObject::animationRemoved(Animation *animation)
{
	animation->removeListener(this);
}

void AnimatedObject::animationChanged(Animation *source)
{
	setChangedFlag (true);
}

void AnimatedObject::setChangedFlag(bool changed)
{
	changedFlag = changed;
}

void AnimatedObject::timingNodeTimeChanged(TimingNode *node)
{
	if (node == &getTimingNode())
	{
		removeFinishedAnimations();

		if (changedFlag)
		{
			applyChanges();
			changedFlag = false;
		}
	}
}

void AnimatedObject::connectToAnimator()
{
	if (!isConnected() && animator != 0)
	{
		animator->getTimingNode().addChild (&getTimingNode());
	}
}

void AnimatedObject::disconnectFromAnimator()
{
	if (isConnected())
	{
		getTimingNode().removeFromParent();
	}
}

bool AnimatedObject::isConnected()
{
	return getTimingNode().getParent() != 0;
}







