#!/usr/bin/python3

import serial
ser = serial.Serial('/dev/ttyACM0', 9600)

filename = "data.csv"
reject_count = 1

try:
    # Read from Arduino and write to file
    with ser, open(file=filename, mode="wt", encoding="utf8", newline="\n") as outfile:
        while reject_count <= 10:
            ser.readline()
            reject_count = reject_count + 1
        while 1:
            inp = ser.readline()
            print(inp.decode("utf8"))
            outfile.write(inp.decode("utf8"))
            outfile.flush()
except KeyboardInterrupt:
    ser.close()
    print("\n\nPressed Ctrl+C. Exiting program\n\n")
    exit(0)
