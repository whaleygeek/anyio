/* servo.ino  (c) 2014 David Whale
 *
 * Read an analog pot and report value via serial port.
 * read a Snn command from the serial port and move a servo
 * to that position.
 *
 * Thus, this code is the data acquisition and actuation system,
 * but the control loop is external.
 */
 
#include <Servo.h> 

#define BAUD            19200
#define PIN_POT         0
#define PIN_SERVO       9
#define ADC_MIN         0
#define ADC_MAX         1023
#define SERVO_MIN       10
#define SERVO_MAX       168
//#define SERVO_UPDATE_MS 100
#define REPORT_PRESCALE 10
#define ADC_DELTA       8

Servo servo;

int prev;
int adc;
int prescale = 0;
int target;
 
void setup() 
{ 
  servo.attach(PIN_SERVO);
  Serial.begin(BAUD);
  prev = analogRead(PIN_POT);
} 
 
void loop() 
{ 
  /* SAMPLE ADC AND REPORT */
  
  adc = analogRead(PIN_POT);
  if (abs(adc-prev) > ADC_DELTA)
  {
    //NOTE: Can't delay here, it toshes servo performance later
    //delay(SERVO_UPDATE_MS);
    prescale++;
    if (prescale >= REPORT_PRESCALE)
    {
      prescale = 0;
      
      Serial.write(tohexch(adc >> 8));
      Serial.write(tohexch(adc >> 4));
      Serial.write(tohexch(adc));
      Serial.write('\n');
      prev = adc;
    }
  }
  

  /* READ SERVO TARGET AND UPDATE */  
  
  if (parseSerial())
  {  
    target = min(target, SERVO_MAX);
    target = max(target, SERVO_MIN);
    servo.write(target);
  } 
} 

int chindex = 0;

boolean parseSerial()
{
  if (! Serial.available())
  {
    return false; /* nothing to do */
  }
  char ch = Serial.read();
  if (ch == 'S')
  { /* resync to start of command */
    chindex = 1;
    return false; /* nothing to do */
  }
  
  switch (chindex)
  {
    case 0: /* Do nothing until S command seen above */
    break;
    
    case 1: /* HIGH */
      target = gethex(ch) << 4;
      chindex++;
    break;
    
    case 2: /* LOW */
    
      target += gethex(ch);
      chindex = 0;
      return true; /* Value to process */
    /* break */
  }
  return false; /* nothing to do */
}


uint8_t gethex(char ch)
{
  if (ch >= '0' && ch <= '9')
  {
    return ch - '0';
  }
  if (ch >= 'A' && ch <= 'F')
  {
    return (ch - 'A') + 10;
  }
  return 0; /* unknown */
}

char tohexch(uint8_t val)
{
  val = val & 0x0F;
  if (val > 9)
  {
    return 'A' + (val-10);
  }
  else
  {
    return '0' + val;
  }
}



