#include "GPS.h"
// #include "Usart.h"
#include "GPS_Usart.h"
#include "string.h"

void GPS_init()
{
    USART2_Init();
}

void GPS_get()
{
    USART2_SET(1);
}
void GPS_close()
{
    USART2_SET(0);
}