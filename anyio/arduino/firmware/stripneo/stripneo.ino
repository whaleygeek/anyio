/* wled  25/06/2014  D.J.Whale
 *
 * Write to a neopixel LED
 * Format is PiiRRGGBB
 * P means pixel
 * ii is the index of the LED (hex), FF means all LEDs
 * RR is the red part (hex)
 * GG is the green part (hex)
 * BB is the blue part (hex)
 */

// perhaps Pzppll configures pin pp as a strip of length ll
// perhaps Pypp selects strip on that pin as the default to write to with the P command

//TODO do these have to be in the libraries folder
//or can they be inside our app folder and still be found??
#include <Adafruit_NeoPixel.h>

#define PINA 9
#define PINB 10

#define ALL_LEDS 0xFF
#define MAX_STRIPS 14

#define BAUD            19200
#define PIN_POT         0
#define ADC_MIN         0
#define ADC_MAX         1023
#define REPORT_PRESCALE 10
#define ADC_DELTA       8

//TODO these will be static when this is a standalone module
//to enforce encapsulation

Adafruit_NeoPixel* pStrips[MAX_STRIPS];

uint8_t stripIndex;
uint8_t chIndex = 0;    // index of char being processed
uint8_t ledIndex;       // index of LED to control, or ALL_LEDS
uint8_t red;
uint8_t green;
uint8_t blue;

int prev;
int adc;
int prescale = 0;
int target;

void setup() 
{
  pStrips[0] = new Adafruit_NeoPixel(180, PINA, NEO_GRB + NEO_KHZ800);
  pStrips[0]->begin();
  pStrips[0]->show();

  pStrips[1] = new Adafruit_NeoPixel(1, PINB, NEO_GRB + NEO_KHZ800);
  pStrips[1]->begin();
  pStrips[1]->show();
  
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
  
  
  if (parseSerial())
  {
    if (ledIndex == ALL_LEDS)
    { /* write to all LEDs in all strips */
      for (int i=0; i<pStrips[0]->numPixels(); i++)
      {
        pStrips[0]->setPixelColor(i, red, green, blue);
      }
      for (int i=0; i<pStrips[1]->numPixels(); i++)
      {
        pStrips[1]->setPixelColor(i, red, green, blue);      
      }      
    }
    else
    { /* write to a single LED in all strips */
      if (ledIndex < pStrips[0]->numPixels())
      {
        pStrips[0]->setPixelColor(ledIndex, red, green, blue);
      }
      if (ledIndex < pStrips[1]->numPixels())
      {
        pStrips[1]->setPixelColor(ledIndex, red, green, blue);  
      }    
    }
    pStrips[0]->show();
    pStrips[1]->show();
  }
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

boolean parseSerial()
{
  char ch;
  
  if (Serial.available() == 0)
  {
    return false; /* Nothing to report */
  }
  
  ch = Serial.read();
  if (ch == 'P')
  { /* resync to command start */
    chIndex = 1;
    return false; /* nothing to report yet */
  }  
  
  switch (chIndex)
  {
    case 0: /* Do nothing until P command seen above */
    break;
    
    case 1: /* IH - high nybble of index */
      ledIndex = gethex(ch)<<4;
      chIndex++;
    break;
    
    case 2: /* IL - low nybble of index */
      ledIndex += gethex(ch);
      chIndex++;
    break;
    
    case 3: /* RH */
      red = gethex(ch) <<4;
      chIndex++;
    break;
    
    case 4: /* RL */
      red += gethex(ch);
      chIndex++;
    break;
    
    case 5: /* GH */
      green = gethex(ch) <<4;
      chIndex++;
    break;
    
    case 6: /* GL */
      green += gethex(ch);
      chIndex++;
    break;
    
    case 7: /* BH */
      blue = gethex(ch) <<4;
      chIndex++;
    break;
    
    case 8: /* BL */
      blue += gethex(ch);
      chIndex = 0;
      return true; /* we have a full command now */
    /* break */
  }
  return false; /* We don't have a command yet */
}


