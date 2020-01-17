#include <rtdevice.h>
#include "string.h"
#include "main.h"

static rt_thread_t my_usart1_thread = RT_NULL;
char str2[] = "my_usart1_thread initialize succeed!\r\n";
struct rt_semaphore rx_sem_connect, rx_sem_uart1_self, rx_sem_uart1_ready;
int uart1_charOffset = 0, uart1_global_charOffset = 0;
char uart1_receivedStrs[100] = "\0", uart1_global_receivedStrs[100] = "\0";

//回调函数
rt_err_t uart1_input(rt_device_t dev, rt_size_t size)
{
	rt_sem_release(&rx_sem_uart1_self);

    return RT_EOK;
}


void my_usart1_thread_entry(void* parameter)
{
	rt_device_t serialuart1;
    serialuart1 = rt_device_find("uart1");
    rt_device_open(serialuart1 , RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX );
    
    rt_device_set_rx_indicate(serialuart1 , uart1_input);
    
	rt_device_t serialuart3;
	serialuart3 = rt_device_find("uart3");
	
	rt_mutex_take(print_mutex , RT_WAITING_FOREVER);
    rt_device_write(serialuart3 , 0, str2, (sizeof(str2) - 1));
	rt_mutex_release(print_mutex);
	
	rt_sem_release(&rx_sem_uart1_ready);
	
	char ch;
//	char* receivedStr;
//	*receivedStr = '\0'; 
	
	while (1)
    {
		//注意是serialuart1还是serialuart3，一开始搞错了查了好久
        while (rt_device_read(serialuart1, -1, &ch, 1) != 1)
        {
            rt_sem_take(&rx_sem_uart1_self, RT_WAITING_FOREVER);
        }
						
//		rt_kprintf(&ch);
//		rt_device_write(serialuart3, 0, &ch, 1);
		
		uart1_receivedStrs[uart1_charOffset++] = ch;
		uart1_receivedStrs[uart1_charOffset] = '\0';
		
//		*(receivedStr + (charOffset)) = ch;
//		charOffset++;
//		*(receivedStr + (charOffset)) = '\0';
		
		if (strstr(uart1_receivedStrs, "\r\n") != NULL) {
			
			
			if (my_nb101_connect_thread != NULL) {
				strcpy(uart1_global_receivedStrs, uart1_receivedStrs);
				uart1_global_charOffset = uart1_charOffset;
				rt_sem_release(&rx_sem_connect);
			} else {
				rt_mutex_take(print_mutex , RT_WAITING_FOREVER);
				rt_kprintf(uart1_receivedStrs);
				rt_device_write(serialuart3 , 0, uart1_receivedStrs, (sizeof(char) * uart1_charOffset) - 1);
				rt_mutex_release(print_mutex);
			}
			
			uart1_receivedStrs[0] = '\0';
			uart1_charOffset = 0;
		}
    }
}

void nb101_uart1(void) {
	if (my_usart1_thread != NULL) { return; }
	
	rt_sem_init(&rx_sem_connect, "rx_sem_connect", 0, RT_IPC_FLAG_FIFO);
	rt_sem_init(&rx_sem_uart1_self, "rx_sem_uart1_self", 0, RT_IPC_FLAG_FIFO);
	rt_sem_init(&rx_sem_uart1_ready, "rx_sem_uart1_ready", 0, RT_IPC_FLAG_FIFO);
	
	my_usart1_thread = rt_thread_create( "my_usart1", my_usart1_thread_entry, RT_NULL, 512, 5, 25);

	if (my_usart1_thread != RT_NULL) {
		rt_mutex_take(print_mutex , RT_WAITING_FOREVER);
		rt_kprintf(str2);
		rt_mutex_release(print_mutex);
		rt_thread_startup(my_usart1_thread);
	} else
		rt_kprintf("my_usart1_thread initialize failed!\r\n");
		return ;
}


MSH_CMD_EXPORT(nb101_uart1, a serial port that will return);
//MSH_CMD_EXPORT_ALIAS(ali_mqtt_main, ali_mqtt_test, Example: ali_mqtt_test start/pub [open/close]/stop);

