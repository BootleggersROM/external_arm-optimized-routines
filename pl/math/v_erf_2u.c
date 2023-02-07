/*
 * Double-precision vector erf(x) function.
 *
 * Copyright (c) 2019-2023, Arm Limited.
 * SPDX-License-Identifier: MIT OR Apache-2.0 WITH LLVM-exception
 */

#include "v_math.h"
#include "include/mathlib.h"
#include "math_config.h"
#include "pl_sig.h"
#include "pl_test.h"

#define AbsMask v_u64 (0x7fffffffffffffff)
#define AbsXMax v_f64 (0x1.8p+2)
#define Scale v_f64 (0x1p+3)

/* Special cases (fall back to scalar calls).  */
VPCS_ATTR
NOINLINE static float64x2_t
specialcase (float64x2_t x, float64x2_t y, uint64x2_t cmp)
{
  return v_call_f64 (erf, x, y, cmp);
}

/* A structure to perform look-up in coeffs and other parameter tables.  */
struct entry
{
  float64x2_t P[V_ERF_NCOEFFS];
  float64x2_t shift;
};

static inline struct entry
lookup (uint64x2_t i)
{
  struct entry e;
  for (int j = 0; j < V_ERF_NCOEFFS; ++j)
    {
      e.P[j][0] = __v_erf_data.coeffs[j][i[0]];
      e.P[j][1] = __v_erf_data.coeffs[j][i[1]];
    }
  e.shift[0] = __v_erf_data.shifts[i[0]];
  e.shift[1] = __v_erf_data.shifts[i[1]];
  return e;
}

/* Optimized double precision vector error function erf. Maximum
   observed error is 1.75 ULP, in [0.110, 0.111]:
   verf(0x1.c5e0c2d5d0543p-4) got 0x1.fe0ed62a54987p-4
			     want 0x1.fe0ed62a54985p-4.  */
VPCS_ATTR
float64x2_t V_NAME (erf) (float64x2_t x)
{
  /* Handle both inf/nan as well as small values (|x|<2^-28)
     If any condition in the lane is true then a loop over
     scalar calls will be performed.  */
  uint64x2_t ix = v_as_u64_f64 (x);
  uint64x2_t atop = (ix >> 48) & v_u64 (0x7fff);
  uint64x2_t special_case
    = v_cond_u64 (atop - v_u64 (0x3e30) >= v_u64 (0x7ff0 - 0x3e30));

  /* Get sign and absolute value.  */
  uint64x2_t sign = v_as_u64_f64 (x) & ~AbsMask;
  float64x2_t a = v_min_f64 (v_abs_f64 (x), AbsXMax);

  /* Compute index by truncating 8 * a with a=|x| saturated to 6.0.  */
  uint64x2_t i = vcvtq_n_u64_f64 (a, 3);

  /* Get polynomial coefficients and shift parameter using lookup.  */
  struct entry dat = lookup (i);

  /* Evaluate polynomial on transformed argument.  */
  float64x2_t z = v_fma_f64 (a, Scale, dat.shift);

  float64x2_t r1 = v_fma_f64 (z, dat.P[1], dat.P[0]);
  float64x2_t r2 = v_fma_f64 (z, dat.P[3], dat.P[2]);
  float64x2_t r3 = v_fma_f64 (z, dat.P[5], dat.P[4]);
  float64x2_t r4 = v_fma_f64 (z, dat.P[7], dat.P[6]);
  float64x2_t r5 = v_fma_f64 (z, dat.P[9], dat.P[8]);

  float64x2_t z2 = z * z;
  float64x2_t y = v_fma_f64 (z2, r5, r4);
  y = v_fma_f64 (z2, y, r3);
  y = v_fma_f64 (z2, y, r2);
  y = v_fma_f64 (z2, y, r1);

  /* y=erf(x) if x>0, -erf(-x) otherwise.  */
  y = v_as_f64_u64 (v_as_u64_f64 (y) ^ sign);

  if (unlikely (v_any_u64 (special_case)))
    return specialcase (x, y, special_case);
  return y;
}
PL_ALIAS (V_NAME (erf), _ZGVnN2v_erf)

PL_SIG (V, D, 1, erf, -6.0, 6.0)
PL_TEST_ULP (V_NAME (erf), 1.26)
PL_TEST_INTERVAL (V_NAME (erf), 0, 0xffff0000, 10000)
PL_TEST_INTERVAL (V_NAME (erf), 0x1p-127, 0x1p-26, 40000)
PL_TEST_INTERVAL (V_NAME (erf), -0x1p-127, -0x1p-26, 40000)
PL_TEST_INTERVAL (V_NAME (erf), 0x1p-26, 0x1p3, 40000)
PL_TEST_INTERVAL (V_NAME (erf), -0x1p-26, -0x1p3, 40000)
PL_TEST_INTERVAL (V_NAME (erf), 0, inf, 40000)
