# servo-proxy
Is an application that manages servo motors by accepting commands from the embedded device's serial ports
- The first port acts as a console interface.
- The second port accepts commands from a device such as a camera.

## Command Syntax
- ? in the first position marks the line as a direct command
otherwise it is passed on to the second port
- command (1-5) to execute
- index (0-8) of servo
- angle (0-180) or position
- speed (1-255) of action
- type (0-255) varies with command

## Commands

1. `HOME` *index*
Home or zero the servo at *index*.
2. `MOVE` *index* *angle* *speed*
Move the servo at *index* to *angle* at *speed*.
The motion is linear.
3. `EASE` *index* *angle* *speed* *type*
Ease the servo at *index* to *angle* at *speed*.
The motion is determined by *type*.
4. `TEST`  *index* *angle* *speed* *angle2*
Test the servo at *index*  from *angle* to *angle2* at *speed*.
The motion is determined by the previous `EASE` or `MOVE` command.
The test command repeats until the servo at *index* receives another command.
5. `STOP` *index*
Do nothing to the servo at *index*.
Used to stop repeating the a previous `TEST` command.


## Examples
- `EASE` servo *index 0* to *angle 180* at *speed-50* default motion
`?3 0 180 50 0`

- `MOVE` *index 0* to *angle 100* at *speed 45*
`?2 0 100 45`

- `HOME` *index 1*
`?1 1`

- `TEST` servo *index 1* from *angle 110* to *angle 15* at *speed 66* default motion
repeat until a new command is requested for servo *index 1*
`?4 1 110 66 15`

- `STOP` servo *index 1*
`?5 1`

