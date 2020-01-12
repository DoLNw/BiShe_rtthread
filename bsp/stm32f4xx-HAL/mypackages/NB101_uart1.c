#include <rtdevice.h>
#include "NB101_uart1.h"

static rt_thread_t my_usart1_thread = RT_NULL;
char str2[] = "my_usart1_thread initialize succeed!\r\n";
struct rt_semaphore rx_sem2;

//»Øµ÷º¯Êý
rt_err_t uart1_input(rt_device_t dev, rt_size_t size)
{
    rt_sem_release(&rx_sem2);

    return RT_EOK;
}


void my_usart1_thread_entry(void* parameter)
{
    rt_sem_init(&rx_sem2, "rx_sem1", 0, RT_IPC_FLAG_FIFO);
	
	rt_device_t serialuart1;
    serialuart1 = rt_device_find("uart1");
    rt_device_open(serialuart1 , RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX );
    
    rt_device_set_rx_indicate(serialuart1 , uart1_input);
    
	rt_device_t serialuart3;
	serialuart3 = rt_device_find("uart3");
    rt_device_write(serialuart3 , 0, str2, (sizeof(str2) - 1));
}

void nb101_uart1(void) {
	if (my_usart1_thread != NULL) { return; }
	
	my_usart1_thread = rt_thread_create( "my_usart1", my_usart1_thread_entry, RT_NULL, 512, 5, 25);

	if (my_usart1_thread != RT_NULL) {
		rt_thread_startup(my_usart1_thread);
		rt_kprintf(str2);
	} else
		rt_kprintf("my_usart1_thread initialize failed!\r\n");
		return ;
}


MSH_CMD_EXPORT(nb101_uart1, a serial port that will return);
//MSH_CMD_EXPORT_ALIAS(ali_mqtt_main, ali_mqtt_test, Example: ali_mqtt_test start/pub [open/close]/stop);
	
