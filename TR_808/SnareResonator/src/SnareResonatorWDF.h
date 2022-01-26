#pragma once

#include <JuceHeader.h>

namespace wdft = chowdsp::WDFT;

/**
 * Resonator from TR-808 snare drum circuit.
 * Reference: http://www.ericarcher.net/wp-content/uploads/2014/07/tr808_snare_diy_project.pdf
 */
class SnareResonatorWDF
{
public:
    SnareResonatorWDF() = default;

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

    void setParameters (float Rfb, float Rg, float Cval)
    {
        R197.setResistanceValue (Rfb);
        R196.setResistanceValue (Rg);
        C58.setCapacitanceValue (Cval);
        C59.setCapacitanceValue (Cval);
    }

    inline float processSample (float x)
    {
        Vin.setVoltage (x);

        Vin.incident (R.reflected());
        R.incident (Vin.reflected());

        return wdft::voltage<float> (R196) + wdft::voltage<float> (C59);
    }

private:
    // Port B
    wdft::ResistorT<float> R197 { 820.0e3f };

    // Port C
    wdft::CapacitorT<float> C58 { 0.027e-6f, 48000.0f };

    // Port D
    wdft::CapacitorT<float> C59 { 0.027e-6f, 48000.0f };

    // Port E
    wdft::ResistorT<float> R196 { 680.0f };

