/* 
 * File:   matrix_keypad.h
 * Author: Shwetha M K
 *
 * Created on 14 May, 2026, 1:57 PM
 */

#ifndef MATRIX_KEYPAD_H
#define	MATRIX_KEYPAD_H

void init_matrix_keypad(void);
unsigned char read_matrix_keypad(unsigned char);
unsigned char scan_key(void);

#define STATE_CHANGE    0
#define LEVEL   1

#define SWITCH1     1
#define SWITCH2     2
#define SWITCH3     3
#define SWITCH4     4
#define SWITCH5     5
#define SWITCH6     6
#define SWITCH7     7
#define SWITCH8     8
#define SWITCH9     9
#define SWITCH10    10
#define SWITCH11    11
#define SWITCH12    12

#define ROW0    RB5
#define ROW1    RB6
#define ROW2    RB7

#define COL0    RB1
#define COL1    RB2
#define COL2    RB3
#define COL3    RB4

#define ALL_RELEASED 0xFF

#endif	/* MATRIX_KEYPAD_H */

