/*
 * 
 The Entropy library comes from:
 https://github.com/pmjdebruijn/Arduino-Entropy-Library/
 
 
 MIDI note player
 
 ***********************************************************************
 **                                                                   **
 **  Teensy Note:                                                     **
 **                                                                   **
 **  This will NOT work on Teensy, because "Serial" talks to the      **
 **  USB virtual serial port if Serial mode is used, or an emulated   **
 **  virtual serial to the Arduino Serial Monitor in non-serial       **
 **  modes.  You can use Teensy's real serial port this way:          **
 **  http://www.pjrc.com/teensy/td_uart.html                          **
 **                                                                   **
 **  The MIDI library offers an easier way to use MIDI                **
 **  http://www.pjrc.com/teensy/td_libs_MIDI.html                     **
 **                                                                   **
 **  Teensy also has USB MIDI, for native MIDI to your computer       **
 **  http://www.pjrc.com/teensy/td_midi.html                          **
 **                                                                   **
 **  You can use both USB MIDI and the MIDI library on Teensy!  :)    **
 **                                                                   **
 ***********************************************************************


 This sketch shows how to use the serial transmit pin (pin 1) to send MIDI note data.
 If this circuit is connected to a MIDI synth, it will play 
 the notes F#-0 (0x1E) to F#-5 (0x5A) in sequence.

 
 The circuit:
 * digital in 1 connected to MIDI jack pin 5
 * MIDI jack pin 2 connected to ground
 * MIDI jack pin 4 connected to +5V through 220-ohm resistor
 Attach a MIDI cable to the jack, then to a MIDI synth, and play music.

 created 13 Jun 2006
 modified 30 Aug 2011
 by Tom Igoe

 This example code is in the public domain.
 
 http://www.arduino.cc/en/Tutorial/MIDI




Note ratios
1.0000000000000000000000000000000
1.0594630943592952645618252949463
1.1224620483093729814335330496792
1.1892071150027210667174999705605
1.2599210498948731647672106072782
1.3348398541700343648308318811845
1.4142135623730950488016887242097
1.4983070768766814987992807320298
1.5874010519681994747517056392723
1.6817928305074290860622509524664
1.7817974362806786094804524111810
1.8877486253633869932838263133351
2.0000000000000000000000000000000



 */
