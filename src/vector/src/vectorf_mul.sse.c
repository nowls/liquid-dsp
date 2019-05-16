/*
 * Copyright (c) 2007 - 2015, 2019 Joseph Gaeddert
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

// 
// Floating-point vector multiplication (SSE)
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "liquid.internal.h"

// include proper SIMD extensions for x86 platforms

#if 0
#include <mmintrin.h>   // MMX
#include <emmintrin.h>  // SSE2
#include <pmmintrin.h>  // SSE3
#include <smmintrin.h>  // SSE4.1/2
#include <immintrin.h>  // AVX
#endif
#include <xmmintrin.h>  // SSE

#define DEBUG_VECTORF_MUL_SSE       0

// internal methods
void liquid_vectorf_mul_sse(float *      _x,
                            float *      _y,
                            unsigned int _n,
                            float *      _z);

void liquid_vectorf_mulscalar_sse(float *      _x,
                                  unsigned int _n,
                                  float        _v,
                                  float *      _y);

// 
void liquid_vectorf_mul(float *      _x,
                        float *      _y,
                        unsigned int _n,
                        float *      _z)
{
    liquid_vectorf_mul_sse(_x, _y, _n, _z);
}

// 
void liquid_vectorf_mulscalar(float *      _x,
                              unsigned int _n,
                              float        _v,
                              float *      _y)
{
    liquid_vectorf_mulscalar_sse(_x, _n, _v, _y);
}

// use MMX/SSE extensions
void liquid_vectorf_mul_sse(float *      _x,
                            float *      _y,
                            unsigned int _n,
                            float *      _z)
{
    __m128 vx;   // first input vector
    __m128 vy;   // first input vector
    __m128 vz;   // intermediate result

    // t = 4*(floor(_n/4))
    unsigned int t = (_n >> 2) << 2;

    //
    unsigned int i;
    for (i=0; i<t; i+=4) {
        // load inputs into registers (unaligned)
        vx = _mm_loadu_ps(&_x[i]);
        vy = _mm_loadu_ps(&_y[i]);

        // compute multiplication
        vz = _mm_mul_ps(vx, vy);
        
        // unload packed array
        _mm_storeu_ps(&_z[i], vz);
    }

    // cleanup
    for (; i<_n; i++)
        _z[i] = _x[i] * _y[i];
}

// use MMX/SSE extensions
void liquid_vectorf_mulscalar_sse(float *      _x,
                                  unsigned int _n,
                                  float        _v,
                                  float *      _y)
{
    __m128 vx;   // input vector
    __m128 vv;   // scalar
    __m128 vy;   // intermediate result

    // t = 4*(floor(_n/4))
    unsigned int t = (_n >> 2) << 2;

    // load scalar into all 4 register slots
    vv = _mm_set_ps(_v, _v, _v, _v);

    //
    unsigned int i;
    for (i=0; i<t; i+=4) {
        // load inputs into registers (unaligned)
        vx = _mm_loadu_ps(&_x[i]);

        // compute multiplication
        vy = _mm_mul_ps(vx, vv);
        
        // unload packed array
        _mm_storeu_ps(&_y[i], vy);
    }

    // cleanup
    for (; i<_n; i++)
        _y[i] = _x[i] * _v;
}

