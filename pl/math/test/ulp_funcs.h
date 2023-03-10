/*
 * Function entries for ulp.
 *
 * Copyright (c) 2022, Arm Limited.
 * SPDX-License-Identifier: MIT OR Apache-2.0 WITH LLVM-exception
 */
F1 (acosh)
F1 (asinh)
F2 (atan2)
F1 (erfc)
F1 (erf)
F1 (log10)
F1 (log1p)
D1 (asinh)
D2 (atan2)
F1 (tan)
D1 (erfc)
D1 (log10)
D1 (log1p)
#if WANT_VMATH
SF1 (asinh)
SF1 (atan)
SD1 (atan)
SF2 (atan2)
SD2 (atan2)
SF1 (erf)
SD1 (erf)
SF1 (erfc)
SD1 (erfc)
SF1 (log10)
SD1 (log10)
SF1 (log1p)
SF1 (log2)
SD1 (log2)
SF1 (tan)
#if __aarch64__
VF1 (asinh)
VF1 (atan)
VD1 (atan)
VF2 (atan2)
VD2 (atan2)
VF1 (erf)
VD1 (erf)
VF1 (erfc)
VD1 (erfc)
VF1 (log10)
VD1 (log10)
VF1 (log1p)
VF1 (log2)
VD1 (log2)
VF1 (tan)
#ifdef __vpcs
ZVNF1 (asinh)
ZVNF1 (atan)
ZVND1 (atan)
ZVNF2 (atan2)
ZVND2 (atan2)
ZVNF1 (erf)
ZVND1 (erf)
ZVNF1 (erfc)
ZVND1 (erfc)
ZVNF1 (log10)
ZVND1 (log10)
ZVNF1 (log1p)
ZVNF1 (log2)
ZVND1 (log2)
ZVNF1 (tan)
#endif
#endif
#if WANT_SVE_MATH
SVF2 (atan2)
ZSVF2 (atan2)
SVD2 (atan2)
ZSVD2 (atan2)
SVF1 (atan)
ZSVF1 (atan)
SVD1 (atan)
ZSVD1 (atan)
SVF1 (cos)
ZSVF1 (cos)
SVD1 (cos)
ZSVD1 (cos)
SVF1 (erf)
ZSVF1 (erf)
SVD1 (erf)
ZSVD1 (erf)
SVF1 (exp)
ZSVF1 (exp)
SVF1 (log)
ZSVF1 (log)
SVD1 (log)
ZSVD1 (log)
SVF1 (log10)
ZSVF1 (log10)
SVD1 (log10)
ZSVD1 (log10)
SVF1 (sin)
ZSVF1 (sin)
SVD1 (sin)
ZSVD1 (sin)
SVF1 (tan)
ZSVF1 (tan)
#endif
#endif
