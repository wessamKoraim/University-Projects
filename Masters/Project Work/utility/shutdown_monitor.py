import RPi.GPIO as GPIO
import os
import time

pin = 26

GPIO.setmode(GPIO.BCM)
GPIO.setup(pin, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)

while(1):
    state = GPIO.input(pin)
    if state:
        os.system("zenity --error --text='Shutdown button was pressed.\nShutting down the system safely...' title='Shutdown!'")
        time.sleep(3)
        os.system("shutdown -h now")
    time.sleep(2)


