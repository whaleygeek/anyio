/* gpio.ino  04/04/2014  D.J.Whale
 *
 * PURPOSE
 *   This simple firmware is designed for an Arduino Pro Micro.
 *   It implements a simple but compact text-based protocol
 *   to allow a host computer (e.g. a PC or a Mac or a Linux PC)
 *   to drive GPIO hardware in the same way that a Raspberry Pi does.
 *
 * CONTEXT
 *   There is a supporting Python module that communicates with this
 *   code over a serial port, using the pyserial open source library.
 *   GPIO functin calls from within a Python program on the Mac/PC cause
 *   outputs to change or inputs to be read, using this firmware.
 *
 * USAGE
 *   Build and download this firmware to your Arduino Pro Micro,
 *   then run the supporting Python program to make GPIO programs
 *   communicate with this code.
 *
 * NOTES
 *   1. Commands are processed "on the fly", and as such, there is no
 *      memory buffer used to process commands. This means that there
 *      are no internal buffers to overflow, so you can hit the serial
 *      port with characters as fast as it can take them. The Arduino
 *      USB library has it's own internal buffers on the endpoints, and
 *      USB will flow-control with the host when these buffers are full.
 *      This keeps this firmware very simple.
 *
 * LIMITATIONS
 *   1. The speed with which hardware can be accessed is moderate but
 *      not blindingly fast. But most GPIO programs written in Python
 *      will work using this firmware. To gain faster access to
 *      hardware, use the advanced firmware which has many more
 *      features designed for fast hardware access.
 *
 *   2. Synchronised access to hardware is not possible with the raw
 *      GPIO command set, as each GPIO is read/written as the command
 *      is received. Proiding you do not need accurate simultaneous
 *      reading/writing of GPIO pins, the speed/skew will be ok.
 *      If you need synchronised pin reading or writing, use the
 *      advanced firmware which has many more features designed to
 *      access pins in a synchronous way.
 *
 * ADVANCED FEATURES
 *   This is more than just a GPIO library, although in this version,
 *   only GPIO is implemented. Other more advanced versions of this
 *   code support a range of other facilities such as I2C, SPI,
 *   PWM, ADC, OneWire, NeoPixels, UART, synchronised hardware access,
 *   pin edge timing, multi-pin read and write, and many other features.
 *
 *   If you are interested in the advanced version, please contact
 *   david@thinkingbinaries.com
 */


/* The version string is sent back via the 'V' command */

#define VERSION_STR "ArduinoProMicro/GPIO V0.1.1"


/* The serial baud rate. Set this to as high as you can to get
 * good performance. If you poll a lot of inputs, this needs to be
 * very high. 115200 is assumed by the standard Python code.
 */
 
#define BAUD_RATE 115200


/* Arduino pins on the Pro Micro are numbered 0..16. The simple
 * protocol labels these as "abcd efgh ijkl mnop q" respectively.
 */

#define MIN_PIN 0
#define MAX_PIN 16

/* Errors are sent back via the 'E' response. */
 
enum
{
  ERROR_BAD_PIN_RANGE   = 1,
  ERROR_UNKNOWN_COMMAND = 2
};


/***** LOCAL FUNCTION PROTOTYPES *****/

static void command(char cmdch, char paramch);
static void gpio(char pinch, char cmdch);
static void error(int code);


/*--------------------------------------------------------------------*/
/* Configure the hardware.
 */
 
void setup()
{
  /* All pins start as INPUT */
  for (int i=MIN_PIN; i<=MAX_PIN; i++)
  {
    pinMode(i, INPUT);
  }
  /* Commands from the USB channel */
  Serial.begin(BAUD_RATE);
}


