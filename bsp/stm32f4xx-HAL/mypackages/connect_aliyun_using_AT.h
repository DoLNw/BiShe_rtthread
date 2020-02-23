#ifndef CONNECT_ALIYUN_USING_AT
#define CONNECT_ALIYUN_USING_AT

#include <rtdevice.h>

extern rt_thread_t my_nb101_connect_thread;
extern char uploadOnandDoorOnDataStr[];
extern char uploadOnandDoorOffDataStr[];

void connect_aliyun(void);

#endif
