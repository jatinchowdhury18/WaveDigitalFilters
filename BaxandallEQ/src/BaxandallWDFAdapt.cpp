#include "BaxandallWDFAdapt.h"

void BaxandallWDFAdapt::prepare (double fs)
{
    Ca.prepare ((float) fs);
    Cb.prepare ((float) fs);
    Cc.prepare ((float) fs);
    Cd.prepare ((float) fs);
    Ce.prepare ((float) fs);
}

void BaxandallWDFAdapt::reset()
{
    Ca.reset();
    Cb.reset();
    Cc.reset();
    Cd.reset();
    Ce.reset();
}

void BaxandallWDFAdapt::setParams (float bassParam, float trebleParam)
{
    Pb_plus.setResistanceValue (Pb * bassParam);
    Pb_minus.setResistanceValue (Pb * (1.0f - bassParam));

    Pt_plus.setResistanceValue (Pt * trebleParam);
    Pt_minus.setResistanceValue (Pt * (1.0f - trebleParam));
}
