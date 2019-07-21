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
