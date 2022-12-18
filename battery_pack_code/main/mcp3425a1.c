/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "mcp3425a1.h"


const float adc_err=1.0033534;
void write_adc(uint8_t *command) {
    int ret;
    esp_task_wdt_reset();
    ret = XI2CWrite(I2C_NUM_1, MCP_ADDR, command, 1);
    if (ret == ESP_FAIL) {
        printf("I2C Fail wr\n");
    }

}

void read_adc(uint8_t *data_rd) {
    int ret;
    esp_task_wdt_reset();
    ret = XI2CRead(I2C_NUM_1, MCP_ADDR, data_rd, 4);
    if (ret == ESP_FAIL) {
        printf("I2C Fail rd\n");
    }

}

float meassureMCP(SAMPLERATE sample, GAINSELECTOR gain) {
    uint8_t data_read[4];
    int16_t adc_res;
     float lsb_volt = 0;
     float volt_res = 0;
     float gain_pick = 0;
     unsigned int a=0;
    uint8_t command = (0b1000 << 4) | (sample << 2) | gain;
    write_adc(&command);
    do {
        a++;
        vTaskDelay(4/portTICK_PERIOD_MS);
        read_adc(data_read);
    } while (((data_read[3] & 0b10000000) != 0) && (a<20));

    adc_res = data_read[0];
    adc_res = adc_res << 8;
    adc_res = adc_res & 0xff00;
    adc_res = adc_res | data_read[1];
    switch (sample) {
        case SPS_240:
            lsb_volt = 0.001;
            break;

        case SPS_60:
            lsb_volt = 0.00025;
            break;

        case SPS_15:
            lsb_volt = 0.0000625;
            break;

        default:
            lsb_volt=1;
            break;
    }
    switch (gain)
    {
        case PGA_1:
            gain_pick = 1;
            break;
        case PGA_2:
            gain_pick = 2;
            break;
        case PGA_4:
            gain_pick = 4;
            break;
        case PGA_8:
            gain_pick = 8;
            break;
        default:
            gain_pick=99;
            break;
        
    }
    volt_res =( (float) (adc_res) * lsb_volt * adc_err) / gain_pick;
    return volt_res; 
}