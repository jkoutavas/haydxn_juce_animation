/*
  ==============================================================================

    TimingNode.h
    Created: 24 Apr 2011 1:41:15pm
    Author:  Haydxn

  ==============================================================================
*/

#ifndef __TIMINGNODE_H_A6C62E40__
#define __TIMINGNODE_H_A6C62E40__

#include "JuceHeader.h"

/**
	This is a handy class to be used by objects which need some time-based
	behaviour. It provides windowing/scheduling and transformation, and can be
	used in a hierarchy to automatically give you a network of auto-updating
	objects driven by a single clocking source.
 
	Each node has an active window, which specifies how long it should run for.
	When its running time changes, it is (optionally) transformed into a 'local'
	time. Any child nodes will then automatically update their running time
	relative to their parent, and so on.
 
	See Animation and Animator for examples of how it can be utilised. 
*/
class TimingNode
{
public:
	
	TimingNode ();
	~TimingNode ();
		
	/** Sets this node's start time (which is used as an offset to the parent
		node's local time). */
	void setStartTime (double newStartTime);
	
	/** Returns this node's start time. */
	double getStartTime () const;

	/** Sets the start time to match the parent's current local time (with an
	 optional additional offset, e.g. to delay it). */
	void resetStartTime (double offset = 0.0);
	
	/** Sets the speed at which this node's time advances relative to its 
	 parent node. */
	void setSpeed (double newSpeed);
	
	/** Returns this node's speed. */
	double getSpeed () const;
	
	/** Sets how long this node's active window is. This node will stop updating
	 if its running time exceeds this value. If it is negative, it will run
	 forever. */
	void setActiveWindowDuration (double newDuration);
	
	/** Returns this node's active window size. */
	double getActiveWindowDuration () const;
	
	/** Sets this node's active window duration to endless (-1). */
	void setEndless ();
	
	/** Returns true if this node's active window duration is negative. */
	bool isEndless () const;

	/** This is the main driving function of the TimingNode system.
		This forces the current running time to the specified value. It will
		automatically apply any windowing and transformation, and then update
		any child nodes before notifying listeners of any changes (if the new
		time is within this node's active window). */
	void setRunningTime (double newRunningTime);

	/** This advances the current running time by the specified amount. It has
		the same effect as calling setCurrentRunningTime(), except it takes a
		relative value. */
	void setRunningTimeRelative (double amount);

	/** This calls setCurrentRunningTime() using a new running time calculated
		from the parent node's current local time (taking into account this 
		node's start time and speed). */
	void refreshRunningTime ();

	/** Returns the current running time for this node. */
	double getCurrentRunningTime () const;
	
	/** Applies this node's transform to the given local running time. */
	double runningTimeToLocalTime (double time);

	/** Returns the current local time (i.e. the transformed running time). */
	double getCurrentLocalTime () const;
	
	/** Returns a value between 0 and 1 indicating the current progress through
		this node's active window. */
	double getCurrentWindowProgress () const;

	/** Returns true if the local running time is within this node's active
		window. */
	bool isRunning ();
	
	////////////////////////////////////
	// Child management functions
	////////////////////////////////////
	
	/** Add a child to this node. */ 
	void addChild (TimingNode* childToAdd);
	
	/** Remove a child from this node. */
	void removeChild (TimingNode* childToRemove);
	
	/** Returns the number of child nodes registered. */
	int getNumChildren () const;
	
	/** Returns the child node at the specified index. */
	TimingNode* getChild (int index);
	
	/** Returns the index of the specified child node (or -1 if not found). */
	int indexOfChild (TimingNode* childToFind) const;
	
	/** Returns true if the specified node is an ancestor of this node. */
	bool isAChildOf (TimingNode* potentialParentNode) const;
	
	/** Removes all child nodes. */
	void removeAllChildren ();
	
	/** Returns this node's parent. */
	TimingNode* getParent ();
	
	/** Removes this node from its parent. */
	void removeFromParent ();
	
	////////////////////////////////////
	// Listener
	////////////////////////////////////

	class Listener
	{
	public:
		Listener () {};
		virtual ~Listener () {};

		/** This is called when the running time of a TimingNode is updated,
			after all of its children have been refreshed. */
		virtual void timingNodeTimeChanged (TimingNode* source) = 0;
		
		/** This is called when a TimingNode's running time enters the active
			window. */
		virtual void timingNodeStarted (TimingNode* source) = 0;
		
		/** This is called when a TimingNode's running time leaves the active
			window. */
		virtual void timingNodeStopped (TimingNode* source) = 0;
		
		/** Called when a TimingNode's parent assignment changes. */
		virtual void timingNodeParentChanged (TimingNode* source, TimingNode* parent) = 0;
		
		/** Called whenever a child is added to a TimingNode. */
		virtual void timingNodeChildAdded (TimingNode* source, TimingNode* child) {}
		
		/** Called whenever a child is removed from a TimingNode. */
		virtual void timingNodeChildRemoved (TimingNode* source, TimingNode* child) {}
	};
	
	/** Register a listener with this node. */
	void addListener (Listener* listener);
	
	/** Unregisters a listener from this node. */
	void removeListener (Listener* listener);

	////////////////////////////////////
	// Transform
	////////////////////////////////////

	/** Defines some transform from 'running time' to 'local time'. */
	
	class Transformer
	{
	public:
		
		Transformer () {}
		virtual ~Transformer () {}
		
		/** This should convert the given 'running time' into a 'local' time. */
		virtual double transformRunningTime (double time) = 0;
	};
	
	/** Sets the transform this node will use for generating its local time. */
	void setTransformer (Transformer* transformerToUse);

	/** Gets this node's current transform. */
	Transformer* getTransformer ();

private:
	
	// Helper for keeping track of running time and active status.

	struct RunningTime
	{
		enum Region
		{
			beforeActiveWindow = -1,
			withinActiveWindow = 0,
			afterActiveWindow = 1
		};
		
		RunningTime ();
		RunningTime (double time_, int region_);
		RunningTime (const RunningTime& other);
		RunningTime& operator= (const RunningTime& other);
		bool operator== (const RunningTime& other);
		
		bool isInSameRegionAs (const RunningTime& other) const;
		bool isWithinActiveWindow () const;
		int getRegionDifference (const RunningTime& other, bool absolute) const;
				
		double time;
		int region;
	};
	
	RunningTime getWindowedRunningTime (double newTime);
	double clampRunningTimeToWindow (double newTime);
	
	void startInternal ();
	void stopInternal ();
	void updateInternal ();
	void setRunningTimeInternal (const RunningTime& newTime);
	void refreshChildren ();
	void removedFromParentNode (TimingNode* parent); 
	void addedToParentNode (TimingNode* parent);
	void childNodeAdded (TimingNode* node);
	void childNodeRemoved (TimingNode* node);
	double getSourceTime ();
	
	Array<TimingNode*> children;
	ListenerList < Listener > listeners;
	TimingNode* parent;
	Transformer* transformer;
	double startTime;
	double speed;
	double activeWindowDuration;
	RunningTime currentRunningTime;
	double currentLocalTime;
};






#endif  // __TIMINGNODE_H_A6C62E40__
