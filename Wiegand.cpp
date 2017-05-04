#include "Wiegand.hpp"

#define MASK_HALF_CODE( x ) ( ((uint32_t)1 << ( x >> 1 )) - 1 ) // wiegandType

//WIEGAND * WIEGAND::instance;
volatile uint8_t WIEGAND::pinD0_;
volatile uint8_t WIEGAND::pinD1_;
volatile uint64_t WIEGAND::buffer_;
volatile uint8_t WIEGAND::bitsCounter_;   // bits counter of Wiegand signal
volatile uint32_t WIEGAND::lastBitTime_;  // time of reading last bit
//struct Packet WIEGAND::packet_;

/*******************************************************************************
 *    Function: WIEGAND
 * Description: Constructor
 *   Parameter: Nothing
 *      Return: None
 ******************************************************************************/
WIEGAND::WIEGAND()
{}

/*******************************************************************************
 *    Function: WIEGAND
 * Description: Constructor
 *   Parameter: [IN] uint8_t pinD0 - sets pin for DATA0
 *              [IN] uint8_t pinD1 - sets pin for DATA1
 *              [IN] uint8_t checkFlag - if to check parity then 1 otherwise 0
 *                                     - default 1 in header file
 *      Return: None
 ******************************************************************************/
WIEGAND::WIEGAND( uint8_t pinD0, uint8_t pinD1, uint8_t checkFlag )
{
  pinD0_ = pinD0;
  pinD1_ = pinD1;
  checkFlag_ = checkFlag;
}

/*******************************************************************************
 *    Function: WIEGAND
 * Description: Destructor
 *   Parameter: Nothing
 *      Return: None
 ******************************************************************************/
WIEGAND::~WIEGAND()
{
  end();
}

/*******************************************************************************
 *    Function: begin
 * Description: Sets pin mode and pin change interrupt
 *   Parameter: Nothing
 *      Return: None
 ******************************************************************************/
void WIEGAND::begin( void )
{
  pinMode( pinD0_, INPUT_PULLUP );
  pinMode( pinD1_, INPUT_PULLUP );

  bitsCounter_ = 0;
  lastBitTime_ = 0;
  buffer_ = 0;
  //instance = this;

  pinChangeIrqEnable( pinD0_ );
  pinChangeIrqEnable( pinD1_ );
}

/*******************************************************************************
 *    Function: begin
 * Description: Sets pin mode and pin change interrupt
 *   Parameter: [IN] uint8_t pinD0 - sets pin for DATA0
 *              [IN] uint8_t pinD1 - sets pin for DATA1
 *              [IN] uint8_t checkFlag - if to check parity then 1 otherwise 0
 *                                     - default 1 in header file
 *      Return: None
 ******************************************************************************/
void WIEGAND::begin( uint8_t pinD0, uint8_t pinD1, uint8_t checkFlag )
{
  pinD0_ = pinD0;
  pinD1_ = pinD1;
  checkFlag_ = checkFlag;
  begin();
}

/*******************************************************************************
 *    Function: end
 * Description: Disable pin change interrupt
 *   Parameter: Nothing
 *      Return: None
 ******************************************************************************/
void WIEGAND::end( void )
{
  pinChangeIrqDisable( pinD0_ );
  pinChangeIrqDisable( pinD1_ );
}

/*******************************************************************************
 *    Function: available
 * Description: Check if no more signal
 *   Parameter: Nothing
 *      Return: uint8_t - 0 - OK
 *              uint8_t - 1 - no WIEGAND or bad parity
 ******************************************************************************/
uint8_t WIEGAND::available( void )
{
  uint8_t ret = 1;
  // If no more signal coming through after EXPIRE_TIME ( default 10 ms )
  if (( millis() - lastBitTime_ > EXPIRE_TIME ) && ( bitsCounter_ > WIEGAND26 )) {
    pinChangeIrqDisable( pinD0_ );
    pinChangeIrqDisable( pinD1_ );

    packet_.wiegandType = bitsCounter_ - 2;
    packet_.parityLSB = buffer_ & 0x01;
    packet_.data = ( buffer_ >> 1 ) & (((uint64_t)1 << packet_.wiegandType ) - 1 ) ;
    packet_.parityMSB = buffer_ >> ( packet_.wiegandType + 1 );

    //Serial.println( packet_.wiegandType );
    //Serial.println( packet_.parityLSB );
    //Serial.println( (uint32_t)packet_.data, BIN );
    //Serial.println( packet_.parityMSB );

    bitsCounter_ = 0;
    lastBitTime_ = 0;
    buffer_ = 0;

    if ( checkFlag_ )
      ret = check();
    else
      ret = 0;

    pinChangeIrqEnable( pinD0_ );
    pinChangeIrqEnable( pinD1_ );
  }

	return ret;
}

