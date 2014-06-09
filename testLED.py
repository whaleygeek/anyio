# testLED.py - 06/06/2014 D.J.Whale
#
# Test flashing an LED

import time
TIME = 0.5

# Raspberry PI
#import RPi.GPIO as GPIO
#LED = 15

# Arduino
import anyio.GPIO as GPIO
LED = 15

GPIO.setmode(GPIO.BCM)
GPIO.setup(LED, GPIO.OUT)

def flash(t):
  GPIO.output(LED, True)
  time.sleep(t)
  GPIO.output(LED, False)
  time.sleep(t)

try:
  while True:
    flash(TIME)
finally:
  GPIO.cleanup()
  
# END