    struct ImpedanceCalc
    {
        template <typename RType>
        static float calcImpedance (RType& R)
        {
            constexpr float Ag = 100.0f; // op-amp gain
            constexpr float Ri = 1.0e9f; // op-amp input impedance
            constexpr float Ro = 1.0e-1f; // op-amp output impedance
            const auto [Rb, Rc, Rd, Re] = R.getPortImpedances();

            // This scattering matrix was derived using the R-Solver python script (https://github.com/jatinchowdhury18/R-Solver),
            // invoked with command: r_solver.py --adapt 0 --out scratch/snare_res_scatt.txt scratch/snare_res2.txt
            R.setSMatrixData ({ { 0, -(Rc * Rd + (Rc + Rd) * Re - Rc * Ro) / ((Rb + Rc) * Rd + (Rb + Rc + Rd) * Re - (Rb + Rc + Rd) * Ro), (Rb * Rd + Rb * Re - (Rb + Rd) * Ro) / ((Rb + Rc) * Rd + (Rb + Rc + Rd) * Re - (Rb + Rc + Rd) * Ro), (Rb * Re + Rc * Ro) / ((Rb + Rc) * Rd + (Rb + Rc + Rd) * Re - (Rb + Rc + Rd) * Ro), (Rb * Rd - (Rb + Rc + Rd) * Ro) / ((Rb + Rc) * Rd + (Rb + Rc + Rd) * Re - (Rb + Rc + Rd) * Ro) },
                                { (Ag * Rb * Rd * Ri - Rb * Rc * Rd + Rb * Rc * Ro - (Rb * Rc + Rb * Rd) * Re) / (Rb * Rc * Rd + (Rb * Rc + Rb * Rd) * Re + (((Ag + 1) * Rc + Rb) * Rd + ((Ag + 1) * Rb + (Ag + 1) * Rc + (Ag + 1) * Rd) * Re) * Ri - (Rb * Rc + Rc * Rd + (Rb + Rc + Rd) * Re + (Rb + Rc + Rd) * Ri) * Ro), -(Rb * Rb * Rc * Rd * Rd + (Rb * Rb * Rc + Rb * Rb * Rd) * Re * Re + (Rb * Rb * Rc - Rc * Rc * Rd - Rc * Rd * Rd + (Rb * Rb - Rc * Rc - 2 * Rc * Rd - Rd * Rd) * Re + (Rb * Rb - Rc * Rc - 2 * Rc * Rd - Rd * Rd) * Ri) * Ro * Ro + (2 * Rb * Rb * Rc * Rd + Rb * Rb * Rd * Rd) * Re - (((Ag + 1) * Rc * Rc - Rb * Rb) * Rd * Rd - ((Ag + 1) * Rb * Rb - (Ag + 1) * Rc * Rc - 2 * (Ag + 1) * Rc * Rd - (Ag + 1) * Rd * Rd) * Re * Re + (2 * (Ag + 1) * Rc * Rd * Rd - ((Ag + 2) * Rb * Rb - 2 * (Ag + 1) * Rc * Rc) * Rd) * Re) * Ri - (2 * Rb * Rb * Rc * Rd - Rc * Rc * Rd * Rd + (Rb * Rb - Rc * Rc - 2 * Rc * Rd - Rd * Rd) * Re * Re + 2 * (Rb * Rb * Rc - Rc * Rd * Rd + (Rb * Rb - Rc * Rc) * Rd) * Re - ((Ag + 2) * Rc * Rd * Rd + ((Ag + 2) * Rc * Rc - 2 * Rb * Rb) * Rd - ((Ag + 2) * Rb * Rb - (Ag + 2) * Rc * Rc - 2 * (Ag + 2) * Rc * Rd - (Ag + 2) * Rd * Rd) * Re) * Ri) * Ro) / ((Rb * Rb * Rc + Rb * Rc * Rc) * Rd * Rd + (Rb * Rb * Rc + Rb * Rc * Rc + Rb * Rd * Rd + (Rb * Rb + 2 * Rb * Rc) * Rd) * Re * Re + (Rb * Rb * Rc + Rb * Rc * Rc + Rc * Rd * Rd + (2 * Rb * Rc + Rc * Rc) * Rd + (Rb * Rb + 2 * Rb * Rc + Rc * Rc + 2 * (Rb + Rc) * Rd + Rd * Rd) * Re + (Rb * Rb + 2 * Rb * Rc + Rc * Rc + 2 * (Rb + Rc) * Rd + Rd * Rd) * Ri) * Ro * Ro + ((Rb * Rb + 2 * Rb * Rc) * Rd * Rd + 2 * (Rb * Rb * Rc + Rb * Rc * Rc) * Rd) * Re + (((Ag + 2) * Rb * Rc + (Ag + 1) * Rc * Rc + Rb * Rb) * Rd * Rd + ((Ag + 1) * Rb * Rb + 2 * (Ag + 1) * Rb * Rc + (Ag + 1) * Rc * Rc + (Ag + 1) * Rd * Rd + 2 * ((Ag + 1) * Rb + (Ag + 1) * Rc) * Rd) * Re * Re + (((Ag + 2) * Rb + 2 * (Ag + 1) * Rc) * Rd * Rd + ((Ag + 2) * Rb * Rb + (3 * Ag + 4) * Rb * Rc + 2 * (Ag + 1) * Rc * Rc) * Rd) * Re) * Ri - ((2 * Rb * Rc + Rc * Rc) * Rd * Rd + (Rb * Rb + 2 * Rb * Rc + Rc * Rc + 2 * (Rb + Rc) * Rd + Rd * Rd) * Re * Re + 2 * (Rb * Rb * Rc + Rb * Rc * Rc) * Rd + 2 * (Rb * Rb * Rc + Rb * Rc * Rc + (Rb + Rc) * Rd * Rd + (Rb * Rb + 3 * Rb * Rc + Rc * Rc) * Rd) * Re + (((Ag + 2) * Rc + 2 * Rb) * Rd * Rd + ((Ag + 4) * Rb * Rc + (Ag + 2) * Rc * Rc + 2 * Rb * Rb) * Rd + ((Ag + 2) * Rb * Rb + 2 * (Ag + 2) * Rb * Rc + (Ag + 2) * Rc * Rc + (Ag + 2) * Rd * Rd + 2 * ((Ag + 2) * Rb + (Ag + 2) * Rc) * Rd) * Re) * Ri) * Ro), -(Rb * Rb * Rc * Rd * Rd + (Rb * Rb * Rc + Rb * Rb * Rd) * Re * Re + (Rb * Rb * Rc + Rb * Rc * Rd + 2 * (Rb * Rb + Rb * Rc + Rb * Rd) * Re + 2 * (Rb * Rb + Rb * Rc + Rb * Rd) * Ri) * Ro * Ro + (2 * Rb * Rb * Rc * Rd + Rb * Rb * Rd * Rd) * Re + (((Ag + 2) * Rb * Rb + 2 * (Ag + 1) * Rb * Rc) * Rd * Rd + 2 * ((Ag + 1) * Rb * Rb + (Ag + 1) * Rb * Rc + (Ag + 1) * Rb * Rd) * Re * Re + (2 * (Ag + 1) * Rb * Rd * Rd + ((3 * Ag + 4) * Rb * Rb + 4 * (Ag + 1) * Rb * Rc) * Rd) * Re) * Ri - (2 * Rb * Rb * Rc * Rd + Rb * Rc * Rd * Rd + 2 * (Rb * Rb + Rb * Rc + Rb * Rd) * Re * Re + (2 * Rb * Rb * Rc + Rb * Rd * Rd + 3 * (Rb * Rb + Rb * Rc) * Rd) * Re + ((Ag + 2) * Rb * Rd * Rd + ((Ag + 4) * Rb * Rb + 2 * (Ag + 2) * Rb * Rc) * Rd + 2 * ((Ag + 2) * Rb * Rb + (Ag + 2) * Rb * Rc + (Ag + 2) * Rb * Rd) * Re) * Ri) * Ro) / ((Rb * Rb * Rc + Rb * Rc * Rc) * Rd * Rd + (Rb * Rb * Rc + Rb * Rc * Rc + Rb * Rd * Rd + (Rb * Rb + 2 * Rb * Rc) * Rd) * Re * Re + (Rb * Rb * Rc + Rb * Rc * Rc + Rc * Rd * Rd + (2 * Rb * Rc + Rc * Rc) * Rd + (Rb * Rb + 2 * Rb * Rc + Rc * Rc + 2 * (Rb + Rc) * Rd + Rd * Rd) * Re + (Rb * Rb + 2 * Rb * Rc + Rc * Rc + 2 * (Rb + Rc) * Rd + Rd * Rd) * Ri) * Ro * Ro + ((Rb * Rb + 2 * Rb * Rc) * Rd * Rd + 2 * (Rb * Rb * Rc + Rb * Rc * Rc) * Rd) * Re + (((Ag + 2) * Rb * Rc + (Ag + 1) * Rc * Rc + Rb * Rb) * Rd * Rd + ((Ag + 1) * Rb * Rb + 2 * (Ag + 1) * Rb * Rc + (Ag + 1) * Rc * Rc + (Ag + 1) * Rd * Rd + 2 * ((Ag + 1) * Rb + (Ag + 1) * Rc) * Rd) * Re * Re + (((Ag + 2) * Rb + 2 * (Ag + 1) * Rc) * Rd * Rd + ((Ag + 2) * Rb * Rb + (3 * Ag + 4) * Rb * Rc + 2 * (Ag + 1) * Rc * Rc) * Rd) * Re) * Ri - ((2 * Rb * Rc + Rc * Rc) * Rd * Rd + (Rb * Rb + 2 * Rb * Rc + Rc * Rc + 2 * (Rb + Rc) * Rd + Rd * Rd) * Re * Re + 2 * (Rb * Rb * Rc + Rb * Rc * Rc) * Rd + 2 * (Rb * Rb * Rc + Rb * Rc * Rc + (Rb + Rc) * Rd * Rd + (Rb * Rb + 3 * Rb * Rc + Rc * Rc) * Rd) * Re + (((Ag + 2) * Rc + 2 * Rb) * Rd * Rd + ((Ag + 4) * Rb * Rc + (Ag + 2) * Rc * Rc + 2 * Rb * Rb) * Rd + ((Ag + 2) * Rb * Rb + 2 * (Ag + 2) * Rb * Rc + (Ag + 2) * Rc * Rc + (Ag + 2) * Rd * Rd + 2 * ((Ag + 2) * Rb + (Ag + 2) * Rc) * Rd) * Re) * Ri) * Ro), -(Rb * Rb * Rc * Rd * Re + (Rb * Rb * Rc + Rb * Rb * Rd) * Re * Re + (2 * Rb * Rb * Rc + Rb * Rc * Rc + 2 * Rb * Rc * Rd + 2 * (Rb * Rb + Rb * Rc + Rb * Rd) * Re + 2 * (Rb * Rb + Rb * Rc + Rb * Rd) * Ri) * Ro * Ro + (((Ag + 2) * Rb * Rb + 2 * (Ag + 1) * Rb * Rc) * Rd * Re + 2 * ((Ag + 1) * Rb * Rb + (Ag + 1) * Rb * Rc + (Ag + 1) * Rb * Rd) * Re * Re) * Ri - (2 * (Rb * Rb + Rb * Rc + Rb * Rd) * Re * Re + (2 * Rb * Rb * Rc + Rb * Rc * Rc) * Rd + (3 * Rb * Rb * Rc + Rb * Rc * Rc + (2 * Rb * Rb + 3 * Rb * Rc) * Rd) * Re + (((Ag + 2) * Rb * Rc + 2 * Rb * Rb) * Rd + 2 * ((Ag + 2) * Rb * Rb + (Ag + 2) * Rb * Rc + (Ag + 2) * Rb * Rd) * Re) * Ri) * Ro) / ((Rb * Rb * Rc + Rb * Rc * Rc) * Rd * Rd + (Rb * Rb * Rc + Rb * Rc * Rc + Rb * Rd * Rd + (Rb * Rb + 2 * Rb * Rc) * Rd) * Re * Re + (Rb * Rb * Rc + Rb * Rc * Rc + Rc * Rd * Rd + (2 * Rb * Rc + Rc * Rc) * Rd + (Rb * Rb + 2 * Rb * Rc + Rc * Rc + 2 * (Rb + Rc) * Rd + Rd * Rd) * Re + (Rb * Rb + 2 * Rb * Rc + Rc * Rc + 2 * (Rb + Rc) * Rd + Rd * Rd) * Ri) * Ro * Ro + ((Rb * Rb + 2 * Rb * Rc) * Rd * Rd + 2 * (Rb * Rb * Rc + Rb * Rc * Rc) * Rd) * Re + (((Ag + 2) * Rb * Rc + (Ag + 1) * Rc * Rc + Rb * Rb) * Rd * Rd + ((Ag + 1) * Rb * Rb + 2 * (Ag + 1) * Rb * Rc + (Ag + 1) * Rc * Rc + (Ag + 1) * Rd * Rd + 2 * ((Ag + 1) * Rb + (Ag + 1) * Rc) * Rd) * Re * Re + (((Ag + 2) * Rb + 2 * (Ag + 1) * Rc) * Rd * Rd + ((Ag + 2) * Rb * Rb + (3 * Ag + 4) * Rb * Rc + 2 * (Ag + 1) * Rc * Rc) * Rd) * Re) * Ri - ((2 * Rb * Rc + Rc * Rc) * Rd * Rd + (Rb * Rb + 2 * Rb * Rc + Rc * Rc + 2 * (Rb + Rc) * Rd + Rd * Rd) * Re * Re + 2 * (Rb * Rb * Rc + Rb * Rc * Rc) * Rd + 2 * (Rb * Rb * Rc + Rb * Rc * Rc + (Rb + Rc) * Rd * Rd + (Rb * Rb + 3 * Rb * Rc + Rc * Rc) * Rd) * Re + (((Ag + 2) * Rc + 2 * Rb) * Rd * Rd + ((Ag + 4) * Rb * Rc + (Ag + 2) * Rc * Rc + 2 * Rb * Rb) * Rd + ((Ag + 2) * Rb * Rb + 2 * (Ag + 2) * Rb * Rc + (Ag + 2) * Rc * Rc + (Ag + 2) * Rd * Rd + 2 * ((Ag + 2) * Rb + (Ag + 2) * Rc) * Rd) * Re) * Ri) * Ro), -(Rb * Rb * Rc * Rd * Rd - (Rb * Rb * Rc + Rb * Rc * Rc + Rb * Rc * Rd) * Ro * Ro + (Rb * Rb * Rc * Rd + Rb * Rb * Rd * Rd) * Re + (((Ag + 2) * Rb * Rb + 2 * (Ag + 1) * Rb * Rc) * Rd * Rd + 2 * ((Ag + 1) * Rb * Rd * Rd + ((Ag + 1) * Rb * Rb + (Ag + 1) * Rb * Rc) * Rd) * Re) * Ri + (Rb * Rc * Rc * Rd - Rb * Rc * Rd * Rd + (Rb * Rb * Rc + Rb * Rc * Rc - Rb * Rb * Rd - Rb * Rd * Rd) * Re - ((Ag + 2) * Rb * Rd * Rd + ((Ag + 2) * Rb * Rb + (Ag + 2) * Rb * Rc) * Rd) * Ri) * Ro) / ((Rb * Rb * Rc + Rb * Rc * Rc) * Rd * Rd + (Rb * Rb * Rc + Rb * Rc * Rc + Rb * Rd * Rd + (Rb * Rb + 2 * Rb * Rc) * Rd) * Re * Re + (Rb * Rb * Rc + Rb * Rc * Rc + Rc * Rd * Rd + (2 * Rb * Rc + Rc * Rc) * Rd + (Rb * Rb + 2 * Rb * Rc + Rc * Rc + 2 * (Rb + Rc) * Rd + Rd * Rd) * Re + (Rb * Rb + 2 * Rb * Rc + Rc * Rc + 2 * (Rb + Rc) * Rd + Rd * Rd) * Ri) * Ro * Ro + ((Rb * Rb + 2 * Rb * Rc) * Rd * Rd + 2 * (Rb * Rb * Rc + Rb * Rc * Rc) * Rd) * Re + (((Ag + 2) * Rb * Rc + (Ag + 1) * Rc * Rc + Rb * Rb) * Rd * Rd + ((Ag + 1) * Rb * Rb + 2 * (Ag + 1) * Rb * Rc + (Ag + 1) * Rc * Rc + (Ag + 1) * Rd * Rd + 2 * ((Ag + 1) * Rb + (Ag + 1) * Rc) * Rd) * Re * Re + (((Ag + 2) * Rb + 2 * (Ag + 1) * Rc) * Rd * Rd + ((Ag + 2) * Rb * Rb + (3 * Ag + 4) * Rb * Rc + 2 * (Ag + 1) * Rc * Rc) * Rd) * Re) * Ri - ((2 * Rb * Rc + Rc * Rc) * Rd * Rd + (Rb * Rb + 2 * Rb * Rc + Rc * Rc + 2 * (Rb + Rc) * Rd + Rd * Rd) * Re * Re + 2 * (Rb * Rb * Rc + Rb * Rc * Rc) * Rd + 2 * (Rb * Rb * Rc + Rb * Rc * Rc + (Rb + Rc) * Rd * Rd + (Rb * Rb + 3 * Rb * Rc + Rc * Rc) * Rd) * Re + (((Ag + 2) * Rc + 2 * Rb) * Rd * Rd + ((Ag + 4) * Rb * Rc + (Ag + 2) * Rc * Rc + 2 * Rb * Rb) * Rd + ((Ag + 2) * Rb * Rb + 2 * (Ag + 2) * Rb * Rc + (Ag + 2) * Rc * Rc + (Ag + 2) * Rd * Rd + 2 * ((Ag + 2) * Rb + (Ag + 2) * Rc) * Rd) * Re) * Ri) * Ro) },
                                { (Ag * Rc * Rd * Ri + Rb * Rc * Rd + Rb * Rc * Re - (Rb * Rc + Rc * Rd) * Ro) / (Rb * Rc * Rd + (Rb * Rc + Rb * Rd) * Re + (((Ag + 1) * Rc + Rb) * Rd + ((Ag + 1) * Rb + (Ag + 1) * Rc + (Ag + 1) * Rd) * Re) * Ri - (Rb * Rc + Rc * Rd + (Rb + Rc + Rd) * Re + (Rb + Rc + Rd) * Ri) * Ro), -(Rb * Rc * Rc * Rd * Rd + (Rb * Rc * Rc + Rb * Rc * Rd) * Re * Re + (Rb * Rc * Rc + Rc * Rc * Rd + 2 * (Rb * Rc + Rc * Rc + Rc * Rd) * Re + 2 * (Rb * Rc + Rc * Rc + Rc * Rd) * Ri) * Ro * Ro + (2 * Rb * Rc * Rc * Rd + Rb * Rc * Rd * Rd) * Re + (((Ag + 2) * Rc * Rc + 2 * Rb * Rc) * Rd * Rd + 2 * ((Ag + 1) * Rb * Rc + (Ag + 1) * Rc * Rc + (Ag + 1) * Rc * Rd) * Re * Re + ((Ag + 2) * Rc * Rd * Rd + (2 * (Ag + 2) * Rb * Rc + (3 * Ag + 4) * Rc * Rc) * Rd) * Re) * Ri - (2 * Rb * Rc * Rc * Rd + Rc * Rc * Rd * Rd + 2 * (Rb * Rc + Rc * Rc + Rc * Rd) * Re * Re + (2 * Rb * Rc * Rc + Rc * Rd * Rd + 3 * (Rb * Rc + Rc * Rc) * Rd) * Re + (2 * Rc * Rd * Rd + ((Ag + 4) * Rc * Rc + 4 * Rb * Rc) * Rd + 2 * ((Ag + 2) * Rb * Rc + (Ag + 2) * Rc * Rc + (Ag + 2) * Rc * Rd) * Re) * Ri) * Ro) / ((Rb * Rb * Rc + Rb * Rc * Rc) * Rd * Rd + (Rb * Rb * Rc + Rb * Rc * Rc + Rb * Rd * Rd + (Rb * Rb + 2 * Rb * Rc) * Rd) * Re * Re + (Rb * Rb * Rc + Rb * Rc * Rc + Rc * Rd * Rd + (2 * Rb * Rc + Rc * Rc) * Rd + (Rb * Rb + 2 * Rb * Rc + Rc * Rc + 2 * (Rb + Rc) * Rd + Rd * Rd) * Re + (Rb * Rb + 2 * Rb * Rc + Rc * Rc + 2 * (Rb + Rc) * Rd + Rd * Rd) * Ri) * Ro * Ro + ((Rb * Rb + 2 * Rb * Rc) * Rd * Rd + 2 * (Rb * Rb * Rc + Rb * Rc * Rc) * Rd) * Re + (((Ag + 2) * Rb * Rc + (Ag + 1) * Rc * Rc + Rb * Rb) * Rd * Rd + ((Ag + 1) * Rb * Rb + 2 * (Ag + 1) * Rb * Rc + (Ag + 1) * Rc * Rc + (Ag + 1) * Rd * Rd + 2 * ((Ag + 1) * Rb + (Ag + 1) * Rc) * Rd) * Re * Re + (((Ag + 2) * Rb + 2 * (Ag + 1) * Rc) * Rd * Rd + ((Ag + 2) * Rb * Rb + (3 * Ag + 4) * Rb * Rc + 2 * (Ag + 1) * Rc * Rc) * Rd) * Re) * Ri - ((2 * Rb * Rc + Rc * Rc) * Rd * Rd + (Rb * Rb + 2 * Rb * Rc + Rc * Rc + 2 * (Rb + Rc) * Rd + Rd * Rd) * Re * Re + 2 * (Rb * Rb * Rc + Rb * Rc * Rc) * Rd + 2 * (Rb * Rb * Rc + Rb * Rc * Rc + (Rb + Rc) * Rd * Rd + (Rb * Rb + 3 * Rb * Rc + Rc * Rc) * Rd) * Re + (((Ag + 2) * Rc + 2 * Rb) * Rd * Rd + ((Ag + 4) * Rb * Rc + (Ag + 2) * Rc * Rc + 2 * Rb * Rb) * Rd + ((Ag + 2) * Rb * Rb + 2 * (Ag + 2) * Rb * Rc + (Ag + 2) * Rc * Rc + (Ag + 2) * Rd * Rd + 2 * ((Ag + 2) * Rb + (Ag + 2) * Rc) * Rd) * Re) * Ri) * Ro), -(Rb * Rc * Rc * Rd * Rd + (Rb * Rc * Rc - Rb * Rb * Rd - Rb * Rd * Rd) * Re * Re + (Rb * Rc * Rc + Rc * Rc * Rd - (Rb * Rb - Rc * Rc + 2 * Rb * Rd + Rd * Rd) * Re - (Rb * Rb - Rc * Rc + 2 * Rb * Rd + Rd * Rd) * Ri) * Ro * Ro + (2 * Rb * Rc * Rc * Rd - Rb * Rb * Rd * Rd) * Re + (((Ag + 1) * Rc * Rc - Rb * Rb) * Rd * Rd - ((Ag + 1) * Rb * Rb - (Ag + 1) * Rc * Rc + 2 * (Ag + 1) * Rb * Rd + (Ag + 1) * Rd * Rd) * Re * Re - ((Ag + 2) * Rb * Rd * Rd + ((Ag + 2) * Rb * Rb - 2 * (Ag + 1) * Rc * Rc) * Rd) * Re) * Ri - (2 * Rb * Rc * Rc * Rd + Rc * Rc * Rd * Rd - (Rb * Rb - Rc * Rc + 2 * Rb * Rd + Rd * Rd) * Re * Re + 2 * (Rb * Rc * Rc - Rb * Rd * Rd - (Rb * Rb - Rc * Rc) * Rd) * Re - (2 * Rb * Rd * Rd - ((Ag + 2) * Rc * Rc - 2 * Rb * Rb) * Rd + ((Ag + 2) * Rb * Rb - (Ag + 2) * Rc * Rc + 2 * (Ag + 2) * Rb * Rd + (Ag + 2) * Rd * Rd) * Re) * Ri) * Ro) / ((Rb * Rb * Rc + Rb * Rc * Rc) * Rd * Rd + (Rb * Rb * Rc + Rb * Rc * Rc + Rb * Rd * Rd + (Rb * Rb + 2 * Rb * Rc) * Rd) * Re * Re + (Rb * Rb * Rc + Rb * Rc * Rc + Rc * Rd * Rd + (2 * Rb * Rc + Rc * Rc) * Rd + (Rb * Rb + 2 * Rb * Rc + Rc * Rc + 2 * (Rb + Rc) * Rd + Rd * Rd) * Re + (Rb * Rb + 2 * Rb * Rc + Rc * Rc + 2 * (Rb + Rc) * Rd + Rd * Rd) * Ri) * Ro * Ro + ((Rb * Rb + 2 * Rb * Rc) * Rd * Rd + 2 * (Rb * Rb * Rc + Rb * Rc * Rc) * Rd) * Re + (((Ag + 2) * Rb * Rc + (Ag + 1) * Rc * Rc + Rb * Rb) * Rd * Rd + ((Ag + 1) * Rb * Rb + 2 * (Ag + 1) * Rb * Rc + (Ag + 1) * Rc * Rc + (Ag + 1) * Rd * Rd + 2 * ((Ag + 1) * Rb + (Ag + 1) * Rc) * Rd) * Re * Re + (((Ag + 2) * Rb + 2 * (Ag + 1) * Rc) * Rd * Rd + ((Ag + 2) * Rb * Rb + (3 * Ag + 4) * Rb * Rc + 2 * (Ag + 1) * Rc * Rc) * Rd) * Re) * Ri - ((2 * Rb * Rc + Rc * Rc) * Rd * Rd + (Rb * Rb + 2 * Rb * Rc + Rc * Rc + 2 * (Rb + Rc) * Rd + Rd * Rd) * Re * Re + 2 * (Rb * Rb * Rc + Rb * Rc * Rc) * Rd + 2 * (Rb * Rb * Rc + Rb * Rc * Rc + (Rb + Rc) * Rd * Rd + (Rb * Rb + 3 * Rb * Rc + Rc * Rc) * Rd) * Re + (((Ag + 2) * Rc + 2 * Rb) * Rd * Rd + ((Ag + 4) * Rb * Rc + (Ag + 2) * Rc * Rc + 2 * Rb * Rb) * Rd + ((Ag + 2) * Rb * Rb + 2 * (Ag + 2) * Rb * Rc + (Ag + 2) * Rc * Rc + (Ag + 2) * Rd * Rd + 2 * ((Ag + 2) * Rb + (Ag + 2) * Rc) * Rd) * Re) * Ri) * Ro), -((Rb * Rb * Rc + 2 * Rb * Rc * Rc) * Rd * Re + (Rb * Rb * Rc + 2 * Rb * Rc * Rc + 2 * Rb * Rc * Rd) * Re * Re + (Rb * Rc * Rc + Rc * Rc * Rd + 2 * (Rb * Rc + Rc * Rc + Rc * Rd) * Re + 2 * (Rb * Rc + Rc * Rc + Rc * Rd) * Ri) * Ro * Ro + (((Ag + 2) * Rb * Rc + 2 * (Ag + 1) * Rc * Rc) * Rd * Re + 2 * ((Ag + 1) * Rb * Rc + (Ag + 1) * Rc * Rc + (Ag + 1) * Rc * Rd) * Re * Re) * Ri - (Rb * Rc * Rc * Rd + 2 * (Rb * Rc + Rc * Rc + Rc * Rd) * Re * Re + (Rb * Rb * Rc + 3 * Rb * Rc * Rc + (3 * Rb * Rc + 2 * Rc * Rc) * Rd) * Re + (((Ag + 2) * Rc * Rc + 2 * Rb * Rc) * Rd + 2 * ((Ag + 2) * Rb * Rc + (Ag + 2) * Rc * Rc + (Ag + 2) * Rc * Rd) * Re) * Ri) * Ro) / ((Rb * Rb * Rc + Rb * Rc * Rc) * Rd * Rd + (Rb * Rb * Rc + Rb * Rc * Rc + Rb * Rd * Rd + (Rb * Rb + 2 * Rb * Rc) * Rd) * Re * Re + (Rb * Rb * Rc + Rb * Rc * Rc + Rc * Rd * Rd + (2 * Rb * Rc + Rc * Rc) * Rd + (Rb * Rb + 2 * Rb * Rc + Rc * Rc + 2 * (Rb + Rc) * Rd + Rd * Rd) * Re + (Rb * Rb + 2 * Rb * Rc + Rc * Rc + 2 * (Rb + Rc) * Rd + Rd * Rd) * Ri) * Ro * Ro + ((Rb * Rb + 2 * Rb * Rc) * Rd * Rd + 2 * (Rb * Rb * Rc + Rb * Rc * Rc) * Rd) * Re + (((Ag + 2) * Rb * Rc + (Ag + 1) * Rc * Rc + Rb * Rb) * Rd * Rd + ((Ag + 1) * Rb * Rb + 2 * (Ag + 1) * Rb * Rc + (Ag + 1) * Rc * Rc + (Ag + 1) * Rd * Rd + 2 * ((Ag + 1) * Rb + (Ag + 1) * Rc) * Rd) * Re * Re + (((Ag + 2) * Rb + 2 * (Ag + 1) * Rc) * Rd * Rd + ((Ag + 2) * Rb * Rb + (3 * Ag + 4) * Rb * Rc + 2 * (Ag + 1) * Rc * Rc) * Rd) * Re) * Ri - ((2 * Rb * Rc + Rc * Rc) * Rd * Rd + (Rb * Rb + 2 * Rb * Rc + Rc * Rc + 2 * (Rb + Rc) * Rd + Rd * Rd) * Re * Re + 2 * (Rb * Rb * Rc + Rb * Rc * Rc) * Rd + 2 * (Rb * Rb * Rc + Rb * Rc * Rc + (Rb + Rc) * Rd * Rd + (Rb * Rb + 3 * Rb * Rc + Rc * Rc) * Rd) * Re + (((Ag + 2) * Rc + 2 * Rb) * Rd * Rd + ((Ag + 4) * Rb * Rc + (Ag + 2) * Rc * Rc + 2 * Rb * Rb) * Rd + ((Ag + 2) * Rb * Rb + 2 * (Ag + 2) * Rb * Rc + (Ag + 2) * Rc * Rc + (Ag + 2) * Rd * Rd + 2 * ((Ag + 2) * Rb + (Ag + 2) * Rc) * Rd) * Re) * Ri) * Ro), -((Rb * Rb * Rc + 2 * Rb * Rc * Rc) * Rd * Rd + (Rb * Rb * Rc + Rb * Rc * Rc + Rc * Rd * Rd + (2 * Rb * Rc + Rc * Rc) * Rd) * Ro * Ro + (2 * Rb * Rc * Rd * Rd + (Rb * Rb * Rc + 2 * Rb * Rc * Rc) * Rd) * Re + (((Ag + 2) * Rb * Rc + 2 * (Ag + 1) * Rc * Rc) * Rd * Rd + 2 * ((Ag + 1) * Rc * Rd * Rd + ((Ag + 1) * Rb * Rc + (Ag + 1) * Rc * Rc) * Rd) * Re) * Ri - (2 * (Rb * Rc + Rc * Rc) * Rd * Rd + (2 * Rb * Rb * Rc + 3 * Rb * Rc * Rc) * Rd + (Rb * Rb * Rc + Rb * Rc * Rc + 2 * Rc * Rd * Rd + (3 * Rb * Rc + 2 * Rc * Rc) * Rd) * Re + ((Ag + 2) * Rc * Rd * Rd + ((Ag + 2) * Rb * Rc + (Ag + 2) * Rc * Rc) * Rd) * Ri) * Ro) / ((Rb * Rb * Rc + Rb * Rc * Rc) * Rd * Rd + (Rb * Rb * Rc + Rb * Rc * Rc + Rb * Rd * Rd + (Rb * Rb + 2 * Rb * Rc) * Rd) * Re * Re + (Rb * Rb * Rc + Rb * Rc * Rc + Rc * Rd * Rd + (2 * Rb * Rc + Rc * Rc) * Rd + (Rb * Rb + 2 * Rb * Rc + Rc * Rc + 2 * (Rb + Rc) * Rd + Rd * Rd) * Re + (Rb * Rb + 2 * Rb * Rc + Rc * Rc + 2 * (Rb + Rc) * Rd + Rd * Rd) * Ri) * Ro * Ro + ((Rb * Rb + 2 * Rb * Rc) * Rd * Rd + 2 * (Rb * Rb * Rc + Rb * Rc * Rc) * Rd) * Re + (((Ag + 2) * Rb * Rc + (Ag + 1) * Rc * Rc + Rb * Rb) * Rd * Rd + ((Ag + 1) * Rb * Rb + 2 * (Ag + 1) * Rb * Rc + (Ag + 1) * Rc * Rc + (Ag + 1) * Rd * Rd + 2 * ((Ag + 1) * Rb + (Ag + 1) * Rc) * Rd) * Re * Re + (((Ag + 2) * Rb + 2 * (Ag + 1) * Rc) * Rd * Rd + ((Ag + 2) * Rb * Rb + (3 * Ag + 4) * Rb * Rc + 2 * (Ag + 1) * Rc * Rc) * Rd) * Re) * Ri - ((2 * Rb * Rc + Rc * Rc) * Rd * Rd + (Rb * Rb + 2 * Rb * Rc + Rc * Rc + 2 * (Rb + Rc) * Rd + Rd * Rd) * Re * Re + 2 * (Rb * Rb * Rc + Rb * Rc * Rc) * Rd + 2 * (Rb * Rb * Rc + Rb * Rc * Rc + (Rb + Rc) * Rd * Rd + (Rb * Rb + 3 * Rb * Rc + Rc * Rc) * Rd) * Re + (((Ag + 2) * Rc + 2 * Rb) * Rd * Rd + ((Ag + 4) * Rb * Rc + (Ag + 2) * Rc * Rc + 2 * Rb * Rb) * Rd + ((Ag + 2) * Rb * Rb + 2 * (Ag + 2) * Rb * Rc + (Ag + 2) * Rc * Rc + (Ag + 2) * Rd * Rd + 2 * ((Ag + 2) * Rb + (Ag + 2) * Rc) * Rd) * Re) * Ri) * Ro) },
                                { (Rb * Rd * Re - (Ag * Rb + Ag * Rc) * Rd * Ri + Rc * Rd * Ro) / (Rb * Rc * Rd + (Rb * Rc + Rb * Rd) * Re + (((Ag + 1) * Rc + Rb) * Rd + ((Ag + 1) * Rb + (Ag + 1) * Rc + (Ag + 1) * Rd) * Re) * Ri - (Rb * Rc + Rc * Rd + (Rb + Rc + Rd) * Re + (Rb + Rc + Rd) * Ri) * Ro), -(Rb * Rc * Rd * Rd * Re + (Rb * Rc * Rd + Rb * Rd * Rd) * Re * Re + (2 * Rc * Rd * Rd + (2 * Rb * Rc + Rc * Rc) * Rd + 2 * ((Rb + Rc) * Rd + Rd * Rd) * Re + 2 * ((Rb + Rc) * Rd + Rd * Rd) * Ri) * Ro * Ro + ((Ag * Rb * Rc + Ag * Rc * Rc) * Rd * Rd + 2 * ((Ag + 1) * Rd * Rd + ((Ag + 1) * Rb + (Ag + 1) * Rc) * Rd) * Re * Re + (((Ag + 2) * Rb + (3 * Ag + 2) * Rc) * Rd * Rd + (Ag * Rb * Rc + Ag * Rc * Rc) * Rd) * Re) * Ri - ((2 * Rb * Rc + Rc * Rc) * Rd * Rd + 2 * ((Rb + Rc) * Rd + Rd * Rd) * Re * Re + ((2 * Rb + 3 * Rc) * Rd * Rd + (3 * Rb * Rc + Rc * Rc) * Rd) * Re + (2 * ((Ag + 1) * Rc + Rb) * Rd * Rd + (Ag * Rb * Rc + Ag * Rc * Rc) * Rd + 2 * ((Ag + 2) * Rd * Rd + ((Ag + 2) * Rb + (Ag + 2) * Rc) * Rd) * Re) * Ri) * Ro) / ((Rb * Rb * Rc + Rb * Rc * Rc) * Rd * Rd + (Rb * Rb * Rc + Rb * Rc * Rc + Rb * Rd * Rd + (Rb * Rb + 2 * Rb * Rc) * Rd) * Re * Re + (Rb * Rb * Rc + Rb * Rc * Rc + Rc * Rd * Rd + (2 * Rb * Rc + Rc * Rc) * Rd + (Rb * Rb + 2 * Rb * Rc + Rc * Rc + 2 * (Rb + Rc) * Rd + Rd * Rd) * Re + (Rb * Rb + 2 * Rb * Rc + Rc * Rc + 2 * (Rb + Rc) * Rd + Rd * Rd) * Ri) * Ro * Ro + ((Rb * Rb + 2 * Rb * Rc) * Rd * Rd + 2 * (Rb * Rb * Rc + Rb * Rc * Rc) * Rd) * Re + (((Ag + 2) * Rb * Rc + (Ag + 1) * Rc * Rc + Rb * Rb) * Rd * Rd + ((Ag + 1) * Rb * Rb + 2 * (Ag + 1) * Rb * Rc + (Ag + 1) * Rc * Rc + (Ag + 1) * Rd * Rd + 2 * ((Ag + 1) * Rb + (Ag + 1) * Rc) * Rd) * Re * Re + (((Ag + 2) * Rb + 2 * (Ag + 1) * Rc) * Rd * Rd + ((Ag + 2) * Rb * Rb + (3 * Ag + 4) * Rb * Rc + 2 * (Ag + 1) * Rc * Rc) * Rd) * Re) * Ri - ((2 * Rb * Rc + Rc * Rc) * Rd * Rd + (Rb * Rb + 2 * Rb * Rc + Rc * Rc + 2 * (Rb + Rc) * Rd + Rd * Rd) * Re * Re + 2 * (Rb * Rb * Rc + Rb * Rc * Rc) * Rd + 2 * (Rb * Rb * Rc + Rb * Rc * Rc + (Rb + Rc) * Rd * Rd + (Rb * Rb + 3 * Rb * Rc + Rc * Rc) * Rd) * Re + (((Ag + 2) * Rc + 2 * Rb) * Rd * Rd + ((Ag + 4) * Rb * Rc + (Ag + 2) * Rc * Rc + 2 * Rb * Rb) * Rd + ((Ag + 2) * Rb * Rb + 2 * (Ag + 2) * Rb * Rc + (Ag + 2) * Rc * Rc + (Ag + 2) * Rd * Rd + 2 * ((Ag + 2) * Rb + (Ag + 2) * Rc) * Rd) * Re) * Ri) * Ro), -((Rb * Rb + 2 * Rb * Rc) * Rd * Rd * Re + (2 * Rb * Rd * Rd + (Rb * Rb + 2 * Rb * Rc) * Rd) * Re * Re + (Rb * Rc * Rd + Rc * Rd * Rd + 2 * ((Rb + Rc) * Rd + Rd * Rd) * Re + 2 * ((Rb + Rc) * Rd + Rd * Rd) * Ri) * Ro * Ro - ((Ag * Rb * Rb + Ag * Rb * Rc) * Rd * Rd - 2 * ((Ag + 1) * Rd * Rd + ((Ag + 1) * Rb + (Ag + 1) * Rc) * Rd) * Re * Re - (2 * ((Ag + 1) * Rc + Rb) * Rd * Rd - (Ag * Rb * Rb + Ag * Rb * Rc) * Rd) * Re) * Ri - (Rb * Rc * Rd * Rd + 2 * ((Rb + Rc) * Rd + Rd * Rd) * Re * Re + ((3 * Rb + 2 * Rc) * Rd * Rd + (Rb * Rb + 3 * Rb * Rc) * Rd) * Re - (((Ag - 2) * Rb - (Ag + 2) * Rc) * Rd * Rd + (Ag * Rb * Rb + Ag * Rb * Rc) * Rd - 2 * ((Ag + 2) * Rd * Rd + ((Ag + 2) * Rb + (Ag + 2) * Rc) * Rd) * Re) * Ri) * Ro) / ((Rb * Rb * Rc + Rb * Rc * Rc) * Rd * Rd + (Rb * Rb * Rc + Rb * Rc * Rc + Rb * Rd * Rd + (Rb * Rb + 2 * Rb * Rc) * Rd) * Re * Re + (Rb * Rb * Rc + Rb * Rc * Rc + Rc * Rd * Rd + (2 * Rb * Rc + Rc * Rc) * Rd + (Rb * Rb + 2 * Rb * Rc + Rc * Rc + 2 * (Rb + Rc) * Rd + Rd * Rd) * Re + (Rb * Rb + 2 * Rb * Rc + Rc * Rc + 2 * (Rb + Rc) * Rd + Rd * Rd) * Ri) * Ro * Ro + ((Rb * Rb + 2 * Rb * Rc) * Rd * Rd + 2 * (Rb * Rb * Rc + Rb * Rc * Rc) * Rd) * Re + (((Ag + 2) * Rb * Rc + (Ag + 1) * Rc * Rc + Rb * Rb) * Rd * Rd + ((Ag + 1) * Rb * Rb + 2 * (Ag + 1) * Rb * Rc + (Ag + 1) * Rc * Rc + (Ag + 1) * Rd * Rd + 2 * ((Ag + 1) * Rb + (Ag + 1) * Rc) * Rd) * Re * Re + (((Ag + 2) * Rb + 2 * (Ag + 1) * Rc) * Rd * Rd + ((Ag + 2) * Rb * Rb + (3 * Ag + 4) * Rb * Rc + 2 * (Ag + 1) * Rc * Rc) * Rd) * Re) * Ri - ((2 * Rb * Rc + Rc * Rc) * Rd * Rd + (Rb * Rb + 2 * Rb * Rc + Rc * Rc + 2 * (Rb + Rc) * Rd + Rd * Rd) * Re * Re + 2 * (Rb * Rb * Rc + Rb * Rc * Rc) * Rd + 2 * (Rb * Rb * Rc + Rb * Rc * Rc + (Rb + Rc) * Rd * Rd + (Rb * Rb + 3 * Rb * Rc + Rc * Rc) * Rd) * Re + (((Ag + 2) * Rc + 2 * Rb) * Rd * Rd + ((Ag + 4) * Rb * Rc + (Ag + 2) * Rc * Rc + 2 * Rb * Rb) * Rd + ((Ag + 2) * Rb * Rb + 2 * (Ag + 2) * Rb * Rc + (Ag + 2) * Rc * Rc + (Ag + 2) * Rd * Rd + 2 * ((Ag + 2) * Rb + (Ag + 2) * Rc) * Rd) * Re) * Ri) * Ro), -((Rb * Rb + 2 * Rb * Rc) * Rd * Rd * Re + (Rb * Rb * Rc + Rb * Rc * Rc) * Rd * Rd - (Rb * Rb * Rc + Rb * Rc * Rc - Rb * Rd * Rd) * Re * Re - (Rb * Rb * Rc + Rb * Rc * Rc - Rc * Rd * Rd + (Rb * Rb + 2 * Rb * Rc + Rc * Rc - Rd * Rd) * Re + (Rb * Rb + 2 * Rb * Rc + Rc * Rc - Rd * Rd) * Ri) * Ro * Ro + (((Ag + 2) * Rb + 2 * (Ag + 1) * Rc) * Rd * Rd * Re + ((Ag + 2) * Rb * Rc + (Ag + 1) * Rc * Rc + Rb * Rb) * Rd * Rd - ((Ag + 1) * Rb * Rb + 2 * (Ag + 1) * Rb * Rc + (Ag + 1) * Rc * Rc - (Ag + 1) * Rd * Rd) * Re * Re) * Ri - ((2 * Rb * Rc + Rc * Rc) * Rd * Rd - (Rb * Rb + 2 * Rb * Rc + Rc * Rc - Rd * Rd) * Re * Re - 2 * (Rb * Rb * Rc + Rb * Rc * Rc - (Rb + Rc) * Rd * Rd) * Re + (((Ag + 2) * Rc + 2 * Rb) * Rd * Rd - ((Ag + 2) * Rb * Rb + 2 * (Ag + 2) * Rb * Rc + (Ag + 2) * Rc * Rc - (Ag + 2) * Rd * Rd) * Re) * Ri) * Ro) / ((Rb * Rb * Rc + Rb * Rc * Rc) * Rd * Rd + (Rb * Rb * Rc + Rb * Rc * Rc + Rb * Rd * Rd + (Rb * Rb + 2 * Rb * Rc) * Rd) * Re * Re + (Rb * Rb * Rc + Rb * Rc * Rc + Rc * Rd * Rd + (2 * Rb * Rc + Rc * Rc) * Rd + (Rb * Rb + 2 * Rb * Rc + Rc * Rc + 2 * (Rb + Rc) * Rd + Rd * Rd) * Re + (Rb * Rb + 2 * Rb * Rc + Rc * Rc + 2 * (Rb + Rc) * Rd + Rd * Rd) * Ri) * Ro * Ro + ((Rb * Rb + 2 * Rb * Rc) * Rd * Rd + 2 * (Rb * Rb * Rc + Rb * Rc * Rc) * Rd) * Re + (((Ag + 2) * Rb * Rc + (Ag + 1) * Rc * Rc + Rb * Rb) * Rd * Rd + ((Ag + 1) * Rb * Rb + 2 * (Ag + 1) * Rb * Rc + (Ag + 1) * Rc * Rc + (Ag + 1) * Rd * Rd + 2 * ((Ag + 1) * Rb + (Ag + 1) * Rc) * Rd) * Re * Re + (((Ag + 2) * Rb + 2 * (Ag + 1) * Rc) * Rd * Rd + ((Ag + 2) * Rb * Rb + (3 * Ag + 4) * Rb * Rc + 2 * (Ag + 1) * Rc * Rc) * Rd) * Re) * Ri - ((2 * Rb * Rc + Rc * Rc) * Rd * Rd + (Rb * Rb + 2 * Rb * Rc + Rc * Rc + 2 * (Rb + Rc) * Rd + Rd * Rd) * Re * Re + 2 * (Rb * Rb * Rc + Rb * Rc * Rc) * Rd + 2 * (Rb * Rb * Rc + Rb * Rc * Rc + (Rb + Rc) * Rd * Rd + (Rb * Rb + 3 * Rb * Rc + Rc * Rc) * Rd) * Re + (((Ag + 2) * Rc + 2 * Rb) * Rd * Rd + ((Ag + 4) * Rb * Rc + (Ag + 2) * Rc * Rc + 2 * Rb * Rb) * Rd + ((Ag + 2) * Rb * Rb + 2 * (Ag + 2) * Rb * Rc + (Ag + 2) * Rc * Rc + (Ag + 2) * Rd * Rd + 2 * ((Ag + 2) * Rb + (Ag + 2) * Rc) * Rd) * Re) * Ri) * Ro), (2 * (Rb * Rb * Rc + Rb * Rc * Rc) * Rd * Rd + (Rc * Rd * Rd + (Rb * Rc + Rc * Rc) * Rd) * Ro * Ro + ((Rb * Rb + 2 * Rb * Rc) * Rd * Rd + 2 * (Rb * Rb * Rc + Rb * Rc * Rc) * Rd) * Re + (((Ag + 2) * Rb * Rb + (3 * Ag + 4) * Rb * Rc + 2 * (Ag + 1) * Rc * Rc) * Rd * Rd + 2 * (((Ag + 1) * Rb + (Ag + 1) * Rc) * Rd * Rd + ((Ag + 1) * Rb * Rb + 2 * (Ag + 1) * Rb * Rc + (Ag + 1) * Rc * Rc) * Rd) * Re) * Ri - ((3 * Rb * Rc + 2 * Rc * Rc) * Rd * Rd + 2 * (Rb * Rb * Rc + Rb * Rc * Rc) * Rd + ((Rb + 2 * Rc) * Rd * Rd + (Rb * Rb + 3 * Rb * Rc + 2 * Rc * Rc) * Rd) * Re + (((Ag + 2) * Rb + (Ag + 2) * Rc) * Rd * Rd + ((Ag + 2) * Rb * Rb + 2 * (Ag + 2) * Rb * Rc + (Ag + 2) * Rc * Rc) * Rd) * Ri) * Ro) / ((Rb * Rb * Rc + Rb * Rc * Rc) * Rd * Rd + (Rb * Rb * Rc + Rb * Rc * Rc + Rb * Rd * Rd + (Rb * Rb + 2 * Rb * Rc) * Rd) * Re * Re + (Rb * Rb * Rc + Rb * Rc * Rc + Rc * Rd * Rd + (2 * Rb * Rc + Rc * Rc) * Rd + (Rb * Rb + 2 * Rb * Rc + Rc * Rc + 2 * (Rb + Rc) * Rd + Rd * Rd) * Re + (Rb * Rb + 2 * Rb * Rc + Rc * Rc + 2 * (Rb + Rc) * Rd + Rd * Rd) * Ri) * Ro * Ro + ((Rb * Rb + 2 * Rb * Rc) * Rd * Rd + 2 * (Rb * Rb * Rc + Rb * Rc * Rc) * Rd) * Re + (((Ag + 2) * Rb * Rc + (Ag + 1) * Rc * Rc + Rb * Rb) * Rd * Rd + ((Ag + 1) * Rb * Rb + 2 * (Ag + 1) * Rb * Rc + (Ag + 1) * Rc * Rc + (Ag + 1) * Rd * Rd + 2 * ((Ag + 1) * Rb + (Ag + 1) * Rc) * Rd) * Re * Re + (((Ag + 2) * Rb + 2 * (Ag + 1) * Rc) * Rd * Rd + ((Ag + 2) * Rb * Rb + (3 * Ag + 4) * Rb * Rc + 2 * (Ag + 1) * Rc * Rc) * Rd) * Re) * Ri - ((2 * Rb * Rc + Rc * Rc) * Rd * Rd + (Rb * Rb + 2 * Rb * Rc + Rc * Rc + 2 * (Rb + Rc) * Rd + Rd * Rd) * Re * Re + 2 * (Rb * Rb * Rc + Rb * Rc * Rc) * Rd + 2 * (Rb * Rb * Rc + Rb * Rc * Rc + (Rb + Rc) * Rd * Rd + (Rb * Rb + 3 * Rb * Rc + Rc * Rc) * Rd) * Re + (((Ag + 2) * Rc + 2 * Rb) * Rd * Rd + ((Ag + 4) * Rb * Rc + (Ag + 2) * Rc * Rc + 2 * Rb * Rb) * Rd + ((Ag + 2) * Rb * Rb + 2 * (Ag + 2) * Rb * Rc + (Ag + 2) * Rc * Rc + (Ag + 2) * Rd * Rd + 2 * ((Ag + 2) * Rb + (Ag + 2) * Rc) * Rd) * Re) * Ri) * Ro) },
                                { (Rb * Rd * Re + (Ag * Rb + Ag * Rc + Ag * Rd) * Re * Ri - (Rb + Rc + Rd) * Re * Ro) / (Rb * Rc * Rd + (Rb * Rc + Rb * Rd) * Re + (((Ag + 1) * Rc + Rb) * Rd + ((Ag + 1) * Rb + (Ag + 1) * Rc + (Ag + 1) * Rd) * Re) * Ri - (Rb * Rc + Rc * Rd + (Rb + Rc + Rd) * Re + (Rb + Rc + Rd) * Ri) * Ro), -(Rb * Rc * Rd * Rd * Re - (Rb * Rc + Rc * Rc + Rc * Rd) * Re * Ro * Ro + (Rb * Rc * Rd + Rb * Rd * Rd) * Re * Re - ((Ag * Rb * Rc + Ag * Rc * Rc - (Ag + 2) * Rd * Rd - ((Ag + 2) * Rb + 2 * Rc) * Rd) * Re * Re - (((Ag + 2) * Rc + 2 * Rb) * Rd * Rd - (Ag * Rb * Rc + Ag * Rc * Rc) * Rd) * Re) * Ri + ((Rb * Rc + Rc * Rc - Rb * Rd - Rd * Rd) * Re * Re + (Ag * Rb * Rc + Ag * Rc * Rc + ((Ag - 2) * Rc - 2 * Rb) * Rd - 2 * Rd * Rd) * Re * Ri + (Rc * Rc * Rd - Rc * Rd * Rd) * Re) * Ro) / ((Rb * Rb * Rc + Rb * Rc * Rc) * Rd * Rd + (Rb * Rb * Rc + Rb * Rc * Rc + Rb * Rd * Rd + (Rb * Rb + 2 * Rb * Rc) * Rd) * Re * Re + (Rb * Rb * Rc + Rb * Rc * Rc + Rc * Rd * Rd + (2 * Rb * Rc + Rc * Rc) * Rd + (Rb * Rb + 2 * Rb * Rc + Rc * Rc + 2 * (Rb + Rc) * Rd + Rd * Rd) * Re + (Rb * Rb + 2 * Rb * Rc + Rc * Rc + 2 * (Rb + Rc) * Rd + Rd * Rd) * Ri) * Ro * Ro + ((Rb * Rb + 2 * Rb * Rc) * Rd * Rd + 2 * (Rb * Rb * Rc + Rb * Rc * Rc) * Rd) * Re + (((Ag + 2) * Rb * Rc + (Ag + 1) * Rc * Rc + Rb * Rb) * Rd * Rd + ((Ag + 1) * Rb * Rb + 2 * (Ag + 1) * Rb * Rc + (Ag + 1) * Rc * Rc + (Ag + 1) * Rd * Rd + 2 * ((Ag + 1) * Rb + (Ag + 1) * Rc) * Rd) * Re * Re + (((Ag + 2) * Rb + 2 * (Ag + 1) * Rc) * Rd * Rd + ((Ag + 2) * Rb * Rb + (3 * Ag + 4) * Rb * Rc + 2 * (Ag + 1) * Rc * Rc) * Rd) * Re) * Ri - ((2 * Rb * Rc + Rc * Rc) * Rd * Rd + (Rb * Rb + 2 * Rb * Rc + Rc * Rc + 2 * (Rb + Rc) * Rd + Rd * Rd) * Re * Re + 2 * (Rb * Rb * Rc + Rb * Rc * Rc) * Rd + 2 * (Rb * Rb * Rc + Rb * Rc * Rc + (Rb + Rc) * Rd * Rd + (Rb * Rb + 3 * Rb * Rc + Rc * Rc) * Rd) * Re + (((Ag + 2) * Rc + 2 * Rb) * Rd * Rd + ((Ag + 4) * Rb * Rc + (Ag + 2) * Rc * Rc + 2 * Rb * Rb) * Rd + ((Ag + 2) * Rb * Rb + 2 * (Ag + 2) * Rb * Rc + (Ag + 2) * Rc * Rc + (Ag + 2) * Rd * Rd + 2 * ((Ag + 2) * Rb + (Ag + 2) * Rc) * Rd) * Re) * Ri) * Ro), -((Rb * Rb + 2 * Rb * Rc) * Rd * Rd * Re + (Rb * Rb + Rb * Rc + (2 * Rb + Rc) * Rd + Rd * Rd) * Re * Ro * Ro + (2 * Rb * Rd * Rd + (Rb * Rb + 2 * Rb * Rc) * Rd) * Re * Re + ((Ag * Rb * Rb + Ag * Rb * Rc + 2 * (Ag + 1) * Rd * Rd + ((3 * Ag + 2) * Rb + 2 * (Ag + 1) * Rc) * Rd) * Re * Re + (((Ag + 2) * Rb + 2 * (Ag + 1) * Rc) * Rd * Rd + (Ag * Rb * Rb + Ag * Rb * Rc) * Rd) * Re) * Ri - ((Rb * Rb + Rb * Rc + (3 * Rb + 2 * Rc) * Rd + 2 * Rd * Rd) * Re * Re + (Ag * Rb * Rb + Ag * Rb * Rc + (Ag + 2) * Rd * Rd + (2 * (Ag + 1) * Rb + (Ag + 2) * Rc) * Rd) * Re * Ri + (2 * (Rb + Rc) * Rd * Rd + (2 * Rb * Rb + 3 * Rb * Rc) * Rd) * Re) * Ro) / ((Rb * Rb * Rc + Rb * Rc * Rc) * Rd * Rd + (Rb * Rb * Rc + Rb * Rc * Rc + Rb * Rd * Rd + (Rb * Rb + 2 * Rb * Rc) * Rd) * Re * Re + (Rb * Rb * Rc + Rb * Rc * Rc + Rc * Rd * Rd + (2 * Rb * Rc + Rc * Rc) * Rd + (Rb * Rb + 2 * Rb * Rc + Rc * Rc + 2 * (Rb + Rc) * Rd + Rd * Rd) * Re + (Rb * Rb + 2 * Rb * Rc + Rc * Rc + 2 * (Rb + Rc) * Rd + Rd * Rd) * Ri) * Ro * Ro + ((Rb * Rb + 2 * Rb * Rc) * Rd * Rd + 2 * (Rb * Rb * Rc + Rb * Rc * Rc) * Rd) * Re + (((Ag + 2) * Rb * Rc + (Ag + 1) * Rc * Rc + Rb * Rb) * Rd * Rd + ((Ag + 1) * Rb * Rb + 2 * (Ag + 1) * Rb * Rc + (Ag + 1) * Rc * Rc + (Ag + 1) * Rd * Rd + 2 * ((Ag + 1) * Rb + (Ag + 1) * Rc) * Rd) * Re * Re + (((Ag + 2) * Rb + 2 * (Ag + 1) * Rc) * Rd * Rd + ((Ag + 2) * Rb * Rb + (3 * Ag + 4) * Rb * Rc + 2 * (Ag + 1) * Rc * Rc) * Rd) * Re) * Ri - ((2 * Rb * Rc + Rc * Rc) * Rd * Rd + (Rb * Rb + 2 * Rb * Rc + Rc * Rc + 2 * (Rb + Rc) * Rd + Rd * Rd) * Re * Re + 2 * (Rb * Rb * Rc + Rb * Rc * Rc) * Rd + 2 * (Rb * Rb * Rc + Rb * Rc * Rc + (Rb + Rc) * Rd * Rd + (Rb * Rb + 3 * Rb * Rc + Rc * Rc) * Rd) * Re + (((Ag + 2) * Rc + 2 * Rb) * Rd * Rd + ((Ag + 4) * Rb * Rc + (Ag + 2) * Rc * Rc + 2 * Rb * Rb) * Rd + ((Ag + 2) * Rb * Rb + 2 * (Ag + 2) * Rb * Rc + (Ag + 2) * Rc * Rc + (Ag + 2) * Rd * Rd + 2 * ((Ag + 2) * Rb + (Ag + 2) * Rc) * Rd) * Re) * Ri) * Ro), ((Rb * Rc + Rc * Rc + Rc * Rd) * Re * Ro * Ro + 2 * (Rb * Rb * Rc + Rb * Rc * Rc) * Rd * Re + (2 * Rb * Rb * Rc + 2 * Rb * Rc * Rc + (Rb * Rb + 2 * Rb * Rc) * Rd) * Re * Re + (2 * ((Ag + 2) * Rb * Rc + (Ag + 1) * Rc * Rc + Rb * Rb) * Rd * Re + ((Ag + 2) * Rb * Rb + (3 * Ag + 4) * Rb * Rc + 2 * (Ag + 1) * Rc * Rc + ((Ag + 2) * Rb + 2 * (Ag + 1) * Rc) * Rd) * Re * Re) * Ri - ((Rb * Rb + 3 * Rb * Rc + 2 * Rc * Rc + (Rb + 2 * Rc) * Rd) * Re * Re + ((Ag + 4) * Rb * Rc + (Ag + 2) * Rc * Rc + 2 * Rb * Rb + ((Ag + 2) * Rc + 2 * Rb) * Rd) * Re * Ri + (2 * Rb * Rb * Rc + 2 * Rb * Rc * Rc + (3 * Rb * Rc + 2 * Rc * Rc) * Rd) * Re) * Ro) / ((Rb * Rb * Rc + Rb * Rc * Rc) * Rd * Rd + (Rb * Rb * Rc + Rb * Rc * Rc + Rb * Rd * Rd + (Rb * Rb + 2 * Rb * Rc) * Rd) * Re * Re + (Rb * Rb * Rc + Rb * Rc * Rc + Rc * Rd * Rd + (2 * Rb * Rc + Rc * Rc) * Rd + (Rb * Rb + 2 * Rb * Rc + Rc * Rc + 2 * (Rb + Rc) * Rd + Rd * Rd) * Re + (Rb * Rb + 2 * Rb * Rc + Rc * Rc + 2 * (Rb + Rc) * Rd + Rd * Rd) * Ri) * Ro * Ro + ((Rb * Rb + 2 * Rb * Rc) * Rd * Rd + 2 * (Rb * Rb * Rc + Rb * Rc * Rc) * Rd) * Re + (((Ag + 2) * Rb * Rc + (Ag + 1) * Rc * Rc + Rb * Rb) * Rd * Rd + ((Ag + 1) * Rb * Rb + 2 * (Ag + 1) * Rb * Rc + (Ag + 1) * Rc * Rc + (Ag + 1) * Rd * Rd + 2 * ((Ag + 1) * Rb + (Ag + 1) * Rc) * Rd) * Re * Re + (((Ag + 2) * Rb + 2 * (Ag + 1) * Rc) * Rd * Rd + ((Ag + 2) * Rb * Rb + (3 * Ag + 4) * Rb * Rc + 2 * (Ag + 1) * Rc * Rc) * Rd) * Re) * Ri - ((2 * Rb * Rc + Rc * Rc) * Rd * Rd + (Rb * Rb + 2 * Rb * Rc + Rc * Rc + 2 * (Rb + Rc) * Rd + Rd * Rd) * Re * Re + 2 * (Rb * Rb * Rc + Rb * Rc * Rc) * Rd + 2 * (Rb * Rb * Rc + Rb * Rc * Rc + (Rb + Rc) * Rd * Rd + (Rb * Rb + 3 * Rb * Rc + Rc * Rc) * Rd) * Re + (((Ag + 2) * Rc + 2 * Rb) * Rd * Rd + ((Ag + 4) * Rb * Rc + (Ag + 2) * Rc * Rc + 2 * Rb * Rb) * Rd + ((Ag + 2) * Rb * Rb + 2 * (Ag + 2) * Rb * Rc + (Ag + 2) * Rc * Rc + (Ag + 2) * Rd * Rd + 2 * ((Ag + 2) * Rb + (Ag + 2) * Rc) * Rd) * Re) * Ri) * Ro), ((Rb * Rb * Rc + Rb * Rc * Rc) * Rd * Rd - (Rb * Rb * Rc + Rb * Rc * Rc + Rb * Rd * Rd + (Rb * Rb + 2 * Rb * Rc) * Rd) * Re * Re + (Rb * Rb * Rc + Rb * Rc * Rc + Rc * Rd * Rd + (2 * Rb * Rc + Rc * Rc) * Rd + (Rb * Rb + 2 * Rb * Rc + Rc * Rc + 2 * (Rb + Rc) * Rd + Rd * Rd) * Ri) * Ro * Ro + (((Ag + 2) * Rb * Rc + (Ag + 1) * Rc * Rc + Rb * Rb) * Rd * Rd - ((Ag + 1) * Rb * Rb + 2 * (Ag + 1) * Rb * Rc + (Ag + 1) * Rc * Rc + (Ag + 1) * Rd * Rd + 2 * ((Ag + 1) * Rb + (Ag + 1) * Rc) * Rd) * Re * Re) * Ri - ((2 * Rb * Rc + Rc * Rc) * Rd * Rd - (Rb * Rb + 2 * Rb * Rc + Rc * Rc + 2 * (Rb + Rc) * Rd + Rd * Rd) * Re * Re + 2 * (Rb * Rb * Rc + Rb * Rc * Rc) * Rd + (((Ag + 2) * Rc + 2 * Rb) * Rd * Rd + ((Ag + 4) * Rb * Rc + (Ag + 2) * Rc * Rc + 2 * Rb * Rb) * Rd) * Ri) * Ro) / ((Rb * Rb * Rc + Rb * Rc * Rc) * Rd * Rd + (Rb * Rb * Rc + Rb * Rc * Rc + Rb * Rd * Rd + (Rb * Rb + 2 * Rb * Rc) * Rd) * Re * Re + (Rb * Rb * Rc + Rb * Rc * Rc + Rc * Rd * Rd + (2 * Rb * Rc + Rc * Rc) * Rd + (Rb * Rb + 2 * Rb * Rc + Rc * Rc + 2 * (Rb + Rc) * Rd + Rd * Rd) * Re + (Rb * Rb + 2 * Rb * Rc + Rc * Rc + 2 * (Rb + Rc) * Rd + Rd * Rd) * Ri) * Ro * Ro + ((Rb * Rb + 2 * Rb * Rc) * Rd * Rd + 2 * (Rb * Rb * Rc + Rb * Rc * Rc) * Rd) * Re + (((Ag + 2) * Rb * Rc + (Ag + 1) * Rc * Rc + Rb * Rb) * Rd * Rd + ((Ag + 1) * Rb * Rb + 2 * (Ag + 1) * Rb * Rc + (Ag + 1) * Rc * Rc + (Ag + 1) * Rd * Rd + 2 * ((Ag + 1) * Rb + (Ag + 1) * Rc) * Rd) * Re * Re + (((Ag + 2) * Rb + 2 * (Ag + 1) * Rc) * Rd * Rd + ((Ag + 2) * Rb * Rb + (3 * Ag + 4) * Rb * Rc + 2 * (Ag + 1) * Rc * Rc) * Rd) * Re) * Ri - ((2 * Rb * Rc + Rc * Rc) * Rd * Rd + (Rb * Rb + 2 * Rb * Rc + Rc * Rc + 2 * (Rb + Rc) * Rd + Rd * Rd) * Re * Re + 2 * (Rb * Rb * Rc + Rb * Rc * Rc) * Rd + 2 * (Rb * Rb * Rc + Rb * Rc * Rc + (Rb + Rc) * Rd * Rd + (Rb * Rb + 3 * Rb * Rc + Rc * Rc) * Rd) * Re + (((Ag + 2) * Rc + 2 * Rb) * Rd * Rd + ((Ag + 4) * Rb * Rc + (Ag + 2) * Rc * Rc + 2 * Rb * Rb) * Rd + ((Ag + 2) * Rb * Rb + 2 * (Ag + 2) * Rb * Rc + (Ag + 2) * Rc * Rc + (Ag + 2) * Rd * Rd + 2 * ((Ag + 2) * Rb + (Ag + 2) * Rc) * Rd) * Re) * Ri) * Ro) } });

            auto Ra = (Rb * Rc * Rd + (Rb * Rc + Rb * Rd) * Re + (((Ag + 1) * Rc + Rb) * Rd + ((Ag + 1) * Rb + (Ag + 1) * Rc + (Ag + 1) * Rd) * Re) * Ri - (Rb * Rc + Rc * Rd + (Rb + Rc + Rd) * Re + (Rb + Rc + Rd) * Ri) * Ro) / ((Rb + Rc) * Rd + (Rb + Rc + Rd) * Re - (Rb + Rc + Rd) * Ro);
            return Ra;
        }
    };

    using RType = wdft::RtypeAdaptor<float, 0, ImpedanceCalc, decltype (R197), decltype (C58), decltype (C59), decltype (R196)>;
    RType R { std::tie (R197, C58, C59, R196) };

    // Port A
    wdft::IdealVoltageSourceT<float, decltype (R)> Vin { R };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SnareResonatorWDF)
};