/*******************************************************************************
 *    Function: check
 * Description: Check parity. In the case of parity, for a given set of bits,
 *              the occurrences of bits whose value is 1 is counted. If that
 *              count is odd, the parity bit value is set to 1, making the total
 *              count of occurrences of 1s in the whole set (including the parity
 *              bit) an even number. If the count of 1s in a given set of bits is
 *              already even, the parity bit's value is 0. If the count of bits
 *              with a value of 1 is even, the parity bit value is set to 1 making
 *              the total count of 1s in the whole set (including the parity bit)
 *              an odd number. If the count of bits with a value of 1 is odd, the
 *              count is already odd so the parity bit's value is 0.
 *   Parameter: Nothing
 *      Return: uint8_t - 0 - OK
 *              uint8_t - 1 - bad parity
 ******************************************************************************/
uint8_t WIEGAND::check( void )
{
  switch ( packet_.wiegandType ) {

    case WIEGAND26:
    case WIEGAND34: {
      // Check odd parity for the lower half of bits
      if ( packet_.parityLSB == 0x01 & countParity( (uint32_t)packet_.data & MASK_HALF_CODE( packet_.wiegandType )))
        return 1; // bad parity
      // Check even parity for the higher half of bits
      if ( packet_.parityMSB != 0x01 & countParity ( packet_.data >> ( packet_.wiegandType >> 1 )))
        return 1; // bad parity

      return 0;
    } break;

    case WIEGAND42: {
      // Parity bit is still setting to 0
      if ( packet_.parityMSB != 0 )
        return 1; // bad parity
      // Check even parity for the all bits
      if ( packet_.parityLSB != 0x01 & ( countParity( packet_.data & 0xFFFFFFFF ) + countParity( packet_.data >> 32 ) ))
        return 1; // bad parity

      return 0;
    } break;
  }

  return 1;
}

/*******************************************************************************
 *    Function: countParity8
 * Description: Counts setting bit in 8-bits number n
 *   Parameter: [IN] uint8_t n - number
 *      Return: uint8_t - number o setting bits
 ******************************************************************************/
/*uint8_t WIEGAND::countParity8( uint8_t n )
{
  n = (( n & 0xAA ) >> 1 ) + ( n & 0x55 );
  n = (( n & 0xC0 ) >> 6 ) + (( n & 0x30 ) >> 4 ) + (( n & 0x0C ) >> 2 ) + ( n & 0x03 );
  return n;
}*/

/*******************************************************************************
 *    Function: countParity
 * Description: Counts setting bit in 8-bits number n
 *   Parameter: [IN] uint32_t n - number
 *      Return: uint8_t - number o setting bits
 ******************************************************************************/
uint8_t WIEGAND::countParity( uint32_t n )
{
  n = (( n & 0xAAAAAAAA ) >> 1 ) + ( n & 0x55555555 );
  n = (( n & 0x30C30C30 ) >> 4 ) + (( n & 0x0C30C30C ) >> 2 ) + ( n & 0xC30C30C3 );
  return ( n % 63 );
}

/*******************************************************************************
 *    Function: getParityMSB
 * Description: Get parity MSB
 *   Parameter: Nothing
 *      Return: uint8_t - parity
 ******************************************************************************/
uint8_t WIEGAND::getParityMSB( void )
{
  return packet_.parityMSB;
}

/*******************************************************************************
 *    Function: getParityLSB
 * Description: Get parity LSB
 *   Parameter: Nothing
 *      Return: uint8_t - parity
 ******************************************************************************/
uint8_t WIEGAND::getParityLSB( void )
{
  return packet_.parityLSB;
}

