#ifndef NB101_UART1
#define NB101_UART1

extern struct rt_semaphore rx_sem_connect, rx_sem_uart1_ready; 
extern int uart1_global_charOffset;
extern char uart1_global_receivedStrs[];

void nb101_uart1(void);

#endif
