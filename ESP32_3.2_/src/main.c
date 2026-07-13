#include <stdio.h>  // стандартна бібліотека вводу/виводу
#include "freertos/FreeRTOS.h" // ініціалізація фриартос
#include "freertos/task.h" // 
#include "esp_adc/adc_oneshot.h"
#include "driver/gpio.h"


#define POT_1 ADC_CHANNEL_3

#define WINDOW 16
#define DARK_THRESHOLD 2000
#define LIGHT_THRESHOLD 2400

static int buffer[WINDOW] = {0};
int running_sum = 0;
int idx = 0;
int count = 0;


int moving_average(int raw){
    running_sum -= buffer[idx];
    buffer[idx]=raw;
    running_sum += raw;
    idx = (idx+1) % WINDOW;
    if (count < WINDOW) count++;
    return (running_sum/count);
}


void app_main() {

    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << GPIO_NUM_14),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&io_conf);


    adc_oneshot_unit_handle_t adc_handle;
    adc_oneshot_unit_init_cfg_t init_cfg = { .unit_id = ADC_UNIT_1, };

    adc_oneshot_new_unit(&init_cfg, &adc_handle);

    adc_oneshot_chan_cfg_t chan_cfg = {
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_12,
    };

    adc_oneshot_config_channel(adc_handle, POT_1, &chan_cfg);

    while(1){
        int raw = 0;

        adc_oneshot_read(adc_handle, POT_1, &raw);

        int filtred = moving_average(raw);

        printf("raw: %d, filtred: %d\n", raw, filtred);

    
        if (filtred < DARK_THRESHOLD)
        {
            gpio_set_level(GPIO_NUM_14, 1);
        }
        else if (filtred > LIGHT_THRESHOLD)
        {
            gpio_set_level(GPIO_NUM_14, 0);
        }

        vTaskDelay(pdMS_TO_TICKS(150));
    }
}

