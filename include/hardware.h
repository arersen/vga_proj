//
// Created by awlik on 10.11.24.
//

#ifndef HARDWARE_H
#define HARDWARE_H

#include <driver/gpio.h>
#include <driver/ledc.h>
#include "types.h"
#include "vga_proj.h"


void set_pwm_timer  (ledc_timer_t timer_num, uint32_t freq_hz);
void set_pwm_channel(gpio_num_t gpio_num, ledc_timer_t timer_sel, ledc_channel_t channel);
void set_pwm_duty   (uint8_t duty, ledc_channel_t channel);
void set_pins_VGA   (gpio_num_t red, gpio_num_t green, gpio_num_t blue);
void set_color_VGA  (Color color);

void VGA_init(vga_config_t vga_config);

#endif //HARDWARE_H
