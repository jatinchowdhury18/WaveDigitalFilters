#ifndef WDF_H_INCLUDED
#define WDF_H_INCLUDED

#include <string>

namespace WaveDigitalFilter
{
/** Wave digital filter base class */
class WDF
{
public:
    WDF (std::string type) : type (type) {}
    virtual ~WDF() {}

    virtual inline void calcImpedance() = 0;
    virtual inline void propagateImpedance() = 0;

    virtual inline void incident (double x) = 0;
    virtual inline double reflected() = 0;


    inline double voltage()
    {
        return (a + b) / 2.0;
    }

    inline double current()
    {
        return (a - b) / (2.0 * R);
    }

public:
    double a = 0.0; // incident wave
    double b = 0.0; // reflected wave
    double R = 1.0e-9;
    double G = 1.0 / R;

private:
    const std::string type;
};

/** WDF node base class */
class WDFNode : public WDF
{
public:
    WDFNode (std::string type) : WDF (type) {}
    virtual ~WDFNode() {}

    void connectToNode (WDF* node)
    {
        next = node;
    }

    inline void propagateImpedance() override
    {
        calcImpedance();

        if (next != nullptr)
            next->propagateImpedance();
    }

protected:
    WDF* next = nullptr;
};

/** WDF Resistor Node */
class Resistor : public WDFNode
{
public:
    Resistor (double value) :
        WDFNode ("Resistor"),
        R_value (value)
    {
        calcImpedance();
    }
    virtual ~Resistor() {}

    void setResistanceValue (double newR)
    {
        if (newR == R_value)
            return;

        R_value = newR;
        propagateImpedance();
    }

    inline void calcImpedance() override
    {
        R = R_value;
        G = 1.0 / R;
    }

    inline void incident (double x) override
    {
        a = x;
    }

    inline double reflected() override
    {
        b = 0.0;
        return b;
    }

private:
    double R_value = 1.0e-9;
};

/** WDF Capacitor Node */
class Capacitor : public WDFNode
{
public:
    Capacitor (double value, double fs) :
        WDFNode ("Capacitor"),
        C_value (value),
        fs (fs)
    {
        calcImpedance();
    }
    virtual ~Capacitor() {}

    void setCapacitanceValue (double newC)
    {
        if (newC == C_value)
            return;

        C_value = newC;
        propagateImpedance();
    }

    inline void calcImpedance() override
    {
        R = 1.0 / (2.0 * C_value * fs);
        G = 1.0 / R;
    }

    inline void incident (double x) override
    {
        a = x;
        z = a;
    }

    inline double reflected() override
    {
        b = z;
        return b;
    }

private:
    double C_value = 1.0e-6;
    double z = 0.0;
    const double fs;
};


/** WDF Inductor Node */
class Inductor : public WDFNode
{
public:
    Inductor (double value, double fs) :
        WDFNode ("Inductor"),
        L_value (value),
        fs (fs)
    {
        calcImpedance();
    }
    virtual ~Inductor() {}

    void setInductanceValue (double newL)
    {
        if (newL == L_value)
            return;

        L_value = newL;
        propagateImpedance();
    }

    inline void calcImpedance() override
    {
        R = 2.0 * L_value * fs;
        G = 1.0 / R;
    }

    inline void incident (double x) override
    {
        a = x;
        z = a;
    }

    inline double reflected() override
    {
        b = -z;
        return b;
    }

private:
    double L_value = 1.0e-6;
    double z = 0.0;
    const double fs;
};

/** WDF Switch */
class Switch : public WDFNode
{
public:
    Switch():
        WDFNode ("Switch")
    {}
    virtual ~Switch() {}

    inline void calcImpedance() override {}

    void setClosed (bool shouldClose) { closed = shouldClose; }

    inline void incident (double x) override
    {
        a = x;
    }

    inline double reflected() override
    {
        b = closed ? -a : a;
        return b;
    }

private:
    bool closed = true;
};

/** WDF Open */
class Open : public WDFNode
{
public:
    Open():
        WDFNode ("Open")
    {}
    virtual ~Open()
    {
        R = 1.0e15;
        G = 1.0 / R;
    }

    inline void calcImpedance() override {}

    inline void incident (double x) override
    {
        a = x;
    }

    inline double reflected() override
    {
        b = a;
        return b;
    }
};

/** WDF Short */
class Short : public WDFNode
{
public:
    Short():
        WDFNode ("Short")
    {}
    virtual ~Short()
    {
        R = 1.0e-15;
        G = 1.0 / R;
    }

    inline void calcImpedance() override {}