/*******************************************************************************
 *    Function: getCodeHex
 * Description: Get code in HEX format
 *   Parameter: [OUT] char * code - pointer for output string
 *              [IN] uint8_t size - size of output string
 *      Return: None
 ******************************************************************************/
void WIEGAND::getCodeHex( char * code, uint8_t size )
{
  if ( size > ( packet_.wiegandType >> 2 )) {
    char * ptr = code;

    for ( int8_t i = ( packet_.wiegandType >> 3 ) - 1; i >= 0; i-- )
      ptr += sprintf( ptr, "%02X", ( packet_.data >> ( i << 3 )) & 0xFF );
    *ptr = '\0';
  }
}

/*******************************************************************************
 *    Function: getCodeDec
 * Description: Get code in DEC format
 *   Parameter: [OUT] char * code - pointer for output string
 *              [IN] uint8_t size - size of output string
 *      Return: None
 ******************************************************************************/
void WIEGAND::getCodeDec( char * code, uint8_t size )
{
  uint64_t temp = packet_.data;
  uint8_t strLen = 1;

  while ( temp ) {
    temp = temp / 10;
    strLen++;
  }

  if ( size >= strLen ) {
    *( code + --strLen ) = '\0';
    temp = packet_.data;

    do {
      *( code + strLen - 1) = ( temp %  10 ) + '0';
      temp = temp / 10;
    } while ( strLen--);
  }
}

/*******************************************************************************
 *    Function: getCode
 * Description: Get code in format by radix
 *   Parameter: [OUT] char * code - pointer for output string
 *              [IN] uint8_t size - size of output string
 *              [IN] uint8_t radix - base
 *      Return: None
 ******************************************************************************/
void WIEGAND::getCode( char * code, uint8_t size, uint8_t radix )
{
  static char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  uint64_t temp = packet_.data;
  uint8_t strLen = 1;

  while ( temp ) {
    temp = temp / radix;
    strLen++;
  }

  if ( size >= strLen ) {
    *( code + --strLen ) = '\0';
    temp = packet_.data;

    do {
      *( code + strLen - 1) = digits[ temp %  radix ];
      temp = temp / radix;
    } while ( strLen--);
  }
}

/*******************************************************************************
 *    Function: getWiegandType
 * Description: Get WIEGAND type
 *   Parameter: Nothing
 *      Return: uint8_t - WIEGAND type
 ******************************************************************************/
uint8_t WIEGAND::getWiegandType( void )
{
	return packet_.wiegandType + 2;
}

/*******************************************************************************
 *    Function: writeBit0
 * Description: Write bit 0
 *   Parameter: Nothing
 *      Return: None
 ******************************************************************************/
void WIEGAND::writeBit0( void )
{
  buffer_ <<=1;
  ++bitsCounter_;
  lastBitTime_ = millis();  // keep track of last wiegand bit received
  Serial.print( "0" );
}

/*******************************************************************************
 *    Function: writeBit1
 * Description: Write bit 1
 *   Parameter: Nothing
 *      Return: None
 ******************************************************************************/
void WIEGAND::writeBit1( void )
{
  buffer_ = ( buffer_ << 1 ) | 0x01;
  ++bitsCounter_;
  lastBitTime_ = millis();  // keep track of last wiegand bit received
  Serial.print( "1" );
}

/*******************************************************************************
 *    Function: readBit
 * Description: Read bits from data pins
 *   Parameter: Nothing
 *      Return: None
 ******************************************************************************/
void WIEGAND::readBit( void )
{
  if ( !digitalRead( pinD0_ )) {
    writeBit0();
  }
  else if ( !digitalRead( pinD1_ )) {
    writeBit1();
  }
}

/*******************************************************************************
 *    Function: ISR
 * Description: Calling interrupt macro ISR from <avr/interrupt.h>
 *   Parameter: [IN] vector
 *      Return: None
 ******************************************************************************/
ISR( PCINT0_vect )
{
  WIEGAND::readBit();
}

/*******************************************************************************
 *    Function: ISR
 * Description: Calling interrupt macro ISR from <avr/interrupt.h>
 *   Parameter: [IN] vector
 *      Return: None
 ******************************************************************************/
ISR( PCINT2_vect )
{
  WIEGAND::readBit();
}
