#ifndef IDS_H_INCLUDED
#define IDS_H_INCLUDED

namespace IDs
{
    enum Leaf
    {
        Resistor,
        Capacitor,
        Inductor,
        ResistiveVs,
        ResistiveIs,
    };

    enum Adaptor
    {
        Series,
        Parallel,
        Inverter,
    };

    enum Root
    {
        IdealVs,
        IdealIs,
        Diode,
        DiodePair,
    };
};

#endif // IDS_H_INCLUDED
