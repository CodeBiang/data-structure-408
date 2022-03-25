/**
 * @file    
 * @date    2021/5/12
 * @author  赵陈淏
 * @brief   
 */
#ifndef __SSTK_MATH_CONF_H_
#define __SSTK_MATH_CONF_H_

#include <math.h>

/*!-- 是否编译几何库 --*/
#define SSTK_MATH_ENABLE_GEO        1
/*!-- 是否编译矩阵计算库 --*/
#define SSTK_MATH_ENABLE_MATRIX     1

#define sstk_fmul(x, y) (x * y)
#define sstk_fdiv(x, y) (x / y)  
#define sstk_mul(x, y) (x * y) 
#define sstk_div(x, y) (x / y)

#ifndef sstk_abs
#define sstk_abs(value) (((value) >= 0) ? (value) : -(value))
#endif

#define sstk_sin(d) sin(d)
#define sstk_sinf(f) sinf(f)
#define sstk_cos(d) cos(d)
#define sstk_cosf(f) cosf(f)
#define sstk_tan(d) tan(d)
#define sstk_tanf(f) tanf(f)

#define sstk_sinh(d) sinh(d)
#define sstk_sinhf(f) sinhf(f)
#define sstk_cosh(d) cosh(d)
#define sstk_coshf(f) coshf(f)
#define sstk_tanh(d) tanh(d)
#define sstk_tanhf(f) tanhf(f)

#define sstk_asin(d) asin(d)
#define sstk_asinf(f) asinf(f)
#define sstk_acos(d) acos(d)
#define sstk_acosf(f) acosf(f)
#define sstk_atan(d) atan(d)
#define sstk_atanf(f) atanf(f)
#define sstk_atan2(d) atan2(d)
#define sstk_atan2f(f) atan2f(f)

#define sstk_pow(x, n) pow(x, n)
#define sstk_powf(x, n) powf(x, n)

#define sstk_exp(x, n) exp(x, n)
#define sstk_expf(x, n) expf(x, n)

#define sstk_mlog(d) log(d)
#define sstk_mlogf(f) logf(f)
#define sstk_mlog10(d) log(d)
#define sstk_mlog10f(f) logf(f)

#define sstk_sqrt(d) sqrt(d)
#define sstk_sqrtf(f) sqrtf(f)

#endif
