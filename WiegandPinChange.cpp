#include "WiegandPinChange.h"

void pinChangeIrqEnable( uint8_t pin )
{
  switch ( pin ) {
    case 2: { // set pin change interrupt for D2
        PCMSK2 |= bit( PCINT18 ); // want pin 2
        PCIFR  |= bit( PCIF2 );   // clear any outstanding interrupts
        PCICR  |= bit( PCIE2 );   // enable pin change interrupts for PCINT[23:16]
      } break;
    case 3: { // set pin change interrupt for D3
        PCMSK2 |= bit( PCINT19 ); // want pin 3
        PCIFR  |= bit( PCIF2 );   // clear any outstanding interrupts
        PCICR  |= bit( PCIE2 );   // enable pin change interrupts for PCINT[23:16]
      } break;
    case 4: { // set pin change interrupt for D4
        PCMSK2 |= bit( PCINT20 ); // want pin 4
        PCIFR  |= bit( PCIF2 );   // clear any outstanding interrupts
        PCICR  |= bit( PCIE2 );   // enable pin change interrupts for PCINT[23:16]
      } break;
    case 5: { // set pin change interrupt for D5
        PCMSK2 |= bit( PCINT21 ); // want pin 5
        PCIFR  |= bit( PCIF2 );   // clear any outstanding interrupts
        PCICR  |= bit( PCIE2 );   // enable pin change interrupts for PCINT[23:16]
      } break;
    case 6: { // set pin change interrupt for D6
        PCMSK2 |= bit( PCINT22 ); // want pin 6
        PCIFR  |= bit( PCIF2 );   // clear any outstanding interrupts
        PCICR  |= bit( PCIE2 );   // enable pin change interrupts for PCINT[23:16]
      } break;
    case 7: { // set pin change interrupt for D7
        PCMSK2 |= bit( PCINT23 ); // want pin 7
        PCIFR  |= bit( PCIF2 );   // clear any outstanding interrupts
        PCICR  |= bit( PCIE2 );   // enable pin change interrupts for PCINT[23:16]
      } break;
    case 8: { // set pin change interrupt for D8
        PCMSK0 |= bit( PCINT0 );  // want pin 8
        PCIFR  |= bit( PCIF0 );   // clear any outstanding interrupts
        PCICR  |= bit( PCIE0 );   // enable pin change interrupts for PCINT[7:0]
      } break;
    case 9: { // set pin change interrupt for D9
        PCMSK0 |= bit( PCINT1 );  // want pin 9
        PCIFR  |= bit( PCIF0 );   // clear any outstanding interrupts
        PCICR  |= bit( PCIE0 );   // enable pin change interrupts for PCINT[7:0]
      } break;
    case 10: { // set pin change interrupt for D10
        PCMSK0 |= bit( PCINT2 );  // want pin 10
        PCIFR  |= bit( PCIF0 );   // clear any outstanding interrupts
        PCICR  |= bit( PCIE0 );   // enable pin change interrupts for PCINT[7:0]
      } break;
    case 11: { // set pin change interrupt for D11
        PCMSK0 |= bit( PCINT3 );  // want pin 11
        PCIFR  |= bit( PCIF0 );   // clear any outstanding interrupts
        PCICR  |= bit( PCIE0 );   // enable pin change interrupts for PCINT[7:0]
      } break;
    case 12: { // set pin change interrupt for D12
        PCMSK0 |= bit( PCINT4 );  // want pin 12
        PCIFR  |= bit( PCIF0 );   // clear any outstanding interrupts
        PCICR  |= bit( PCIE0 );   // enable pin change interrupts for PCINT[7:0]
      } break;
    case 13: { // set pin change interrupt for D13
        PCMSK0 |= bit( PCINT5 );  // want pin 13
        PCIFR  |= bit( PCIF0 );   // clear any outstanding interrupts
        PCICR  |= bit( PCIE0 );   // enable pin change interrupts for PCINT[7:0]
      } break;
    default:
    // If nothing else matches, do the default, default is optional
      break;
  }
}

void pinChangeIrqDisable( uint8_t pin )
{
  switch ( pin ) {
    case 2: { // set pin change interrupt for 2
        PCMSK2 &= ~bit( PCINT18 );  // want pin 2
        PCIFR  |= bit( PCIF2 );     // clear any outstanding interrupts
      } break;
    case 3: { // set pin change interrupt for D3
        PCMSK2 &= ~bit( PCINT19 );  // want pin 3
        PCIFR  |= bit( PCIF2 );     // clear any outstanding interrupts
      } break;
    case 4: { // set pin change interrupt for 4
        PCMSK2 &= ~bit( PCINT20 );  // want pin 4
        PCIFR  |= bit( PCIF2 );     // clear any outstanding interrupts
      } break;
    case 5: { // set pin change interrupt for D5
        PCMSK2 &= ~bit( PCINT21 );  // want pin 5
        PCIFR  |= bit( PCIF2 );     // clear any outstanding interrupts
      } break;
    case 6: { // set pin change interrupt for D6
        PCMSK2 &= ~bit( PCINT22 );  // want pin 6
        PCIFR  |= bit( PCIF2 );     // clear any outstanding interrupts
      } break;
    case 7: { // set pin change interrupt for 7
        PCMSK2 &= ~bit( PCINT23 );  // want pin 7
        PCIFR  |= bit( PCIF2 );     // clear any outstanding interrupts
      } break;
    case 8: { // set pin change interrupt for 8
        PCMSK0 &= ~bit( PCINT0 );   // want pin 8
        PCIFR  |= bit( PCIF0 );     // clear any outstanding interrupts
      } break;
    case 9: { // set pin change interrupt for 9
        PCMSK0 &= ~bit( PCINT1 );   // want pin 9
        PCIFR  |= bit( PCIF0 );     // clear any outstanding interrupts
      } break;
    case 10: { // set pin change interrupt for D10
        PCMSK0 &= ~bit( PCINT2 );   // want pin 10
        PCIFR  |= bit( PCIF0 );     // clear any outstanding interrupts
      } break;
    case 11: { // set pin change interrupt for D11
        PCMSK0 &= ~bit( PCINT3 );   // want pin 11
        PCIFR  |= bit( PCIF0 );     // clear any outstanding interrupts
      } break;
    case 12: { // set pin change interrupt for D12
        PCMSK0 &= ~bit( PCINT4 );   // want pin 12
        PCIFR  |= bit( PCIF0 );     // clear any outstanding interrupts
      } break;
    case 13: { // set pin change interrupt for D13
        PCMSK0 &= ~bit( PCINT5 );   // want pin 13
        PCIFR  |= bit( PCIF0 );     // clear any outstanding interrupts
      } break;
    default:
    // If nothing else matches, do the default, default is optional
      break;
  }
}

void pinChangeIrqDisable( void )
{
  PCICR &= ( uint8_t )~( 1 << PCIE0 );  // disable pin change interrupts for D8 to D13 - PCINT[7:0]
  PCICR &= ( uint8_t )~( 1 << PCIE2 );  // disable pin change interrupts for D0 to D7 - PCINT[23:16]
}

/*
void pinChangeIrqEnable()
{
  // Set interrupt for pins A0 - A5 (PCINT[13:8])
  PCMSK1 = 0;
  for ( uint8_t i = 0; i < sizeof(INPUT_PINS_PCMSK1); i++ ) {
    PCMSK1 |= 1 << INPUT_PINS_PCMSK1[ i ];
  }
  PCIFR |= 1 << PCIF1;  // clear any outstanding interrupts
  PCICR = 1 << PCIE1;   // enable pin change interrupts for PCINT[15:8]
}
*/

