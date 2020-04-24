#ifndef PROPERTIESCOMPONENT_H_INCLUDED
#define PROPERTIESCOMPONENT_H_INCLUDED

#include "../Nodes/Node.h"

class PropertiesComponent : public Component
{
public:
    PropertiesComponent (Node& node);

    void paint (Graphics& g) override;
    void resized() override;

private:
    Node& node;

    OwnedArray<Label> labels;
    OwnedArray<Label> values;

    TextButton enter { "Enter" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PropertiesComponent)
};

#endif // PROPERTIESCOMPONENT_H_INCLUDED
