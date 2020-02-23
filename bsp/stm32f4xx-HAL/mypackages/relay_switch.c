#include "relay_switch.h"

void init_switch(void){
	
	/* 设置PIN脚模式为输出 */
	rt_pin_mode(RELAY_SWITCH_NUM, PIN_MODE_OUTPUT);
}

void open_switch(void) {
	/* 拉低PIN脚 */
	rt_pin_write(RELAY_SWITCH_NUM, PIN_LOW);
	rt_kprintf("switch on!\r\n");
}

void close_switch(void) {
	/* 拉高PIN脚 */
	rt_pin_write(RELAY_SWITCH_NUM, PIN_HIGH);
	rt_kprintf("switch off!\r\n");
}

MSH_CMD_EXPORT(open_switch, open_switch);
MSH_CMD_EXPORT(close_switch, close_switch);

