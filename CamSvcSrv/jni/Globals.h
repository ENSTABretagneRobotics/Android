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
#include "OSCriticalSection.h"
#ifndef DISABLE_TIMER_RECORDING
#include "OSTimer.h"
#endif // DISABLE_TIMER_RECORDING
#include "OSNet.h"
#include "CvCore.h"
#ifdef ENABLE_CVKINECT2SDKHOOK
#ifndef INCLUDE_HEADERS_OUTSIDE_CVKINECT2SDKHOOK
#define INCLUDE_HEADERS_OUTSIDE_CVKINECT2SDKHOOK
#endif // INCLUDE_HEADERS_OUTSIDE_CVKINECT2SDKHOOK
#include <Kinect.h>
#include "CvKinect2SDKHook.h"
#endif // ENABLE_CVKINECT2SDKHOOK

// Use SOMAXCONN as the max number of simultaneous clients for multithreaded version...

EXTERN_C CvCapture* webcam;
EXTERN_C CvVideoWriter* videorecordfile;
#ifndef DISABLE_TIMER_RECORDING
EXTERN_C TIMER timer;
#endif // DISABLE_TIMER_RECORDING
EXTERN_C CHRONO chrono;
EXTERN_C CvFont font;
EXTERN_C CRITICAL_SECTION imageCS;
EXTERN_C IplImage* frame;
EXTERN_C IplImage* resizedframe;
EXTERN_C IplImage* image;
EXTERN_C IplImage* previmage;
EXTERN_C IplImage* detectimage;
EXTERN_C char* databuf;
EXTERN_C int databuflen;
EXTERN_C char* sharedbuf;
EXTERN_C int sharedbuflen;
EXTERN_C CRITICAL_SECTION sharedbufCS;
EXTERN_C int encodeparams[2];
EXTERN_C BOOL bStop;

// Parameters.
EXTERN_C char szDevPath[256];
EXTERN_C char srvport[256];
EXTERN_C int videoimgwidth; 
EXTERN_C int videoimgheight; 
EXTERN_C int captureperiod;
EXTERN_C int timeout;
EXTERN_C BOOL bForceSoftwareResize;
EXTERN_C double hcenter;
EXTERN_C double vcenter;
EXTERN_C double hscale;
EXTERN_C double vscale;
EXTERN_C double angle;
EXTERN_C double hshift;
EXTERN_C double vshift;
EXTERN_C int bFlip;
EXTERN_C BOOL bUDP;
EXTERN_C int pixcolorchgthreshold; 
EXTERN_C int timecompressiondividerthreshold; 
EXTERN_C int fullimgperiod; 
EXTERN_C int encodequality; 
EXTERN_C char encodetype[32];
EXTERN_C int method; 
EXTERN_C BOOL bDisableVideoRecording;

#endif // GLOBALS_H
