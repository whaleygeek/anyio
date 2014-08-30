/* blink.ino  28/07/2014  D.J.Whale */

#define LED       13
#define DELAY_ON  250
#define DELAY_OFF 250

void setup() 
{                
  pinMode(LED, OUTPUT);     
}

void loop() 
{
  digitalWrite(LED, HIGH);
  delay(DELAY_ON);
  digitalWrite(LED, LOW);
  delay(DELAY_OFF);
}

/* END OF FILE */

