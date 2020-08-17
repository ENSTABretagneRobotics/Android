// Prevent Visual Studio Intellisense from defining _WIN32 and _MSC_VER when we use 
// Visual Studio to edit Linux or Borland C++ code.
#ifdef __linux__
#	undef _WIN32
#endif // __linux__
#if defined(__GNUC__) || defined(__BORLANDC__)
#	undef _MSC_VER
#endif // defined(__GNUC__) || defined(__BORLANDC__)

#include "Globals.h"

#ifndef USE_OPENCV_HIGHGUI_CPP_API
CvCapture* webcam = NULL;
CvVideoWriter* videorecordfile = NULL;
#else
cv::VideoCapture* webcam = NULL;
cv::Mat* pframemat = NULL;
IplImage frameipl;
cv::VideoWriter videorecordfile;
#endif // !USE_OPENCV_HIGHGUI_CPP_API
#ifndef DISABLE_TIMER_RECORDING
TIMER timer;
#endif // !DISABLE_TIMER_RECORDING
CHRONO chrono;
#ifndef DISABLE_GUI_REMOTEWEBCAMMULTISRV
CvFont font;
#endif // !DISABLE_GUI_REMOTEWEBCAMMULTISRV
CRITICAL_SECTION imageCS;
IplImage* frame = NULL;
IplImage* resizedframe = NULL;
IplImage* image = NULL;
IplImage* previmage = NULL;
#ifndef DISABLE_GUI_REMOTEWEBCAMMULTISRV
IplImage* detectimage = NULL;
#endif // !DISABLE_GUI_REMOTEWEBCAMMULTISRV
char* databuf = NULL;
int databuflen = 0;
char* sharedbuf = NULL;
int sharedbuflen = 0;
CRITICAL_SECTION sharedbufCS;
#ifndef USE_OPENCV_HIGHGUI_CPP_API
int encodeparams[2];
#else
std::vector<int> encodeparams;
#endif // !USE_OPENCV_HIGHGUI_CPP_API
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
char szVideoRecordCodec[5];
