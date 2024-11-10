//
// Created by awlik on 10.11.24.
//

#include "hardware.h"

void set_pwm_timer(ledc_timer_t timer_num, uint32_t freq_hz) {
    // Конфігурація таймера ШІМ
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_LOW_SPEED_MODE,   // Швидкість для генерації сигналу
        .timer_num        = timer_num,          // Номер таймера
        .duty_resolution  = LEDC_TIMER_8_BIT,     // Роздільна здатність (8 біт)
        .freq_hz          = freq_hz,                  // Частота ШІМ (Гц)
        .clk_cfg          = LEDC_AUTO_CLK
      };
    ledc_timer_config(&ledc_timer);
}

void set_pwm_channel(gpio_num_t gpio_num, ledc_timer_t timer_sel, ledc_channel_t channel) {


    // Конфігурація каналу ШІМ
    ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_LOW_SPEED_MODE,
        .channel        = channel,         // Номер каналу
        .timer_sel      = timer_sel,           // Використовуємо налаштований вище таймер
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = gpio_num,               // Номер GPIO піну для ШІМ
        .duty           = 0,                      // Початковий duty cycle (заповнення)
        .hpoint         = 0
      };
    ledc_channel_config(&ledc_channel);
}

void set_pwm_duty(uint8_t duty, ledc_channel_t channel) {
    ledc_set_duty(LEDC_LOW_SPEED_MODE, channel, duty);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, channel);
}



void set_pins_VGA(gpio_num_t red, gpio_num_t green, gpio_num_t blue) {
    set_pwm_channel(red,   LEDC_TIMER_0, LEDC_CHANNEL_0);
    set_pwm_channel(green,  LEDC_TIMER_0, LEDC_CHANNEL_1);
    set_pwm_channel(blue, LEDC_TIMER_0, LEDC_CHANNEL_2);
}

void set_color_VGA(Color color) {
    set_pwm_duty(color.red, LEDC_CHANNEL_0);
    set_pwm_duty(color.green, LEDC_CHANNEL_1);
    set_pwm_duty(color.blue, LEDC_CHANNEL_2);
}

static void IRAM_ATTR v_sync_callback(void* arg);
static void IRAM_ATTR h_sync_isr_handler(void* arg);

void VGA_init(vga_config_t vga_config) {


    set_pwm_timer(LEDC_TIMER_0, 1000000);
    set_pins_VGA(vga_config.gpio_red, vga_config.gpio_green, vga_config.gpio_blue);

    Color color = {0, 255, 0};
    init_framebuffer(vga_config.framebuffer);
    fill_color_VGA(&vga_config, color);

    // Ініціалізація таймера для Vsync (період 16.67 мс)
    esp_timer_create_args_t timer_args_vsync = {
        .callback = &v_sync_callback,
        .arg = &vga_config,
        .dispatch_method = ESP_TIMER_TASK,
        .name = "Vsync_Timer"
    };

    // Таймери для Hsync і Vsync
    esp_timer_handle_t v_sync_timer;
    esp_timer_create(&timer_args_vsync, &v_sync_timer);

    // Запуск таймера для Vsync
    esp_timer_start_periodic(v_sync_timer, 35);

    /*
     *
     *  60 Гц
     *  16,7 мс період
     *  16,7 мс / 480 = приблизно 35 мікросекунд
     *
     *  /
     *
     *  */




    // Ініціалізація GPIO для вводу
    gpio_config_t oscillator_conf = {
        .pin_bit_mask = (1ULL << GPIO_HSYNC),
        .mode = GPIO_MODE_INPUT, // Встановлюємо пін як вхід
        .intr_type = GPIO_INTR_POSEDGE, // Встановлюємо переривання на позитивний фронт
        .pull_up_en = GPIO_PULLUP_ENABLE, // Підтягуємо пін
        .pull_down_en = GPIO_PULLDOWN_DISABLE // Вимкнення підсосу
    };
    gpio_config(&oscillator_conf);

    // Реєстрація переривання
    gpio_install_isr_service(ESP_INTR_FLAG_LEVEL1);
    gpio_isr_handler_add(GPIO_HSYNC, h_sync_isr_handler, &vga_config);

    /*
     *ПРоблема в тому що для генерації одного пікселя при роздільній здатності 640х480 60гц нам потрібен таймер
     *з частотою 18,5 МГц, оскільки таких апаратних таймерів в данному мікроконтроллері немає, нам треба
     *скористатися кварцовим генератором з частотою 18,5 МГц, за допомогою нього ми можемо вловлювати
     *переривання на піні мікроконтроллера, таким чином ми можем робити горизонтальну синхронізацію
     *
     */


}

// Обробник для Vsync
static void IRAM_ATTR v_sync_callback(void* arg) {
    // Генерація імпульсу Vsync
    gpio_set_level(GPIO_VSYNC, 0); // Логічний нуль на Vsync


    vga_config_t* vga_config = (vga_config_t*)arg;

    if(++*vga_config->framebuffer->y_pointer == HEIGHT) vga_config->framebuffer->y_pointer = 0;

    gpio_set_level(GPIO_VSYNC, 1); // Завершення імпульсу Vsync
}

// Обробник для Hsync
static void IRAM_ATTR h_sync_isr_handler(void* arg) {
    gpio_set_level(GPIO_HSYNC, 0);


    vga_config_t* vga_config = (vga_config_t*)arg;


    set_color_VGA(vga_config->framebuffer->pixel[*vga_config->framebuffer->y_pointer][*vga_config->framebuffer->x_pointer]);
    if(++*vga_config->framebuffer->x_pointer == WIDTH) vga_config->framebuffer->x_pointer = 0;

    gpio_set_level(GPIO_HSYNC, 1);

}
