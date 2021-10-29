# Commodore 128D USB keyboard adapter

So... I came up on a Commodore [128DCR](https://www.c64-wiki.com/wiki/C128DCR) on Craigslist here in Portland, OR, and boy howdy was I excited! However, this particular unit, like most of these particular units did not come with a keyboard! 

There are a number of projects and products out there that allow for adapting an AT or PS/2 keyboard for the C128D but I haven't found anything the directly uses a USB keyboard. Not all USB keyboards can utilize a PS/2 adapter and honestly, I was excited for another pandemic project, so I created this thing!

For the hardware I chose the amazing [Teensy 3.6](https://www.pjrc.com/store/teensy36.html) since it has an onboard USB host interface, USB host library code and an Arduino compatible library and most necessarily a _lot_ of PIO pins. This code should work exactly the same with the [Teensy 4.1](https://www.pjrc.com/store/teensy41.html), but I haven't personally tested this.

As for the software, I used the [Platform IO](https://platformio.org/) platform since it allowed me to write all my code in standard C++ while also leveraging all of the [Teensyduino](https://www.pjrc.com/teensy/teensyduino.html) library code and, most importantly, I didn't have to write a dang **Makefile** to build, upload and run tests.

The repo is arranged in such a way that the only Teensy-specific code is located in `src/main.cpp` and `include/`, so the core logic code could actually be used on any platform supported by Platform IO with a litte refactoring. Most of the unit tests will actually run on your local machine without even needing to send anything to an embedded system. In fact, a good deal of this code was written and tested at bars and coffee shops!

## How It Works

Other than a Teensy 3.6/4.1 you will need a [Teensy USB host cable](https://www.pjrc.com/store/cable_usb_host_t36.html), a couple LED's, a couple resistors, and some way to connect all 23 of the output pins to the Commodore 128D's keyboard port which is, thankfully, a standard female DB25 port.

### Output pins

make some images to help out here

### Key map

### Lock keys

mention the 40/80 and capslock LEDS, capslock emulating shift-lock (and why) and numlock doing the weird C128 cursor keys.

## Running Tests

## Building and uploading the project

mentiond the python script which generates the keymapping

