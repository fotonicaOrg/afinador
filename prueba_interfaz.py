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

class Afinador(object):
    
    def __init__(self, port, baud_rate, timeout = 2):
        
        self.sleep_time = 0.01
        
        self.tolerance = 20.0
        self.reference = 440.0
        self.differnece = 0.0
        self.tuned = 0
        self.interface_run = False
        
        self.serial = serial.Serial(port, baud_rate, timeout = timeout, write_timeout = timeout)
        print(self.serial.readline())

        self.fig = plt.figure(figsize = (3,3))
        
        self.ui = dict()
        self.ui['ref_textbox_ax'] = self.fig.add_axes([0.4, 0.05, 0.3, 0.1])
        self.ui['ref_textbox'] = widgets.TextBox(self.ui['ref_textbox_ax'], 'Reference = ', initial = '440')
        self.ui['ref_textbox'].on_submit(self.set_ref)
        self.ui['ref_units_text_ax'] = self.fig.add_axes([0.73, 0.071, 0.075, 0.075])
        self.ui['ref_units_text_ax'].axis('off')
        self.ui['ref_units_text'] = self.ui['ref_units_text_ax'].text(0, 0, 'Hz', ha = 'left', va = 'bottom')
        
        self.ui['tol_textbox_ax'] = self.fig.add_axes([0.4, 0.2, 0.3, 0.1])
        self.ui['tol_textbox'] = widgets.TextBox(self.ui['tol_textbox_ax'], 'Tolerance = ', initial = '20')
        self.ui['tol_textbox'].on_submit(self.set_tol)
        self.ui['tol_units_text_ax'] = self.fig.add_axes([0.73, 0.221, 0.075, 0.075])
        self.ui['tol_units_text_ax'].axis('off')
        self.ui['tol_units_text'] = self.ui['tol_units_text_ax'].text(0, 0, 'Hz', ha = 'left', va = 'bottom')
        
        self.ui['tuned_ax'] = self.fig.add_axes([.3, 0.35, .4, .2])
        self.ui['tuned_ax'].axis('off')
        self.ui['tuned_text'] = self.ui['tuned_ax'].text(.5, 0.48, "Tuned", ha = "center", va = 'center', color = 'white')
        self.ui['tuned_circle'] = self.ui['tuned_ax'].add_patch(matplotlib.patches.Ellipse(
                                                                (.5, .5), .4/.7, 1,
                                                                fill = True,
                                                                facecolor = '#EE3333',
                                                                edgecolor = None,
                                                                ))
        
        self.ui['diff_ax'] = self.fig.add_axes([0.1, 0.55, 0.8, 0.4])
        self.ui['diff_ax'].axis('off')
        self.ui['diff_text'] = self.ui['diff_ax'].text(.5, .1, "0 Hz", ha = "center")
        self.ui['diff_rect'] = self.ui['diff_ax'].add_patch(matplotlib.patches.Rectangle(
                                                            (0, .3), 1, .7,
                                                            fill = True,
                                                            facecolor = '#3333FF',
                                                            edgecolor = None,
                                                            ))
        
        self.interface_thread = threading.Thread(target = self.update)

    def set_ref(self, value):
        
        self.serial.write(bytes('REF {:0.4f}\n'.format(float(value)), 'ASCII'))
        time.sleep(self.sleep_time)
        status = self.serial.readline().decode('ASCII').split('\r\n')[0]
        
        print(status)
    
    def set_tol(self, value):
        
        self.ser.write(bytes('TOL {:0.4f}\n'.format(float(value)), 'ASCII'))
        time.sleep(self.sleep_time)
        status = self.serial.readline().decode('ASCII').split('\r\n')[0]
        
        print(status)
    
    def get_diff(self):
        
        self.serial.write(bytes('DIFF?\n', 'ASCII'))
        time.sleep(self.sleep_time)
        try:
            result = float(self.serial.readline().decode('ASCII').split('\r\n')[0])
        except:
            result = 0
        
        return result
    
    def get_tun(self):
        
        self.serial.write(bytes('TUN?\n', 'ASCII'))
        time.sleep(self.sleep_time)
        try:
            result = int(self.serial.readline().decode('ASCII').split('\r\n')[0])
        except:
            result = 0
        
        return result
    
    def update(self):
        
        while self.interface_run:
            
            a = time.time()
            
            self.difference = self.get_diff()
            self.tuned = self.get_tun()
            
            new_width = min(max(afinador.difference/400, -0.5), 0.5)
            new_x = 0.5 if afinador.difference >= 0 else 0.5 + new_width
            
            self.ui['diff_text'].set_text('Difference = {0:0.2f}'.format(self.difference))
            self.ui['diff_rect'].set_x(new_x)
            self.ui['diff_rect'].set_width(new_width)
            
            if self.tuned == 0:
                self.ui['tuned_circle'].set_facecolor('#EE3333')
            else:
                self.ui['tuned_circle'].set_facecolor('#33EE33')
            
            plt.draw()
            
            b = time.time()
            
            print(b-a)
    
    def start(self):

        self.interface_run = True
        self.interface_thread.setDaemon(True)
        self.interface_thread.start()
    
    def stop(self):
        
        self.interface_run = False
        self.serial.close()
        
#class InputThread(threading.Thread):
#    
#    def run(self):
#        while True:
#            get_diff(ser, UI['diff_text'])
#            plt.draw()


afinador = Afinador(port = 'COM6', baud_rate = BAUD_RATE)
afinador.start()

