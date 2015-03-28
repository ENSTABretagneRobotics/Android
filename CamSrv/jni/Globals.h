// Prevent Visual Studio Intellisense from defining _WIN32 and _MSC_VER when we use 
// Visual Studio to edit Linux or Borland C++ code.
#ifdef __linux__
#	undef _WIN32
#endif // __linux__
#if defined(__GNUC__) || defined(__BORLANDC__)
#	undef _MSC_VER
#endif // defined(__GNUC__) || defined(__BORLANDC__)

#ifndef GLOBALS_H
#define GLOBALS_H

#include "OSMisc.h"
#include "OSNet.h"
#include "CvCore.h"

EXTERN_C CvCapture* webcam;
EXTERN_C CHRONO chrono;
EXTERN_C CvFont font;
EXTERN_C IplImage* image;
EXTERN_C IplImage* previmage;
EXTERN_C IplImage* detectimage;
EXTERN_C char* databuf;
EXTERN_C int jpegparams[2];
EXTERN_C BOOL bForceSendFullImg;
EXTERN_C BOOL bStop;

EXTERN_C int camid;
EXTERN_C int videoimgwidth; 
EXTERN_C int videoimgheight; 
EXTERN_C int pixcolorchgthreshold; 
EXTERN_C int timecompressiondividerthreshold; 
EXTERN_C int fullimgperiod; 
EXTERN_C int jpegquality; 
EXTERN_C int method; 
EXTERN_C int timeout;
EXTERN_C char srvport[MAX_BUF_LEN];

#endif // GLOBALS_H
