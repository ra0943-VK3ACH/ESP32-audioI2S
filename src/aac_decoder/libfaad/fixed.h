/*
** FAAD2 - Freeware Advanced Audio (AAC) Decoder including SBR decoding
** Copyright (C) 2003-2005 M. Bakker, Nero AG, http://www.nero.com
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
**
** Any non-GPL usage of this software or parts of this software is strictly
** forbidden.
**
** The "appropriate copyright message" mentioned in section 2c of the GPLv2
** must read: "Code from FAAD2 is copyright (c) Nero AG, www.nero.com"
**
** Commercial non-GPL licensing of this software is possible.
** For more info contact Nero AG through Mpeg4AAClicense@nero.com.
**
** $Id: fixed.h,v 1.32 2007/11/01 12:33:30 menno Exp $
**/
#pragma once
#pragma GCC diagnostic ignored "-Wunused-function"
#include "common.h"


#define ZERO_HCB       0
#define FIRST_PAIR_HCB 5
#define ESC_HCB        11
#define QUAD_LEN       4
#define PAIR_LEN       2
#define NOISE_HCB      13
#define INTENSITY_HCB2 14
#define INTENSITY_HCB  15
#define DRC_REF_LEVEL 20*4 /* -20 dB */
#define DRM_PARAMETRIC_STEREO    0
#define DRM_NUM_SA_BANDS         8
#define DRM_NUM_PAN_BANDS       20
#define NUM_OF_LINKS             3
#define NUM_OF_QMF_CHANNELS     64
#define NUM_OF_SUBSAMPLES       30
#define MAX_SA_BAND             46
#define MAX_PAN_BAND            64
#define MAX_DELAY                5
#define EXTENSION_ID_PS 2
#define MAX_PS_ENVELOPES 5
#define NO_ALLPASS_LINKS 3
#define BYTE_NUMBIT     8
#define BYTE_NUMBIT_LD  3
#define bit2byte(a) ((a+7)>>BYTE_NUMBIT_LD)
#define NUM_ERROR_MESSAGES 34
#ifdef DRM
#define DECAY_CUTOFF         3
#define DECAY_SLOPE          0.05f
/* type definitaions */
typedef const int8_t (*drm_ps_huff_tab)[2];
#endif
#define FLOAT_SCALE (1.0f/(1<<15))
#define DM_MUL REAL_CONST(0.3203772410170407) // 1/(1+sqrt(2) + 1/sqrt(2))
#define RSQRT2 REAL_CONST(0.7071067811865475244) // 1/sqrt(2)






#ifdef FIXED_POINT  /* int32_t */
    #include "Arduino.h"
    #define COEF_BITS      28
    #define COEF_PRECISION (1 << COEF_BITS)
    #define REAL_BITS      14 // MAXIMUM OF 14 FOR FIXED POINT SBR
    #define REAL_PRECISION (1 << REAL_BITS)
    /* FRAC is the fractional only part of the fixed point number [0.0..1.0) */
    #define FRAC_SIZE      32 /* frac is a 32 bit integer */
    #define FRAC_BITS      31
    #define FRAC_PRECISION ((uint32_t)(1 << FRAC_BITS))
    #define FRAC_MAX       0x7FFFFFFF
    typedef int32_t real_t;
    #define REAL_CONST(A) (((A) >= 0) ? ((real_t)((A) * (REAL_PRECISION) + 0.5)) : ((real_t)((A) * (REAL_PRECISION) - 0.5)))
    #define COEF_CONST(A) (((A) >= 0) ? ((real_t)((A) * (COEF_PRECISION) + 0.5)) : ((real_t)((A) * (COEF_PRECISION) - 0.5)))
    #define FRAC_CONST(A) (((A) == 1.00) ? ((real_t)FRAC_MAX) : (((A) >= 0) ? ((real_t)((A) * (FRAC_PRECISION) + 0.5)) : ((real_t)((A) * (FRAC_PRECISION) - 0.5))))
    // #define FRAC_CONST(A) (((A) >= 0) ? ((real_t)((A)*(FRAC_PRECISION)+0.5)) : ((real_t)((A)*(FRAC_PRECISION)-0.5)))
    #define Q2_BITS      22
    #define Q2_PRECISION (1 << Q2_BITS)
    #define Q2_CONST(A)  (((A) >= 0) ? ((real_t)((A) * (Q2_PRECISION) + 0.5)) : ((real_t)((A) * (Q2_PRECISION) - 0.5)))
    /* multiply with real shift */
    #define MUL_R(A, B) (real_t)(((int64_t)(A) * (int64_t)(B) + (1 << (REAL_BITS - 1))) >> REAL_BITS)
    /* multiply with coef shift */
    #define MUL_C(A, B) (real_t)(((int64_t)(A) * (int64_t)(B) + (1 << (COEF_BITS - 1))) >> COEF_BITS)
    /* multiply with fractional shift */
    #define _MulHigh(A, B)    (real_t)(((int64_t)(A) * (int64_t)(B) + (1 << (FRAC_SIZE - 1))) >> FRAC_SIZE)
    #define MUL_F(A, B)       (real_t)(((int64_t)(A) * (int64_t)(B) + (1 << (FRAC_BITS - 1))) >> FRAC_BITS)
    #define MUL_Q2(A, B)      (real_t)(((int64_t)(A) * (int64_t)(B) + (1 << (Q2_BITS - 1))) >> Q2_BITS)
    #define MUL_SHIFT6(A, B)  (real_t)(((int64_t)(A) * (int64_t)(B) + (1 << (6 - 1))) >> 6)
    #define MUL_SHIFT23(A, B) (real_t)(((int64_t)(A) * (int64_t)(B) + (1 << (23 - 1))) >> 23)
    #define DIV_R(A, B)       (((int64_t)A << REAL_BITS) / B)
    #define DIV_C(A, B)       (((int64_t)A << COEF_BITS) / B)

