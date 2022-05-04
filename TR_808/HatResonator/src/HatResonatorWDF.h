#pragma once

#include <chowdsp_dsp/chowdsp_dsp.h>
#include <chowdsp_wdf/chowdsp_wdf.h>

namespace wdft = chowdsp::wdft;

/**
 * Resonator from TR-808 hi-hat circuit.
 * Reference: http://www.ericarcher.net/wp-content/uploads/2014/07/tr-808-hihat-diy-project-revised-dec-2009.pdf (page 3)
 */
class HatResonatorWDF
{
public:
    HatResonatorWDF() = default;

    void prepare (double sampleRate)
    {
        C58.prepare ((float) sampleRate);
        C59.prepare ((float) sampleRate);
    }

    void reset()
    {
        C58.reset();
        C59.reset();
    }

    void setParameters (float freqHz, float Rmult01)
    {
        constexpr auto Rfb = 82.0e3f;
        const auto R_g = 100.0f * std::pow (10000.0f / 100.0f, std::pow (Rmult01, 0.37f));
        float C = 1.0f / (juce::MathConstants<float>::twoPi * freqHz * std::sqrt (Rfb * R_g));

        setParameters (Rfb, R_g, C);
    }

    void setParameters (float Rfb, float R_g, float Cval)
    {
        R197.setResistanceValue (Rfb);
        R196.setResistanceValue (R_g);
        C58.setCapacitanceValue (Cval);
        C59.setCapacitanceValue (Cval);
    }

    inline float processSample (float x)
    {
        Vin.setVoltage (-x); // inverting op-amp

        R.compute();

        return wdft::voltage<float> (R196) + wdft::voltage<float> (C59);
    }

private:
    // Port A
    wdft::ResistiveVoltageSourceT<float> Vin { 22.0e3f };
    wdft::CapacitorT<float> C4 { 1.0e-9f };
    wdft::WDFSeriesT<float, decltype (Vin), decltype (C4)> S1 { Vin, C4 };

    // Port B
    wdft::ResistorT<float> R197 { 820.0e3f };

    // Port C
    wdft::CapacitorT<float> C58 { 0.027e-6f };

    // Port D
    wdft::CapacitorT<float> C59 { 0.027e-6f };

    // Port E
    wdft::ResistorT<float> R196 { 680.0f };

