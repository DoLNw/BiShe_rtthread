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
	print_mutex = rt_mutex_create("print_mutex", RT_IPC_FLAG_FIFO);  //�û��������������շ�һ���仰�������
	
	serialuart1 = rt_device_find("uart1");
	serialuart3 = rt_device_find("uart3");
	
	
	static rt_thread_t shinning = RT_NULL;
	shinning = rt_thread_create("shinning",led_shinning, RT_NULL, 512, 25, 20);
	rt_thread_startup(shinning);
	
//	connect_aliyun();

    return 0;
}
























//�������б�������ʾ��msh >�� Ȼ�������˺þã��������������shell.c�У�����104�У��Ҹĳ���msh JiaCheng >������
//����ֱ����msh >�������Ҳ���������1.#define _MSH_PROMPT "msh " 2.strcpy(finsh_prompt, _PROMPT);  3.strcat(finsh_prompt, "JiaCheng >");
//��ͦ���ӣ�����ָ��ָ��ָȥ��
//��Ϊ���Լ��Ĵ����д�ӡ������rt_mutex_take(print_mutex , RT_WAITING_FOREVER);��rt_mutex_release(print_mutex);��shell.c�е�#include "main.c"���Ҽӵ��ˣ�
//�������msh JiaCheng >��ӡҲ���˻���ô���ˣ�shell.c���ж�������ӡ�ģ�������Ҳ�������tshell������Ī���Ĵ�ӡ���е����ҵ�����Ŀǰ�һ��������
//������������ǲ�������һ�仰�������Ҵ�ӡ���ҵ���������uart3�ж��������ģ����Ǵ�ӡ��tshell�еľͲ�����������Ϊtshell����mshʲô�Ļ���rt_kprintfʲô�������أ���


