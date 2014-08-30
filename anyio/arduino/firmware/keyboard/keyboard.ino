/* keyboard.ino  27/07/2014  D.J.Whale
 *
 * ProMicro HID driver to press keys
 */
 
#define BUTTON_RED    2
#define BUTTON_YELLOW 3
#define BUTTON_GREEN  4
#define BUTTON_BLUE   5

#define HOLD_DELAY 250


void setup()
{
  pinMode(BUTTON_RED, INPUT);
  pinMode(BUTTON_YELLOW, INPUT);
  pinMode(BUTTON_GREEN, INPUT);
  pinMode(BUTTON_BLUE, INPUT);
}

void loop()
{
  if (digitalRead(BUTTON_RED) == 0)
  {
    Keyboard.write('R');
    delay(HOLD_DELAY);
  }
  else if (digitalRead(BUTTON_YELLOW) == 0)
  {
    Keyboard.write('Y');
    delay(HOLD_DELAY);
  }
  else if (digitalRead(BUTTON_GREEN) == 0)
  {
    Keyboard.write('G');
    delay(HOLD_DELAY);
  }
  else if (digitalRead(BUTTON_BLUE) == 0)
  {
    Keyboard.write('B');
    delay(HOLD_DELAY);
  }
}

/* END */

