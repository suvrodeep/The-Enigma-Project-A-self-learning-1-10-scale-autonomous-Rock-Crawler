#!/usr/bin/python3

import serial
ser = serial.Serial('/dev/ttyACM0', 9600)

fname = "data.csv"
reject_count = 1

# Read from Arduino and write to file
with ser, open(file=fname, mode="wt", encoding="utf8", newline="\n") as outfile :
        while (reject_count <= 10) :
                ser.readline()
                reject_count = reject_count + 1
        while 1 :
                input = ser.readline()
                print(input.decode("utf8"))
                outfile.write(input.decode("utf8"))
                outfile.flush()


