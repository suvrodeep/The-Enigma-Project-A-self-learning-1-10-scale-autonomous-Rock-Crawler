#!/usr/bin/python3

import serial
import time

ser = serial.Serial('/dev/ttyACM0', 9600)
reject_count = 1
steering = 1045
throttle = 2100

try:
    while reject_count <= 10:
        ser.readline()
        reject_count = reject_count + 1
    while 1:
        inp = ser.readline()
        print(inp.decode("utf8"))

        ser.write(steering)
        time.sleep(0.01)
        ser.write(throttle)
except KeyboardInterrupt:
    ser.close()
    print("\n\nPressed Ctrl+C. Exiting program\n\n")
    exit(0)
