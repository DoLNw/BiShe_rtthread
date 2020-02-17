#ifndef LED1_PG3
#define LED1_PG3

#include <rtdevice.h>

#define LED_PIN_NUM    88

void led_shinning(void *p);

void deviceConnecting(void);
void deviceConnected(void);
void deviceDisconnected(void);

#endif
