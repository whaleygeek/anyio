/* seg7.ino  June 2014  D.J.Whale
 *
 * Drive a 4 way 7-segment LED display
 */

//TODO bring in our seg7char.h to allow all chars to be used
//initialise a flash table with all char patterns and replace
//existing digit table

#define BAUD_RATE       9600
#define CMD_CHAR        'M'
#define NUM_COMMONS     4

#define ON              LOW
#define OFF             HIGH

#define PATTERN_0       0xC0
#define PATTERN_1       0xF9
#define PATTERN_2       0xA4
#define PATTERN_3       0xB0
#define PATTERN_4       0x99
#define PATTERN_5       0x92
#define PATTERN_6       0x82
#define PATTERN_7       0xF8
#define PATTERN_8       0x80
#define PATTERN_9       0x98
#define PATTERN_UNKNOWN 0xFF

const uint8_t patterns[] = {
  PATTERN_0, PATTERN_1, PATTERN_2, PATTERN_3,
  PATTERN_4, PATTERN_5, PATTERN_6, PATTERN_7,
  PATTERN_8, PATTERN_9
};

//TODO need to store this in a buffer that can be configured
//via a config command from serial

#define LED_A  8
#define LED_B  9
#define LED_C  2
#define LED_D  3
#define LED_E  4
#define LED_F  5
#define LED_G  6
#define LED_DP 7

#define CM0    10
#define CM1    11
#define CM2    12
#define CM3    13

const uint8_t commons[NUM_COMMONS] = {
  CM0, CM1, CM2, CM3
};

uint8_t displaybuf[NUM_COMMONS] = {
  PATTERN_UNKNOWN, PATTERN_UNKNOWN, PATTERN_UNKNOWN, PATTERN_UNKNOWN
};

int index = 0;


void allOff() 
{
  int i;
  
  digitalWrite(LED_A,  OFF);
  digitalWrite(LED_B,  OFF);
  digitalWrite(LED_C,  OFF);
  digitalWrite(LED_D,  OFF);
  digitalWrite(LED_E,  OFF);
  digitalWrite(LED_F,  OFF);
  digitalWrite(LED_G,  OFF);
  digitalWrite(LED_DP, OFF);

  for (i=0; i<NUM_COMMONS; i++)
  {
    digitalWrite(commons[i], LOW);
  }
}


void displayPattern(uint8_t common, uint8_t pattern)
{
  /* Split out the A and B segements and write them to pins 8 and 9
   * Pins 0 and 1 used for serial port
   */

  left = pattern & 0xFC;

  PORTD = left;

  /* Pattern constants inverted so OFF = ON and ON = OFF */
  if ((pattern & 0x01) == 0x01) 
  {
    digitalWrite(LED_A, OFF);
  }
  else 
  {
    digitalWrite(LED_A, ON); 
  }

  if ((pattern & 0x02) == 0x02) 
  {
    digitalWrite(LED_B, OFF);
  }
  else 
  {
    digitalWrite(LED_B, ON); 
  }

  if (common < NUM_COMMONS)
  {
    common = commons[common];
    digitalWrite(common, HIGH);
    delayMicroseconds(500);
    digitalWrite(common, LOW);
  }
}


void pollSerial(void)
{
  /* If there is new data, read it into the display buffer */  
  if (Serial.available() > 0)
  {
    char ch = Serial.read();
    if (ch == CMD_CHAR)
    { /* Always resync index when we see the cmd char */
      index = 0;
    }
    else
    { /* Fill the display buffer if valid data seen */
      if (index < NUM_COMMONS)
      {
        if (ch >= '0' && ch <= '9')
        { /* Our pattern table only holds digits at the moment */
          uint8_t digit = ch - '0';
          pattern = patterns[digit];
        }
        else
        {
          pattern = PATTERN_UNKNOWN;
        }
        displaybuf[index++] = pattern;        
      }
    }
  }
}


void updateDisplay(void)
{
  int i;
  
  for (i=0; i<NUM_COMMONS; i++)
  {
    displayPattern(i, displaybuf[i]);
  }
}


void setup() 
{
  int i;
  
  pinMode(LED_A,  OUTPUT);
  pinMode(LED_B,  OUTPUT);
  pinMode(LED_C,  OUTPUT);
  pinMode(LED_D,  OUTPUT);
  pinMode(LED_E,  OUTPUT);
  pinMode(LED_F,  OUTPUT);
  pinMode(LED_G,  OUTPUT);
  pinMode(LED_DP, OUTPUT);

  for (i=0; i<NUM_COMMONS; i++)
  {
    pinMode(commons[i], OUTPUT);
  }

  allOff();

  Serial.begin(BAUD_RATE);
}


void loop() 
{
  pollSerial();
  updateDisplay();
}

/* END */
