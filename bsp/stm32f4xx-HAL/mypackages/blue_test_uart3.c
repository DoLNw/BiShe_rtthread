#include <rtdevice.h>


static rt_thread_t my_usart3_thread = RT_NULL;
char str[] = "Hello, I'm blue_test_uart3!\r\n";
struct rt_semaphore rx_sem1; 

//回调函数
rt_err_t uart3_input(rt_device_t dev, rt_size_t size)
{
    rt_sem_release(&rx_sem1);

    return RT_EOK;
}


void my_usart3_thread_entry(void* parameter)
{
    rt_sem_init(&rx_sem1, "rx_sem1", 0, RT_IPC_FLAG_FIFO);
	
	rt_device_t serialuart3;
    serialuart3 = rt_device_find("uart3");
    rt_device_open(serialuart3 , RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX );
    
    rt_device_set_rx_indicate(serialuart3 , uart3_input);
    
    rt_device_write(serialuart3 , 0, str, (sizeof(str) - 1));
	
	char ch;
	
	rt_device_t serialuart1;  
	serialuart1 = rt_device_find("uart1");

    while (1)
    {
        while (rt_device_read(serialuart3 , -1, &ch, 1) != 1)
        {
            rt_sem_take(&rx_sem1, RT_WAITING_FOREVER);
        }
        
		rt_kprintf(&ch);                //串口2，即tshell的串口，在rtconfig.h中的#define RT_CONSOLE_DEVICE_NAME "uart2"
        rt_device_write(serialuart1 , 0, &ch, 1);
		//		rt_device_write(serialuart3 , 0, &ch, 1);//给串口3发消息，由于tshell用的串口3，所以一初始化串口3就已经配置好了的，我只需要找一下设备，发送消息即可，不用再次配置了
    }
}

void blue_test_uart3(void) {
	if (my_usart3_thread != NULL) { return; }
	
	my_usart3_thread = rt_thread_create( "my_usart3", my_usart3_thread_entry, RT_NULL, 512, 5, 25);

	if (my_usart3_thread != RT_NULL) {
		rt_thread_startup(my_usart3_thread);
		rt_kprintf(str);
	} else
		rt_kprintf("my_usart3_thread initialize failed!\r\n");
        return ;
}


MSH_CMD_EXPORT(blue_test_uart3, a serial port that will return);
//MSH_CMD_EXPORT_ALIAS(ali_mqtt_main, ali_mqtt_test, Example: ali_mqtt_test start/pub [open/close]/stop);
