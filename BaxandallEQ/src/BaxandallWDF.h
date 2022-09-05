#pragma once

#include <chowdsp_dsp/chowdsp_dsp.h>

#include <chowdsp_wdf/chowdsp_wdf.h>

namespace wdft = chowdsp::wdft;

/**
 * Implentation based on Werner et. al:
 * https://ieeexplore.ieee.org/stamp/stamp.jsp?tp=&arnumber=8371321
 */
class BaxandallWDF
{
public:
    BaxandallWDF() = default;

    void prepare (double fs);
    void reset();
    void setParams (float bassParam, float trebleParam);

    inline float processSample (float x)
    {
        Vin.setVoltage (x);
        R.compute();

        return wdft::voltage<float> (Rl);
    }

private:
    static constexpr auto Pt = 100.0e3f;
    static constexpr auto Pb = 100.0e3f;

    // Port A
    wdft::ResistorT<float> Pt_plus { Pt * 0.5f };
    wdft::ResistorT<float> Resd { 10.0e3f };
    wdft::WDFParallelT<float, decltype (Pt_plus), decltype (Resd)> P4 { Pt_plus, Resd };
    wdft::CapacitorT<float> Cd { 6.4e-9f };
    wdft::WDFSeriesT<float, decltype (Cd), decltype (P4)> S4 { Cd, P4 };

    // Port B
    wdft::ResistorT<float> Pt_minus { Pt * 0.5f };
    wdft::ResistorT<float> Rese { 1.0e3f };
    wdft::WDFParallelT<float, decltype (Pt_minus), decltype (Rese)> P5 { Pt_minus, Rese };
    wdft::CapacitorT<float> Ce { 64.0e-9f };
    wdft::WDFSeriesT<float, decltype (Ce), decltype (P5)> S5 { Ce, P5 };
    wdft::ResistorT<float> Rl { 1.0e6f };
    wdft::WDFParallelT<float, decltype (Rl), decltype (S5)> P1 { Rl, S5 };

    // Port C
    wdft::ResistorT<float> Resc { 10.0e3f };

    // Port D
    wdft::ResistorT<float> Pb_minus { Pb * 0.5f };
    wdft::CapacitorT<float> Cc { 220.0e-9f };
    wdft::WDFParallelT<float, decltype (Pb_minus), decltype (Cc)> P3 { Pb_minus, Cc };
    wdft::ResistorT<float> Resb { 1.0e3f };
    wdft::WDFSeriesT<float, decltype (Resb), decltype (P3)> S3 { Resb, P3 };

    // Port E
    wdft::ResistorT<float> Pb_plus { Pb * 0.5f };
    wdft::CapacitorT<float> Cb { 22.0e-9f };
    wdft::WDFParallelT<float, decltype (Pb_plus), decltype (Cb)> P2 { Pb_plus, Cb };
    wdft::ResistorT<float> Resa { 10.0e3f };
    wdft::WDFSeriesT<float, decltype (Resa), decltype (P2)> S2 { Resa, P2 };

    // Port F
    wdft::ResistiveVoltageSourceT<float> Vin; // @TODO make this ideal when we make the R-node adaptable
    wdft::CapacitorT<float> Ca { 1.0e-6f };
    wdft::WDFSeriesT<float, decltype (Vin), decltype (Ca)> S1 { Vin, Ca };

