/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "init_ESP.h"

void initSW()
{
    gpio_config_t initSW;
    initSW.pin_bit_mask =  V_OUT_MASK | OUT_GND_NEG_MASK | D_GND_MASK | OUT_I_MASK | V_BAT_MASK | BAT_I_MASK | V_IN_MASK | IN_I_MASK | OUT_GND_POS_MASK;
    initSW.mode =  GPIO_MODE_OUTPUT;
    initSW.pull_up_en = GPIO_PULLUP_DISABLE;                                             
    initSW.pull_down_en = GPIO_PULLDOWN_DISABLE;  
    initSW.intr_type = GPIO_INTR_DISABLE; 
    gpio_config(&initSW);
}


void displayInit()
{
    int i2c_master_port = I2C_DISPLAY_MASTER_NUM;
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_DISPLAY_MASTER_SDA_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = I2C_DISPLAY_MASTER_SCL_IO;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_DISPLAY_MASTER_FREQ_HZ;
    i2c_param_config(i2c_master_port, &conf);
    i2c_driver_install(i2c_master_port, conf.mode,
                       I2C_DISPLAY_MASTER_RX_BUF_DISABLE,
                       I2C_DISPLAY_MASTER_TX_BUF_DISABLE, 0);
    
    printf("init displ\n");
}