    inline void incident (double x) override
    {
        a = x;
    }

    inline double reflected() override
    {
        b = -a;
        return b;
    }
};

/** WDF Voltage Polarity Inverter */
class PolarityInverter : public WDFNode
{
public:
    PolarityInverter (WDFNode* port1) :
        WDFNode ("Polarity Inverter"),
        port1 (port1)
    {
        port1->connectToNode (this);
        calcImpedance();
    }
    virtual ~PolarityInverter() {}

    inline void calcImpedance() override
    {
        R = port1->R;
        G = 1.0 / R;
    }

    inline void incident (double x) override
    {
        a = x;
        port1->incident (-x);
    }

    inline double reflected() override
    {
        b = -port1->reflected();
        return b;
    }

private:
    WDFNode* port1;
};

/** WDF 3-port adapter base class */
class WDFAdaptor : public WDFNode
{
public:
    WDFAdaptor (WDFNode* port1, WDFNode* port2, std::string type) :
        WDFNode (type),
        port1 (port1),
        port2 (port2)
    {
        port1->connectToNode (this);
        port2->connectToNode (this);
    }
    virtual ~WDFAdaptor() {}

protected:
    WDFNode* port1;
    WDFNode* port2;
};

/** WDF 3-port parallel adaptor */
class WDFParallel : public WDFAdaptor
{
public:
    WDFParallel (WDFNode* port1, WDFNode* port2) :
        WDFAdaptor (port1, port2, "Parallel")
    {
        calcImpedance();
    }
    virtual ~WDFParallel() {}

    inline void calcImpedance()
    {
        G = port1->G + port2->G;
        R = 1.0 / G;
    }

    inline double reflected() override
    {
        b = (port1->G/G) * port1->reflected() + (port2->G/G) * port2->reflected();
        return b;
    }

    inline void incident (double x) override
    {
        port1->incident (x + (port2->b - port1->b) * (port2->G/G));
        port2->incident (x + (port2->b - port1->b) * (-port1->G/G));
        a = x;
    }

private:
    
};

/** WDF 3-port series adaptor */
class WDFSeries : public WDFAdaptor
{
public:
    WDFSeries (WDFNode* port1, WDFNode* port2) :
        WDFAdaptor (port1, port2, "Series")
    {
        calcImpedance();
    }
    virtual ~WDFSeries() {}

    inline void calcImpedance()
    {
        R = port1->R + port2->R;
        G = 1.0 / R;
    }

    inline double reflected() override
    {
        b = -(port1->reflected() + port2->reflected());
        return b;
    }

    inline void incident (double x) override
    {
        port1->incident (port1->b - (port1->R/R) * (x + port1->b + port2->b));
        port2->incident (port2->b - (port2->R/R) * (x + port1->b + port2->b)); 

        a = x;
    }

private:
    
};

/** WDF Voltage source with 1 pOhm resistance */
class ResistiveVoltageSource : public WDFNode
{
public:
    ResistiveVoltageSource() : WDFNode ("Resistive Voltage")
    {
        calcImpedance();
    }
    virtual ~ResistiveVoltageSource() {}

    inline void calcImpedance()
    {
        R = 1.0e-9;
        G = 1.0 / R;
    }

    void setVoltage (double newV) { Vs = newV; }

    inline void incident (double x) override
    {
        a = x;
    }

    inline double reflected() override
    {
        b = Vs;
        return b;
    }

private:
    double Vs;
};

/** WDF Voltage source with 1 pOhm resistance */
class IdealVoltageSource : public WDFNode
{
public:
    IdealVoltageSource() : WDFNode ("IdealVoltage")
    {
        calcImpedance();
    }
    virtual ~IdealVoltageSource() {}

    inline void calcImpedance()
    {}

    void setVoltage (double newV) { Vs = newV; }

    inline void incident (double x) override
    {
        a = x;
    }

    inline double reflected() override
    {
        b = -a + 2.0 * Vs;
        return b;
    }

private:
    double Vs;
};

/** WDF Leaky Capacitor Node */
class LeakyCapacitor : public WDFParallel
{
public:
    LeakyCapacitor (double value, double fs) :
        C (value, fs),
        Rp (100e6),
        WDFParallel (&C, &Rp)
    {
        calcImpedance();
    }
    virtual ~LeakyCapacitor() {}

    void setCapacitanceValue (double newC)
    {
        C.setCapacitanceValue (newC);
    }

    void setParallelResistance (double newR)
    {
        Rp.setResistanceValue (newR);
    }

private:
    Capacitor C;
    Resistor Rp;
};

}

#endif // WDF_H_INCLUDED
