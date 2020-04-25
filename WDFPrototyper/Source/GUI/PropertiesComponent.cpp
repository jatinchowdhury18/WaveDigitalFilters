#include "PropertiesComponent.h"

PropertiesComponent::PropertiesComponent (Node& node) :
    node (node)
{
    int height = 50;

    for (auto p : node.getProps())
    {
        Label* pLabel = new Label;
        pLabel->setText (p->name, dontSendNotification);
        addAndMakeVisible (pLabel);
        pLabel->setInterceptsMouseClicks (false, false);
        labels.add (pLabel);

        auto pValLabel = values.add (new Label);
        addAndMakeVisible (pValLabel);
        pValLabel->setText (String (p->value, 3, true), dontSendNotification);
        pValLabel->setEditable (true);
        pValLabel->setColour (Label::backgroundColourId, Colours::black);
        pValLabel->onTextChange = [=]
        {
            p->setValue (pValLabel->getText (true).getFloatValue());
            pValLabel->setText (String (p->value, 3, true), dontSendNotification);
        };

        height += 50;
    }


    setSize (300, height);

    addAndMakeVisible (enter);
    enter.onClick = [=] ()
    {
        setVisible (false);
        getParentComponent()->repaint();
    };
}

void PropertiesComponent::mouseDrag (const MouseEvent& e)
{
    setTopLeftPosition (e.getEventRelativeTo (getParentComponent()).getPosition());
    getParentComponent()->repaint();
}

void PropertiesComponent::paint (Graphics& g)
{
    g.fillAll (Colours::darkgrey);
    
    g.setColour (Colours::white);
    g.drawRect (getLocalBounds().toFloat(), 2.0f);
}

void PropertiesComponent::resized()
{
    for (int i = 0; i < node.getProps().size(); ++i)
    {
        labels[i]->setBounds (  0, i * 50,      100, 50);
        values[i]->setBounds (105, i * 50 + 10, 190, 30);
    }

    enter.setBounds (110, getHeight() - 40, 80, 30);
}
