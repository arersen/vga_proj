//
// Created by awlik on 10.11.24.
//

#include "software.h"


void fill_color_VGA(const vga_config_t* vga_config, const Color color) {
    for(int y = 0; y < HEIGHT; y++) {
        for(int x = 0; x < WIDTH; x++) {
            vga_config->framebuffer->pixel[HEIGHT][WIDTH] = color;
        }
    }
}

void init_framebuffer(framebuffer_t* framebuffer) {
    framebuffer->x_pointer = (uint32_t*)malloc(sizeof(uint32_t) * WIDTH);
    framebuffer->y_pointer = (uint32_t*)malloc(sizeof(uint32_t) * HEIGHT);

    framebuffer->pixel = (Color**)malloc(sizeof(Color*) * HEIGHT);

    for(int y = 0; y < HEIGHT; y++) {
        framebuffer->pixel[y] = (Color*)malloc(sizeof(Color) * WIDTH);
    }
}

void VGA_set_frame(const vga_config_t* vga_config, Color** frame) {
    vga_config->framebuffer->pixel = frame;
}