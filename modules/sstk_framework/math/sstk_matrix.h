/**
 * @file    
 * @date    2021/5/12
 * @author  赵陈淏
 * @brief   
 */
#ifndef __SSTK_MATRIX_H_
#define __SSTK_MATRIX_H_

#include <sstk_math.h>

void matrix_add(double* out, double* a, double* b, int r, int c);
void matrix_addf(float* out, float* a, float* b, int r, int c);
void matrix_sub(double* out, double* a, double* b, int r, int c);
void matrix_subf(float* out, float* a, float* b, int r, int c);

void matrix_addn(double* out, double* a, double n, int r, int c);
void matrix_addnf(float* out, float* a, float n, int r, int c);
void matrix_subn(double* out, double* a, double n, int r, int c);
void matrix_subnf(float* out, float* a, float n, int r, int c);
void matrix_muln(double* out, double* a, double n, int r, int c);
void matrix_mulnf(float* out, float* a, float n, int r, int c);
void matrix_divn(double* out, double* a, double n, int r, int c);
void matrix_divnf(float* out, float* a, float n, int r, int c);

void matrix_mul(double* out, double* a, double* b, int ar, int ac, int bc);
void matrix_mulf(float* out, float* a, float* b, int ar, int ac, int bc);

void matrix_trans(double* out, double* a, int r, int c);
void matrix_transf(float* out, float* a, int r, int c);

void matrix_inverse(double* out, double* a, int r, int c);
void matrix_inversef(float* out, float* a, int r, int c);

#define matrix_declare(r, c) double[r * c]
#define matrix_declaref(r, c) float[r * c]


#endif
