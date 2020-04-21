#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

#include "../GUI/Cell.h"
#include "../MyDefs.h"
#include "wdf.h"

class Node
{
    CREATE_LISTENER (Listener, listeners, virtual void addNode (Node* /*node*/, Node* /*newNode*/) {}\
                                          virtual void changeProbe (Node* /*newProbe*/) {}\
                                          virtual void changeInput (Node* /*newInput*/) {})
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

    WaveDigitalFilter::WDFNode* getWDF() { return wdf.get(); }

protected:
    Node* parent = nullptr;
    std::unique_ptr<Cell> cell;

    double fs = 44100.0;

    std::unique_ptr<WaveDigitalFilter::WDFNode> wdf;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Node)
};

#endif // NODE_H_INCLUDED
