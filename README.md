# Joynardo

DIY joystick built with an [Arduino Leonardo](http://arduino.cc/en/pmwiki.php?n=Main/arduinoBoardLeonardo).

It uses the patches from [this](http://www.imaginaryindustries.com/blog/?p=80) website to turn the Leonardo into a USB HID Joystick.

The code will basically work on (most) other Arduinos except for the actual [Joystick Implementation](src/JoystickImpl.cpp).

It is shared with the public for research and learning purposes.


## Build

The project is created using [ino](http://inotool.org/). See the [ino.ini](ino.ini) file for configuration options.


