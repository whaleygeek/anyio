anyio
=====

A GPIO Package that works on any platform


The anyio package aims to mimic the basic functionality of the RPi.GPIO package that is used on the Raspberry Pi computer. 'Mimic' is used in the loosest sense of the word, because all it does is to implement about 5 methods of the same name as the RPi.GPIO package.


This package consists of two parts - a Python module that runs on a PC, Mac or Linux machine (well, anything that can run Python and pyserial), and some firmware that is programmed into an Arduino board.
At the moment we only support an Arduino Pro Micro from SparkFun, but there's nothing really
stopping it working on any Arduino. The two are linked together by a serial port controlled by the pyserial library. Calls to the anyio.GPIO methods on the host computer will cause reads or writes to the GPIO pins on the arduino platform.


In this way, it is possible to write a hardware control program on any platform, that can easily be ported between different platforms (including the Raspberry Pi).


The package contains a console based (text mode) simulator that can be used to test your programs on before you connect to real hardware, and this supports both inputs and outputs. This console package
works, but is not completely documented yet.


The serial link between the two parts runs at 115200bps, and each command is only a few characters, so the system performs reasonably well unless you are repeatedly polling or changing lots of GPIO's at the same time.


PROGRAMMING THE PRO MICRO

You will soon be able to buy a pre-programmed and pre-soldered ProMicro. 
In the meantime, you can do the programming yourself following the guide here:

https://learn.sparkfun.com/tutorials/pro-micro--fio-v3-hookup-guide/introduction


TESTING THE PRO MICRO

At the moment, this has only been tested with python 2.7.3 - there are know issues
we are looking at with some slightly earlier versions of 2.6 (to do with reading the
version number of python), and some known issues with all versions of python 3
that we are looking at (to do with the way sub packages are imported). These will
be fixed before final release.


On Mac, you should just be able to plug the device in once it is programmed, and it
should appear as a serial port.

On windows, you have to install the ProMicro.inf using the instructions in the SparkFun
hookup guide above.

Once the device has it's firmware installed and it is plugged in, you are ready to
run the test programs. Wire up a LED to pin 15, then run:

  python testLED.py


The first time this runs, it will work out the serial port number. It will prompt
you to remove the device and press ENTER (at which point it scans your system for
serial ports), and then it will prompt you to insert the device and press ENTER
(at which point it will re-scan to detect the newly inserted port). We have found
this to be the most reliable method on all platforms for detecting a new serial
port, compared to opening and testing each port (which can sometimes cause devices
to lock up).

You will be shown the name of your serial port, and asked if you want to remember
it - always choose Y so that your program will run correctly the next time you
run it. It creates a small file portscan.cache which remembers the serial port
number, you can delete this file if your com port number changes and you want
it to re-scan.


Your LED should now be flashing.


Once you have tried that, wire a button up to pin 4 and run:

  python testButton.py


If you are getting excited at this point, wire up a 7-segment display to
pins 7=a, 6=b, 14=c, 16=d, 10=e, 8=f, 9=g, 15=dp

If your display is Common Anode, edit testSeg7.py and set ON=True
If your display is Common Cathode, edit testSeg7.py and set ON=False

Now run this:

  python testSeg7.py



FUTURE WORK


The GPIO interface itself could be anything, not just an Arduino. There is a very simple protocol between the python module and the target GPIO hardware, that is written in a way to allow future extension to support other hardware peripherals such as I2C, SPI, UART, PWM, Analog, OneWire and other protocols and features aimed at near real time control and sensing.


While you can use any platform for the target hardware side, I have selected a specific Arduino platform for use, and in future there will be a web shop where you can buy a pre-soldered and pre-programmed board, that you just plug into your Mac or PC and it works out of the box.


There are placeholders in the design for a tkinter GUI simulator that I am plannign to write soon, and also a network aware simulator that allows GPIO controls to be sent remotely over a network connection to a GPIO server running on any arbitrary host computer (e.g. a Raspberry Pi).


NOTES ABOUT COPYRIGHTED MATERIAL

The source code in the anyio package is (c) 2014 David Whale.

There is an embedded version of pyserial inside the anyio package, and this is provided
in it's entirely complete with it's original licence, which allows for it to be embedded
inside other packages.


David Whale
June 2014

