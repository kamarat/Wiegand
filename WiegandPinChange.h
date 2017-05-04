#ifndef WIEGAND_PIN_CHANGE_H
#define WIEGAND_PIN_CHANGE_H

#include "Arduino.h"
#include <avr/interrupt.h>

void pinChangeIrqEnable( uint8_t pin );
void pinChangeIrqDisable( uint8_t pin );
void pinChangeIrqDisable( void );

#endif  // #define WIEGAND_PIN_CHANGE_H
