#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "GUI/RootCell.h"

WdfprototyperAudioProcessorEditor::WdfprototyperAudioProcessorEditor (WdfprototyperAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    setSize (600, 600);

    refresh (processor.root.get());

    // addAndMakeVisible (processor.root->getCell());
    // processor.root->getCell()->setTopLeftPosition (275, 50);
}

WdfprototyperAudioProcessorEditor::~WdfprototyperAudioProcessorEditor()
{
}

void WdfprototyperAudioProcessorEditor::refresh (Node* node, int center)
{
    if (node == nullptr)
        return;

    auto cell = node->getCell();

    int isAdded = getIndexOfChildComponent (cell);
    if (isAdded < 0)
    {
        addAndMakeVisible (cell);

        if (auto rootCast = dynamic_cast<RootCell*> (cell))
        {
            rootCast->setTopLeftPosition (275, 50);
        }
        else
        {
            auto point = node->getParent()->getCell()->getPosition();
            point.addXY (center * 75, 100);
            cell->setTopLeftPosition (point);
        }
    }

    if (auto rootNode = dynamic_cast<RootNode*> (node))
    {
        refresh (rootNode->getChild());
    }
    else if (auto twoPortNode = dynamic_cast<TwoPort*> (node))
    {
        refresh (twoPortNode->getChild (0), -1);
        refresh (twoPortNode->getChild (1),  1);
    }
}

void WdfprototyperAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::black);

    paintConnectionTree (processor.root.get(), g);
}

void WdfprototyperAudioProcessorEditor::paintConnectionTree (Node* node, Graphics& g)
{
    if (node == nullptr)
        return;

    //  base case
    if (auto rootNode = dynamic_cast<RootNode*> (node))
    {
        paintConnectionTree (rootNode->getChild(), g);
        return;
    }

    // draw connection to parent
    auto start = node->getCell()->getBounds().toFloat().getCentre();
    auto end = node->getParent()->getCell()->getBounds().toFloat().getCentre();

    g.setColour (Colours::yellow);
    g.drawLine (start.x, start.y, end.x, end.y);

    // recurse
    if (auto twoPortNode = dynamic_cast<TwoPort*> (node))
    {
        paintConnectionTree (twoPortNode->getChild (0), g);
        paintConnectionTree (twoPortNode->getChild (1), g);
    }
}

void WdfprototyperAudioProcessorEditor::resized()
{
    
}
