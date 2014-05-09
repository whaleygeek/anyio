anyio
=====

A GPIO Package that works on any platform


The anyio package aims to mimic the basic functionality of the RPi.GPIO package that is used on the Raspberry Pi computer. 'Mimic' is used in the loosest sense of the word, because all it does is to implement about 5 methods of the same name as the RPi.GPIO package.


This package consists of two parts - a python module that runs on a PC, Mac or Linux machine (well, anything that can run Python and pyserial), and some firmware that is programmed into an Arduino board. The two are linked together by a serial port controlled by the pyserial library. Calls to the anyio.GPIO methods on the host computer will cause reads or writes to the GPIO pins on the arduino platform.


In this way, it is possible to write a hardware control program on any platform, that can easily be ported between different platforms (including the Raspberry Pi).


The package contains a console based (text mode) simulator that can be used to test your programs on before you connect to real hardware, and this supports both inputs and outputs.


The serial link between the two parts runs at 115200bps, and each command is only a few characters, so the system performs reasonably well unless you are repeatedly polling or changing lots of GPIO's at the same time.


FUTURE WORK


The GPIO interface itself could be anything, not just an Arduino. There is a very simple protocol between the python module and the target GPIO hardware, that is written in a way to allow future extension to support other hardware peripherals such as I2C, SPI, UART, PWM, Analog, OneWire and other protocols and features aimed at near real time control and sensing.


While you can use any platform for the target hardware side, I have selected a specific Arduino platform for use, and in future there will be a web shop where you can buy a pre-soldered and pre-programmed board, that you just plug into your Mac or PC and it works out of the box.


There are placeholders in the design for a tkinter GUI simulator that I am plannign to write soon, and also a network aware simulator that allows GPIO controls to be sent remotely over a network connection to a GPIO server running on any arbitrary host computer.


David Whale
May 2014
