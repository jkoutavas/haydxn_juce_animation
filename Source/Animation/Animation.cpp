/*
  ==============================================================================

    Animation.cpp
    Created: 24 Apr 2011 11:06:21am
    Author:  Haydxn

  ==============================================================================
*/

#include "Animation.h"

////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////

Animation::Animation ()
:	hasPlayed(false)
{
	timingNode.addListener(this);
	timingNode.setTransformer(this);
}

Animation::~Animation ()
{
	timingNode.removeListener(this);	
}

void Animation::update (double newTime)
{
	
}

void Animation::started ()
{
	
}

void Animation::stopped ()
{
	
}

bool Animation::hasFinished()
{
	return (hasPlayed && !timingNode.isRunning());
}

void Animation::reset (double startTimeOffset)
{
	timingNode.resetStartTime (startTimeOffset);
	hasPlayed = false;
}

double Animation::getLocalProgress () const
{
	if (getDuration() > 0)
		return timingNode.getCurrentLocalTime() / getDuration();
	
	return timingNode.getCurrentWindowProgress();
}

TimingNode& Animation::getTimingNode()
{
	return timingNode;
}

void Animation::timingNodeTimeChanged(TimingNode *source)
{
	if (source == &timingNode)
	{
		update (timingNode.getCurrentLocalTime());
		hasPlayed = true;
	}
}

void Animation::timingNodeStarted(TimingNode *source)
{
	if (source == &timingNode)
	{
		started();
		hasPlayed = true;
		notifyChanged();
	}
}

void Animation::timingNodeStopped(TimingNode *source)
{
	if (source == &timingNode)
	{
		stopped();
		notifyChanged();
	}
}

void Animation::timingNodeParentChanged(TimingNode *source, TimingNode *parent)
{
	if (source == &timingNode)
	{
	}
}

void Animation::loopConfigChanged()
{
	timingNode.setActiveWindowDuration(getTotalDuration());
}

double Animation::transformRunningTime(double time)
{
	return getLoopedTime (time, loopStatus);
}

void Animation::addListener(Animation::Listener *listener)
{
	listeners.add(listener);
}

void Animation::removeListener(Animation::Listener *listener)
{
	listeners.remove(listener);
}

void Animation::notifyChanged ()
{
	listeners.call (&Listener::animationChanged, this);
}

////////////////////////////////////////////////////////////////////////////////

ComplexAnimation::ComplexAnimation ()
:	changedFlag(false)
{
	
}

ComplexAnimation::~ComplexAnimation()
{
	clear ();
}

void ComplexAnimation::update(double newTime)
{
	if (changedFlag)
	{
		applyChanges();
		notifyChanged();
		changedFlag = false;
	}
}

void ComplexAnimation::addChild(Animation *anim, double offset)
{
	if (!children.contains(anim))
	{
		children.add(anim);
		getTimingNode().addChild (&anim->getTimingNode());
		anim->reset(offset);
	}
}

void ComplexAnimation::removeChild(Animation *anim)
{
	if (children.contains(anim))
	{
		anim->getTimingNode().removeFromParent();
		children.removeObject(anim);
	}
}

void ComplexAnimation::removeChild(int index)
{
	Animation* child = children[index];
	if (child != 0)
	{
		child->getTimingNode().removeFromParent();
		children.removeObject(child);
	}
}

void ComplexAnimation::clear ()
{
	for (int i=children.size()-1; i>=0; --i)
	{
		removeChild (i);
	}
}

Animation* ComplexAnimation::getChild(int index)
{
	return children[index];
}

int ComplexAnimation::indexOfChild(Animation *anim)
{
	return children.indexOf(anim);
}

int ComplexAnimation::getNumChildren()
{
	return children.size();
}

void ComplexAnimation::animationChanged(Animation *anim)
{
	markAsChanged();	
}

void ComplexAnimation::markAsChanged()
{
	if (!changedFlag)
	{
		changedFlag = true;
	}
}

void ComplexAnimation::applyChanges()
{
	
}



















