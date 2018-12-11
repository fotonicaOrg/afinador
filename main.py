# -*- coding: utf-8 -*-
"""
Created on Thu Dec  6 18:21:30 2018

@author: Axel Lacapmesure
"""

import serial
import numpy as np
from matplotlib import pyplot as plt
import time
import threading

#LENGTH = 200
BAUD_RATE = 115200 #115200

#data = np.zeros((2*LENGTH,))

# define a thread which takes input
class InputThread(threading.Thread):
    def run(self):
        self.daemon = True
        while True:
            self.last_user_input = input('input something: ')
            # do something based on the user input here
            # alternatively, let main do something with
            # self.last_user_input

# main
it = InputThread()
it.start()
while True:
    # do something  
    # do something with it.last_user_input if you feel like it



ser = serial.Serial('COM6', BAUD_RATE, timeout = 2, write_timeout = 2)
print(ser.readline())
time.sleep(1)
ser.write(b'REF 1000\n')
time.sleep(1)
ser.write(b'DIFF?\n')
print(ser.readline())
ser.close()

