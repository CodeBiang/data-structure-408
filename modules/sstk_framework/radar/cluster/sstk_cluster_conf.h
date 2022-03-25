/**
 * @file    
 * @date    2021/5/12
 * @author  赵陈淏
 * @brief   
 */
#ifndef __SSTK_CLUSTER_CONF_H_
#define __SSTK_CLUSTER_CONF_H_

#ifndef SSTK_CLUSTER_CONF_PACKED
#error "include <cluster/sstk_cluster.h> instead of <cluster/sstk_cluster_conf.h>"
#endif

typedef struct {
    int x;

    float eps_x;
} sstk_cluster_conf_t;

#define SSTK_CLUSTER_CONF 0x01

#endif
