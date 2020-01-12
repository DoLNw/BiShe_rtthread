#include <rtdevice.h>
#include "NB101_uart1.h"

int countChars(char* s);

char atStr0[] = "Let's connect to aliyun.\r\n\0";

char atStr1[] = "AT+CGATT?\r\n\0";

char atStr2[] = "AT+CSQ\r\n\0";

char atStr3[] = "AT+QMTCFG=\"aliauth\",0,\"a1n5qqGX7PA\",\"NB101\",\"5iSvkRRHlzzWc9HZMvlfGdKPG8g9AFBd\"\r\n\0";

char atStr4[] = "AT+QMTOPEN=0,\"iot-as-mqtt.cn-shanghai.aliyuncs.com\",1883\r\n\0";
//（AT+QMTCLOSE=0\r\n）

char atStr5[] = "AT+QMTCONN=0,\"clientExample\"\r\n\0";

//订阅
char atStr6[] = "AT+QMTSUB=0,1,\"/a1n5qqGX7PA/NB101/user/get\",0\r\n\0";

//发布
char atStr7[] = "AT+QMTPUB=0,0,0,0,\"/sys/a1n5qqGX7PA/NB101/thing/event/property/post\"\r\n\0";
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

void connect_aliyun(void) {
	nb101_uart1();
		
	rt_device_t serialuart1;
	serialuart1 = rt_device_find("uart1");
	
	int step = 0;
	char ch;
	char isOK[4] = {0, 0, 0, 0};
	char canPost[5] = {0, 0, 0, 0, 0};
	
	rt_device_t serialuart3;
	serialuart3 = rt_device_find("uart3");
	
	rt_kprintf(stepStrs[step]);
	unsigned char num = countChars(stepStrs[step]);
	rt_device_write(serialuart3 , 0, stepStrs[step], (sizeof(char) * num - 1));
	step++;
	
	rt_kprintf(stepStrs[step]);
	

	rt_device_write(serialuart3 , 0, stepStrs[step], (sizeof(char) * num - 1));
	rt_device_write(serialuart1 , 0, stepStrs[step], (sizeof(char) * num - 1));
	step++;

    while (1)
    {
        while (rt_device_read(serialuart1 , -1, &ch, 1) != 1)
        {
            rt_sem_take(&rx_sem2, RT_WAITING_FOREVER);
        }
        
		if (ch == 'O' || ch == 'o') {
			isOK[0] = 1;
		} else if ((ch == 'K' || ch == 'k') && isOK[0]) {
			isOK[1] = 1;
		} else if ((ch == '\r') && isOK[0] && isOK[1]) {
			isOK[2] = 1;
		} else if ((ch == '\n') && isOK[0] && isOK[1] && isOK[2]) {
			isOK[3] = 1;
		} else if (isOK[3] != 1) {
			isOK[0] = 0;
			isOK[1] = 0;
			isOK[2] = 0;
			isOK[3] = 0;
		}
		rt_kprintf(&ch);
        rt_device_write(serialuart3 , 0, &ch, 1);
		
		if (isOK[3]) {
			if (step == 5) {
				if (ch == '0' && !canPost[0]) {
					canPost[0] = 1;
				} else if ((ch == ',') && canPost[0]) {
					canPost[1] = 1;
				} else if ((ch == '0') && canPost[0] && canPost[1]) {
					canPost[2] = 1;
				} else if ((ch == '\r') && canPost[0] && canPost[1] && canPost[2]) {
					canPost[3] = 1;
				} else if ((ch == '\n') && canPost[0] && canPost[1] && canPost[2] && canPost[3]) {
					canPost[4] = 1;
				} else {
					canPost[0] = 0;
					canPost[1] = 0;
					canPost[2] = 0;
					canPost[3] = 0;
					canPost[4] = 0;
				}
				if (canPost[4]) {
					rt_kprintf(stepStrs[step]);
					num = countChars(stepStrs[step]);
					rt_device_write(serialuart3 , 0, stepStrs[step], (sizeof(char) * num - 1));
					rt_device_write(serialuart1 , 0, stepStrs[step], (sizeof(char) * num - 1));
					step++;
					
					isOK[0] = 0;
					isOK[1] = 0;
					isOK[2] = 0;
					isOK[3] = 0;
					
					canPost[0] = 0;
					canPost[1] = 0;
					canPost[2] = 0;
					canPost[3] = 0;
					canPost[4] = 0;
				}
				
			} else if (step <= 5) {
				rt_kprintf(stepStrs[step]);
				num = countChars(stepStrs[step]);
				rt_device_write(serialuart3 , 0, stepStrs[step], (sizeof(char) * num - 1));
				rt_device_write(serialuart1 , 0, stepStrs[step], (sizeof(char) * num - 1));
				step++;
				
				isOK[0] = 0;
				isOK[1] = 0;
				isOK[2] = 0;
				isOK[3] = 0;
			}
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









MSH_CMD_EXPORT(connect_aliyun, let nb101 connect to aliyun with the help of uart1);
