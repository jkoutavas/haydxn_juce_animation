/*
  ==============================================================================

    AnimatedValueTree.cpp
    Created: 1 May 2011 8:32:13pm
    Author:  Haydxn Gemson

  ==============================================================================
*/

#include "AnimatedValueTree.h"

BasicValueAnimation::BasicValueAnimation (const Value& targetValue)
:	target(targetValue)
{
}

BasicValueAnimation::~BasicValueAnimation()
{
}

void BasicValueAnimation::setStartValue(const juce::var &startValue_)
{
	startValue = startValue_;
}

var BasicValueAnimation::getStartValue() const
{
	return startValue;
}

void BasicValueAnimation::setStartValueToCurrentValue()
{
	startValue = getCurrentTargetValue();
}

void BasicValueAnimation::setEndValue(const juce::var &endValue_)
{
	endValue = endValue_;
}

var BasicValueAnimation::getEndValue() const
{
	return endValue;
}

void BasicValueAnimation::setCurrentTargetValue(const juce::var &value)
{
	if (target != value)
	{
		target = value;
		notifyChanged ();
	}
}

var BasicValueAnimation::getCurrentTargetValue() const
{
	return target.getValue();
}

void BasicValueAnimation::setTarget (const Value& targetValue)
{
	target.referTo (targetValue);
}

Value BasicValueAnimation::getTarget () const
{
	return target;
}

void BasicValueAnimation::updateProgress(double progress)
{
	// This is where we interpolate the values...
	if (startValue.isDouble())
	{
		double v1 = (double) startValue;
		double v2 = (double) endValue;
		var value = v1 + (v2 - v1) * progress;
		setCurrentTargetValue (value);
	}
	else if (startValue.isInt())
	{
		int v1 = (int) startValue;
		int v2 = (int) endValue;
		double value = v1 + (v2 - v1) * progress;
		setCurrentTargetValue(roundToInt(value));
		//NOTE: may get funky for negative values. This conversion will
		// likely need to be tweaked at some point for accuracy 
	}
	else
	{
		// For now, for all other types, we'll just snap at the end...
		if (progress == 1.0)
			setCurrentTargetValue(endValue);
		else setCurrentTargetValue(startValue);
	}
}

////////////////////////////////////////////////////////////////////////////////

AnimatedValueTree::AnimatedValueTree ()
{
	
}

AnimatedValueTree::AnimatedValueTree (const Identifier& name)
:	node (name)
{
	
}

AnimatedValueTree::AnimatedValueTree (const ValueTree& source)
:	node (source)
{
	
}

AnimatedValueTree::~AnimatedValueTree()
{
}

void AnimatedValueTree::setNode(const juce::ValueTree &nodeToUse)
{
	node = nodeToUse;
}

ValueTree AnimatedValueTree::getNode ()
{
	return node;
}

Value AnimatedValueTree::findValue(const juce::String &locator)
{
	bool validIdentifier = locator.containsOnly ("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_") && locator.isNotEmpty();

	if (validIdentifier)
		return node.getPropertyAsValue(locator, 0);
	
	return Value();
}

void AnimatedValueTree::animateValueTo(const juce::String &locator, const juce::var &newValue, double duration)
{
	BasicValueAnimation::Ptr anim = new BasicValueAnimation(findValue(locator));
	anim->setStartValueToCurrentValue ();
	anim->setEndValue(newValue);
	anim->setDuration(duration);
	playAnimation(anim);
}

void AnimatedValueTree::animateValueFromTo(const juce::String &locator, const juce::var &startValue, const juce::var &newValue, double duration)
{
	BasicValueAnimation::Ptr anim = new BasicValueAnimation(findValue(locator));
	anim->setStartValue(startValue);
	anim->setEndValue(newValue);
	anim->setDuration(duration);
	playAnimation(anim);
}

void AnimatedValueTree::applyValueTreeChanges(const juce::ValueTree &node)
{
}

void AnimatedValueTree::applyChanges()
{
	applyValueTreeChanges(node);
}





















