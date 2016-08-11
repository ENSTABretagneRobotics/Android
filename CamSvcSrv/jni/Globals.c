// Prevent Visual Studio Intellisense from defining _WIN32 and _MSC_VER when we use 
// Visual Studio to edit Linux or Borland C++ code.
#ifdef __linux__
#	undef _WIN32
#endif // __linux__
#if defined(__GNUC__) || defined(__BORLANDC__)
#	undef _MSC_VER
#endif // defined(__GNUC__) || defined(__BORLANDC__)

#include "Globals.h"

CvCapture* webcam = NULL;
CvVideoWriter* videorecordfile = NULL;
#ifndef DISABLE_TIMER_RECORDING
TIMER timer;
#endif // DISABLE_TIMER_RECORDING
CHRONO chrono;
CvFont font;
CRITICAL_SECTION imageCS;
IplImage* frame = NULL;
IplImage* resizedframe = NULL;
IplImage* image = NULL;
IplImage* previmage = NULL;
IplImage* detectimage = NULL;
char* databuf = NULL;
int databuflen = 0;
char* sharedbuf = NULL;
int sharedbuflen = 0;
CRITICAL_SECTION sharedbufCS;
int encodeparams[2];
BOOL bStop = FALSE;

// Parameters.
char szDevPath[256];
char srvport[256];
int videoimgwidth = 0; 
int videoimgheight = 0; 
int captureperiod = 0;
int timeout = 0;
BOOL bForceSoftwareResize = FALSE;
double hcenter = 0;
double vcenter = 0;
double hscale = 1;
double vscale = 1;
double angle = 0;
double hshift = 0;
double vshift = 0;
int bFlip = FALSE;
BOOL bUDP = FALSE;
int pixcolorchgthreshold = 0; 
int timecompressiondividerthreshold = 0; 
int fullimgperiod = 0; 
int encodequality = 0; 
char encodetype[32];
int method = 0; 
BOOL bDisableVideoRecording = FALSE;
