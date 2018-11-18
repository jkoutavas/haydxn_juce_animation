/*
  ==============================================================================

    TimingNode.cpp
    Created: 24 Apr 2011 1:41:15pm
    Author:  Haydxn

  ==============================================================================
*/

#include "TimingNode.h"

////////////////////////////////////////////////////////////////////////////////
// Position
////////////////////////////////////////////////////////////////////////////////

TimingNode::RunningTime::RunningTime ()
:	time(0.0),
	region(0)
{
}

TimingNode::RunningTime::RunningTime (const RunningTime& other)
:	time(other.time),
	region(other.region)
{
}

TimingNode::RunningTime::RunningTime (double time_, int region_)
:	time(time_),
	region(region_)
{
}

TimingNode::RunningTime& TimingNode::RunningTime::operator=(const TimingNode::RunningTime &other)
{
	time = other.time;
	region = other.region;
	return *this;
}

bool TimingNode::RunningTime::isWithinActiveWindow() const
{
	return (region == withinActiveWindow);
}

bool TimingNode::RunningTime::isInSameRegionAs(const TimingNode::RunningTime &other) const
{
	return region == other.region;
}

int TimingNode::RunningTime::getRegionDifference(const TimingNode::RunningTime &other, bool absolute) const
{
	if (absolute)
		return abs(region - other.region);
	return region - other.region;
}

////////////////////////////////////////////////////////////////////////////////
// Timing Node
////////////////////////////////////////////////////////////////////////////////

TimingNode::TimingNode ()
:	parent(0),
	transformer(0),
	startTime (0.0),
	speed(1.0),
	activeWindowDuration(-1),
	currentLocalTime (0.0)
{
}

TimingNode::~TimingNode ()
{
	jassert (parent == 0); // Must remove from parent before deleting!
	removeAllChildren ();
}

void TimingNode::addChild (TimingNode* childToAdd)
{
	if (childToAdd != 0 && childToAdd->parent != this)
	{
		if (!isAChildOf (childToAdd))
		{
			if (childToAdd->parent != 0)
			{
				jassertfalse; // nodes can only be a child of a single parent
				childToAdd->removeFromParent ();
			}

			children.add (childToAdd);
			childToAdd->parent = this;

			childNodeAdded(childToAdd);
			childToAdd->addedToParentNode(this);
		}
	}
}


void TimingNode::removeChild (TimingNode* childToRemove)
{
	if (childToRemove != 0)
	{
		int n = children.indexOf (childToRemove);
		if (n > -1)
		{
			jassert (childToRemove->parent == this);

			childToRemove->parent = 0;
			children.remove (n);

			childNodeRemoved (childToRemove);
			childToRemove->removedFromParentNode (this);
		}
	}
}

void TimingNode::removeAllChildren ()
{
	while (children.size() > 0)
	{
		removeChild (children.getLast());
	}
}

int TimingNode::getNumChildren () const
{
	return children.size();
}

TimingNode* TimingNode::getChild (int index)
{
	return children[index];
}

int TimingNode::indexOfChild (TimingNode* childToFind) const
{
	return children.indexOf (childToFind);
}

bool TimingNode::isAChildOf (TimingNode* potentialParentNode) const
{
	TimingNode* p = parent;
	while (p != 0)
	{
		if (p == potentialParentNode)
			return true;

		p = p->parent;
	}
	return false;
}

TimingNode* TimingNode::getParent ()
{
	return parent;
}

void TimingNode::setStartTime(double newStartTime)
{
	startTime = newStartTime;
}

double TimingNode::getStartTime () const
{
	return startTime;
}

void TimingNode::setSpeed(double newSpeed)
{
	speed = newSpeed;
}

double TimingNode::getSpeed() const
{
	return speed;
}

void TimingNode::setActiveWindowDuration(double newDuration)
{
	activeWindowDuration = newDuration;
}

double TimingNode::getActiveWindowDuration() const
{
	return activeWindowDuration;
}

void TimingNode::setEndless()
{
	setActiveWindowDuration(-1.0);
}

bool TimingNode::isEndless() const
{
	return activeWindowDuration < 0.0;
}

bool TimingNode::isRunning()
{
	return currentRunningTime.isWithinActiveWindow();
}

double TimingNode::clampRunningTimeToWindow(double newTime)
{
	if (activeWindowDuration < 0.0)
		return newTime;
	
	return jlimit(0.0, activeWindowDuration, newTime);
}

TimingNode::RunningTime TimingNode::getWindowedRunningTime (double newTime)
{
	int region;	
	if (newTime < 0.0)
		region = RunningTime::beforeActiveWindow;
	else if (isEndless() || newTime < activeWindowDuration)
		region = RunningTime::withinActiveWindow;
	else region = RunningTime::afterActiveWindow;
	
	return RunningTime (clampRunningTimeToWindow(newTime), region);
}


