# -*- coding: utf-8 -*-
"""
Created on Mon Dec 10 11:07:43 2018

@author: Axel Lacapmesure
"""

from lantz.ino import INODriver, BoolFeat, QuantityFeat, IntFeat

class Afinador2(INODriver):

    reference = QuantityFeat('REF')
    difference = QuantityFeat('DIFF')
    tuned = BoolFeat('TUN')
    tolerance = QuantityFeat('TOL')
    lenght = IntFeat('LEN')