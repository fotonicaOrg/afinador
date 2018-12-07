# -*- coding: utf-8 -*-
"""
Created on Thu Dec  6 18:21:30 2018

@author: Axel Lacapmesure
"""

import serial
import numpy as np
from matplotlib import pyplot as plt

LENGTH = 750
BAUD_RATE = 115200

data = np.zeros((2*LENGTH,))

with serial.Serial('COM3', BAUD_RATE, timeout = 1) as ser:
    while True:
        print(ser.readline())
#    print(ser.readline()) # Tiramos la primer linea que es basura
#    print(ser.readline()) # Tiramos la primer linea que es basura
#    arr = ser.readline().decode('ASCII')
#    arr = arr.split(',')[:-1]
#    arr = [float(x) for x in arr]
#    data = np.array(arr)/2**15
    
plt.plot(data)
plt.xlabel('Muestra')
plt.ylabel('Tensión [u.a.]')
plt.title('Calibración de frecuencia Arduino')