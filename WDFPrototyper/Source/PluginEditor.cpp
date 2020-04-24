#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "GUI/RootCell.h"

WdfprototyperAudioProcessorEditor::WdfprototyperAudioProcessorEditor (WdfprototyperAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    setSize (600, 600);

    refresh (processor.root.get());
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
        cell->addChangeListener (this);

        if (auto rootCast = dynamic_cast<RootCell*> (cell))
        {
            rootCast->setTopLeftPosition (275, 50);
        }
        else
        {
            auto point = node->getParent()->getCell()->getPosition();
            point.addXY (center * 50, 70);
            cell->setTopLeftPosition (point);
        }
    }

    if (auto rootNode = dynamic_cast<RootNode*> (node))
    {
        refresh (rootNode->getChild());
    }
    else if (auto onePortNode = dynamic_cast<OnePort*> (node))
    {
        refresh (onePortNode->getChild());
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
    if (auto onePortNode = dynamic_cast<OnePort*> (node))
    {
        paintConnectionTree (onePortNode->getChild(), g);
    }
    else if (auto twoPortNode = dynamic_cast<TwoPort*> (node))
    {
        paintConnectionTree (twoPortNode->getChild (0), g);
        paintConnectionTree (twoPortNode->getChild (1), g);
    }
}

Node* getNodeForCell (Cell* cell, Node* startNode)
{
    if (startNode == nullptr)
        return nullptr;

    if (startNode->getCell() == cell)
        return startNode;

    if (auto rootCast = dynamic_cast<RootNode*> (startNode))
    {
        if (auto node = getNodeForCell (cell, rootCast->getChild()))
            return node;
    }
    else if (auto onePortCast = dynamic_cast<OnePort*> (startNode))
    {
        if (auto node = getNodeForCell (cell, onePortCast->getChild()))
            return node;
    }
    else if (auto twoPortCast = dynamic_cast<TwoPort*> (startNode))
    {
        if (auto node = getNodeForCell (cell, twoPortCast->getChild (0)))
            return node;
        else if (auto otherNode = getNodeForCell (cell, twoPortCast->getChild (1)))
            return otherNode;
    }

    return nullptr;
}

void WdfprototyperAudioProcessorEditor::changeListenerCallback (ChangeBroadcaster* source)
{
    if (auto cellCast = dynamic_cast<Cell*> (source))
    {
        auto sourceNode = getNodeForCell (cellCast, processor.root.get());
        if (sourceNode == nullptr)
            return;

        propsComp = std::make_unique<PropertiesComponent> (*sourceNode);
        addAndMakeVisible (propsComp.get());
        propsComp->setTopLeftPosition (150, 200);
    }
}

void WdfprototyperAudioProcessorEditor::resized()
{
}
