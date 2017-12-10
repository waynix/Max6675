#include <SPI.h>

const int CS_PIN = 2;
const int MISO_PIN = 12;
const int CLK_PIN = 13;

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600); 
  pinMode(CS_PIN, OUTPUT);
  pinMode(MISO_PIN, INPUT);
  pinMode(CLK_PIN, OUTPUT);
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV32);
  SPI.setDataMode(SPI_MODE0); //SCK idle low, latch data rising edge
  SPI.begin();
}

//Returns The Temperature
//On error returns -1
float readtemp()
{
  digitalWrite(CS_PIN,0);
  // initialize SPI:
  delay(10);
  int16_t retval = SPI.transfer16(0x0000);
  //bit 15 = sign (dummy)
  //bit 14-3 = 12 bit value
  //bit 2 thermocouple input: 1 = error, 0 = Ok
  //bit 1 = 0
  //bit 0 threestate 
  Serial.print("temp = ");
  float temp12bit = (retval >> 3) / 4.0f ;//3;
  Serial.print(temp12bit);  
  if(retval & 0x4)
  {
    Serial.print("No Probe Connected");
    return -1.0f;
  }
  if(retval & 0x2)
  {
    Serial.print("wrong device:");
    return -1.0f;
  }
  
  Serial.println();
  digitalWrite(CS_PIN,1);
  return temp12bit;
}

void loop() {
  readtemp();
  delay(200);                     
}
