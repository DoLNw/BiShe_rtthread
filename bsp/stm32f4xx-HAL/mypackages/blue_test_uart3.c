//HC-02 write 2 4 notify 2 3
//HC-42 write 1 1 notify 1 1


#include <rtdevice.h>
#include "string.h"
#include "main.h"

static rt_thread_t my_usart3_thread = RT_NULL;
char str[] = "my_usart3_thread initialize succeed!\r\n";
struct rt_semaphore rx_sem_uart3_self; 

//回调函数
rt_err_t uart3_input(rt_device_t dev, rt_size_t size)
{
    rt_sem_release(&rx_sem_uart3_self);

    return RT_EOK;
}


void my_usart3_thread_entry(void* parameter)
{
    rt_sem_init(&rx_sem_uart3_self, "rx_sem1", 0, RT_IPC_FLAG_FIFO);
	
    rt_device_open(serialuart3 , RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX );
    
    rt_device_set_rx_indicate(serialuart3 , uart3_input);
    
	rt_mutex_take(print_mutex , RT_WAITING_FOREVER);
    rt_device_write(serialuart3 , 0, str, (sizeof(str) - 1));
	rt_mutex_release(print_mutex);
	
	
	char ch;
	int uart3_charOffset = 0;
	char uart3_receivedStrs[100] = "\0";
	
    while (1)
    {
        while (rt_device_read(serialuart3 , -1, &ch, 1) != 1)
        {
            rt_sem_take(&rx_sem_uart3_self, RT_WAITING_FOREVER);
        }
		
//		rt_device_write(serialuart1, 0, &ch, 1);
		
		uart3_receivedStrs[uart3_charOffset++] = ch;
		uart3_receivedStrs[uart3_charOffset] = '\0';
		
		if (strstr(uart3_receivedStrs, "\r\n") != NULL) {
			rt_mutex_take(print_mutex , RT_WAITING_FOREVER);
			rt_kprintf(uart3_receivedStrs);
			rt_device_write(serialuart1, 0, uart3_receivedStrs, (sizeof(char) * uart3_charOffset) - 1);
			rt_device_write(serialuart3, 0, uart3_receivedStrs, (sizeof(char) * uart3_charOffset) - 1);
			rt_mutex_release(print_mutex);
			
			uart3_receivedStrs[0] = '\0';
			uart3_charOffset = 0;
		}
		
    }
}

void blue_test_uart3(void) {
	if (my_usart3_thread != NULL) { return; }
	
	my_usart3_thread = rt_thread_create( "my_usart3", my_usart3_thread_entry, RT_NULL, 512, 5, 25);

	if (my_usart3_thread != RT_NULL) {
		rt_mutex_take(print_mutex , RT_WAITING_FOREVER);
		rt_kprintf(str);               //注意这个打印语句看似在一个不会被打断的blue_test_uart3()函数里面，但是实际上我是在tshell中调用该函数，所以该函数的运行还是会被齐其它高优先线程打断，比如这个打印，所以我打印都用一个全局的互斥量来互斥住。
		rt_mutex_release(print_mutex);
		
		rt_thread_startup(my_usart3_thread);
	} else
		rt_kprintf("my_usart3_thread initialize failed!\r\n");
        return ;
}


MSH_CMD_EXPORT(blue_test_uart3, a serial port that will return);
//MSH_CMD_EXPORT_ALIAS(ali_mqtt_main, ali_mqtt_test, Example: ali_mqtt_test start/pub [open/close]/stop);