/*--------------------------------------------------------------------*/
/* The main program loop.
 *
 * Read in and process a single command at a time.
 *
 * Commands are at least 2 characters. The first character is always 
 * the command char, which may be followed by optional parameters. 
 * Newlines are totally ignored, which means it is possible to send a 
 * long string of commands in one go like this: aIbIcIa1b1c0
 *
 * Because GPIO commands always start with the pin char (lower case
 * letter), we already know it is a GPIO command. GPIO commands
 * are the most common command, so this prevents the need to always
 * preceed a GPIO command with a 'G' character, as each character
 * sent on the serial port inserts a delay of 8 bit periods.
 *
 * Other commands start with an upper case letter followed by
 * optional parameters.
 */
 
void loop()
{
  if (Serial.available()>=2)
  { /* A command is waiting */
    char pinch = (char) Serial.read();
    if (pinch != '\r' && pinch != '\n')
    { /* not a newline */
      if (pinch >= 'A' && pinch <= 'Z')
      { /* Upper case, so it's a global command */
        char paramch = (char) Serial.read();
        command(pinch, paramch);
      }
      else if (pinch >= 'a' && pinch <= 'z')
      { /* Lower case, so it's a pin number for default GPIO module */
        char cmdch = (char) Serial.read();
        gpio(pinch, cmdch);
      }
      else
      { /* Anything else is rejected */
        error(ERROR_UNKNOWN_COMMAND);
      }
    }
  }
}


/*--------------------------------------------------------------------*/
/* Send an error message to the host.
 *
 * Error messages are always E + <number> followed by a newline.
 */
 
static void error(int code)
{
  Serial.print('E');
  Serial.println(code);
}


/*--------------------------------------------------------------------*/
/* Process a command.
 *
 * A command is a single character that identifies the command,
 * followed by parameters. In most cases parameters are a single
 * character, but there is no need for them to be. Each command knows
 * how to detect the end of it's parameter string, be that length
 * or magic character based.
 */
 
static void command(char cmdch, char paramch)
{
  switch (cmdch)
  {
    case 'V': /* Version */
      Serial.println(VERSION_STR);
    break;
    
    case 'G': /* GPIO */
    {
      while (Serial.available() == 0)
      {
        /* busy wait for parameter to arrive */
      }
      /* GPIO commands are two chars */
      char pinch = paramch;
      char cmdch = (char)Serial.read();
      gpio(pinch, cmdch);
    }
    break;
    
    default:
      /* All other commands are rejected in this simple version */
      error(ERROR_UNKNOWN_COMMAND);
    break;
  }
}


/*--------------------------------------------------------------------*/
/* Process a GPIO command.
 *
 * GPIO Commands are two characters.
 * <pinch> <cmdch>
 *
 * pinch is the pin character (a..z) where 'a' represents pin 0.
 * cmdch is the command to perform on that pin:
 *   I: Set this pin to a digital input,        eg: aI
 *   O: Set this pin to a digital output,       eg: aO
 *   0: Write a digital low to this output,     eg: a0
 *   1: Write a digital high to this output,    eg: a1
 *   ?: Read the state of this digital input,   eg: a?
 *      State is returned as pinch + state(0|1) eg: a0
 */
 
static void gpio(char pinch, char cmdch)
{
  int pin = pinch - 'a';
  
  if (pin < MIN_PIN || pin > MAX_PIN)
  { /* out of range pin label */
    error(ERROR_BAD_PIN_RANGE);
  }
  else
  { /* valid pin label */
    switch (cmdch)
    {
      case 'I': /* set as digital input */
        pinMode(pin, INPUT);
      break;
      
      case 'O': /* set as digital output */
        pinMode(pin, OUTPUT);
      break;

      case '0': /* write to digital LOW */
        digitalWrite(pin, LOW);
      break;
      
      case '1': /* write to digital HIGH */
        digitalWrite(pin, HIGH);
      break;
      
      case '?': /* read value of digital input pin */
      {
        int p = digitalRead(pin);
        Serial.print(pinch);
        Serial.println(p?"1":"0");
      }
      break;
      
      default: /* unhandled */
        error(ERROR_UNKNOWN_COMMAND);
      break;
    }
  }
}


/***** END OF FILE *****/
