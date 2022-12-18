
#include "traffic_light.h"
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "freertos/task.h"
#include <esp_task_wdt.h>

void initTrafficLight()
{
    gpio_config_t traficIO;
    traficIO.pin_bit_mask =  RED_SELECT_MASK | GREEN_SELECT_MASK | YELLOW_SELECT_MASK;
    traficIO.mode =  GPIO_MODE_OUTPUT;
    traficIO.pull_up_en = GPIO_PULLUP_DISABLE;                                             
    traficIO.pull_down_en = GPIO_PULLDOWN_DISABLE;  
    traficIO.intr_type = GPIO_INTR_DISABLE; 
    gpio_config(&traficIO);
}

void lightOnOff(gpio_num_t light)
{
    gpio_set_level(light, 1);
    vTaskDelay(LIGHT_TIME_MS/portTICK_PERIOD_MS);
    gpio_set_level(light, 0);
}

void trafficLightThread()
{
    while(1)
    {
        esp_task_wdt_reset();
        
        lightOnOff(RED_IO);
        lightOnOff(YELLOW_IO);
        lightOnOff(GREEN_IO);
    }
}