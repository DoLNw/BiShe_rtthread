#include <rtdevice.h>
#include "string.h"
#include "main.h"

char uploadOnDataStr[] = "AT+QMTPUB=0,0,0,0,\"/a1n5qqGX7PA/NB101/user/update\"\r\n{\"statue\": 1, \"deviceName\": \"NB101\"}";
//char uploadOffDataStr[] = "AT+QMTPUB=0,0,0,0,\"/a1n5qqGX7PA/NB101/user/update\"\r\n{\"statue\": 1, \"deviceName\": \"NB101\"}";


rt_thread_t my_nb101_connect_thread = RT_NULL;
int countChars(char* s);
char str3[] = "my_nb101_connect_thread initialize succeed!\r\n";
int isFirst = 1;

char atStr0[] = "Let's connect to aliyun.\r\n";
char atStr1[] = "AT+CGATT?\r\n";
char atStr2[] = "AT+CSQ\r\n";
char atStr3[] = "AT+QMTCFG=\"aliauth\",0,\"a1n5qqGX7PA\",\"NB101\",\"5iSvkRRHlzzWc9HZMvlfGdKPG8g9AFBd\"\r\n";
char atStr4[] = "AT+QMTOPEN=0,\"iot-as-mqtt.cn-shanghai.aliyuncs.com\",1883\r\n";//（AT+QMTCLOSE=0\r\n）
char atStr5[] = "AT+QMTCONN=0,\"clientExample\"\r\n";
char atStr6[] = "AT+QMTSUB=0,1,\"/a1n5qqGX7PA/NB101/user/get\",0\r\n";//订阅
char atStr7[] = "AT+QMTPUB=0,0,0,0,\"/sys/a1n5qqGX7PA/NB101/thing/event/property/post\"\r\n";//发布
//{"params" : {"ContactState": 1, "Error": 0}}
//上面这个发送就可以了
//按住CTRL+Z结束
//这段是rtthread里面拷贝的
////"{"id" : "%d","version":"1.0","params" : "
////            "{"RGBColor" : {"Red":%d,"Green":%d,"Blue":%d},"
////            ""LightSwitch" : %d},"
////            ""method":"thing.event.property.post"}"




char* stepStrs[10] = {
	"Let's connect to aliyun.\r\n",
	"AT+CGATT?\r\n",
	"AT+CSQ\r\n",
	"AT+QMTCFG=\"aliauth\",0,\"a1n5qqGX7PA\",\"NB101\",\"5iSvkRRHlzzWc9HZMvlfGdKPG8g9AFBd\"\r\n",
	"AT+QMTOPEN=0,\"iot-as-mqtt.cn-shanghai.aliyuncs.com\",1883\r\n",
	"AT+QMTCONN=0,\"clientExample\"\r\n",
	"AT+QMTSUB=0,1,\"/sys/a1n5qqGX7PA/NB101/thing/service/property/set\",0\r\n",
	"AT+QMTSUB=0,1,\"/a1n5qqGX7PA/NB101/user/get\",0\r\n",
	"AT+QMTPUB=0,0,0,0,\"/sys/a1n5qqGX7PA/NB101/thing/event/property/post\"\r\n",
	"AT+QMTDISC=0\r\n"
};

