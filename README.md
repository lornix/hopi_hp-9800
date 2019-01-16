# hopi_hp-9800
Simple code to read data from HOPI HP-9800 Power Meter.  Need libmodbus.  Basic functionality at the moment, but useful.

Received my new HOPI HP-9800 Power Meter yesterday.  It included a micro-usb serial port, claiming the ability to read the data from the meter.

Of course, it wasn't simple serial output, it's using the [ModBus-RTU](https://en.wikipedia.org/wiki/Modbus) protocol, evidently because it wants to be a 'Professional Grade' meter.  Bleah! Whatever!  There is very little information on the Hopi HP-9800 available on the web, like, surprisingly little.  Nothing really more than images of the single-page data sheet included with the device in the box.  And THAT was sparse too!

I have now read extensively on the [ModBus](https://en.wikipedia.org/wiki/Modbus) Protocol, it's interesting, a little dated, and a little slow, but rather simple to implement (At least it doesn't involve XML!).  I'm using the excellent [libmodbus library](https://github.com/stephane/libmodbus) by [Stéphane Raimbault](https://github.com/stephane), available in most Linux distributions, and also at [libmodbus.org](https://libmodbus.org), including documentation.

You'll need to install the `libmodbus-dev` and `libmodbus*` packages (I'm using v3.1.4 today) from your distro, or clone, compile and install Stéphane's library.

While I have plans for multiple options and increased usability of this tool, at the moment, it's hard-coded to use `/dev/ttyUSB0`, so adjust the source if your meter registers as a different port.  Should work for Windows users too, just using `//./COM??` as the port string.

My uber-generic Makefile® should be easily readable to tweak if you desire, perhaps I'll autoconf the source at some point, but for the moment, it's quite sufficent.  Compiling is as expected:

```
@reko:~/projects/hopi_hp-9800$ make
gcc -std=gnu99  -ggdb3 -O2 -Wall -Wextra -Wunused -Wl,--as-needed -fdiagnostics-color=always    -c hopi.c -o hopi.o -lmodbus
gcc -std=gnu99  -ggdb3 -O2 -Wall -Wextra -Wunused -Wl,--as-needed -fdiagnostics-color=always    hopi.o -o hopi -lmodbus

@reko:~/projects/hopi_hp-9800$ ./hopi
  0.00000 W   Active Power
  0.00000 A   RMS Current
119.04472 V   Voltage
 60.03597 Hz  Frequency
  0.00000 pf  Power Factor
  0.00000 KWH Annual Power Consumption
  0.02239 KWH Active Consumption
  0.04847 KWH Reactive Consumption
  4.97500 Hrs Load Time
       24 Hrs Work Hours per Day
        1     Device Address
@reko:~/projects/hopi_hp-9800$
```

It's basically a dump of all the information I know about, an instantaneous snapshot of the meter's current state.  Here you can see it's just idling, I haven't anything plugged into it currently (ha!), but have had a few devices connected for testing, as evidenced by the 'Consumption' and 'Load Time' fields.

First options in the works are likely to be the ability to specify the serial port, and to output the values in a CSV format for ease of importing into various other tools.

Five decimal digits of precision seemed decent, the values involved are 4 byte IEEE754 floats, so really only 7-8 digits of accuracy at best.

Interesting to watch, try:
```
$ clear ; while true; do tput home ; ./hopi ; sleep 1 ; done
```
or
```
$ while true; do clear ; ./hopi ; sleep 1; done
```
The second version flickers, so I prefer the first loop with tput.

lornix