void TimingNode::setRunningTime (double t)
{
	// Get the new position, constrained to the window
	RunningTime newRunningTime = getWindowedRunningTime(t);
	
	if (newRunningTime.isInSameRegionAs(currentRunningTime))
	{
		setRunningTimeInternal(newRunningTime);
		
		if (newRunningTime.isWithinActiveWindow())
			updateInternal();
	}
	else
	{
		// We've changed regions...
		// There are only three valid regions (before, within, after).
		int regionDifference = newRunningTime.getRegionDifference(currentRunningTime, true);
		if (regionDifference == 1)
		{
			// We must have either started or stopped...
			if (newRunningTime.isWithinActiveWindow())
			{
				// Started...
				setRunningTimeInternal(newRunningTime);
				startInternal();
				updateInternal();
			}
			else
			{
				// Ensure a normal update for the end...
				setRunningTimeInternal (RunningTime (newRunningTime.time, RunningTime::withinActiveWindow));
				updateInternal();
				
				setRunningTimeInternal(newRunningTime);
				stopInternal();
			}
		}
		else
		{
			// We've crossed from one side to the other.
			// We should simulate it having started and stopped. 

			jassert (newRunningTime.region == RunningTime::beforeActiveWindow 
					 || newRunningTime.region == RunningTime::afterActiveWindow);
			// If this assert has been hit, then the region must be invalid.
			// The biggest valid region difference is 2 (before to after), so
			// the new position must be in one of those.
			
			// We want to start it at the exited end of the window...
			setRunningTimeInternal (RunningTime (newRunningTime.time, RunningTime::withinActiveWindow));
			startInternal ();
			updateInternal ();
			
			setRunningTimeInternal (newRunningTime);
			stopInternal ();
		}
	}
}

void TimingNode::startInternal ()
{
	listeners.call (&Listener::timingNodeStarted, this);
}

void TimingNode::stopInternal ()
{
	listeners.call (&Listener::timingNodeStopped, this);
}

void TimingNode::updateInternal ()
{
	refreshChildren ();
	listeners.call (&Listener::timingNodeTimeChanged, this);
}

void TimingNode::setRunningTimeInternal(const TimingNode::RunningTime &newTime)
{
	currentRunningTime = newTime;
	currentLocalTime = runningTimeToLocalTime (newTime.time);
}

void TimingNode::refreshChildren ()
{
	for (int i=0; i<children.size(); i++)
	{
		TimingNode* child = children.getUnchecked(i);
		child->refreshRunningTime();
	}	
}

double TimingNode::getCurrentRunningTime() const
{
	return currentRunningTime.time;
}

double TimingNode::getCurrentLocalTime() const
{
	return currentLocalTime;
}

double TimingNode::getCurrentWindowProgress() const
{
	if (isEndless())
		return 0.0;

	if (activeWindowDuration > 0)
		return currentRunningTime.time / activeWindowDuration;
	
	return (currentRunningTime.region > 0) ? 1.0 : 0.0;
}

double TimingNode::runningTimeToLocalTime(double time)
{
	if (transformer != 0)
		return transformer->transformRunningTime(time);
	return time;
}

void TimingNode::setRunningTimeRelative(double amount)
{
	setRunningTime (currentRunningTime.time + (amount * speed));
}

void TimingNode::refreshRunningTime ()
{
	double t = (getSourceTime() - startTime) * speed;
	setRunningTime (t);
}

void TimingNode::resetStartTime (double offset)
{
	setStartTime (getSourceTime () - offset);
}

void TimingNode::removeFromParent ()
{
	if (parent != 0)
	{
		parent->removeChild (this);
	}
}

double TimingNode::getSourceTime ()
{
	if (parent != 0)
		return parent->getCurrentLocalTime ();

	return 0.0;
}

void TimingNode::setTransformer(TimingNode::Transformer *transformerToUse)
{
	transformer = transformerToUse;
}

TimingNode::Transformer* TimingNode::getTransformer()
{
	return transformer;
}

void TimingNode::addListener (Listener* listener)
{
	listeners.add (listener);
}

void TimingNode::removeListener (Listener* listener)
{
	listeners.remove (listener);
}

void TimingNode::addedToParentNode (TimingNode* newParent)
{
	setRunningTimeInternal (RunningTime());
	listeners.call (&Listener::timingNodeParentChanged, this, newParent);
}

void TimingNode::removedFromParentNode(TimingNode *parent)
{
	listeners.call (&Listener::timingNodeParentChanged, this, nullptr);	
}

void TimingNode::childNodeAdded(TimingNode *node)
{
	listeners.call (&Listener::timingNodeChildAdded, this, node);
}

void TimingNode::childNodeRemoved(TimingNode *node)
{	
 	listeners.call (&Listener::timingNodeChildRemoved, this, node);
}























