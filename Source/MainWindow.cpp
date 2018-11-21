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
	
    static const Identifier Angle;
	static const Identifier Width;
	static const Identifier Height;

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
        node.setProperty(Angle, angle, nullptr);
		node.setProperty(Width, target->getWidth(), nullptr);
		node.setProperty(Height, target->getHeight(), nullptr);
	}
	
	void applyValueTreeChanges (const ValueTree& node)
	{
        angle = node.getProperty(Angle);
		int w = node.getProperty(Width);
		int h = node.getProperty(Height);
		target->setSize (w, h);

        auto bounds = target->getBounds();
        auto cx = bounds.getCentreX();
        auto cy = bounds.getCentreY();
        target->setTransform(AffineTransform::rotation(angle, cx, cy));
	}
	
private:
	float angle;
	Component* target;
	
};

const Identifier RubbishAnimator::Angle = "angle";
const Identifier RubbishAnimator::Width = "width";
const Identifier RubbishAnimator::Height = "height";



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
            float newA;
			double newH, newW;
			if (button.getToggleState())
			{
                newA = 3.1415926*2;
				newH = 100;
				newW = 200;
			}
			else
			{
                newA = 0;
				newH = 50;
				newW = 50;
			}
			double d = 0.3;
			
			rubbish.refreshValues (); // make sure we're holding the current values
			//TODO: perhaps change to using a AnimatedValueSet instead, where the
			// values can have sources which directly wrap existing data, removing
			// the need to manually refresh the local state.
            rubbish.animateValueTo (RubbishAnimator::Angle.toString(), newA, d);
			rubbish.animateValueTo (RubbishAnimator::Width.toString(), newW, d);
			rubbish.animateValueTo (RubbishAnimator::Height.toString(), newH, d);
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
