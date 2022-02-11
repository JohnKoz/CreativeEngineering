#start here on Saturday. So far I have a button working to open and close the claw.
#Next I want to rig up 2 more servos together but I need a 5 volt power source first. 4xAA or 4xAAA batteries will work
#Work on 3d printing the rest of the claws
#figure out how to not have to do sudo pigpiod in terminal every time I launch

from gpiozero.pins.pigpio import PiGPIOFactory
from gpiozero import Device, LED, Button, AngularServo
from time import sleep


factory = PiGPIOFactory()
# led = LED(12,pin_factory = factory)
# 
# led.blink()
pinbutton = int(10)

red = LED(2)
yellow = LED(3)
green = LED(4)

green.on()
yellow.off()
red.off()

##Below is example for the servo motor from gpizero.readthedocs
servo1 = AngularServo(17,min_angle=-90, max_angle=90,pin_factory=factory)
servo2 = AngularServo(22,min_angle=-90, max_angle=90,pin_factory=factory)
servo3 = AngularServo(23,min_angle=-90, max_angle=90,pin_factory=factory)

def servo_open():
    green.on()
    sleep(1)
    green.off()
    yellow.on()
    sleep(1)
    yellow.off()
    red.on()
    sleep(1)
    yellow.on()
    green.on()
    sleep(1)
    sleep(0.5)
    servo1.angle = 90 #open Claw #1
    sleep(0.5)
    servo3.angle = 90 #open Claw #3
    sleep(0.5)
    servo2.angle = 90 #open Claw #2
    sleep(1)
    yellow.off()
    red.off()
    green.off()
    print("Open Sesame!")
    
def servo_closed():
    sleep(0.75)
    servo1.angle = -20
    sleep(1.25)#in between sleep and turning motor off, need to sleep or else the claws don't close all the way.
    servo1.value = 0 #This line sets the motor to 0, turning it off, which avoids overheating. This line is the same as changedutycycle(
    servo2.angle = -20
    sleep(0.75)
    servo2.value = 0 #turn off motor
    sleep(0.75)
    servo3.angle = -20
    sleep(0.75)
    servo3.value = 0 #turn off motor
    print("Closed all 3 claws")

button = Button(pinbutton)
# try:
while True:
    button.when_activated = servo_open
    button.when_deactivated = servo_closed


# finally:
#     #Clean things up at the end
#     servo1.off()
#     GPIO.cleanup()
#     print("Bye Sweetheart")

#example 2 below
# def say_hello():
#     print("Hello!")
#     
# button = Button(3)
# button.when_pressed = say_hello
# 
# pause()
# 

#example 1 below
# button = Button(3)
# while True:
#     if button.is_pressed:
#         print("Button is pressed")
#     else:
#         print("Button is not pressed!")
#         