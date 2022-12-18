/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   mcp3425a1.h
 * Author: dida
 *
 * Created on July 16, 2021, 5:19 PM
 */


#include <esp_task_wdt.h>

#include "xi2c.h"

#ifndef MCP3425A1_H
#define MCP3425A1_H

#define MCP_ADDR 0b1101001

   typedef enum {
        SPS_240=00,
        SPS_60,
        SPS_15
    }SAMPLERATE;
    
    typedef enum {
        PGA_1=00,
        PGA_2,
        PGA_4,
        PGA_8
    }GAINSELECTOR;

    
    
    void write_adc(uint8_t *command);

    void read_adc(uint8_t *data_rd);

    float meassureMCP(SAMPLERATE sample, GAINSELECTOR gain);


#ifdef __cplusplus
}
#endif

#endif /* MCP3425A1_H */

