/*
  ==============================================================================

    NamedSet.h
    Created: 23 Feb 2011 1:17:05pm
    Author:  ukhstock

  ==============================================================================
*/

#ifndef __NAMEDSET_H_7A4F7036__
#define __NAMEDSET_H_7A4F7036__

#include "../../JuceLibraryCode/JuceHeader.h"

template <class ObjectType>
class NamedSet
{
public:

	/** Creates an empty set. */
	NamedSet() throw() {}

	/** Destructor. */
    ~NamedSet()
	{
		clear ();
	}

    //==============================================================================
    /** Returns the total number of values that the set contains. */
    int size() const throw()
	{
		return objects.size ();
	}

    /** Returns the value of a named item.
        If the name isn't found, this will return a void variant.
        @see getProperty
    */
    ObjectType* operator[] (const Identifier& name) const
	{
		for (NamedObject* i = objects; i != 0; i = i->nextListItem)
			if (i->name == name)
				return i->object;

		return 0;
	}

    /** Changes or adds a named value.
        @returns    true if a value was changed or added; false if the
                    value was already set the the value passed-in.
    */
    bool set (const Identifier& name, ObjectType* newValue)
	{
		LinkedListPointer<NamedObject>* i = &objects;

		while (i->get() != 0)
		{
			NamedObject* const v = i->get();

			if (v->name == name)
			{
				if (v->object == newValue)
					return false;

				v->object = newValue;
				return true;
			}

			i = &(v->nextListItem);
		}

		i->insertNext (new NamedObject (name, newValue));
		return true;
	}

    /** Returns true if the set contains an item with the specified name. */
    bool contains (const Identifier& name) const
	{
		return operator[] (name) != 0;
	}

    /** Removes a value from the set.
        @returns    true if a value was removed; false if there was no value
                    with the name that was given.
    */
    bool remove (const Identifier& name)
	{
		LinkedListPointer<NamedObject>* i = &objects;

		for (;;)
		{
			NamedObject* const v = i->get();

			if (v == 0)
				break;

			if (v->name == name)
			{
				delete i->removeNext();
				return true;
			}

			i = &(v->nextListItem);
		}

		return false;
	}

    /** Returns the name of the value at a given index.
        The index must be between 0 and size() - 1.
    */
    const Identifier getName (int index) const
	{
		const NamedObject* const v = objects[index];
		jassert (v != 0);
		return v->name;
	}

    /** Returns the value of the item at a given index.
        The index must be between 0 and size() - 1.
    */
    ObjectType* getObjectAt (int index) const
	{
		const NamedObject* const v = objects[index];
		jassert (v != 0);
		return v->object;
	}

    /** Removes all values. */
    void clear()
	{
		objects.deleteAll ();
	}

private:

    //==============================================================================

	class NamedObject
    {
    public:

        NamedObject (const Identifier& nameToUse, ObjectType* const objectToHold)
			:	name (nameToUse),
				object (objectToHold)
		{
		}

        LinkedListPointer<NamedObject> nextListItem;
        Identifier name;
        ScopedPointer<ObjectType> object;

    private:
        JUCE_LEAK_DETECTOR (NamedObject);
    };

    friend class LinkedListPointer<NamedObject>;
    LinkedListPointer<NamedObject> objects;
};



#endif  // __NAMEDSET_H_7A4F7036__
