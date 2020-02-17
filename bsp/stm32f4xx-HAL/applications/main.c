/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2015-07-29     Arda.Fu      first implementation
 */
#include <rtthread.h>
#include <board.h>
#include "main.h"

rt_mutex_t print_mutex = RT_NULL;
rt_device_t serialuart1;
rt_device_t serialuart3;



int main(void)
{
    /* user app entry */
//	blue_test_uart3();

	
	rt_kprintf("\r\nBiShe_JiaCheng is ready!\r\n");
    /* user app entry */
	print_mutex = rt_mutex_create("print_mutex", RT_IPC_FLAG_FIFO);  //该互斥量保护串口收发一整句话不被打断
	
	serialuart1 = rt_device_find("uart1");
	serialuart3 = rt_device_find("uart3");
	
	
	static rt_thread_t shinning = RT_NULL;
	shinning = rt_thread_create("shinning",led_shinning, RT_NULL, 512, 25, 20);
	rt_thread_startup(shinning);
	
//	connect_aliyun();

    return 0;
}
























//命令行中本来的提示是msh >， 然后我找了好久，发现这个代码在shell.c中，在行104中，我改成了msh JiaCheng >哈哈哈
//本想直接找msh >，发现找不到，它是1.#define _MSH_PROMPT "msh " 2.strcpy(finsh_prompt, _PROMPT);  3.strcat(finsh_prompt, "JiaCheng >");
//还挺复杂，还有指针指来指去的
//因为我自己的代码中打印都加了rt_mutex_take(print_mutex , RT_WAITING_FOREVER);和rt_mutex_release(print_mutex);（shell.c中的#include "main.c"是我加得了）
//本想这个msh JiaCheng >打印也加了互斥么好了（shell.c中有多个这个打印的），但是也解决不了tshell串口中莫名的打印会有点点错乱的现象，目前我还解决不了
//而且这个错乱是不完整的一句话，而且我打印到我的蓝牙串口uart3中都是正常的，但是打印到tshell中的就不正常，是因为tshell多了msh什么的还是rt_kprintf什么的问题呢？？


