#include "common.h"
#include "cst816t.h"          // capacitive touch

cst816t *touchpad = new cst816t(Wire1, TP_RST_PIN, TP_INT_PIN);

static int16_t clamp_touch_coord(uint32_t value, int16_t limit)
{
  if (limit <= 0) {
    return 0;
  }

  uint32_t max_value = (uint32_t)(limit - 1);
  if (value > max_value) {
    value = max_value;
  }

  return (int16_t)value;
}

/*Read the touchpad*/
void my_touchpad_read(lv_indev_t *indev, lv_indev_data_t *data)
{
#if 1
  static uint32_t tp_x = 0;
  static uint32_t tp_y = 0;
  static uint32_t tp_fingers = 0;
  if (touchpad->available()) {
    tp_x = touchpad->x;
    tp_y = touchpad->y;
    tp_fingers = touchpad->finger_num;
  }
  data->point.x = clamp_touch_coord(tp_x, LCD_WIDTH);
  data->point.y = clamp_touch_coord(tp_y, LCD_HEIGHT);
  if (tp_fingers != 0) {
    data->state = LV_INDEV_STATE_PRESSED;
  } else {
    data->state = LV_INDEV_STATE_RELEASED;
  }

#else
  if (touchpad->available()) {
    tft->setCursor(touchpad->x, touchpad->y);
    tft->fillScreen(BLACK);
    switch (touchpad->gesture_id) {
      case GESTURE_NONE:
        tft->print("NONE");
        break;
      case GESTURE_SWIPE_DOWN:
        tft->print("SWIPE DOWN");
        break;
      case GESTURE_SWIPE_UP:
        tft->print("SWIPE UP");
        break;
      case GESTURE_SWIPE_LEFT:
        tft->print("SWIPE LEFT");
        break;
      case GESTURE_SWIPE_RIGHT:
        tft->print("SWIPE RIGHT");
        break;
      case GESTURE_SINGLE_CLICK:
        tft->print("SINGLE CLICK");
        break;
      case GESTURE_DOUBLE_CLICK:
        tft->print("DOUBLE CLICK");
        break;
      case GESTURE_LONG_PRESS:
        tft->print("LONG PRESS");
        break;
      default:
        tft->print("?");
        break;
    }
  }
#endif
}

bool lv_port_indev_init(void)
{
  // Init touch device
  Wire1.begin(TP_SDA_PIN, TP_SCK_PIN, I2C_SPEED);
  touchpad->begin(mode_change);

  DBLOG("Touchpad check OK!");

  /*Initialize the (dummy) input device driver*/
  lv_indev_t *indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER); /*Touchpad should have POINTER type*/
  lv_indev_set_read_cb(indev, my_touchpad_read);

  return true;
}
