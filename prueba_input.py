# -*- coding: utf-8 -*-
"""
Created on Tue Dec 11 11:17:00 2018

@author: Axel Lacapmesure
"""

import time
import threading

class InputThread(threading.Thread):
    def run(self):
        self.daemon = True
        while True:
            self.last_user_input = input('input something: ')
            print(self.last_user_input)
            time.sleep(1)
            # do something based on the user input here
            # alternatively, let main do something with
            # self.last_user_input
            
# main
it = InputThread()
it.start()
while True:
    print("Loop constante")
    # do something  
    # do something with it.last_user_input if you feel like it