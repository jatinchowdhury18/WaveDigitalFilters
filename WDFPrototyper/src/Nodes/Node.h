#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

#include <chowdsp_dsp/chowdsp_dsp.h>
#include <chowdsp_wdf/chowdsp_wdf.h>

#include "../MyDefs.h"
#include "Cell.h"
#include "Property.h"

//using namespace chowdsp::wdf;
using WDFNode = chowdsp::wdf::WDF<double>;

class Node
{
    // clang-format off
    CREATE_LISTENER (
        Listener,
        listeners,
        virtual void addNode (Node* /*node*/, Node* /*newNode*/) {} \
        virtual void replaceNode (Node* /*oldNode*/, Node* /*newNode*/) {} \
        virtual void changeProbe (Node* /*newProbe*/) {} \
        virtual void changeInput (Node* /*newInput*/) {} \
        virtual void unprepare() {})
    // clang-format on
public:
    Node() = default;
    virtual ~Node() = default;

    virtual Cell* getCell() { return cell.get(); }

    Node* getParent() { return parent; }
    void setParent (Node* newParent) { parent = newParent; }

    virtual bool prepare (double sampleRate)
    {
        fs = sampleRate;
        return true;
    }

    WDFNode* getWDF() { return wdf; }

    juce::OwnedArray<Property>& getProps() { return props; }

protected:
    Node* parent = nullptr;
    std::unique_ptr<Cell> cell;

    double fs = 44100.0;

    WDFNode* wdf = nullptr;

    juce::OwnedArray<Property> props;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Node)
};

#endif // NODE_H_INCLUDED
