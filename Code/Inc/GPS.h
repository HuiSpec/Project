#ifndef __GPS_H
#define __GPS_H

#include "GPS_Usart.h"
#include "stm32f10x.h"

// void data_copy();
void GPS_init();
void GPS_get();
void GPS_close();

#endif