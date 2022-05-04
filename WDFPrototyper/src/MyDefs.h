#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED

#define CREATE_LISTENER(ListenerName, listName, funcs)                   \
public:                                                                  \
    struct ListenerName                                                  \
    {                                                                    \
        virtual ~ListenerName() {}                                       \
        funcs                                                            \
    };                                                                   \
    void add##ListenerName (ListenerName* l) { listName.add (l); }       \
    void remove##ListenerName (ListenerName* l) { listName.remove (l); } \
                                                                         \
protected:                                                               \
    juce::ListenerList<ListenerName> listName;

#endif // DEFS_H_INCLUDED
