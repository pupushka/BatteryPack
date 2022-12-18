/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   i2c_manual.h
 * Author: dida
 *
 * Created on 27 Юни 2020, 14:36
 */

#ifndef I2C_MANUAL_H
#define I2C_MANUAL_H

#ifdef __cplusplus
extern "C" {
#endif




#ifdef __cplusplus
}
#endif

//To be changed -- BEGIN --
#define SCL_PORT_MANUAL 23 //SCL output port definition
#define SDA_PORT_MANUAL 19 //SDA output port definition
#define SCL_PORT_MASK GPIO_SEL_23 //SCL output port mask definition
#define SDA_PORT_MASK GPIO_SEL_19 //SDA output port mask definition
//To be changed -- END --

#define START_I2C_DELAY 50
#define SCL_PULSE_TIME  30
#define STOP_I2C_DELAY 50
typedef enum{
        ACK,
        NO_ACK    
    }ack_state;
    
    void startCondition();
    void stopCondition();
    unsigned char readByte(ack_state Byte);
    ack_state writeByte(unsigned char byte);
    void initeMLXsensor();
    void initI2cMan();
#endif /* I2C_MANUAL_H */

