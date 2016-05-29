INSTRUCTIONS
====

This package consists of two parts - a Python module that runs on a PC, 
Mac or Linux machine (well, anything that can run Python and pyserial), 
and some firmware that is programmed into an Arduino board.
At the moment we only support an Arduino Pro Micro from SparkFun, but 
there's nothing really stopping it working on any Arduino. 
The two are linked together by a serial port controlled by the pyserial 
library. Calls to the anyio.GPIO methods on the host computer will 
cause reads or writes to the GPIO pins on the arduino platform.


In this way, it is possible to write a hardware control program on any 
platform, that can easily be ported between different platforms 
(including the Raspberry Pi). Just change the "import RPi.GPIO as GPIO"
to "import anyio.GPIO as GPIO" and change your pin numbers, and you'll
be working in no time!


The serial link between the two parts runs at 115200bps, and each 
command is only a few characters, so the system performs reasonably well
unless you are repeatedly polling or changing lots of GPIO's at the same 
time.


USING A PRE-PROGRAMMED PRO-MICRO
----

For your convenience, you can buy an Arduino Pro Micro 3.3V 8MHz
device with pre-soldered pin headers and pre-programmed anyio firmware
from here:

http://skpang.co.uk/catalog/pro-micro-33v8mhz-with-headers-and-anyio-firmware-p-1327.html

All you need to do is plug it in, download this module by choosing
the "Download as Zip" button, unzip the file and run the test programs,
and you'll be working in no time!


PROGRAMMING THE PRO MICRO YOURSELF
----

If you are already familiar with Arduino programming and soldering, 
you might like to buy the raw components yourself and solder some
headers on and program it yourself.


You can buy the bare unprogrammed Sparkfun ProMicro from here:
https://www.sparkfun.com/products/12587


You can learn how to program the firmware here:

https://learn.sparkfun.com/tutorials/pro-micro--fio-v3-hookup-guide/introduction


The firmware that I use is in the following folder:
anyio/arduino/firmware/gpio/gpio.ino


TESTING THE PRO MICRO
----

At the moment, this has only been tested with Python 2.7 - there are 
know issues we are looking at with some slightly earlier versions of 
2.6 (to do with reading the version number of Python), and some known 
issues with all versions of Python 3 that we are looking at 
(to do with the way sub packages are imported). Our specific need 
was for a GPIO attachment for PC/Mac/Linux that worked with Python 2.7,
but I will investigate and fix these issues later in 2014.


On Mac, you should just be able to plug the device in once it is 
programmed, and it hould appear as a serial port. When it pops up
a box saying that a keyboard has been found, just close that box
by pressing the red X button in the corner.


On windows, you have to install the ProMicro.inf using the instructions 
in the SparkFun hookup guide above. Note that this is a bit complex
on Windows 8.1 due to the new "Driver enforcement mode" of windows,
but the hookup guide on the sparkfun site does work if you follow
it very carefully.


