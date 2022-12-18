/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   traffic_light.h
 * Author: dida
 *
 * Created on 27 Юни 2020, 15:24
 */

#ifndef TRAFFIC_LIGHT_H
#define TRAFFIC_LIGHT_H

#ifdef __cplusplus
extern "C" {
#endif




#ifdef __cplusplus
}
#endif
#define RED_IO 13
#define GREEN_IO  14
#define YELLOW_IO 12
#define RED_SELECT_MASK GPIO_SEL_13
#define GREEN_SELECT_MASK GPIO_SEL_14
#define YELLOW_SELECT_MASK GPIO_SEL_12
#define LIGHT_TIME_MS 1000

void initTrafficLight();

void trafficLightThread();

#endif /* TRAFFIC_LIGHT_H */

