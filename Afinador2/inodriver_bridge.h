//// ****** THIS FILE IS AUTOGENERATED ******
////
////          >>>> DO NOT CHANGE <<<<
////
/// 
///  Filename; C:\Users\Axel Lacapmesure\Google Drive\Facultad\2018_InstrumentacionYControl\afinador\prueba.py
///  Source class: Afinador2
///  Generation timestamp: 2018-12-10T15:34:52.162883
///  Class code hash: d8b86d2c295de2e888b64d479673cf2192e638e3
///
/////////////////////////////////////////////////////////////

#ifndef inodriver_bridge_h
#define inodriver_bridge_h

#include <Arduino.h>

#include "SerialCommand.h"

#include "inodriver_user.h"

const char COMPILE_DATE_TIME[] = __DATE__ " " __TIME__;

void ok();
void error(const char*);
void error_i(int);
void bridge_loop();
void bridge_setup();

void getInfo();
void unrecognized(const char *);
void wrapperGet_TUN(); 
void wrapperSet_TUN(); 
void wrapperGet_REF(); 
void wrapperSet_REF(); 
void wrapperGet_DIFF(); 
void wrapperSet_DIFF(); 
void wrapperGet_LEN(); 
void wrapperSet_LEN(); 
void wrapperGet_TOL(); 
void wrapperSet_TOL(); 


#endif // inodriver_bridge_h