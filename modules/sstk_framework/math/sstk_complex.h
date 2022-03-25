#ifndef __SSTK_COMPLEX_H__
#define __SSTK_COMPLEX_H__

#include <cmath>

// static inline void test() {}

typedef struct sstk_complex_s {
    double real, imag;
} sstk_complex_t;

typedef struct sstk_complexf_s {
    float real, imag;
} sstk_complexf_t;


#define sstk_complex_add(cplx_src, cplx) \
    do { \
        cplx_src->real += cplx->real;\
    } while (0)

#define sstk_complex_add_real(cplx_src, dreal) \
    do { \
        cplx_src->real += dreal;\
    } while (0)

#endif
