/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "i2c_manual.h"
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"

void startCondition()
{
    gpio_set_level(SCL_PORT_MANUAL, 1);
    gpio_set_level(SDA_PORT_MANUAL, 0);
    ets_delay_us(START_I2C_DELAY);
}

void sendBit(unsigned char Bit)
{
    if(Bit != 0)
    {
        Bit=1;
    }
    gpio_set_level(SCL_PORT_MANUAL, 0);
    ets_delay_us(SCL_PULSE_TIME/2);
    gpio_set_level(SDA_PORT_MANUAL, Bit);
    ets_delay_us(SCL_PULSE_TIME/2);
    gpio_set_level(SCL_PORT_MANUAL, 1);
    ets_delay_us(SCL_PULSE_TIME);
    gpio_set_level(SCL_PORT_MANUAL, 0);
    
}

ack_state getACK()
{
    ack_state res;
    gpio_set_level(SDA_PORT_MANUAL, 1);
    ets_delay_us(SCL_PULSE_TIME);
    gpio_set_level(SCL_PORT_MANUAL, 1);
    ets_delay_us(SCL_PULSE_TIME/2);
    if(gpio_get_level(SDA_PORT_MANUAL) != 0)
    {
        res=NO_ACK;
    }
    else
    {
        res=ACK;
    }
    ets_delay_us(SCL_PULSE_TIME/2);
    gpio_set_level(SCL_PORT_MANUAL, 0);
    return(res);
}

void stopCondition()
{
    gpio_set_level(SDA_PORT_MANUAL, 0);
    ets_delay_us(SCL_PULSE_TIME);
    gpio_set_level(SCL_PORT_MANUAL, 1);
    ets_delay_us(STOP_I2C_DELAY);
    gpio_set_level(SDA_PORT_MANUAL, 1);
    ets_delay_us(10);
}

unsigned char readBit()
{
    unsigned char res;
    gpio_set_level(SDA_PORT_MANUAL, 1);
    ets_delay_us(SCL_PULSE_TIME);
    gpio_set_level(SCL_PORT_MANUAL, 1);
    ets_delay_us(SCL_PULSE_TIME/2);
    res=gpio_get_level(SDA_PORT_MANUAL);
    ets_delay_us(SCL_PULSE_TIME/2);
    gpio_set_level(SCL_PORT_MANUAL, 0);
    return(res);
}

unsigned char readByte(ack_state Byte)
{
    unsigned char read;
    read=0;
    for(int i=8; i>0; i--)
    {
        read = read | ((readBit() & 1) << (i-1));
        
    }
    if(Byte == NO_ACK)
    {
        sendBit(1);
    }
    else
    {
        sendBit(0);
    }
    return(read);
}

ack_state writeByte(unsigned char byte)
{
    for(int t=8; t>0; t--)
    {
        sendBit(byte & (1<<(t-1)));
    }
    return getACK();
}

void initI2cMan()
{
    gpio_config_t i2cPortManual;
    i2cPortManual.pin_bit_mask = SCL_PORT_MASK| SDA_PORT_MASK ;
    i2cPortManual.mode =  GPIO_MODE_INPUT_OUTPUT_OD;
    i2cPortManual.pull_up_en = GPIO_PULLUP_ENABLE ;                                             
    i2cPortManual.pull_down_en = GPIO_PULLDOWN_DISABLE;  
    i2cPortManual.intr_type = GPIO_INTR_DISABLE; 
    gpio_config(&i2cPortManual);
    gpio_set_level(SCL_PORT_MANUAL, 1);
    gpio_set_level(SDA_PORT_MANUAL, 1);
}
