/* mouse.ino  27/07/2014  D.J.Whale
 *
 * Move a mouse by pressing 4 direction buttons
 */
 
#define BUTTON_LEFT  2
#define BUTTON_RIGHT 3
#define BUTTON_UP    4
#define BUTTON_DOWN  5

#define SENSITIVITY 1
#define RATE        10


void setup()
{
  pinMode(BUTTON_LEFT, INPUT);
  pinMode(BUTTON_RIGHT, INPUT);
  pinMode(BUTTON_UP, INPUT);
  pinMode(BUTTON_DOWN, INPUT);
}
  

void loop()
{
  if (digitalRead(BUTTON_LEFT) == 0)
  {
    horizontal(-SENSITIVITY);
    delay(RATE);
  }
  else if (digitalRead(BUTTON_RIGHT) == 0)
  {
    horizontal(SENSITIVITY);
    delay(RATE);
  }
  else if (digitalRead(BUTTON_UP) == 0)
  {
    vertical(-SENSITIVITY);
    delay(RATE);
  }
  else if (digitalRead(BUTTON_DOWN) == 0)
  {
    vertical(SENSITIVITY);
    delay(RATE);
  }
}

void vertical(int amount)
{
  Mouse.move(0, amount, 0);
}

void horizontal(int amount)
{
  Mouse.move(amount, 0, 0);
}

/* END */

