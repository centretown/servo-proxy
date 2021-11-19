# servo-proxy
Is an embedded application that manages servo motors, led strips and cameras by accepting commands from the console or attached device.
- The first port acts as a console interface.
- The second port accepts commands from an attached device such as a camera.

## Command Syntax
- ? in the first position identifies servo commands
- ! in the first position identifies led commands
- everything else passed on to the attached device

See `lib README's` for more details.
