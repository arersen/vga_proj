//
// Created by awlik on 10.11.24.
//

#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <driver/gpio.h>

typedef struct {
    uint8_t red, green, blue;
} Color;

typedef struct {
    Color** pixel;
    uint32_t* x_pointer;
    uint32_t* y_pointer;
} framebuffer_t;

typedef struct {
    gpio_num_t gpio_red;
    gpio_num_t gpio_green;
    gpio_num_t gpio_blue;
    framebuffer_t* framebuffer;

} vga_config_t;

#endif //TYPES_H
