# anyio/net/GPIO.py  21/04/2014  D.J.Whale
#
# A network (remote) based GPIO simulator

from GPIOClient import *


# CONFIGURATION ========================================================
        
  
# STATIC REDIRECTORS ===================================================

instance = GPIOClient()
    
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
