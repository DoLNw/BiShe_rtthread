#include <rtdevice.h>
#include "NB101_uart1.h"
#include "string.h"

static rt_thread_t my_nb101_connect_thread = RT_NULL;
int countChars(char* s);
char str3[] = "my_nb101_connect_thread initialize succeed!\r\n";

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

char atStr8[] = "AT+QMTDISC=0\r\n\0"; 
//断开连接


char* stepStrs[9] = {
	"Let's connect to aliyun.\r\n",
	"AT+CGATT?\r\n",
	"AT+CSQ\r\n",
	"AT+QMTCFG=\"aliauth\",0,\"a1n5qqGX7PA\",\"NB101\",\"5iSvkRRHlzzWc9HZMvlfGdKPG8g9AFBd\"\r\n",
	"AT+QMTOPEN=0,\"iot-as-mqtt.cn-shanghai.aliyuncs.com\",1883\r\n",
	"AT+QMTCONN=0,\"clientExample\"\r\n",
	"AT+QMTSUB=0,1,\"/a1n5qqGX7PA/NB101/user/get\",0\r\n",
	"AT+QMTPUB=0,0,0,0,\"/sys/a1n5qqGX7PA/NB101/thing/event/property/post\"\r\n",
	"AT+QMTDISC=0\r\n"
};

void my_nb101_connect_thread_entry(void* parameter) {
	nb101_uart1();
	
	rt_device_t serialuart1;
	serialuart1 = rt_device_find("uart1");
	
//	char* receivedStr;
//	*receivedStr = '\0'; 
	int charOffset = 0;
	char receivedStrs[] = "";
	int step = 0;
	char ch;
	
	rt_device_t serialuart3;
	serialuart3 = rt_device_find("uart3");
	
	rt_device_write(serialuart3 , 0, str3, (sizeof(str3) - 1));
	step++;
	
	rt_kprintf(stepStrs[step]);
	unsigned char num = countChars(stepStrs[step]);
	rt_device_write(serialuart3 , 0, stepStrs[step], ((sizeof(char) * num) - 1));
	rt_device_write(serialuart1 , 0, stepStrs[step], ((sizeof(char) * num) - 1));
	step++;

    while (1)
    {
        while (rt_device_read(serialuart1 , -1, &ch, 1) != 1)
        {
            rt_sem_take(&rx_sem2, RT_WAITING_FOREVER);
        }
        
//		rt_kprintf(&ch);
//        rt_device_write(serialuart3 , 0, &ch, 1);
//		*(receivedStr + (charOffset)) = ch;
//		charOffset++;
//		*(receivedStr + (charOffset)) = '\0';
		
		receivedStrs[charOffset++] = ch;
		receivedStrs[charOffset] = '\0';
		
//		rt_kprintf(receivedStrs);
//		rt_device_write(serialuart3 , 0, receivedStrs, (sizeof(char) * charOffset) - 1);
		
		if (strstr(receivedStrs, "\r\n") != NULL) {
			rt_kprintf(receivedStrs);
			rt_device_write(serialuart3 , 0, receivedStrs, (sizeof(char) * charOffset) - 1);
			
			if (step <= 4 && strstr(receivedStrs, "OK\r\n") != NULL) {
				
				rt_kprintf(stepStrs[step]);
				num = countChars(stepStrs[step]);
				rt_device_write(serialuart3 , 0, stepStrs[step], (sizeof(char) * num - 1));
				rt_device_write(serialuart1 , 0, stepStrs[step], (sizeof(char) * num - 1));
				step++;
				
			} else if (step == 5) {
				
				if (strstr(receivedStrs, "0,0\r\n") != NULL) {
					rt_kprintf(stepStrs[step]);
					num = countChars(stepStrs[step]);
					rt_device_write(serialuart3 , 0, stepStrs[step], (sizeof(char) * num - 1));
					rt_device_write(serialuart1 , 0, stepStrs[step], (sizeof(char) * num - 1));
					step++;
					
				}
			} else if (step == 6 && strstr(receivedStrs, "0,0,0\r\n") != NULL) {
				my_nb101_connect_thread = NULL;
				return; 
			}
			
			
			receivedStrs[0] = '\0';
			charOffset = 0;
		}
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
		rt_thread_startup(my_nb101_connect_thread);
		rt_kprintf(str3);
	} else
		rt_kprintf("my_nb101_connect_thread initialize failed!\r\n");
		return ;
}





MSH_CMD_EXPORT(connect_aliyun, let nb101 connect to aliyun with the help of uart1);



void disConnect_aliyun(void) {
	rt_device_t serialuart1;
	serialuart1 = rt_device_find("uart1");
	
	rt_device_write(serialuart1 , 0, atStr8, (sizeof(atStr8)));
	rt_thread_mdelay(500);
	rt_device_write(serialuart1 , 0, atStr8, (sizeof(atStr8)));
}

MSH_CMD_EXPORT(disConnect_aliyun, let nb101 connect to aliyun with the help of uart1);
