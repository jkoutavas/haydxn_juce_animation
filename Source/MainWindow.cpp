/*
  ==============================================================================

    This file was auto-generated by the Jucer!

    It contains the basic outline for a simple desktop window.

  ==============================================================================
*/

#include "MainWindow.h"
#include "Animation/TimerBasedAnimator.h"
#include "Animation/AnimatedValueTree.h"

//==============================================================================

// Really rubbish test class to control the size of a component.
// no effort taken here to be safe or nice.

class RubbishAnimator	:	public AnimatedValueTree
{
public:
	
	static const Identifier width;
	static const Identifier height;

	RubbishAnimator (Component* target_)
	:	AnimatedValueTree ("test"),
		target(target_)
	{
		
	}
	
	~RubbishAnimator ()
	{
		
	}
	
	void refreshValues ()
	{
		node.setProperty(width, target->getWidth(), 0);
		node.setProperty(height, target->getHeight(), 0);		
	}
	
	void applyValueTreeChanges (const ValueTree& node)
	{
		int w = node.getProperty(width);
		int h = node.getProperty(height);
		target->setSize (w, h);
	}
	
private:
	
	Component* target;
	
};

const Identifier RubbishAnimator::width = "width";
const Identifier RubbishAnimator::height = "height";



class TestComp	:	public Component,
					public Button::Listener
{
public:
	
	TestComp ()
	:	rubbish(&button),
		animator(20)
	{
		rubbish.setAnimator (&animator);
		
		button.setButtonText ("Test");
		button.setClickingTogglesState (true);
		button.setBounds (50,50,100,60);
		addAndMakeVisible(&button);
		button.addListener (this);
	}
	
	~TestComp ()
	{
		
	}
	
	void paint (Graphics& g)
	{
		g.fillAll (Colours::hotpink);
	}
	
	void buttonClicked (Button* b)
	{
		if (b == &button)
		{
			double newH, newW;
			if (button.getToggleState())
			{
				newH = 100;
				newW = 200;
			}
			else
			{
				newH = 50;
				newW = 50;
			}
			double d = 0.3;
			
			rubbish.refreshValues (); // make sure we're holding the current values
			//TODO: perhaps change to using a AnimatedValueSet instead, where the
			// values can have sources which directly wrap existing data, removing
			// the need to manually refresh the local state.
			
			rubbish.animateValueTo (RubbishAnimator::width.toString(), newW, d);
			rubbish.animateValueTo (RubbishAnimator::height.toString(), newH, d);
		}
	}
	
private:
	
	RubbishAnimator rubbish;
	
	TimerBasedAnimator animator; // the main animator
	// note: there would ideally be a globally accessible one (e.g. in the
	// Desktop class).
	
	TextButton button;
	
};


//==============================================================================
MainAppWindow::MainAppWindow()
    : DocumentWindow (JUCEApplication::getInstance()->getApplicationName(),
                      Colours::lightgrey,
                      DocumentWindow::allButtons)
{
	TestComp* comp = new TestComp();
	setContentOwned(comp, false);
    centreWithSize (500, 400);
    setVisible (true);
}

MainAppWindow::~MainAppWindow()
{
}

void MainAppWindow::closeButtonPressed()
{
    JUCEApplication::getInstance()->systemRequestedQuit();
}