#include "BaxandallWDF.h"

BaxandallWDF::BaxandallWDF()
{
    R.calcImpedanceFunc = [=] (const float (&portImpedances)[decltype (R)::numPorts])
    {
        const auto Ra = portImpedances[0];
        const auto Rb = portImpedances[1];
        const auto Rc = portImpedances[2];
        const auto Rd = portImpedances[3];
        const auto Re = portImpedances[4];
        const auto Rf = portImpedances[5];

        // This scattering matrix was derived using the R-Solver python script (https://github.com/jatinchowdhury18/R-Solver),
        // invoked with command: r_solver.py --datum 0 --out scratch/baxandall_uscatt.txt netlists/baxandall.txt
        R.setSMatrixData ({ { -2 * ((Rb + Rc) * Rd + (Rb + Rc + Rd) * Re + (Rb + Rc + Rd) * Rf) * Ra / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf) + 1, -2 * (Rc * Rd + (Rc + Rd) * Re + Rc * Rf) * Ra / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf), 2 * (Rb * Rd + Rb * Re + (Rb + Rd) * Rf) * Ra / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf), -2 * (Rb * Re - Rc * Rf) * Ra / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf), 2 * (Rb * Rd + (Rb + Rc + Rd) * Rf) * Ra / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf), -2 * (Rc * Rd + (Rb + Rc + Rd) * Re) * Ra / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf) },
                            { -2 * (Rc * Rd + (Rc + Rd) * Re + Rc * Rf) * Rb / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf), -2 * ((Ra + Rc) * Rd + (Ra + Rc + Rd) * Re + (Ra + Rc + Re) * Rf) * Rb / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf) + 1, -2 * (Ra * Rd + Ra * Re + (Ra + Re) * Rf) * Rb / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf), 2 * (Ra * Re + (Ra + Rc + Re) * Rf) * Rb / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf), -2 * (Ra * Rd - Rc * Rf) * Rb / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf), -2 * ((Ra + Rc) * Rd + (Rc + Rd) * Re) * Rb / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf) },
                            { 2 * (Rb * Rd + Rb * Re + (Rb + Rd) * Rf) * Rc / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf), -2 * (Ra * Rd + Ra * Re + (Ra + Re) * Rf) * Rc / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf), -2 * ((Ra + Rb) * Rd + (Ra + Rb) * Re + (Ra + Rb + Rd + Re) * Rf) * Rc / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf) + 1, 2 * ((Ra + Rb) * Re + (Ra + Re) * Rf) * Rc / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf), -2 * ((Ra + Rb) * Rd + (Rb + Rd) * Rf) * Rc / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf), -2 * (Ra * Rd - Rb * Re) * Rc / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf) },
                            { -2 * (Rb * Re - Rc * Rf) * Rd / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf), 2 * (Ra * Re + (Ra + Rc + Re) * Rf) * Rd / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf), 2 * ((Ra + Rb) * Re + (Ra + Re) * Rf) * Rd / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf), -2 * (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Re + (Ra + Rc + Re) * Rf) * Rd / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf) + 1, -2 * (Ra * Rb + (Ra + Rb) * Rc + Rc * Rf) * Rd / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf), -2 * (Ra * Rb + (Ra + Rb) * Rc + Rb * Re) * Rd / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf) },
                            { 2 * (Rb * Rd + (Rb + Rc + Rd) * Rf) * Re / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf), -2 * (Ra * Rd - Rc * Rf) * Re / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf), -2 * ((Ra + Rb) * Rd + (Rb + Rd) * Rf) * Re / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf), -2 * (Ra * Rb + (Ra + Rb) * Rc + Rc * Rf) * Re / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf), -2 * (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd + (Rb + Rc + Rd) * Rf) * Re / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf) + 1, -2 * (Ra * Rb + (Ra + Rb) * Rc + Ra * Rd) * Re / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf) },
                            { -2 * (Rc * Rd + (Rb + Rc + Rd) * Re) * Rf / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf), -2 * ((Ra + Rc) * Rd + (Rc + Rd) * Re) * Rf / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf), -2 * (Ra * Rd - Rb * Re) * Rf / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf), -2 * (Ra * Rb + (Ra + Rb) * Rc + Rb * Re) * Rf / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf), -2 * (Ra * Rb + (Ra + Rb) * Rc + Ra * Rd) * Rf / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf), -2 * (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf / ((Ra * Rb + (Ra + Rb) * Rc) * Rd + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rb) * Rd) * Re + (Ra * Rb + (Ra + Rb) * Rc + (Ra + Rc) * Rd + (Rb + Rc + Rd) * Re) * Rf) + 1 } });
    };
}

void BaxandallWDF::prepare (double fs)
{
    Ca.prepare ((float) fs);
    Cb.prepare ((float) fs);
    Cc.prepare ((float) fs);
    Cd.prepare ((float) fs);
    Ce.prepare ((float) fs);
}

void BaxandallWDF::reset()
{
    Ca.reset();
    Cb.reset();
    Cc.reset();
    Cd.reset();
    Ce.reset();
}

void BaxandallWDF::setParams (float bassParam, float trebleParam)
{
    Pb_plus.setResistanceValue (Pb * bassParam);
    Pb_minus.setResistanceValue (Pb * (1.0f - bassParam));

    Pt_plus.setResistanceValue (Pt * trebleParam);
    Pt_minus.setResistanceValue (Pt * (1.0f - trebleParam));
}
