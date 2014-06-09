# Adventure 5: testDisplay2.py

import anyio.seg7 as display
import time

#import RPi.GPIO as GPIO
#LED_PINS = [10,22,25,8,7,9,11,15]
import anyio.GPIO as GPIO
LED_PINS = [7,6,14,16,10,8,9,15]

GPIO.setmode(GPIO.BCM)

ON = True # common-anode. Set to True for a common cathode display

display.setup(GPIO, LED_PINS, ON)

try:
  while True:
    for d in range(10):
      display.write(str(d))
      time.sleep(0.5)
finally:
  GPIO.cleanup()
  
# END