/* Complex multiplication */
static inline void ComplexMult(real_t* y1, real_t* y2, real_t x1, real_t x2, real_t c1, real_t c2) { // FIXED POINT
    *y1 = (_MulHigh(x1, c1) + _MulHigh(x2, c2)) << (FRAC_SIZE - FRAC_BITS);
    *y2 = (_MulHigh(x2, c1) - _MulHigh(x1, c2)) << (FRAC_SIZE - FRAC_BITS);
}
#define DIV(A, B) (((int64_t)A << REAL_BITS) / B)
#define step(shift)                                  \
    if ((0x40000000l >> shift) + root <= value) {    \
        value -= (0x40000000l >> shift) + root;      \
        root = (root >> 1) | (0x40000000l >> shift); \
    } else {                                         \
        root = root >> 1;                            \
    }
real_t fp_sqrt(real_t value) {
    real_t root = 0;
    step(0);
    step(2);
    step(4);
    step(6);
    step(8);
    step(10);
    step(12);
    step(14);
    step(16);
    step(18);
    step(20);
    step(22);
    step(24);
    step(26);
    step(28);
    step(30);
    if (root < value) ++root;
    root <<= (REAL_BITS / 2);
    return root;
}
real_t const pow2_table[] = {COEF_CONST(1.0), COEF_CONST(1.18920711500272), COEF_CONST(1.41421356237310), COEF_CONST(1.68179283050743)};




#endif // FIXED_POINT

#ifndef FIXED_POINT
    #define DIV_R(A, B) ((A) / (B))
    #define DIV_C(A, B) ((A) / (B))

    #ifdef USE_DOUBLE_PRECISION  /* double */
        typedef double real_t;
        #include <math.h>
        #define MUL_R(A, B) ((A) * (B))
        #define MUL_C(A, B) ((A) * (B))
        #define MUL_F(A, B) ((A) * (B))
        /* Complex multiplication */
        static void ComplexMult(real_t* y1, real_t* y2, real_t x1, real_t x2, real_t c1, real_t c2) {
            *y1 = MUL_F(x1, c1) + MUL_F(x2, c2);
            *y2 = MUL_F(x2, c1) - MUL_F(x1, c2);
        }
        #define REAL_CONST(A) ((real_t)(A))
        #define COEF_CONST(A) ((real_t)(A))
        #define Q2_CONST(A)   ((real_t)(A))
        #define FRAC_CONST(A) ((real_t)(A)) /* pure fractional part */

    #else /* Normal floating point operation */
        typedef float real_t;
        #define MUL_R(A, B)   ((A) * (B))
        #define MUL_C(A, B)   ((A) * (B))
        #define MUL_F(A, B)   ((A) * (B))
        #define REAL_CONST(A) ((real_t)(A))
        #define COEF_CONST(A) ((real_t)(A))
        #define Q2_CONST(A)   ((real_t)(A))
        #define FRAC_CONST(A) ((real_t)(A)) /* pure fractional part */
        /* Complex multiplication */

        static void ComplexMult(real_t* y1, real_t* y2, real_t x1, real_t x2, real_t c1, real_t c2) {
            *y1 = MUL_F(x1, c1) + MUL_F(x2, c2);
            *y2 = MUL_F(x2, c1) - MUL_F(x1, c2);
        }
    #endif /* USE_DOUBLE_PRECISION */
#endif // FIXED_POINT


#ifdef SBR_LOW_POWER
    #define qmf_t     real_t
    #define QMF_RE(A) (A)
    #define QMF_IM(A)
#else
    #define qmf_t     complex_t
    #define QMF_RE(A) RE(A)
    #define QMF_IM(A) IM(A)
#endif

typedef real_t complex_t[2];
#define RE(A) A[0]
#define IM(A) A[1]