On linux (I've only tested on Ubunto 12.04LTS), plug it in and it
should detect the device the first time you run one of your programs.


Once the device has it's firmware installed and it is plugged in, you 
are ready to run the test programs. Wire up a LED to pin 15, then run:


    python testLED.py


The first time this runs, it will work out the serial port number. 
It will prompt you to remove the device and press ENTER (at which point 
it scans your system for serial ports), and then it will prompt you to 
insert the device and press ENTER (at which point it will re-scan to 
detect the newly inserted port). We have found this to be the most 
reliable method on all platforms for detecting a new serial port, 
compared to opening and testing each port (which can sometimes cause 
devices to lock up).


You will be shown the name of your serial port, and asked if you want 
to remember it - always choose Y so that your program will run correctly 
the next time you run it. It creates a small file called 'portscan.cache'
which remembers the serial port number, you can delete this file if your 
serial port number changes for any reason and you want it to re-scan
your system.


Your LED should now be flashing.


Once you have tried that, wire a button up to pin 4 and run:


    python testButton.py


If you are getting excited at this point, wire up a 7-segment display to
pins 7=a, 6=b, 14=c, 16=d, 10=e, 8=f, 9=g, 15=dp

If your display is Common Anode, edit testSeg7.py and set ON=True
If your display is Common Cathode, edit testSeg7.py and set ON=False

Now run this to count through the numbers 0 to 9 on the display:


    python testSeg7.py


USE OF PYSERIAL
----

This module uses pyserial to communicate with the Arduino Pro Micro.

anyio modifies the Python PACKAGEPATH for you when it runs,
to make sure that it uses this embedded pyserial rather than one
that might or might not be installed on your system. This means
that if you don't have pyserial installed on your system, you should
still be able to just run this out of the box and it should work.

If you already have pyserial installed and want to use your installed
version for any reason, you can change the anyio/arduino/GPIO.py
USE_EMBEDDED_PYSERIAL = False


USING THE CONSOLE SIMULATOR
----

If you don't have a ProMicro or any other Arduino, or if perhaps you
want to use this library to help you simulate GPIO on a Mac/PC/Linux
machine for later running on a Raspberry Pi, here is how you can
do that.

I will explain a full worked example using the sample test code.

Edit the testLED.py and change the arduino line so it says this:

    import anyio.console.GPIO as GPIO
    LED = 15

This imports a 'simulated' GPIO that runs inside the console window
of the python program when you run it.

Now run the code like this (python2 only at the moment!):


    python testLED.py


You will get this output:


    PIN   0123 4567 89AB CDEF G
    MODE  XXXX XXXX XXXX XXXO X
    STATE XXXX XXXX XXXX XXX0 X

    PIN   0123 4567 89AB CDEF G
    MODE  XXXX XXXX XXXX XXXO X
    STATE XXXX XXXX XXXX XXX1 X

    PIN   0123 4567 89AB CDEF G
    MODE  XXXX XXXX XXXX XXXO X
    STATE XXXX XXXX XXXX XXX0 X

    PIN   0123 4567 89AB CDEF G
    MODE  XXXX XXXX XXXX XXXO X
    STATE XXXX XXXX XXXX XXX1 X


You can see column 'F' (GPIO 15) is set as MODE output, and it's STATE
changes from 0 to 1 - this is your simulated LED flashing!!


Now to test a GPIO input.

Modify the testButton.py so that the arduino line looks like this:


    # Arduino
    import anyio.console.GPIO as GPIO
    BUTTON = 4


Run it like this (python2 only at moment!):

    python testButton.py


You will see this on the screen:

    released
    released
    released
    released
    released
    released


The GPIO 4 for the button is currently high, so this is detected as the testButton.py
as a released button.

Now on the console (while it is scrolling) type 40 then press ENTER.

The first character is the PIN identifier (0-9 A-F) and the second character
is a command (0=setlow, 1=sethigh, I=makeinput, O=makeoutput)

Now you should see this:


    4released
    released
    released
    0released
    released
    
    pressed
    pressed
    pressed
    pressed
    pressed


You have just pressed the button! (GPIO 4 is now zero/low so the testButton.py
program sees this as a pressed button).

To release the button, type 41 ENTER into the console to set the input pin high.

It's not pretty, but it does work, and it's good enough sometimes for me to
write new code on the train when I don't have a Raspberry Pi to hand. When I get
home I load the code onto my Raspberry Pi, modify the 'import' back to RPi.GPIO
and the code usually works first time!




FUTURE WORK
----

This package contains a console based (text mode) simulator that can be 
used to test your programs on before you connect to real hardware, and 
this supports both inputs and outputs. This console package
works, but is not completely documented yet. You can try it with this:
import anyio.console.GPIO as GPIO


The GPIO interface itself could be anything, not just an Arduino. 
There is a very simple protocol between the python module and the target 
GPIO hardware, that is written in a way to allow future extension to 
support other hardware peripherals such as I2C, SPI, UART, PWM, Analog, 
OneWire and other protocols and features aimed at near real time control 
and sensing.


There are placeholders in the design for a tkinter GUI simulator that I 
am planning to write soon, and also a network aware version, that 
allows GPIO controls to be sent remotely over a network connection to 
a GPIO server running on any arbitrary host computer (e.g. a Raspberry Pi).


NOTES ABOUT COPYRIGHTED MATERIAL
----

The source code in the anyio package is (c) 2014 David Whale.

There is an embedded version of pyserial inside the anyio package, and 
this is provided in it's entirely complete form, with it's original 
licence, which allows for it to be embedded inside other packages with 
out any special install. 

There is an embedded version of the ProMicro.inf file, which came from
the SparkFun github repository. It is included here for convenience,
but the latest copy can always be retrieved from here:

https://github.com/sparkfun/SF32u4_boards/blob/master/driver/ProMicro.inf

David Whale

@whaleygeek

June 2014
