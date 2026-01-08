#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <string>
#include <vector>
#include <map>

#ifdef ARDUINO
#include "Arduino.h"
#include "SPI.h"
#include "Wire.h"
#endif

#include "lvgl.h"


#include "app_hal.h"
#include "config.h"
#include "hal_common.h"
