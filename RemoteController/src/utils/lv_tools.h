#pragma once

#ifdef __cplusplus
extern "C" {
#endif

lv_indev_t *find_indev_by_type(lv_indev_type_t type);
int64_t my_map(int64_t x, int64_t in_min, int64_t in_max, int64_t out_min, int64_t out_max);

#ifdef __cplusplus
}
#endif
