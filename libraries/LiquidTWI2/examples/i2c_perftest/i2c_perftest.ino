/*
 Benchmark sketch for Adafruit i2c/SPI LCD backpack
 using MCP23008 I2C expander
 ( http://www.ladyada.net/products/i2cspilcdbackpack/index.html )

 Times how long it takes to write 47 characters (the estimated amount
 of off-screen characters on a 16x2 LCD) to the display under various
 i2c clock rates.
 
 Written to demonstrate the performance of the modified pure i2c LCD
 library by FalconFour ( http://falconfour.com )
 
  The circuit for Duemilanove:
 * 5V to Arduino 5V pin
 * GND to Arduino GND pin
 * CLK to Analog #5
 * DAT to Analog #4

  For other boards, use the following chart (CLK=SCL,DAT=SDA)
  Board      I2C / TWI pins
  ---------  --------------------------
  Uno, Yun   A4 (SDA), A5 (SCL)
  Mega2560   20 (SDA), 21 (SCL)
  Leonardo    2 (SDA),  3 (SCL)
  Due        20 (SDA), 21 (SCL)
*/

#include <Wire.h>
#include <LiquidTWI2.h>
// I2C address 0x20 ... change the address if your board uses a different one
LiquidTWI2 lcd(0x20);

byte digit = 0;
unsigned long freqMult = 0;

void setup() {
  // set the LCD type
  lcd.setMCPType(LTI_TYPE_MCP23008); 
//  lcd.setMCPType(LTI_TYPE_MCP23017); 
  lcd.begin(16,2);
  lcd.setBacklight(true);
  lcd.clear();
  lcd.print("Performance test");
  delay(1000);
  lcd.clear();
  lcd.print("Default speed.");
}

void loop() {
  byte x;
  long msec = millis();
  long minitimer;
  while (millis() - msec < 5000) {
    minitimer = millis();
    lcd.setCursor(16,0);
    for (x=0; x<47; x++) {
      lcd.write(digit++);
    }
    lcd.setCursor(0,1);
    lcd.print("47 char, ");
    lcd.print(millis() - minitimer,DEC);
    lcd.print("msec");
  }
  delay(1000);
  ++freqMult;
  bitSet(TWSR, TWPS0);
  lcd.clear();
  lcd.print("Freq = ");
  lcd.print(12500UL * freqMult);
  TWBR = (((F_CPU/(12500ul*freqMult))-16)/2)/4;
}
