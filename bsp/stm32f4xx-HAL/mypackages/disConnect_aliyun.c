#include <rtdevice.h>
#include "main.h"

char atStr8[] = "AT+QMTDISC=0\r\n\0"; 
//¶Ï¿ªÁ¬½Ós

void disConnect_aliyun(void) {
	if (my_nb101_connect_thread != NULL) {
		rt_thread_delete(my_nb101_connect_thread);
		my_nb101_connect_thread = NULL;
	}
	
	rt_mutex_take(print_mutex , RT_WAITING_FOREVER);
	rt_device_write(serialuart1 , 0, atStr8, (sizeof(atStr8)));
	rt_mutex_release(print_mutex);
	
	deviceDisconnected();
}

MSH_CMD_EXPORT(disConnect_aliyun, let nb101 connect to aliyun with the help of uart1);
