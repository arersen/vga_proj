//
// Created by awlik on 10.11.24.
//

#ifndef SOFTWARE_H
#define SOFTWARE_H

#include "types.h"
#include "vga_proj.h"

void fill_color_VGA  (const vga_config_t* vga_config, Color color);
void init_framebuffer(      framebuffer_t* framebuffer);
void VGA_set_frame   (const vga_config_t* vga_config, Color** frame);

#endif //SOFTWARE_H
