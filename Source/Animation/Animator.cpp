/*
  ==============================================================================

    Animator.cpp
    Created: 24 Apr 2011 12:52:57pm
    Author:  Haydxn

  ==============================================================================
*/

#include "Animator.h"

Animator::Animator ()
:	options (fullyAutomatic)
{
	timingNode.setActiveWindowDuration(-1.0);
	timingNode.addListener(this);
}

Animator::~Animator ()
{
	timingNode.removeFromParent ();
}

void Animator::setOptionFlags(Animator::OptionFlags optionFlags)
{
	options = optionFlags;
}

Animator::OptionFlags Animator::getOptionFlags() const
{
	return options;
}

void Animator::playAnimation (Animation* anim, double startOffset)
{
	if (!animations.contains (anim))
	{
		animations.add (anim);
		timingNode.addChild (&anim->getTimingNode());
		anim->reset (startOffset);
		animationAdded (anim);
	}
}

void Animator::stopAnimation (Animation* anim)
{
	if (animations.contains(anim))
	{
		Animation::Ptr retainer = anim;
		
		anim->getTimingNode().removeFromParent();
		animations.removeObject (anim);
		animationRemoved(anim);
		
		retainer = 0;
	}
}

void Animator::animationAdded(Animation *anim)
{
	
}

void Animator::animationRemoved(Animation *anim)
{
	
}

int Animator::getNumPlayingAnims () const
{
	return animations.size();
}

Animation* Animator::getAnimation (int index)
{
	return animations[index];
}

TimingNode& Animator::getTimingNode ()
{
	return timingNode;
}

void Animator::timingNodeParentChanged(TimingNode *source, TimingNode *parent)
{
	if (source == &timingNode)
	{
		if (parent)
		{
			// Keep the current running time...
			timingNode.resetStartTime(-timingNode.getCurrentRunningTime());
		}
	}
}

void Animator::timingNodeTimeChanged (TimingNode* source)
{
	if (source == &timingNode)
	{
		// All the animations should have already been updated automatically...
		removeFinishedAnimations ();
	}
}

void Animator::timingNodeChildAdded(TimingNode *node, TimingNode *child)
{
	if ( (timingNode.getNumChildren () == 1)
		&& (options & autoStart)
		&& !isActive() )
	{
		start();
	}
}

void Animator::timingNodeChildRemoved(TimingNode *node, TimingNode *child)
{
	if ( (timingNode.getNumChildren() == 0)
		&& (options & autoStop)
		&& isActive() )
	{
		stop();
	}
}

void Animator::timingNodeStarted(TimingNode *source)
{
	
}

void Animator::timingNodeStopped(TimingNode* source)
{
	
}

void Animator::removeFinishedAnimations ()
{
	if (options & autoRemove)
	{
		for (int i = animations.size()-1; i >= 0; --i)
		{
			Animation::Ptr anim = animations.getUnchecked(i);
			if (anim->hasFinished())
			{
				stopAnimation(anim);
			}
		}
	}
}












