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

#include <sys/resource.h>
#include "liquid.h"

// Helper function to keep code base small
void vectorf_mul_bench(struct rusage *_start,
                       struct rusage *_finish,
                       unsigned long int *_num_iterations,
                       unsigned int _n)
{
    // normalize number of iterations
    *_num_iterations *= 128;
    *_num_iterations /= _n;
    if (*_num_iterations < 1) *_num_iterations = 1;

    float x[_n];
    float y[_n];
    float z[_n];
    unsigned int i;
    for (i=0; i<_n; i++) {
        x[i] = randnf();
        y[i] = randnf();
    }

    // start trials
    getrusage(RUSAGE_SELF, _start);
    for (i=0; i<(*_num_iterations); i++) {
        liquid_vectorf_mul(x, y, _n, z);
        liquid_vectorf_mul(x, y, _n, z);
        liquid_vectorf_mul(x, y, _n, z);
        liquid_vectorf_mul(x, y, _n, z);
    }
    getrusage(RUSAGE_SELF, _finish);
    *_num_iterations *= 4;
}

#define VECTORF_MUL_BENCHMARK_API(N)   \
(   struct rusage *_start,              \
    struct rusage *_finish,             \
    unsigned long int *_num_iterations) \
{ vectorf_mul_bench(_start, _finish, _num_iterations, N); }

void benchmark_vectorf_mul_7      VECTORF_MUL_BENCHMARK_API(7)
void benchmark_vectorf_mul_8      VECTORF_MUL_BENCHMARK_API(8)
void benchmark_vectorf_mul_15     VECTORF_MUL_BENCHMARK_API(15)
void benchmark_vectorf_mul_16     VECTORF_MUL_BENCHMARK_API(16)
void benchmark_vectorf_mul_31     VECTORF_MUL_BENCHMARK_API(31)
void benchmark_vectorf_mul_32     VECTORF_MUL_BENCHMARK_API(32)
void benchmark_vectorf_mul_63     VECTORF_MUL_BENCHMARK_API(63)
void benchmark_vectorf_mul_64     VECTORF_MUL_BENCHMARK_API(64)
void benchmark_vectorf_mul_127    VECTORF_MUL_BENCHMARK_API(127)
void benchmark_vectorf_mul_128    VECTORF_MUL_BENCHMARK_API(128)
void benchmark_vectorf_mul_255    VECTORF_MUL_BENCHMARK_API(255)
void benchmark_vectorf_mul_256    VECTORF_MUL_BENCHMARK_API(256)
void benchmark_vectorf_mul_511    VECTORF_MUL_BENCHMARK_API(511)
void benchmark_vectorf_mul_512    VECTORF_MUL_BENCHMARK_API(512)
void benchmark_vectorf_mul_1023   VECTORF_MUL_BENCHMARK_API(1023)
void benchmark_vectorf_mul_1024   VECTORF_MUL_BENCHMARK_API(1024)
void benchmark_vectorf_mul_2047   VECTORF_MUL_BENCHMARK_API(2047)
void benchmark_vectorf_mul_2048   VECTORF_MUL_BENCHMARK_API(2048)
void benchmark_vectorf_mul_4095   VECTORF_MUL_BENCHMARK_API(4095)
void benchmark_vectorf_mul_4096   VECTORF_MUL_BENCHMARK_API(4096)
