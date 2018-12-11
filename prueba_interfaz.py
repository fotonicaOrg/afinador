# -*- coding: utf-8 -*-
"""
Created on Tue Dec 11 11:18:59 2018

@author: Axel Lacapmesure
"""

from matplotlib import pyplot as plt
from matplotlib import widgets
import matplotlib

import time
import serial
import threading

BAUD_RATE = 115200 #115200



ser = serial.Serial('COM6', BAUD_RATE, timeout = 2, write_timeout = 2)



def set_ref(value, ser):
    
    ser.write(bytes('REF {:0.4f}\n'.format(float(value)), 'ASCII'))
    time.sleep(0.1)
    status = ser.readline().decode('ASCII').split('\r\n')[0]
    
    print(status)

def get_diff(ser, text):
    
    ser.write(bytes('DIFF?\n', 'ASCII'))
    time.sleep(0.1)
    result = ser.readline().decode('ASCII').split('\r\n')[0]
    
    text.set_text('Difference = {0}'.format(result))
    
    return result

class InputThread(threading.Thread):
    def run(self):
        while True:
            get_diff(ser, UI['diff_text'])
            plt.draw()

Fig = plt.figure(figsize = (3,3))

UI = dict()
UI['ref_textbox_ax'] = Fig.add_axes([0.4, 0.05, 0.3, 0.1])
UI['ref_textbox'] = widgets.TextBox(UI['ref_textbox_ax'], 'Reference = ', initial = '440')
UI['ref_textbox'].on_submit(lambda x : set_ref(x, ser))
UI['ref_units_text_ax'] = Fig.add_axes([0.73, 0.071, 0.075, 0.075])
UI['ref_units_text_ax'].axis('off')
UI['ref_units_text'] = UI['ref_units_text_ax'].text(0, 0, 'Hz', ha = 'left', va = 'bottom')

UI['tol_textbox_ax'] = Fig.add_axes([0.4, 0.2, 0.3, 0.1])
UI['tol_textbox'] = widgets.TextBox(UI['tol_textbox_ax'], 'Tolerance = ', initial = '20')
UI['tol_textbox'].on_submit(lambda x : set_ref(x, ser))
UI['tol_units_text_ax'] = Fig.add_axes([0.73, 0.221, 0.075, 0.075])
UI['tol_units_text_ax'].axis('off')
UI['tol_units_text'] = UI['tol_units_text_ax'].text(0, 0, 'Hz', ha = 'left', va = 'bottom')

UI['tuned_ax'] = Fig.add_axes([.3, 0.35, .4, .2])
UI['tuned_ax'].axis('off')
UI['tuned_text'] = UI['tuned_ax'].text(.5, 0.48, "Tuned", ha = "center", va = 'center', color = 'white')
UI['tuned_circle'] = UI['tuned_ax'].add_patch(matplotlib.patches.Ellipse(
                                              (.5, .5), .4/.7, 1,
                                              fill = True,
                                              facecolor = '#EE3333',
                                              edgecolor = None,
                                              ))

UI['diff_ax'] = Fig.add_axes([0.1, 0.55, 0.8, 0.4])
UI['diff_ax'].axis('off')
UI['diff_text'] = UI['diff_ax'].text(.5, .1, "0 Hz", ha = "center")
UI['diff_rect'] = UI['diff_ax'].add_patch(matplotlib.patches.Rectangle(
                                          (0, .3), 1, .7,
                                          fill = True,
                                          facecolor = '#3333FF',
                                          edgecolor = None,
                                          ))

#it = InputThread()
#it.setDaemon(True)
#it.start()

ser.close()

