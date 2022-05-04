#include "PropertiesComponent.h"

PropertiesComponent::PropertiesComponent (Node& node) : node (node)
{
    int height = 50;

    for (auto p : node.getProps())
    {
        auto pLabel = labels.add (std::make_unique<juce::Label>());
        pLabel->setText (p->name, juce::dontSendNotification);
        addAndMakeVisible (pLabel);
        pLabel->setInterceptsMouseClicks (false, false);

        auto pValLabel = values.add (std::make_unique<juce::Label>());
        addAndMakeVisible (pValLabel);
        pValLabel->setText (juce::String (p->value, 3, true), juce::dontSendNotification);
        pValLabel->setEditable (true);
        pValLabel->setColour (juce::Label::backgroundColourId, juce::Colours::black);
        pValLabel->onTextChange = [=]
        {
            p->setValue (pValLabel->getText (true).getFloatValue());
            pValLabel->setText (juce::String (p->value, 3, true), juce::dontSendNotification);
        };

        height += 50;
    }

    setSize (300, height);

    addAndMakeVisible (enter);
    enter.onClick = [=]()
    {
        setVisible (false);
        getParentComponent()->repaint();
    };
}

void PropertiesComponent::mouseDrag (const juce::MouseEvent& e)
{
    setTopLeftPosition (e.getEventRelativeTo (getParentComponent()).getPosition());
    getParentComponent()->repaint();
}

void PropertiesComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::darkgrey);

    g.setColour (juce::Colours::white);
    g.drawRect (getLocalBounds().toFloat(), 2.0f);
}

void PropertiesComponent::resized()
{
    for (int i = 0; i < node.getProps().size(); ++i)
    {
        labels[i]->setBounds (0, i * 50, 100, 50);
        values[i]->setBounds (105, i * 50 + 10, 190, 30);
    }

    enter.setBounds (110, getHeight() - 40, 80, 30);
}
