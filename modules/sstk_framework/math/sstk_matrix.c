#include <sstk_math.h>

#define __matrix_ijloop_start(im, jm)      \
    int i, j;                              \
    for (i = 0; i < r; i++) {              \
        for (j = 0; j < c; j++) {          

#define __matrix_ijloop_end                \
        }                                  \
    }

#define __matrix_add(out, a, b, r, c)      \
    __matrix_ijloop_start(r, c)            \
    out[i * c + j] = a[i * c + j] + b[i * c + j];         \
    __matrix_ijloop_end

#define __matrix_sub(out, a, b, r, c)      \
    __matrix_ijloop_start(r, c)            \
    out[i * c + j] = a[i * c + j] - b[i * c + j];         \
    __matrix_ijloop_end

#define __matrix_addn(out, a, n, r, c)     \
    __matrix_ijloop_start(r, c)            \
    out[i * c + j] = a[i * c + j] + n;               \
    __matrix_ijloop_end

#define __matrix_subn(out, a, n, r, c)     \
    __matrix_ijloop_start(r, c)            \
    out[i * c + j] = a[i * c + j] - n;               \
    __matrix_ijloop_end

void matrix_add(double* out, double* a, double* b, int r, int c) {
    __matrix_add(out, a, b, r, c)
}

void matrix_addf(float* out, float* a, float* b, int r, int c) {
    __matrix_add(out, a, b, r, c)
}

void matrix_sub(double* out, double* a, double* b, int r, int c) {
    __matrix_sub(out, a, b, r, c)
}

void matrix_subf(float* out, float* a, float* b, int r, int c) {
    __matrix_sub(out, a, b, r, c)
}

void matrix_addn(double* out, double* a, double n, int r, int c) {
    __matrix_addn(out, a, n, r, c)
}

void matrix_addnf(float* out, float* a, float n, int r, int c) {
    __matrix_addn(out, a, n, r, c)
}

void matrix_subn(double* out, double* a, double n, int r, int c) {
    __matrix_subn(out, a, n, r, c)
}

void matrix_subnf(float* out, float* a, float n, int r, int c) {
    __matrix_subn(out, a, n, r, c)
}

void matrix_muln(double* out, double* a, double n, int r, int c) {
    __matrix_ijloop_start(r, c)
    out[i * c + j] = sstk_mul(a[i * c + j], n);
    __matrix_ijloop_end
}

void matrix_mulnf(float* out, float* a, float n, int r, int c) {
    __matrix_ijloop_start(r, c)
    out[i * c + j] = sstk_fmul(a[i * c + j], n);
    __matrix_ijloop_end
}

void matrix_divn(double* out, double* a, double n, int r, int c) {
    __matrix_ijloop_start(r, c)
    out[i * c + j] = sstk_div(a[i * c + j], n);
    __matrix_ijloop_end
}

void matrix_divnf(float* out, float* a, float n, int r, int c) {
    __matrix_ijloop_start(r, c)
    out[i * c + j] = sstk_fdiv(a[i * c + j], n);
    __matrix_ijloop_end
}

void matrix_mul(double* out, double* a, double* b, int ar, int ac, int bc) {
    int i, j, k;
    for (i = 0; i < ar; i++) {
        for (j = 0; j < bc; j++) {
            out[i * bc + j] = 0;
            for (k = 0; k < ac; k++) {
                out[i * bc + j] += sstk_mul(a[i * ac + k], b[k * bc + j]);
            }
        }
    }
}

void matrix_mulf(float* out, float* a, float* b, int ar, int ac, int bc) {
    int i, j, k;
    for (i = 0; i < ar; i++) {
        for (j = 0; j < bc; j++) {
            out[i * bc + j] = 0;
            for (k = 0; k < ac; k++) {
                out[i * bc + j] += sstk_fmul(a[i * ac + k], b[k * bc + j]);
            }
        }
    }
}

void matrix_trans(double* out, double* a, int r, int c) {
    __matrix_ijloop_start(r, c)
    out[j * c + i] = a[i * c + j];
    __matrix_ijloop_end
}

void matrix_transf(float* out, float* a, int r, int c) {
    __matrix_ijloop_start(r, c)
    out[j * c + i] = a[i * c + j];
    __matrix_ijloop_end
}

