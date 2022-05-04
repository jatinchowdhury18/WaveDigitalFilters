#ifndef PROPERTIESCOMPONENT_H_INCLUDED
#define PROPERTIESCOMPONENT_H_INCLUDED

#include "Node.h"

class PropertiesComponent : public juce::Component
{
public:
    explicit PropertiesComponent (Node& node);

    void paint (juce::Graphics& g) override;
    void resized() override;

    void mouseDrag (const juce::MouseEvent& e) override;

private:
    Node& node;

    juce::OwnedArray<juce::Label> labels;
    juce::OwnedArray<juce::Label> values;

    juce::TextButton enter { "Enter" };

    bool shouldMove = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PropertiesComponent)
};

#endif // PROPERTIESCOMPONENT_H_INCLUDED
