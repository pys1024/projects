#include "common.h"
#include "lv_tools.h"

lv_indev_t *find_indev_by_type(lv_indev_type_t type)
{
  lv_indev_t *indev = NULL;
  while (true) {
    indev = lv_indev_get_next(indev);
    if (indev == NULL) {
      return NULL;
    } else if (lv_indev_get_type(indev) == type) {
      return indev;
    }
  }
}
