#!/usr/bin/env python
import serial

ser = serial.Serial("/dev/ttyUSB.SSC", 115200)
ser.write("#12 P1550\r")
ser.write("#13 P1600\r")
ser.close()
