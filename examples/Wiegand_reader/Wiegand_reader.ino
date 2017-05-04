#include <Wiegand.hpp>

// D0 - pin 4, D1 - pin 5, without check parity - 0
WIEGAND wg( 4, 5, 0 );
char codeHex[ 11 ];
char codeDec[ 20 ];

void setup() {
  Serial.begin(57600);
  wg.begin();
}

void loop() {
  if( !wg.available() )
  {
    uint8_t type = wg.getWiegandType();
    Serial.print( "Type: W" );
    Serial.println( type );

    wg.getCodeHex( codeHex, 11 );
    Serial.print("Wiegand HEX code: ");
    Serial.println( codeHex );

    wg.getCodeDec( codeDec, 20 );
    Serial.print("Wiegand DEC code: ");
    Serial.println( codeDec );

    // Using function with radix
    wg.getCode( codeHex, 11, 16 );
    Serial.print("Wiegand HEX code: ");
    Serial.println( codeHex );

    wg.getCode( codeDec, 20, 10 );
    Serial.print("Wiegand DEC code: ");
    Serial.println( codeDec );
  }
}
