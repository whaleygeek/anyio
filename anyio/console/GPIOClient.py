# anyio/console/GPIOClient  22/04/2014  D.J.Whale
#
# A class based interface to the console GPIO simulator

import sys

try:
  import thread # python2
except ImportError:
  import _thread as thread # python3

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
  
if sys.version_info.major > 2:
  def ask(msg=""):
    return input(msg) # python 3
else:
  def ask(msg=""):
    return raw_input(msg) # python 2
  
  
# CLASS ================================================================

class GPIOClient:
  pinmode  = {}
  pinstate = {}

  def __init__(self, server=False):
    if server:
      self.controlInputs(True)
      
  def setup(mode):
    # BCM or BOARD
    pass

  def setmode(self, channel, mode):
    self.pinmode[channel] = mode

    if mode == INPUT:
      self.pinstate[channel] = HIGH

    elif mode == OUTPUT:
      self.pinstate[channel] = LOW
      self._show()

  def input(self, channel):
    try:
      return self.pinstate[channel]
    except KeyError:
      return HIGH
      
  def output(self, channel, value):
    self.pinstate[channel] = self._pinValue(value)
    self._show()

  def cleanup(self):
    self.pinmode = {}
    self.pinstate = {}
    
  def _pinValue(self, v):
    if v == None or v == False or v == 0:
      return LOW
    return HIGH

  def _show2(self):
    write("mode:" + str(self.pinmode))
    write("state:" + str(self.pinstate))
    write("")
    
  def _show(self):
    line = "PIN   "
    for p in range(MIN_PIN, MAX_PIN+1):
      line += LABELS[p]
      if (p+1) % 4 == 0:
        line += " "
    write(line)
   
    line = "MODE  "
    for p in range(MIN_PIN, MAX_PIN+1):
      try:
        if self.pinmode[p] == INPUT:
          line += "I"
        elif self.pinmode[p] == OUTPUT:
          line += "O"
        else:
          line += "?"
      except KeyError:
        line += "X"
      if (p+1) % 4 == 0:
        line += " "
    write(line)
    
    line = "STATE "
    for p in range(MIN_PIN, MAX_PIN+1):
      try:
        if self.pinstate[p] == 1:
          line += "1"
        elif self.pinstate[p] == 0:
          line += "0"
        else:
          trace("unknown[" + str(p) + "]:" + str(self.pinstate[p]))
          line += "?"
      except KeyError:
        line += "X"
      if (p+1) % 4 == 0:
        line += " "
    write(line)
    write("")

  def changeInput(self, channel, value):
    if self.pinmode[channel] != INPUT:
      raise ValueError("Pin is not an input")
    self.pinstate[channel] = self._pinValue(value)        

    
  # INPUT CONTROL ======================================================

  _serverRunning = False
  _kbdThread = None
      
  def controlInputs(self, flag):
    if flag and not self._serverRunning:
      self._startServer()
    elif not flag and self._serverRunning:
      self._stopServer()

  def _startServer(self):
    self._kbdThread = thread.start_new_thread(self._server, ())
    self._serverRunning = True
    
  def _stopServer(self):
    self._kbdThread.stop()
    self._kbdThread = None
    self._serverRunning = False

  def _parse_pinch(self, ch):
    # the index into LABELS is the channel number
    return LABELS.index(ch)
    
  def _getcmd(self):
    while True:
      cmdstr = ask()
      cmdstr = cmdstr.strip()
      if len(cmdstr) >= 2:
        return cmdstr

  def _parsecmd(self, cmdstr):
    pinch   = cmdstr[0]
    valuech = cmdstr[1]
    channel = self._parse_pinch(pinch)
    return channel, valuech
    
  def _process(self, channel, valuech):
    if valuech == "I":
      self.setmode(channel, INPUT)
    elif valuech == "O":
      self.setmode(channel, OUTPUT)
    elif valuech == "1":
      self.changeInput(channel, True)
    elif valuech == "0":
      self.changeInput(channel, False)
    
  def _server(self):
    while True:
      cmdstr = self._getcmd() 
      channel, valuech = self._parsecmd(cmdstr)
      #trace("processing: channel:" + str(channel) + " cmd:" + valuech)
      self._process(channel, valuech)
  
# END
