#ifndef RELAY_SWITCH
#define RELAY_SWITCH

#include <rtdevice.h>

#define RELAY_SWITCH_NUM 1

void init_switch(void);
void open_switch(void);
void close_switch(void);

#endif