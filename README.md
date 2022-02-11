# CreativeEngineering
A place to store code related to Mark Rober's creative engineering course. 1/8/22-2/8/22 
# Build #2 - Lunar_Calculator
This file is currently run on an Arduino Uno 3 calculates the lunar orbit and then finds the days until the next full. This information is displayed on an LCD display. There is a stepper motor on the top that rotates constantly (future enhancement to enable turning off and on) and displays a replica moon.

# Build #3 - Build3_Jugglebot3000
This file is currently run on a Raspberry Pi 3 model B. There are 3 servos, 3 LEDS, a passive buzzer, and a switch connected to the breadboard. On startup, the device kicks off two scripts, one is a setup and the second runs the .py file. This way every time I turn on the RPi, the program runs without any manual intervention. This is designed to hold 3 juggling balls and when I feel like juggling I flip a switch and it counts down with 3 short buzzes and the LEDs follow a traffic light pattern until all LEDs display, and the servo's connected to 'claws' open releasing the juggling balls in a specific pattern to then be immediately juggled. When I am done juggling, I flip the switch closed, another count down initiates and the 'claws' close one by one to allow me to reset the device for the next time I want to juggle.

# Improvements

## Build #2:
1. 3D Print out bigger moon
2. Remove stepper motor and insert the slip ring to allow for an LED inside the Moon.
3. Create a way to visually represent the moon's current phase with an LED instead of on the LCD screen

## Build #3:
1. Calculate time between juggles and once time>24hrs and PIR sensor is triggered, start release process. 
2. Solder to a prototype board for longevity and to re-use the breadboard
3. Re-wire the raspberry pi to another raspberry pi
4. After #2 and #3, create cool enclosure for the electronics.
