# ports_win32.py  22/04/2014  D.J.Whale
#
# Scan the windows registry to find a list of COM ports

try:
  import _winreg as registry # python2
except ImportError:
  import winreg as registry # python3

KEY = r"HARDWARE\DEVICEMAP\SERIALCOMM"

def scan():
  reg = registry.ConnectRegistry(None, registry.HKEY_LOCAL_MACHINE)
  key = registry.OpenKey(reg, KEY)

  ports = []

  i = 0
  while True:
    try:
      value = registry.EnumValue(key, i)   
      name, value, vtype = value
      print("port[" + str(i) + "]:" + str(value))
      ports.append(value)
      i += 1
    
    except EnvironmentError:
      break
    
  return ports


# TESTER

if __name__ == "__main__":
  d = find()
  print(str(d))
  
# END
