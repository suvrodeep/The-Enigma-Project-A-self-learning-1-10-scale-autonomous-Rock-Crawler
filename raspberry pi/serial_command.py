#!/usr/bin/python3

import serial


ser = serial.Serial('/dev/ttyACM0', 9600)
reject_count = 1
steering = 1045
throttle = 2100

try:
    while reject_count <= 10:
        ser.readline()
        reject_count = reject_count + 1
    while 1:
        ser.flushInput()
        inp = ser.readline()
        inp = inp.decode("utf8")
        if inp == -1:
            ser.flush()
            command = 1000 + steering
            ser.write(command)
        else:
            print(inp)
        ser.flushOutput()
except KeyboardInterrupt:
    ser.close()
    print("\n\nPressed Ctrl+C. Exiting program\n\n")
    exit(0)
