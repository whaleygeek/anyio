# testHardware.py  08/04/2014  D.J.Whale
#
# Flash LEDs and read switches
# Designed to test the Raspberry Pi and Arduino hardware

import time
TIME = 1

#for Raspberry Pi
#import RPI.GPIO as GPIO
#BUTTON_GPIO  = [4, 14, 23, 24]
#LED_GPIO     = [10, 22, 25, 8, 7, 9, 11, 15]


# for arduino
import anyio.GPIO as GPIO
BUTTON_GPIO  = [4, 5, 2, 3]
LED_GPIO     = [7, 6, 14, 16, 10, 8, 9, 15]


SWITCH_LEFT  = BUTTON_GPIO[0]
SWITCH_RIGHT = BUTTON_GPIO[1]


def setup():
  GPIO.setmode(GPIO.BCM)
  for l in LED_GPIO:
    GPIO.setup(l, GPIO.OUT)
    GPIO.output(l, False)

  GPIO.setup(SWITCH_LEFT, GPIO.IN)
  GPIO.setup(SWITCH_RIGHT, GPIO.IN)  
  
def loop():
  old_left  = False
  old_right = False
  index = 0
  
  while True:
    # poll switches, remember they are inverted
    left   = not GPIO.input(SWITCH_LEFT)
    right  = not GPIO.input(SWITCH_RIGHT)
    #left  = True # TESTING
    #right = True # TESTING
    
    # report changes only
    if left != old_left or right != old_right:
      print("LEFT=" + str(left) + " RIGHT=" + str(right))
      old_left  = left
      old_right = right
      
    # process held switches in priority order
    if right:
      index -= 1
      if index < 0:
        index = 0
    elif left:
      index += 1
      if index > len(LED_GPIO)-1:
        index = len(LED_GPIO)-1
 
    GPIO.output(LED_GPIO[index], True)
    time.sleep(TIME)

    GPIO.output(LED_GPIO[index], False)
    time.sleep(TIME)

try:
  setup()
  loop()
finally:
  GPIO.cleanup()

# END
