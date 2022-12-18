/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   init_ESP.h
 * Author: dida
 *
 * Created on July 9, 2021, 5:58 PM
 */


#include "driver/gpio.h"
#include "driver/i2c.h"
#include "stdlib.h"
#include <driver/adc.h>
#include <esp_task_wdt.h>

#include "xi2c.h"

#ifndef INIT_ESP_H
#define INIT_ESP_H

#define V_OUT    23
#define OUT_GND_NEG 33 //34
#define D_GND    32 //35
#define OUT_I    19  
#define V_BAT    18 
#define BAT_I    5
#define V_IN     4  
#define IN_I     2
#define OUT_GND_POS 15


#define V_OUT_MASK GPIO_SEL_23
#define OUT_GND_NEG_MASK GPIO_SEL_33
#define D_GND_MASK GPIO_SEL_32
#define OUT_I_MASK GPIO_SEL_19
#define V_BAT_MASK GPIO_SEL_18
#define BAT_I_MASK GPIO_SEL_5
#define V_IN_MASK GPIO_SEL_4
#define IN_I_MASK GPIO_SEL_2
#define OUT_GND_POS_MASK GPIO_SEL_15

#define I2C_DISPLAY_MASTER_SCL_IO    22   /*!< gpio number for I2C master clock */
#define I2C_DISPLAY_MASTER_SDA_IO    21    /*!< gpio number for I2C master data  */
#define I2C_DISPLAY_MASTER_NUM I2C_NUM_1   /*!< I2C port number for master dev */
#define I2C_DISPLAY_MASTER_TX_BUF_DISABLE   0   /*!< I2C master do not need buffer */
#define I2C_DISPLAY_MASTER_RX_BUF_DISABLE   0   /*!< I2C master do not need buffer */
#define I2C_DISPLAY_MASTER_FREQ_HZ    400000     /*!< I2C master clock frequency */


#ifdef __cplusplus
extern "C" {
#endif

void initSW();
void displayInit();

#ifdef __cplusplus
}
#endif



#endif /* INIT_ESP_H */

