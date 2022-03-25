#include <cfar/sstk_cfar.h>
#include <sstk_math.h>

#include "sstk_cluster.h"
static inline int find_neighbors(sstk_cluster_t* cluster,
                                 sstk_cfar_t** cfars, int idle, int start,
                                 int end);
static inline void init_region(sstk_cluster_related_region_t* region,
                               const sstk_cfar_t* cfar);
static inline void update_region(sstk_cluster_related_region_t* region,
                                 const sstk_cfar_t* cfar);
static inline void append_point(sstk_cluster_t* cluster, sstk_cfar_t* c);

static inline sstk_bool_t is_region_related(const sstk_cluster_related_region_t* r, const sstk_cfar_t* c) {
    return c->x >= r->region_x_min 
           && c->x <= r->region_x_max
           && c->y >= r->region_y_min 
           && c->y <= r->region_y_max
           && c->v_idx >= r->region_v_idx_min
           && c->v_idx <= r->region_v_idx_max;
}

sstk_uint_t sstk_cluster_process(sstk_cfar_t** in, sstk_uint_t in_cnt,
                                 sstk_cluster_t* clusters, sstk_uint_t capacity,
                                 sstk_cluster_t** out, sstk_conf_t* conf,
                                 const sstk_radar_conf_ctx_t* rconf) {
    sstk_uint_t end_index = in_cnt;
    sstk_uint_t cur_end_index = -1;
    sstk_uint_t cur_start_index = -1;
    sstk_uint_t idle_index = 0;
    sstk_uint_t add_points_cnt = 0;
    sstk_uint_t out_cnt = 0;
    sstk_uint_t nelts = 0;

    if (in_cnt == 0) return 0;

    while (cur_end_index < end_index) {
        if (out_cnt >= capacity) break;
        if (idle_index >= in_cnt) break;

        init_region(&clusters[out_cnt].related_region, in[idle_index]);
        cur_start_index = idle_index;
        cur_end_index = idle_index;
        idle_index++;

        do {
            add_points_cnt = find_neighbors(&clusters[out_cnt], in, cur_end_index,
                                            idle_index, end_index);

            cur_end_index += add_points_cnt;
            idle_index += add_points_cnt;
        } while (idle_index < end_index && add_points_cnt > 0);

        nelts = cur_end_index - cur_start_index + 1;

        /* why 2? */
        if (nelts >= 2) {
            out[out_cnt] = &clusters[out_cnt];
            out_cnt++;
        }
    }

    return out_cnt;
}

int find_neighbors(sstk_cluster_t* cluster, sstk_cfar_t** cfars,
                   int cur, int idle_start, int idle_end) {
    int cnt = 0, i;
    sstk_cluster_related_region_t* r = &cluster->related_region;
    sstk_cfar_t* swap_tmp;
    sstk_bool_t related = false;

    for (i = idle_start; i < idle_end; i++) {
        if (sstk_abs(cfars[cur]->r_idx - cfars[i]->r_idx) >= 40) {
            // 保证ridx有序后，过大跳出循环 
            break;
        }

        related = is_region_related(r, cfars[i]);
        if (related) {
            
            // 吞并
            append_point(cluster, cfars[i]);

            // 更新区域
            update_region(r, cfars[i]);

            // 交换以连续, TODO 需要改
            swap_tmp = cfars[i];
            cfars[i] = cfars[idle_start + cnt];
            cfars[idle_start + cnt] = swap_tmp;

            cnt++;
        }
    }

    return cnt;
}

void init_region(sstk_cluster_related_region_t* r, const sstk_cfar_t* c) {
    // TODO 门限即使是与速度相关, 也不能与聚类逻辑产生耦合, 需要改
    if (sstk_abs(c->speed) < 12) {
        r->y_th = 1.8;
    } else {
        r->y_th = 3;
    }

    r->x_th = 1.0F;
    r->v_idx_th = 2;

    r->region_x_min = c->x - r->x_th;
    r->region_x_max = c->x + r->x_th;
    r->region_y_min = c->y - r->y_th;
    r->region_y_max = c->y + r->y_th;
    r->region_v_idx_min = c->v_idx - r->v_idx_th;
    r->region_v_idx_max = c->v_idx + r->v_idx_th;

    r->angle_min = r->angle_max = c->azimuth;
    r->v_idx_min = r->v_idx_max = c->v_idx;
    r->r_idx_min = r->r_idx_max = c->r_idx;
    r->amplitude = c->amplitude;

    r->rect.l = c->x;
    r->rect.t = c->y;
    r->rect.r = c->x;
    r->rect.b = c->y;
}

void update_region(sstk_cluster_related_region_t* r, const sstk_cfar_t* c) {
    r->region_x_min = sstk_min(r->region_x_min, c->x - r->x_th);
    r->region_x_max = sstk_max(r->region_x_max, c->x + r->x_th);
    r->region_y_min = sstk_min(r->region_y_min, c->y - r->y_th);
    r->region_y_max = sstk_max(r->region_y_max, c->y + r->y_th);
    r->region_v_idx_min = sstk_min(r->region_v_idx_min, c->v_idx - r->v_idx_th);
    r->region_v_idx_max = sstk_max(r->region_v_idx_max, c->v_idx + r->v_idx_th);
    
    r->angle_min = sstk_min(r->angle_min, c->azimuth);
    r->angle_max = sstk_max(r->angle_max, c->azimuth);
    r->r_idx_min = sstk_min(r->r_idx_min, c->r_idx);
    r->r_idx_max = sstk_max(r->r_idx_max, c->r_idx);
    r->v_idx_min = sstk_min(r->v_idx_min, c->v_idx);
    r->v_idx_max = sstk_max(r->v_idx_max, c->v_idx);
    
    r->rect.l = sstk_min(r->rect.l, c->x);
    r->rect.t = sstk_max(r->rect.t, c->y);
    r->rect.r = sstk_max(r->rect.r, c->x);
    r->rect.b = sstk_min(r->rect.b, c->y);
}

void append_point(sstk_cluster_t* cluster, sstk_cfar_t* c) {
    if (cluster->pts_tail) {
        cluster->pts_tail->next = c;
        c->next = NULL;
    } else {
        cluster->pts = c;
        cluster->pts_tail = c;
        cluster->pts_cnt = 0;
        c->next = NULL;
    }
    cluster->pts_cnt++;
}
