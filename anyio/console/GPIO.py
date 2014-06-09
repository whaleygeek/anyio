# anyio/console/GPIO.py  21/04/2014  D.J.Whale
#
# A console based GPIO simulator

from GPIOClient import *


# CONFIGURATION ========================================================

SERVER_AUTOSTART = True
        
  
# STATIC REDIRECTORS ===================================================

instance = GPIOClient(server=SERVER_AUTOSTART)
    
def setup(mode):
  instance.setup(mode)
  
def setmode(channel, mode):
  instance.setmode(channel, mode)
  
def input(channel):
  return instance.input(channel)
  
def output(channel, value):
  instance.output(channel, value)
  
def cleanup():
  instance.cleanup()
  
# END