void my_nb101_connect_thread_entry(void* parameter) {
	deviceConnecting();
	
	nb101_uart1();
	
	if (isFirst) {
		rt_sem_take(&rx_sem_uart1_ready, RT_WAITING_FOREVER);
		isFirst = 0;
	}
	
	
	int step = 0;
	
	
	rt_mutex_take(print_mutex , RT_WAITING_FOREVER);
	rt_device_write(serialuart3 , 0, str3, (sizeof(str3) - 1));
	rt_mutex_release(print_mutex);
	step++;
	
	rt_mutex_take(print_mutex , RT_WAITING_FOREVER);
	rt_kprintf(stepStrs[step]);
	unsigned char num = countChars(stepStrs[step]);
	rt_device_write(serialuart3 , 0, stepStrs[step], ((sizeof(char) * num) - 1));
	rt_device_write(serialuart1 , 0, stepStrs[step], ((sizeof(char) * num) - 1));
	rt_mutex_release(print_mutex);
	step++;

    while (1)
    {
		rt_sem_take(&rx_sem_connect, RT_WAITING_FOREVER);
		
		rt_mutex_take(print_mutex , RT_WAITING_FOREVER);
		rt_kprintf(uart1_global_receivedStrs);
		rt_device_write(serialuart3 , 0, uart1_global_receivedStrs, (sizeof(char) * uart1_global_charOffset) - 1);
		rt_mutex_release(print_mutex);
		
		if (step <= 4 && strstr(uart1_global_receivedStrs, "OK\r\n") != NULL) {
			
			rt_mutex_take(print_mutex , RT_WAITING_FOREVER);
			rt_kprintf(stepStrs[step]);
			num = countChars(stepStrs[step]);
			rt_device_write(serialuart3 , 0, stepStrs[step], (sizeof(char) * num - 1));
			rt_device_write(serialuart1 , 0, stepStrs[step], (sizeof(char) * num - 1));
			rt_mutex_release(print_mutex);
			step++;
			
		} else if (step == 5) {
			if (strstr(uart1_global_receivedStrs, "QMTOPEN: 0,0\r\n") != NULL){
				rt_mutex_take(print_mutex , RT_WAITING_FOREVER);
				rt_kprintf(stepStrs[step]);
				num = countChars(stepStrs[step]);
				
				rt_device_write(serialuart3 , 0, stepStrs[step], (sizeof(char) * num - 1));
				rt_device_write(serialuart1 , 0, stepStrs[step], (sizeof(char) * num - 1));
				rt_mutex_release(print_mutex);
				step++;
			} else if (strstr(uart1_global_receivedStrs, "QMTOPEN: 0,") != NULL){
				deviceDisconnected();
				
				rt_mutex_take(print_mutex , RT_WAITING_FOREVER);
				rt_kprintf("Connecting failed!\r\n");
				rt_mutex_release(print_mutex);
								
				my_nb101_connect_thread = NULL;
				return;
			}
		} else if (step == 6) {
			if (strstr(uart1_global_receivedStrs, "QMTCONN: 0,0,0\r\n") != NULL) {   //连接成功
				deviceConnected();
				
				rt_mutex_take(print_mutex , RT_WAITING_FOREVER);
				rt_kprintf(stepStrs[step]);
				num = countChars(stepStrs[step]);
				rt_kprintf("Connecting successed!\r\n");
				
				rt_device_write(serialuart3 , 0, stepStrs[step], (sizeof(char) * num - 1));
				rt_device_write(serialuart1 , 0, stepStrs[step], (sizeof(char) * num - 1));
				rt_mutex_release(print_mutex);
				step++;
			} else if (strstr(uart1_global_receivedStrs, "QMTCONN: 0,") != NULL) {   //连接失败
				deviceDisconnected();

				rt_mutex_take(print_mutex , RT_WAITING_FOREVER);
				rt_kprintf("Connecting failed!\r\n");
				rt_mutex_release(print_mutex);
								
				my_nb101_connect_thread = NULL;
				return;
			}
		} else if (step == 7 && strstr(uart1_global_receivedStrs, "QMTSUB: 0,1,0,1\r\n") != NULL) {
			rt_mutex_take(print_mutex , RT_WAITING_FOREVER);
			rt_kprintf(stepStrs[step]);
			num = countChars(stepStrs[step]);
			rt_device_write(serialuart3 , 0, stepStrs[step], (sizeof(char) * num - 1));
			rt_device_write(serialuart1 , 0, stepStrs[step], (sizeof(char) * num - 1));
			rt_mutex_release(print_mutex);
			step++;
		} else if (step == 8 && strstr(uart1_global_receivedStrs, "QMTSUB: 0,1,0,1\r\n") != NULL) {
			rt_mutex_take(print_mutex , RT_WAITING_FOREVER);
			char end_str[3];
			end_str[0] = 0x1A;
			end_str[1] = 0x0D;
			end_str[2] = 0x0A;
			rt_device_write(serialuart1 , 0, uploadOnDataStr, (sizeof(uploadOnDataStr))-1);
			rt_device_write(serialuart1 , 0, end_str, 3);
			rt_mutex_release(print_mutex);
			
			my_nb101_connect_thread = NULL;
			return; 
		}
		
		
		uart1_global_receivedStrs[0] = '\0';
		uart1_global_charOffset = 0;
    }
}


int countChars(char* s) {
	unsigned char num = 0;
	while(*s != '\0') {
		num++;
		s++;
	}
	
	return num;
}


void connect_aliyun(void) {
	if (my_nb101_connect_thread != NULL) { return; }
	
	my_nb101_connect_thread = rt_thread_create( "my_usart1", my_nb101_connect_thread_entry, RT_NULL, 512, 5, 25);

	if (my_nb101_connect_thread != RT_NULL) {
		rt_mutex_take(print_mutex , RT_WAITING_FOREVER);
		rt_kprintf(str3);
		rt_mutex_release(print_mutex);
		rt_thread_startup(my_nb101_connect_thread);
	} else
		rt_mutex_take(print_mutex , RT_WAITING_FOREVER);
//		rt_kprintf("my_nb101_connect_thread initialize failed!\r\n");
		rt_mutex_release(print_mutex);
		return ;
}


MSH_CMD_EXPORT(connect_aliyun, let nb101 connect to aliyun with the help of uart1);




