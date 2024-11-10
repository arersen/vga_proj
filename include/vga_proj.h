//
// Created by awlik on 10.11.24.
//

#ifndef VGA_PROJ_H
#define VGA_PROJ_H

#include <esp_timer.h>
#include <driver/gpio.h>
#include <esp_attr.h>
#include <driver/ledc.h>
#include "types.h"
#include <malloc.h>


#define GPIO_HSYNC_OSCILLATOR GPIO_NUM_0

#define GPIO_HSYNC GPIO_NUM_1
#define GPIO_VSYNC GPIO_NUM_2

#define GPIO_RED   GPIO_NUM_3
#define GPIO_GREEN GPIO_NUM_4
#define GPIO_BLUE  GPIO_NUM_5

#define WIDTH  640
#define HEIGHT 480




#include "hardware.h"
#include "software.h"



#endif //VGA_PROJ_H
