#ifndef MAIN
#define MAIN
#include "NB101_uart1.h"
#include "connect_aliyun_using_AT.h"
#include "LED1_PG3.h"
#include "post_to_aliyun.h"
#include "blue_test_uart3.h"

extern rt_mutex_t print_mutex;
extern rt_device_t serialuart1;
extern rt_device_t serialuart3;

void nb101_uart1(void);

#endif




