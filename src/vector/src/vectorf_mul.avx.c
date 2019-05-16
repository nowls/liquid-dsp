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
// Floating-point vector multiplication (AVX)
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "liquid.internal.h"

// include proper SIMD extensions for x86 platforms

#if 0
#include <mmintrin.h>   // MMX
#include <xmmintrin.h>  // SSE
#include <emmintrin.h>  // SSE2
#include <pmmintrin.h>  // SSE3
#include <smmintrin.h>  // SSE4.1/2
#endif
#include <immintrin.h>  // AVX

#define DEBUG_VECTORF_MUL_AVX       0

// internal methods
void liquid_vectorf_mul_avx(float *      _x,
                            float *      _y,
                            unsigned int _n,
                            float *      _z);

void liquid_vectorf_mulscalar_avx(float *      _x,
                                  unsigned int _n,
                                  float        _v,
                                  float *      _y);

// 
void liquid_vectorf_mul(float *      _x,
                        float *      _y,
                        unsigned int _n,
                        float *      _z)
{
    liquid_vectorf_mul_avx(_x, _y, _n, _z);
}

// 
void liquid_vectorf_mulscalar(float *      _x,
                              unsigned int _n,
                              float        _v,
                              float *      _y)
{
    liquid_vectorf_mulscalar_avx(_x, _n, _v, _y);
}

// use AVX extensions
void liquid_vectorf_mul_avx(float *      _x,
                            float *      _y,
                            unsigned int _n,
                            float *      _z)
{
    __m256 vx;   // first input vector
    __m256 vy;   // first input vector
    __m256 vz;   // intermediate result

    // t = 8*(floor(_n/8))
    unsigned int t = (_n >> 3) << 3;

    //
    unsigned int i;
    for (i=0; i<t; i+=8) {
        // load inputs into registers (unaligned)
        vx = _mm256_loadu_ps(&_x[i]);
        vy = _mm256_loadu_ps(&_y[i]);

        // compute multiplication
        vz = _mm256_mul_ps(vx, vy);
        
        // unload packed array
        _mm256_storeu_ps(&_z[i], vz);
    }

    // cleanup
    for (; i<_n; i++)
        _z[i] = _x[i] * _y[i];
}

// use AVX extensions
void liquid_vectorf_mulscalar_avx(float *      _x,
                                  unsigned int _n,
                                  float        _v,
                                  float *      _y)
{
    __m256 vx;   // input vector
    __m256 vv;   // scalar
    __m256 vy;   // intermediate result

    // t = 8*(floor(_n/8))
    unsigned int t = (_n >> 3) << 3;

    // load scalar into all 8 register slots
    vv = _mm256_set_ps(_v, _v, _v, _v, _v, _v, _v, _v);

    //
    unsigned int i;
    for (i=0; i<t; i+=8) {
        // load inputs into registers (unaligned)
        vx = _mm256_loadu_ps(&_x[i]);

        // compute multiplication
        vy = _mm256_mul_ps(vx, vv);
        
        // unload packed array
        _mm256_storeu_ps(&_y[i], vy);
    }

    // cleanup
    for (; i<_n; i++)
        _y[i] = _x[i] * _v;
}