    struct ImpedanceCalc
    {
        template <typename RType>
        static void calcImpedance (RType& R)
        {
            constexpr float Ag = 100.0f; // op-amp gain
            constexpr float Ri = 1.0e9f; // op-amp input impedance
            constexpr float Ro = 1.0e-1f; // op-amp output impedance
            const auto [Ra, Rb, Rc, Rd, Re] = R.getPortImpedances();

            // This scattering matrix was derived using the R-Solver python script (https://github.com/jatinchowdhury18/R-Solver),
            // invoked with command: r_solver.py --out scratch/hat_ress_uscatt.txt scratch/hat_res.txt
            R.setSMatrixData ({ { -((Ra * Rb + (Ra - Rb) * Rc) * Rd + (Ra * Rb + (Ra - Rb) * Rc + (Ra - Rb) * Rd) * Re - (((Ag + 1) * Rc + Rb) * Rd + ((Ag + 1) * Rb + (Ag + 1) * Rc + (Ag + 1) * Rd) * Re) * Ri - (Ra * Rb + (Ra - Rb) * Rc + (Ra - Rc) * Rd - (Rb + Rc + Rd) * Re - (Rb + Rc + Rd) * Ri) * Ro) / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (((Ag + 1) * Rc + Rb) * Rd + ((Ag + 1) * Rb + (Ag + 1) * Rc + (Ag + 1) * Rd) * Re) * Ri - (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re + (Rb + Rc + Rd) * Ri) * Ro), -2 * (Ra * Rc * Rd - Ra * Rc * Ro + (Ra * Rc + Ra * Rd) * Re) / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (((Ag + 1) * Rc + Rb) * Rd + ((Ag + 1) * Rb + (Ag + 1) * Rc + (Ag + 1) * Rd) * Re) * Ri - (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re + (Rb + Rc + Rd) * Ri) * Ro), 2 * (Ra * Rb * Rd + Ra * Rb * Re - (Ra * Rb + Ra * Rd) * Ro) / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (((Ag + 1) * Rc + Rb) * Rd + ((Ag + 1) * Rb + (Ag + 1) * Rc + (Ag + 1) * Rd) * Re) * Ri - (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re + (Rb + Rc + Rd) * Ri) * Ro), 2 * (Ra * Rb * Re + Ra * Rc * Ro) / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (((Ag + 1) * Rc + Rb) * Rd + ((Ag + 1) * Rb + (Ag + 1) * Rc + (Ag + 1) * Rd) * Re) * Ri - (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re + (Rb + Rc + Rd) * Ri) * Ro), 2 * (Ra * Rb * Rd - (Ra * Rb + Ra * Rc + Ra * Rd) * Ro) / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (((Ag + 1) * Rc + Rb) * Rd + ((Ag + 1) * Rb + (Ag + 1) * Rc + (Ag + 1) * Rd) * Re) * Ri - (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re + (Rb + Rc + Rd) * Ri) * Ro) },
                                { 2 * (Ag * Rb * Rd * Ri - Rb * Rc * Rd + Rb * Rc * Ro - (Rb * Rc + Rb * Rd) * Re) / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (((Ag + 1) * Rc + Rb) * Rd + ((Ag + 1) * Rb + (Ag + 1) * Rc + (Ag + 1) * Rd) * Re) * Ri - (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re + (Rb + Rc + Rd) * Ri) * Ro), -((Ra * Rb - (Ra - Rb) * Rc) * Rd + (Ra * Rb - (Ra - Rb) * Rc - (Ra - Rb) * Rd) * Re - (((Ag + 1) * Rc - Rb) * Rd - ((Ag + 1) * Rb - (Ag + 1) * Rc - (Ag + 1) * Rd) * Re) * Ri - (Ra * Rb - (Ra - Rb) * Rc - (Ra + Rc) * Rd + (Rb - Rc - Rd) * Re + (Rb - Rc - Rd) * Ri) * Ro) / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (((Ag + 1) * Rc + Rb) * Rd + ((Ag + 1) * Rb + (Ag + 1) * Rc + (Ag + 1) * Rd) * Re) * Ri - (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re + (Rb + Rc + Rd) * Ri) * Ro), -2 * (Ra * Rb * Rd + Ra * Rb * Re + ((Ag + 1) * Rb * Rd + (Ag + 1) * Rb * Re) * Ri - (Ra * Rb + Rb * Re + Rb * Ri) * Ro) / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (((Ag + 1) * Rc + Rb) * Rd + ((Ag + 1) * Rb + (Ag + 1) * Rc + (Ag + 1) * Rd) * Re) * Ri - (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re + (Rb + Rc + Rd) * Ri) * Ro), -2 * ((Ag + 1) * Rb * Re * Ri + Ra * Rb * Re - (Ra * Rb + Rb * Rc + Rb * Re + Rb * Ri) * Ro) / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (((Ag + 1) * Rc + Rb) * Rd + ((Ag + 1) * Rb + (Ag + 1) * Rc + (Ag + 1) * Rd) * Re) * Ri - (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re + (Rb + Rc + Rd) * Ri) * Ro), -2 * ((Ag + 1) * Rb * Rd * Ri + Ra * Rb * Rd + Rb * Rc * Ro) / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (((Ag + 1) * Rc + Rb) * Rd + ((Ag + 1) * Rb + (Ag + 1) * Rc + (Ag + 1) * Rd) * Re) * Ri - (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re + (Rb + Rc + Rd) * Ri) * Ro) },
                                { 2 * (Ag * Rc * Rd * Ri + Rb * Rc * Rd + Rb * Rc * Re - (Rb * Rc + Rc * Rd) * Ro) / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (((Ag + 1) * Rc + Rb) * Rd + ((Ag + 1) * Rb + (Ag + 1) * Rc + (Ag + 1) * Rd) * Re) * Ri - (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re + (Rb + Rc + Rd) * Ri) * Ro), -2 * (Ra * Rc * Rd + Ra * Rc * Re + ((Ag + 1) * Rc * Re + Rc * Rd) * Ri - (Ra * Rc + Rc * Re + Rc * Ri) * Ro) / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (((Ag + 1) * Rc + Rb) * Rd + ((Ag + 1) * Rb + (Ag + 1) * Rc + (Ag + 1) * Rd) * Re) * Ri - (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re + (Rb + Rc + Rd) * Ri) * Ro), ((Ra * Rb - (Ra + Rb) * Rc) * Rd + (Ra * Rb - (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re - (((Ag + 1) * Rc - Rb) * Rd - ((Ag + 1) * Rb - (Ag + 1) * Rc + (Ag + 1) * Rd) * Re) * Ri - (Ra * Rb - (Ra + Rb) * Rc + (Ra - Rc) * Rd + (Rb - Rc + Rd) * Re + (Rb - Rc + Rd) * Ri) * Ro) / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (((Ag + 1) * Rc + Rb) * Rd + ((Ag + 1) * Rb + (Ag + 1) * Rc + (Ag + 1) * Rd) * Re) * Ri - (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re + (Rb + Rc + Rd) * Ri) * Ro), -2 * ((Ag + 1) * Rc * Re * Ri + (Ra + Rb) * Rc * Re - (Ra * Rc + Rc * Re + Rc * Ri) * Ro) / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (((Ag + 1) * Rc + Rb) * Rd + ((Ag + 1) * Rb + (Ag + 1) * Rc + (Ag + 1) * Rd) * Re) * Ri - (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re + (Rb + Rc + Rd) * Ri) * Ro), -2 * ((Ag + 1) * Rc * Rd * Ri + (Ra + Rb) * Rc * Rd - (Rb * Rc + Rc * Rd) * Ro) / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (((Ag + 1) * Rc + Rb) * Rd + ((Ag + 1) * Rb + (Ag + 1) * Rc + (Ag + 1) * Rd) * Re) * Ri - (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re + (Rb + Rc + Rd) * Ri) * Ro) },
                                { 2 * (Rb * Rd * Re - (Ag * Rb + Ag * Rc) * Rd * Ri + Rc * Rd * Ro) / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (((Ag + 1) * Rc + Rb) * Rd + ((Ag + 1) * Rb + (Ag + 1) * Rc + (Ag + 1) * Rd) * Re) * Ri - (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re + (Rb + Rc + Rd) * Ri) * Ro), -2 * (Ra * Rd * Re + (Ag * Rc * Rd + (Ag + 1) * Rd * Re) * Ri - ((Ra + Rc) * Rd + Rd * Re + Rd * Ri) * Ro) / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (((Ag + 1) * Rc + Rb) * Rd + ((Ag + 1) * Rb + (Ag + 1) * Rc + (Ag + 1) * Rd) * Re) * Ri - (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re + (Rb + Rc + Rd) * Ri) * Ro), -2 * ((Ra + Rb) * Rd * Re - (Ag * Rb * Rd - (Ag + 1) * Rd * Re) * Ri - (Ra * Rd + Rd * Re + Rd * Ri) * Ro) / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (((Ag + 1) * Rc + Rb) * Rd + ((Ag + 1) * Rb + (Ag + 1) * Rc + (Ag + 1) * Rd) * Re) * Ri - (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re + (Rb + Rc + Rd) * Ri) * Ro), -((Ra * Rb + (Ra + Rb) * Rc) * Rd - (Ra * Rb + (Ra + Rb) * Rc - (Ra + Rb) * Rd) * Re + (((Ag + 1) * Rc + Rb) * Rd - ((Ag + 1) * Rb + (Ag + 1) * Rc - (Ag + 1) * Rd) * Re) * Ri + (Ra * Rb + (Ra + Rb) * Rc - (Ra + Rc) * Rd + (Rb + Rc - Rd) * Re + (Rb + Rc - Rd) * Ri) * Ro) / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (((Ag + 1) * Rc + Rb) * Rd + ((Ag + 1) * Rb + (Ag + 1) * Rc + (Ag + 1) * Rd) * Re) * Ri - (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re + (Rb + Rc + Rd) * Ri) * Ro), 2 * (((Ag + 1) * Rb + (Ag + 1) * Rc) * Rd * Ri - Rc * Rd * Ro + (Ra * Rb + (Ra + Rb) * Rc) * Rd) / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (((Ag + 1) * Rc + Rb) * Rd + ((Ag + 1) * Rb + (Ag + 1) * Rc + (Ag + 1) * Rd) * Re) * Ri - (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re + (Rb + Rc + Rd) * Ri) * Ro) },
                                { 2 * (Rb * Rd * Re + (Ag * Rb + Ag * Rc + Ag * Rd) * Re * Ri - (Rb + Rc + Rd) * Re * Ro) / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (((Ag + 1) * Rc + Rb) * Rd + ((Ag + 1) * Rb + (Ag + 1) * Rc + (Ag + 1) * Rd) * Re) * Ri - (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re + (Rb + Rc + Rd) * Ri) * Ro), -2 * (Ra * Rd * Re - (Ag * Rc - Rd) * Re * Ri + Rc * Re * Ro) / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (((Ag + 1) * Rc + Rb) * Rd + ((Ag + 1) * Rb + (Ag + 1) * Rc + (Ag + 1) * Rd) * Re) * Ri - (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re + (Rb + Rc + Rd) * Ri) * Ro), -2 * ((Ra + Rb) * Rd * Re + (Ag * Rb + (Ag + 1) * Rd) * Re * Ri - (Rb + Rd) * Re * Ro) / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (((Ag + 1) * Rc + Rb) * Rd + ((Ag + 1) * Rb + (Ag + 1) * Rc + (Ag + 1) * Rd) * Re) * Ri - (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re + (Rb + Rc + Rd) * Ri) * Ro), 2 * (((Ag + 1) * Rc + Rb) * Re * Ri - Rc * Re * Ro + (Ra * Rb + (Ra + Rb) * Rc) * Re) / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (((Ag + 1) * Rc + Rb) * Rd + ((Ag + 1) * Rb + (Ag + 1) * Rc + (Ag + 1) * Rd) * Re) * Ri - (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re + (Rb + Rc + Rd) * Ri) * Ro), ((Ra * Rb + (Ra + Rb) * Rc) * Rd - (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (((Ag + 1) * Rc + Rb) * Rd - ((Ag + 1) * Rb + (Ag + 1) * Rc + (Ag + 1) * Rd) * Re) * Ri - (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd - (Rb + Rc + Rd) * Re + (Rb + Rc + Rd) * Ri) * Ro) / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (((Ag + 1) * Rc + Rb) * Rd + ((Ag + 1) * Rb + (Ag + 1) * Rc + (Ag + 1) * Rd) * Re) * Ri - (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re + (Rb + Rc + Rd) * Ri) * Ro) } });
        }
    };

    using RType = wdft::RootRtypeAdaptor<float, ImpedanceCalc, decltype (S1), decltype (R197), decltype (C58), decltype (C59), decltype (R196)>;
    RType R { std::tie (S1, R197, C58, C59, R196) };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HatResonatorWDF)
};
