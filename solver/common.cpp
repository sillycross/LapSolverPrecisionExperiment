#include "common.h"

#ifdef USE_MPFR
const char* precisionDesc = "MPFR (" MACROTOSTRING(MPFR_PRECISION) " bits)";
#else
#ifdef USE_FLOAT128
const char* precisionDesc = "__float128 (113 bits)";
#else
const char* precisionDesc = "double (53 bits)";
#endif
#endif

const char* GetFloatingPointPrecision()
{
    return precisionDesc;
}

#ifdef USE_MPFR
double FloatToDouble(FLOAT f)
{
    double out;
    f.conv(out);
    return out;
}
#else
double FloatToDouble(FLOAT f)
{
    return static_cast<double>(f);
}
#endif
