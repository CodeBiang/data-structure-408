/**
 * @file    
 * @date    2021/5/12
 * @author  赵陈淏
 * @brief   
 */
#ifndef __SSTK_RECT_
#define __SSTK_RECT_

#include <sstk_geo.h>

typedef struct {
    float xp; // 左上角点
    float yp;
    float w;
    float h;
} sstk_rect_t;

#define sstk_rect_move_center(rect, cx, cy) \
    (rect)->xp = cx - (rect)->w / 2;\
    (rect)->yp = cy - (rect)->h / 2;

#define sstk_rect_left(rect) (rect)->xp

#define sstk_rect_right(rect) ((rect)->xp + w)

#define sstk_rect_top(rect) ((rect)->yp)

#define sstk_rect_bottom(rect) ((rect)->yp + h)

#endif
