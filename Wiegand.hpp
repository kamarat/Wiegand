#ifndef WIEGAND_HPP
#define WIEGAND_HPP

#include <stdint.h>
#include <avr/interrupt.h>

#include "Arduino.h"
#include "WiegandPinChange.h"

const uint8_t WIEGAND26 = 24;   // 3 bytes data
const uint8_t WIEGAND34 = 32;   // 4 bytes data
const uint8_t WIEGAND42 = 40;   // 5 bytes data

//const uint8_t WIEGAND26 = 3;   // data bytes
//const uint8_t WIEGAND34 = 4;   // data bytes
//const uint8_t WIEGAND42 = 5;   // data bytes
const uint8_t WIEGAND_MAX = WIEGAND42;

const uint8_t EXPIRE_TIME = 25; // ms

/*struct Packet {
  uint8_t parityMSB;
  uint8_t data[ WIEGAND_MAX ];
  uint8_t parityLSB;
};*/

struct Packet {
  uint8_t parityMSB;
  uint64_t data;
  //char codeDec[ 14 ]; // 40 bits number is 13 decimal digits + null
  //char codeHex[ 11 ]; // 40 bits number is 10 hexadecimal digits + null
  uint8_t parityLSB;
  uint8_t wiegandType;
};

/*extern "C" {
  void PCINT0_vect( void ) __attribute__((interrupt));
  void PCINT2_vect( void ) __attribute__((interrupt));
}*/

class WIEGAND {

public:
	WIEGAND();
  WIEGAND( uint8_t pinD0, uint8_t pinD1, uint8_t checkFlag = 1 );
  ~WIEGAND();
	void begin( void );
  void begin( uint8_t pindD0, uint8_t pinD1, uint8_t checkFlag = 1 );
  void end( void );
  uint8_t available( void );
  uint8_t getParityMSB( void );
  uint8_t getParityLSB( void );
  void getCodeHex( char * code, uint8_t size );
  void getCodeDec( char * code, uint8_t size );
  void getCode( char * code, uint8_t size, uint8_t radix );
  uint8_t getWiegandType( void );
  static void readBit( void );

private:
  //static WIEGAND * instance;
  static volatile uint8_t pinD0_;
  static volatile uint8_t pinD1_;
  static volatile uint64_t buffer_;
  static volatile uint8_t bitsCounter_; // bits counter of Wiegand signal
  static volatile uint32_t lastBitTime_;    // keep track of last wiegand bit received
  uint8_t checkFlag_;
  struct Packet packet_;

  uint8_t check( void );
  //uint8_t countParity8( uint8_t n );
  uint8_t countParity( uint32_t n );
  static void writeBit0( void );
  static void writeBit1( void );

/*friend
      void PCINT0_vect( void ) __attribute__((interrupt));
      void PCINT0_vect( void ) __attribute__((interrupt));*/
};

#endif
