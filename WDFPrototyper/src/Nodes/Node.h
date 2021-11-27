#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

#include "../MyDefs.h"
#include "Cell.h"
#include "Property.h"

//using namespace chowdsp::WDF;
using WDFNode = chowdsp::WDF::WDF<double>;

class Node
{
    CREATE_LISTENER (
        Listener,
        listeners,
        virtual void addNode (Node* /*node*/, Node* /*newNode*/) {} virtual void replaceNode (Node* /*oldNode*/, Node* /*newNode*/) {} virtual void changeProbe (Node* /*newProbe*/) {} virtual void changeInput (Node* /*newInput*/) {} virtual void unprepare() {})
public:
    Node() {}
    virtual ~Node() {}

    virtual Cell* getCell() { return cell.get(); }

    Node* getParent() { return parent; }
    void setParent (Node* newParent) { parent = newParent; }

    virtual bool prepare (double sampleRate)
    {
        fs = sampleRate;
        return true;
    }

    WDFNode* getWDF() { return wdf.get(); }

    OwnedArray<Property>& getProps() { return props; }

protected:
    Node* parent = nullptr;
    std::unique_ptr<Cell> cell;

    double fs = 44100.0;

    std::unique_ptr<WDFNode> wdf;

    OwnedArray<Property> props;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Node)
};

#endif // NODE_H_INCLUDED
