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

int main(void)
{
    /* user app entry */
	print_mutex = rt_mutex_create("print_mutex", RT_IPC_FLAG_FIFO);
	
    return 0;
}
