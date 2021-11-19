# ledstrip
Is a library that manages rgb led's; accepting commands from the embedded device's serial ports
## Definitions
### Pixel
The smallest addressable unit on an LED strip.
### Strip
The physical strip attached to a PWM pin. The strip is comprised of one or more contiguous pixels.
### Segment
A segment is defined as one or more contiguous pixels which can be a part of one or more strips.  All commands are issued to segments.


## Command Syntax
- ! in the first position marks the line as a direct command
otherwise it is passed on
- command (1-9) to execute
- index (0-8) of led segment
- red (0-255)
- green (0-255) 
- blue (0-255)
- frequency (1-30000) in milli-seconds of action
- flags (0-255) varies with command

## Commands

1. `RESET index`
2. `SOLID index red green blue frequency flags`
3. `BLINK index red green blue frequency flags`
4. `WIPE index red green blue frequency flags`
5. `RAINBOW index red green blue frequency flags`
6. `RAINBOW_CYCLE index red green blue frequency flags`
7. `THEATRE index red green blue frequency flags`
8. `THEATRE_RAINBOW index red green blue frequency flags`


## Examples
