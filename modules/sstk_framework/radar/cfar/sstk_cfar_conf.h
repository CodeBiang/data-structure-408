/**
 * @file    
 * @date    2021/5/12
 * @author  赵陈淏
 * @brief   
 */
#ifndef __SSTK_CFAR_CONF_H_
#define __SSTK_CFAR_CONF_H_

#ifndef SSTK_CFAR_CONF_PACKED
#error "include <cfar/sstk_cfar.h> instead of <cfar/sstk_cfar_conf.h>"
#endif

typedef struct {
    int x;
} sstk_cfar_conf_t;

#define SSTK_CFAR_CONF 0x01

#endif
