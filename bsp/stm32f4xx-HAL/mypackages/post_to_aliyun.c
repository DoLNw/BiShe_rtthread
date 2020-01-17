#include <rtdevice.h>
#include "main.h"
#include "string.h"

void post_to_aliyun(int argc, char **argv) {
	if (argc != 2) {
		rt_kprintf("Usage: door status\n");
		rt_kprintf("Like: post_to_aliyun 1\n");
	}
	
	char postStr[] = "{\"params\" : {\"ContactState\": ";
	
	const char* doorStatus = argv[1];
	if (strlen(doorStatus) != 1) {
		rt_kprintf("Usage: door status\n");
		rt_kprintf("Like: post_to_aliyun 1\n");
	}
	
	strcat(postStr, doorStatus);
	strcat(postStr, ", \"Error\": 0}}");
	
	rt_device_t serialuart1;
	serialuart1 = rt_device_find("uart1");
	rt_device_t serialuart3;
	serialuart3 = rt_device_find("uart3");
	
	char post_command[] = "AT+QMTPUB=0,0,0,0,\"/sys/a1n5qqGX7PA/NB101/thing/event/property/post\"\r\n";
	char end_str[3];
	end_str[0] = 0x1A;
	end_str[1] = 0x0D;
	end_str[2] = 0x0A;
	
	rt_mutex_take(print_mutex , RT_WAITING_FOREVER);
	rt_device_write(serialuart1 , 0, post_command, (sizeof(post_command))-1);
	rt_thread_mdelay(1000);
	rt_device_write(serialuart1 , 0, postStr,  (strlen(postStr)));
	rt_device_write(serialuart1 , 0, end_str, 3);
	
	rt_device_write(serialuart3 , 0, post_command, (sizeof(post_command))-1);  //减1是因为不减多个\0，因为这个是sizeof是算\0的，而对于char字符串的strlen是差了个1就是差在这个\0，而且sizeof的postStr是不算strcat后的不知为啥。
	rt_device_write(serialuart3 , 0, postStr,  (strlen(postStr)));
	rt_device_write(serialuart3 , 0, end_str, 3);
	rt_mutex_release(print_mutex);
}

MSH_CMD_EXPORT(post_to_aliyun, post_to_aliyun);




















///*
// * Copyright (c) 2006-2018, RT-Thread Development Team
// *
// * SPDX-License-Identifier: Apache-2.0
// *
// * Change Logs:
// * Date             Author      Notes
// *
// */
///*
// * 程序清单：tcp 客户端
// *
// * 这是一个 tcp 客户端的例程
// * 导出 tcpclient 命令到控制终端
// * 命令调用格式：tcpclient URL PORT
// * URL：服务器地址 PORT:：端口号
// * 程序功能：接收并显示从服务端发送过来的信息，接收到开头是 'q' 或 'Q' 的信息退出程序
//*/
//#include <rtthread.h>
//#include <sys/socket.h> /* 使用BSD socket，需要包含socket.h头文件 */
//#include <netdb.h>
//#include <string.h>
//#include <finsh.h>

//#define BUFSZ   1024

//static const char send_data[] = "This is TCP Client from RT-Thread."; /* 发送用到的数据 */
//void tcpclient(int argc, char **argv)
//{
//    int ret;
//    char *recv_data;
//    struct hostent *host;
//    int sock, bytes_received;
//    struct sockaddr_in server_addr;
//    const char *url;
//    int port;

//    if (argc < 3)
//    {
//        rt_kprintf("Usage: tcpclient URL PORT\n");
//        rt_kprintf("Like: tcpclient 192.168.12.44 5000\n");
//        return ;
//    }

//    url = argv[1];
//    port = strtoul(argv[2], 0, 10);

//    /* 通过函数入口参数url获得host地址（如果是域名，会做域名解析） */
//    host = gethostbyname(url);

//    /* 分配用于存放接收数据的缓冲 */
//    recv_data = rt_malloc(BUFSZ);
//    if (recv_data == RT_NULL)
//    {
//        rt_kprintf("No memory\n");
//        return;
//    }

//    /* 创建一个socket，类型是SOCKET_STREAM，TCP类型 */
//    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
//    {
//        /* 创建socket失败 */
//        rt_kprintf("Socket error\n");

//        /* 释放接收缓冲 */
//        rt_free(recv_data);
//        return;
//    }

//    /* 初始化预连接的服务端地址 */
//    server_addr.sin_family = AF_INET;
//    server_addr.sin_port = htons(port);
//    server_addr.sin_addr = *((struct in_addr *)host->h_addr);
//    rt_memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));

//    /* 连接到服务端 */
//    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
//    {
//        /* 连接失败 */
//        rt_kprintf("Connect fail!\n");
//        closesocket(sock);

//        /*释放接收缓冲 */
//        rt_free(recv_data);
//        return;
//    }
//    else
//    {
//        /* 连接成功 */
//        rt_kprintf("Connect successful\n");
//    }

//    while (1)
//    {
//        /* 从sock连接中接收最大BUFSZ - 1字节数据 */
//        bytes_received = recv(sock, recv_data, BUFSZ - 1, 0);
//        if (bytes_received < 0)
//        {
//            /* 接收失败，关闭这个连接 */
//            closesocket(sock);
//            rt_kprintf("\nreceived error,close the socket.\r\n");

//            /* 释放接收缓冲 */
//            rt_free(recv_data);
//            break;
//        }
//        else if (bytes_received == 0)
//        {
//            /* 默认 recv 为阻塞模式，此时收到0认为连接出错，关闭这个连接 */
//            closesocket(sock);
//            rt_kprintf("\nreceived error,close the socket.\r\n");

//            /* 释放接收缓冲 */
//            rt_free(recv_data);
//            break;
//        }

//        /* 有接收到数据，把末端清零 */
//        recv_data[bytes_received] = '\0';

//        if (strncmp(recv_data, "q", 1) == 0 || strncmp(recv_data, "Q", 1) == 0)
//        {
//            /* 如果是首字母是q或Q，关闭这个连接 */
//            closesocket(sock);
//            rt_kprintf("\n got a 'q' or 'Q',close the socket.\r\n");

//            /* 释放接收缓冲 */
//            rt_free(recv_data);
//            break;
//        }
//        else
//        {
//            /* 在控制终端显示收到的数据 */
//            rt_kprintf("\nReceived data = %s ", recv_data);
//        }

//        /* 发送数据到sock连接 */
//        ret = send(sock, send_data, strlen(send_data), 0);
//        if (ret < 0)
//        {
//            /* 接收失败，关闭这个连接 */
//            closesocket(sock);
//            rt_kprintf("\nsend error,close the socket.\r\n");

//            rt_free(recv_data);
//            break;
//        }
//        else if (ret == 0)
//        {
//            /* 打印send函数返回值为0的警告信息 */
//            rt_kprintf("\n Send warning,send function return 0.\r\n");
//        }
//    }
//    return;
//}

//MSH_CMD_EXPORT(tcpclient, a tcp client sample);
