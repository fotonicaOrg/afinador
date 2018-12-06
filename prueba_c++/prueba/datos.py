# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""

import numpy as np
from matplotlib import pyplot as plt
from scipy.io import wavfile as wave

#a = np.sin(2*np.pi/(50)*np.arange(256)) +  + 0.5 * np.random.randn(256)
#a.tofile('prueba.txt', sep = '\r\n')
#
#plt.plot(a)
#
#corr = np.fromfile("prueba2.txt", sep = '\r\n')
#plt.plot(corr)

(rate, data) = wave.read('prueba.wav')
data.tofile('prueba.txt', sep = '\r\n')

corr = np.fromfile("corr.txt", sep = '\r\n')
raw = np.fromfile("raw.txt", sep = '\r\n')

plt.plot(data, label = 'data')
plt.plot(raw, label = 'raw')
plt.plot(corr, label = 'corr')
plt.plot([0,corr.size], [corr[0]*0.9,corr[0]*0.9])
plt.legend()