    struct ImpedanceCalc
    {
        template <typename RType>
        static void calcImpedance (RType& R)
        {
            auto [Ra, Rb, Rc, Rd, Re, Rf] = R.getPortImpedances();

            // This scattering matrix was derived using the R-Solver python script (https://github.com/jatinchowdhury18/R-Solver),
            // invoked with command: r_solver.py --datum 0 --out scratch/baxandall_uscatt.txt netlists/baxandall.txt
            R.setSMatrixData ({ { -2 * ((Rb + Rc) * Rd + (Rb + Rc + Rd) * Re + (Rb + Rc + Rd) * Rf) * Ra / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf) + 1, -2 * (Rc * Rd + (Rc + Rd) * Re + Rc * Rf) * Ra / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf), 2 * (Rb * Rd + Rb * Re + (Rb + Rd) * Rf) * Ra / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf), -2 * (Rb * Re - Rc * Rf) * Ra / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf), 2 * (Rb * Rd + (Rb + Rc + Rd) * Rf) * Ra / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf), -2 * (Rc * Rd + (Rb + Rc + Rd) * Re) * Ra / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf) },
                                { -2 * (Rc * Rd + (Rc + Rd) * Re + Rc * Rf) * Rb / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf), -2 * ((Ra + Rc) * Rd + (Ra + Rc + Rd) * Re + (Ra + Rc + Re) * Rf) * Rb / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf) + 1, -2 * (Ra * Rd + Ra * Re + (Ra + Re) * Rf) * Rb / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf), 2 * (Ra * Re + (Ra + Rc + Re) * Rf) * Rb / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf), -2 * (Ra * Rd - Rc * Rf) * Rb / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf), -2 * ((Ra + Rc) * Rd + (Rc + Rd) * Re) * Rb / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf) },
                                { 2 * (Rb * Rd + Rb * Re + (Rb + Rd) * Rf) * Rc / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf), -2 * (Ra * Rd + Ra * Re + (Ra + Re) * Rf) * Rc / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf), -2 * ((Ra + Rb) * Rd + (Ra + Rb) * Re + (Ra + Rb + Rd + Re) * Rf) * Rc / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf) + 1, 2 * ((Ra + Rb) * Re + (Ra + Re) * Rf) * Rc / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf), -2 * ((Ra + Rb) * Rd + (Rb + Rd) * Rf) * Rc / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf), -2 * (Ra * Rd - Rb * Re) * Rc / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf) },
                                { -2 * (Rb * Re - Rc * Rf) * Rd / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf), 2 * (Ra * Re + (Ra + Rc + Re) * Rf) * Rd / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf), 2 * ((Ra + Rb) * Re + (Ra + Re) * Rf) * Rd / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf), -2 * (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Re + (Ra + Rc + Re) * Rf) * Rd / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf) + 1, -2 * (Ra * Rb + (Ra + Rb) * Rc + Rc * Rf) * Rd / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf), -2 * (Ra * Rb + (Ra + Rb) * Rc + Rb * Re) * Rd / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf) },
                                { 2 * (Rb * Rd + (Rb + Rc + Rd) * Rf) * Re / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf), -2 * (Ra * Rd - Rc * Rf) * Re / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf), -2 * ((Ra + Rb) * Rd + (Rb + Rd) * Rf) * Re / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf), -2 * (Ra * Rb + (Ra + Rb) * Rc + Rc * Rf) * Re / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf), -2 * (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd + (Rb + Rc + Rd) * Rf) * Re / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf) + 1, -2 * (Ra * Rb + (Ra + Rb) * Rc + Ra * Rd) * Re / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf) },
                                { -2 * (Rc * Rd + (Rb + Rc + Rd) * Re) * Rf / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf), -2 * ((Ra + Rc) * Rd + (Rc + Rd) * Re) * Rf / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf), -2 * (Ra * Rd - Rb * Re) * Rf / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf), -2 * (Ra * Rb + (Ra + Rb) * Rc + Rb * Re) * Rf / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf), -2 * (Ra * Rb + (Ra + Rb) * Rc + Ra * Rd) * Rf / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf), -2 * (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf) + 1 } });
        }
    };

    using RType = wdft::RootRtypeAdaptor<float, ImpedanceCalc, decltype (S4), decltype (P1), decltype (Resc), decltype (S3), decltype (S2), decltype (S1)>;
    RType R { S4, P1, Resc, S3, S2, S1 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BaxandallWDF)
};
