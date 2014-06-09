# anyio/net/GPIOClient  22/04/2014  D.J.Whale
#
# A class based interface to the net GPIO connector


# CONFIGURATION ========================================================

MIN_PIN = 0
MAX_PIN = 16
LABELS  = "0123456789ABCDEFGH"

IN      = 0
OUT     = 1
BCM     = 0
BOARD   = 1
HIGH    = 1
LOW     = 0

def trace(msg):
  print(str(msg))
  
def write(msg):
  print(str(msg))
  
  
#TODO wrap a protocol() around a net() around a network.py instance
#All of this will then come out in the wash.
  
# CLASS ================================================================

class GPIOClient:
  pinmode  = {}
  pinstate = {}

  def __init__(self):
    write("TODO - not yet implemented")
    #TODO use net to start a client and connect
    pass
      
  def setup(mode):
    # BCM or BOARD
    pass

  def setmode(self, channel, mode):
    self.pinmode[channel] = mode

    if mode == INPUT:
      self.pinstate[channel] = HIGH

    elif mode == OUTPUT:
      self.pinstate[channel] = LOW
      self._update()

  def input(self, channel):
    try:
      return self.pinstate[channel]
    except KeyError:
      return HIGH
      
  def output(self, channel, value):
    self.pinstate[channel] = self._pinValue(value)
    self._update()

  def cleanup(self):
    self.pinmode = {}
    self.pinstate = {}
    
  def _pinValue(self, v):
    if v == None or v == False or v == 0:
      return LOW
    return HIGH
    
  def _update(self):
    #TODO use net to write a pin change message
    #NOTE: We will use protocol()
    # show mode of each pin
    # show state of each pin
    pass # TODO   

  def changeInput(self, channel, value):
    if self.pinmode[channel] != INPUT:
      raise ValueError("Pin is not an input")
    self.pinstate[channel] = self._pinValue(value)        
  

# END

