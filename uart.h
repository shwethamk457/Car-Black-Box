/* 
 * File:   uart.h
 * Author: Shwetha M K
 *
 * Created on 14 May, 2026, 1:57 PM
 */

#ifndef UART_H
#define	UART_H

#define RX_PIN					TRISC7
#define TX_PIN					TRISC6

void init_uart(void);
void putch(unsigned char byte);
int puts(const char *s);
unsigned char getch(void);
unsigned char getch_with_timeout(unsigned short max_time);
unsigned char getche(void);

#endif	/* UART_H */

