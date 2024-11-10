#include <stdio.h>

// Created by awlik 10.11.2024

#include "vga_proj.h"



void app_main(void)
{

    /*
    *typedef struct {
    gpio_num_t gpio_red;
    gpio_num_t gpio_green;
    gpio_num_t gpio_blue;
    framebuffer_t* framebuffer;

    } vga_config_t;
    */


    vga_config_t vga_config = {
        .gpio_red = GPIO_RED,
        .gpio_green = GPIO_GREEN,
        .gpio_blue = GPIO_BLUE,
    };

    VGA_init(vga_config);

}
