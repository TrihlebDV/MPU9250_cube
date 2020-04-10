#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import serial
import threading
import time

class ArdHandler():
    def __init__(self, stopEvent):
        self.ser = serial.Serial('/dev/ttyACM0', 115200)
        #self.ser = serial.Serial('/dev/ttyUSB0', 9600)
        self._stopped = stopEvent
        self.angs = [0, 0, 0]
        self.dataFlag = threading.Event()

    def read(self):
        buff = self.ser.readline()
        while not self._stopped.is_set():
            buff = self.ser.readline()
            if len(buff) == 0:
                continue
            if not self.dataFlag.is_set():
                buff = buff[:-2]
                sp = buff.split(b' ')
                nums = list(map(float, sp))
                self.angs = nums
                self.dataFlag.set()
        self.ser.close()
 

if __name__  == "__main__":
    ardH = ArdHandler(threading.Event())
    ardH.read()
