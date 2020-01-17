#include <rtdevice.h>
#include "main.h"

char atStr8[] = "AT+QMTDISC=0\r\n\0"; 
//¶Ï¿ªÁ¬½Ós

void disConnect_aliyun(void) {
	rt_device_t serialuart1;
	serialuart1 = rt_device_find("uart1");
	
	rt_mutex_take(print_mutex , RT_WAITING_FOREVER);
	rt_device_write(serialuart1 , 0, atStr8, (sizeof(atStr8)));
	rt_mutex_release(print_mutex);
}

MSH_CMD_EXPORT(disConnect_aliyun, let nb101 connect to aliyun with the help of uart1);