#!/usr/bin/python3

import serial
ser = serial.Serial('/dev/ttyACM0', 9600)

# read from Arduino
while 1 :
    input = ser.readline()
    print (input.decode("utf-8"))

