#include "LED1_PG3.h"

int onTime = 800;
int offTime = 800;

void led_shinning(void *p){
//	while(1){
//		bsp_LedToggle(1);
//	}
	
	/* ����PIN��ģʽΪ��� */
	rt_pin_mode(LED_PIN_NUM, PIN_MODE_OUTPUT);

    while (1)
    {
        /* ����PIN�� */
        rt_pin_write(LED_PIN_NUM, PIN_LOW);
//        rt_kprintf("led on!\r\n");
        
		/* ��ʱ1000ms */ 
        rt_thread_mdelay(onTime);

        /* ����PIN�� */
        rt_pin_write(LED_PIN_NUM, PIN_HIGH);
//        rt_kprintf("led off!\r\n");
        rt_thread_mdelay(offTime);
    }
}

void deviceConnecting(void) {
	onTime = 200;
	offTime = 200;
}

void deviceConnected(void) {
	onTime = 200;
	offTime = 1800;
}

void deviceDisconnected(void) {
	onTime = 800;
	offTime = 800;
}

