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
#endif // !DISABLE_TIMER_RECORDING
#include "OSNet.h"
#include "CvInc.h"
#ifdef ENABLE_CVKINECT2SDKHOOK
#include "CvKinect2SDKHook.h"
#endif // ENABLE_CVKINECT2SDKHOOK

// Use SOMAXCONN as the max number of simultaneous clients for multithreaded version...

#ifdef __ANDROID__
#define VID_FOLDER "/storage/sdcard0/download/"
#else
#define VID_FOLDER ""
#endif // __ANDROID__

#ifndef USE_OPENCV_HIGHGUI_CPP_API
EXTERN_C CvCapture* webcam;
EXTERN_C CvVideoWriter* videorecordfile;
#else
extern cv::VideoCapture* webcam;
extern cv::Mat* pframemat;
extern IplImage frameipl;
extern cv::VideoWriter videorecordfile;
#endif // !USE_OPENCV_HIGHGUI_CPP_API
#ifndef DISABLE_TIMER_RECORDING
EXTERN_C TIMER timer;
#endif // !DISABLE_TIMER_RECORDING
EXTERN_C CHRONO chrono;
#ifndef DISABLE_GUI_REMOTEWEBCAMMULTISRV
EXTERN_C CvFont font;
#endif // !DISABLE_GUI_REMOTEWEBCAMMULTISRV
EXTERN_C CRITICAL_SECTION imageCS;
EXTERN_C IplImage* frame;
EXTERN_C IplImage* resizedframe;
EXTERN_C IplImage* image;
EXTERN_C IplImage* previmage;
#ifndef DISABLE_GUI_REMOTEWEBCAMMULTISRV
EXTERN_C IplImage* detectimage;
#endif // !DISABLE_GUI_REMOTEWEBCAMMULTISRV
EXTERN_C char* databuf;
EXTERN_C int databuflen;
EXTERN_C char* sharedbuf;
EXTERN_C int sharedbuflen;
EXTERN_C CRITICAL_SECTION sharedbufCS;
#ifndef USE_OPENCV_HIGHGUI_CPP_API
EXTERN_C int encodeparams[2];
#else
extern std::vector<int> encodeparams;
#endif // !USE_OPENCV_HIGHGUI_CPP_API
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
EXTERN_C char szVideoRecordCodec[5];

#endif // !GLOBALS_H
