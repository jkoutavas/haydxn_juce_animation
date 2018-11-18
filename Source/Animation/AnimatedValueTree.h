/*
  ==============================================================================

    AnimatedValueTree.h
    Created: 1 May 2011 8:32:13pm
    Author:  Haydxn Gemson

  ==============================================================================
*/

#ifndef __ANIMATEDVALUETREE_H_19B21801__
#define __ANIMATEDVALUETREE_H_19B21801__

#include "AnimatedObject.h"
#include "BasicAnimation.h"

/** Basic Animation for manipulating Value objects. This is about the easiest
	kind of animation to use, as Value objects implement the application of new
	values themselves. */
	
class BasicValueAnimation	:	public BasicAnimation
{
public:
	
	typedef ReferenceCountedObjectPtr<BasicValueAnimation> Ptr;
	
	BasicValueAnimation (const Value& data);
	virtual ~BasicValueAnimation ();
	
	void setStartValue (const var& startValue);
	void setStartValueToCurrentValue ();
	var getStartValue () const;
	
	void setTarget (const Value& targetValue);
	Value getTarget () const;
	
	void setCurrentTargetValue (const var& value);
	var getCurrentTargetValue () const;
	
	void setEndValue (const var& endValue);
	var getEndValue () const;
	
	void updateProgress (double progress);
	
private:
	
	var startValue;
	var endValue;

	Value target;
};


/** This holds a ValueTree and gives you some functions to quickly and easily
	animate values held within it. */

class AnimatedValueTree	:	public AnimatedObject
{
public:
	
	AnimatedValueTree (const Identifier& name);
	AnimatedValueTree (const ValueTree& source);
	AnimatedValueTree ();
	virtual ~AnimatedValueTree ();
	
	void setNode (const ValueTree& nodeToUse);
	ValueTree getNode ();
	
	/** Retrieve a Value from the tree. By default it just assumes it's a simple
		Identifier for a root property; this can be overridden to parse it and
		use it like a path, for example. */
	virtual Value findValue (const String& locator);
	
	void animateValueTo (const String& locator, const var& newValue, double duration);
	void animateValueFromTo (const String& locator, const var& startValue, const var& endValue, double duration);

	/** This will be called at the end of a time update if any animations have
		changed anything in the tree. Use this to apply any changes. */
	virtual void applyValueTreeChanges (const ValueTree& node);
	
	//TODO: Maybe change this to use a listener, reducing the need to subclass

	void applyChanges ();
	
protected:
	
	ValueTree node;
};



#endif  // __ANIMATEDVALUETREE_H_19B21801__
