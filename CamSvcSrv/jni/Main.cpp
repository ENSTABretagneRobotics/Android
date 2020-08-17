#include "Globals.h"

// min and max might cause incompatibilities with GCC...
#ifndef _MSC_VER
#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif // !max
#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif // !min
#endif // !_MSC_VER

int LoadConfig()
{
	FILE* file = NULL;
	char line[256];
	double d0 = 0;

	// Default values.
	memset(szDevPath, 0, sizeof(szDevPath));
	sprintf(szDevPath, "0");
	memset(srvport, 0, sizeof(srvport));
	sprintf(srvport, "4014");
	videoimgwidth = 640; 
	videoimgheight = 480; 
	captureperiod = 40;
	timeout = 5;
	bForceSoftwareResize = 1;
	hcenter = 0;//+videoimgwidth/2
	vcenter = 0;//+videoimgheight/2
	hscale = 1;
	vscale = 1;
	angle = 0*M_PI/180.0;
	hshift = 0;//+videoimgwidth/2
	vshift = 0;//+videoimgheight/2
	bFlip = 0;
	bUDP = 0;
	pixcolorchgthreshold = 12; 
	timecompressiondividerthreshold = 50;
	fullimgperiod = 1000; 
	encodequality = 50;
	memset(encodetype, 0, sizeof(encodetype));
	sprintf(encodetype, ".JPEG");
	method = 3;
	bDisableVideoRecording = 0;
	memset(szVideoRecordCodec, 0, sizeof(szVideoRecordCodec));
	sprintf(szVideoRecordCodec, "WMV2");

#ifdef __ANDROID__
	file = fopen("/storage/sdcard0/download/RemoteWebcamMultiSrv.txt", "r");
#else
	file = fopen("RemoteWebcamMultiSrv.txt", "r");
#endif // __ANDROID__
	if (file != NULL)
	{
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%[^\r\n]255s", szDevPath) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%255s", srvport) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%d", &videoimgwidth) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%d", &videoimgheight) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%d", &captureperiod) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%d", &timeout) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%d", &bForceSoftwareResize) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &hcenter) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &vcenter) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &hscale) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &vscale) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &d0) != 1) printf("Invalid configuration file.\n");
		angle = d0*M_PI/180.0;
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &hshift) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &vshift) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%d", &bFlip) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%d", &bUDP) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%d", &pixcolorchgthreshold) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%d", &timecompressiondividerthreshold) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%d", &fullimgperiod) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%d", &encodequality) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%31s", encodetype) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%d", &method) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%d", &bDisableVideoRecording) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%[^\r\n]4s", szVideoRecordCodec) != 1) printf("Invalid configuration file.\n");
		if (fclose(file) != EXIT_SUCCESS) printf("fclose() failed.\n");
	}
	else
	{
		printf("Configuration file not found.\n");
	}

	if (videoimgwidth <= 0)
	{
		printf("Invalid parameter : videoimgwidth.\n");
		videoimgwidth = 640;
	}
	if (videoimgheight <= 0)
	{
		printf("Invalid parameter : videoimgheight.\n");
		videoimgheight = 480;
	}
	if (captureperiod < 0)
	{
		printf("Invalid parameter : captureperiod.\n");
		captureperiod = 40;
	}
	if (hscale == 0)
	{
		printf("Invalid parameter : hscale.\n");
		hscale = 1;
	}
	if (vscale == 0)
	{
		printf("Invalid parameter : vscale.\n");
		vscale = 1;
	}
	if (timecompressiondividerthreshold < 1)
	{
		printf("Invalid parameter : timecompressiondividerthreshold.\n");
		timecompressiondividerthreshold = 50;
	}

	return EXIT_SUCCESS;
}

#ifndef USE_OPENCV_HIGHGUI_CPP_API
inline int MovementDetection(IplImage* previmg, IplImage* img, IplImage* detectimg, char* buf, int* pnbBytes, 
	char* enctype, int* encparams, int pixcolorchgthresh)
#else
inline int MovementDetection(IplImage* previmg, IplImage* img, IplImage* detectimg, char* buf, int* pnbBytes, 
	char* enctype, std::vector<int>& encparams, int pixcolorchgthresh)
#endif // !USE_OPENCV_HIGHGUI_CPP_API
{
	unsigned int i = 0, val = 0, count = 0, indexb = 0, indexg = 0, indexr = 0;
	int r = 0, g = 0, b = 0;
	unsigned char* prevdata = (unsigned char*)previmg->imageData;
	unsigned char* data = (unsigned char*)img->imageData;
	unsigned char* detectdata = NULL;
#ifndef USE_OPENCV_HIGHGUI_CPP_API
	CvMat* mat = NULL;
#else
	std::vector<uchar> bufmatvector;
#endif // !USE_OPENCV_HIGHGUI_CPP_API

	if (detectimg) detectdata = (unsigned char*)detectimg->imageData;

	// First compute the static compression to be able to compare with the time compression.
#ifndef USE_OPENCV_HIGHGUI_CPP_API
	mat = cvEncodeImage(enctype, img, encparams);
	if (mat == NULL)
	{
		printf("cvEncodeImage() failed.\n");
		return EXIT_FAILURE;
	}
#else
	if (!cv::imencode(enctype, cv::cvarrToMat(img), bufmatvector, encparams))
	{
		printf("cv::imencode() failed.\n");
		return EXIT_FAILURE;
	}
#endif // !USE_OPENCV_HIGHGUI_CPP_API

	// Reserved for number of bytes sent and image dimensions.
	count = 3*sizeof(unsigned int); 

	i = img->imageSize;
	while (i -= 3)
	{
		indexb = i;
		indexg = i+1;
		indexr = i+2;

		b = data[indexb]-prevdata[indexb];
		g = data[indexg]-prevdata[indexg];
		r = data[indexr]-prevdata[indexr];
		if ((abs(b)+abs(g)+abs(r)) > 3*pixcolorchgthresh)
		{
			// Blue index value of the pixel.
			memcpy(buf+count, (char*)&i, sizeof(unsigned int));
			count += sizeof(unsigned int);
			// BGR values.
			memcpy(buf+count, img->imageData+i, 3*sizeof(char));
			count += 3*sizeof(char);
			if (detectimg)
			{
				// Show in green pixels that significantly changed.
				detectdata[indexb] = 0;
				detectdata[indexg] = 255;
				detectdata[indexr] = 0;
			}
		}

		// A full image will be sent if the time compression is worse than the static one.
#ifndef USE_OPENCV_HIGHGUI_CPP_API
		if (count > (unsigned int)(mat->rows*mat->cols)) break;
#else
		if (count > bufmatvector.size()) break;
#endif // !USE_OPENCV_HIGHGUI_CPP_API
	}

#ifndef USE_OPENCV_HIGHGUI_CPP_API
	if (count > (unsigned int)(mat->rows*mat->cols)) 
	{
		val = UINT_MAX; // Special number to indicate a full image.
		memcpy(buf, (char*)&val, sizeof(unsigned int));
		// Static compressed image dimensions.
		memcpy(buf+sizeof(unsigned int), (char*)&mat->rows, sizeof(unsigned int));
		memcpy(buf+2*sizeof(unsigned int), (char*)&mat->cols, sizeof(unsigned int));
		// Full image data (with static compression).
		memcpy(buf+3*sizeof(unsigned int), mat->data.ptr, (size_t)(mat->rows*mat->cols));
		count = 3*sizeof(unsigned int)+(unsigned int)(mat->rows*mat->cols);
	}
#else
	if (count > bufmatvector.size()) 
	{
		val = UINT_MAX; // Special number to indicate a full image.
		memcpy(buf, (char*)&val, sizeof(unsigned int));
		// Static compressed image dimensions.
		i = bufmatvector.size();
		memcpy(buf+sizeof(unsigned int), (char*)&i, sizeof(unsigned int));
		i = 1;
		memcpy(buf+2*sizeof(unsigned int), (char*)&i, sizeof(unsigned int));
		// Full image data (with static compression).
		i = bufmatvector.size();
		while (i--)
		{
			buf[3*sizeof(unsigned int)+i] = (char)bufmatvector[i];
		}
		count = 3*sizeof(unsigned int)+bufmatvector.size();
	}
#endif // !USE_OPENCV_HIGHGUI_CPP_API
	else
	{
		val = count; // Number of bytes sent.
		memcpy(buf, (char*)&val, sizeof(unsigned int));
		// Image dimensions.
		memcpy(buf+sizeof(unsigned int), (char*)&img->width, sizeof(unsigned int));
		memcpy(buf+2*sizeof(unsigned int), (char*)&img->height, sizeof(unsigned int));
	}

	*pnbBytes = (int)count;

#ifndef USE_OPENCV_HIGHGUI_CPP_API
	cvReleaseMat(&mat);
#else
	bufmatvector.clear();
#endif // !USE_OPENCV_HIGHGUI_CPP_API

	return EXIT_SUCCESS;
}

#ifndef USE_OPENCV_HIGHGUI_CPP_API
inline int MovementDetection2(IplImage* previmg, IplImage* img, IplImage* detectimg, char* buf, int* pnbBytes, 
	char* enctype, int* encparams, int pixcolorchgthresh, int timecompressiondividerthresh)
#else
inline int MovementDetection2(IplImage* previmg, IplImage* img, IplImage* detectimg, char* buf, int* pnbBytes, 
	char* enctype, std::vector<int>& encparams, int pixcolorchgthresh, int timecompressiondividerthresh)
#endif // !USE_OPENCV_HIGHGUI_CPP_API
{
	unsigned int i = 0, val = 0, count = 0, indexb = 0, indexg = 0, indexr = 0;
	int r = 0, g = 0, b = 0;
	unsigned char* prevdata = (unsigned char*)previmg->imageData;
	unsigned char* data = (unsigned char*)img->imageData;
	unsigned char* detectdata = NULL;
#ifndef USE_OPENCV_HIGHGUI_CPP_API
	CvMat* mat = NULL;
#else
	std::vector<uchar> bufmatvector;
#endif // !USE_OPENCV_HIGHGUI_CPP_API

	if (detectimg) detectdata = (unsigned char*)detectimg->imageData;

	// Reserved for number of bytes sent and image dimensions.
	count = 3*sizeof(unsigned int); 

	i = img->imageSize;
	while (i -= 3)
	{
		indexb = i;
		indexg = i+1;
		indexr = i+2;

		b = data[indexb]-prevdata[indexb];
		g = data[indexg]-prevdata[indexg];
		r = data[indexr]-prevdata[indexr];
		if ((abs(b)+abs(g)+abs(r)) > 3*pixcolorchgthresh)
		{
			// Blue index value of the pixel.
			memcpy(buf+count, (char*)&i, sizeof(unsigned int));
			count += sizeof(unsigned int);
			// BGR values.
			memcpy(buf+count, img->imageData+i, 3*sizeof(char));
			count += 3*sizeof(char);
			if (detectimg)
			{
				// Show in green pixels that significantly changed.
				detectdata[indexb] = 0;
				detectdata[indexg] = 255;
				detectdata[indexr] = 0;
			}
		}

		// A full image will be sent if the time compression is bad.
		if (count > (unsigned int)(img->imageSize/timecompressiondividerthresh)) break;
	}	

	if (count > (unsigned int)(img->imageSize/timecompressiondividerthresh)) 
	{
		val = UINT_MAX; // Special number to indicate a full image.
		memcpy(buf, (char*)&val, sizeof(unsigned int));
#ifndef USE_OPENCV_HIGHGUI_CPP_API
		mat = cvEncodeImage(enctype, img, encparams);
		if (mat == NULL)
		{
			printf("cvMat() failed.\n");
			return EXIT_FAILURE;
		}
		// Static compressed image dimensions.
		memcpy(buf+sizeof(unsigned int), (char*)&mat->rows, sizeof(unsigned int));
		memcpy(buf+2*sizeof(unsigned int), (char*)&mat->cols, sizeof(unsigned int));
		// Full image data (with static compression).
		memcpy(buf+3*sizeof(unsigned int), mat->data.ptr, (size_t)(mat->rows*mat->cols));
		count = 3*sizeof(unsigned int)+(unsigned int)(mat->rows*mat->cols);
		cvReleaseMat(&mat);
#else
		if (!cv::imencode(enctype, cv::cvarrToMat(img), bufmatvector, encparams))
		{
			printf("cv::imencode() failed.\n");
			return EXIT_FAILURE;
		}
		// Static compressed image dimensions.
		i = bufmatvector.size();
		memcpy(buf+sizeof(unsigned int), (char*)&i, sizeof(unsigned int));
		i = 1;
		memcpy(buf+2*sizeof(unsigned int), (char*)&i, sizeof(unsigned int));
		// Full image data (with static compression).
		i = bufmatvector.size();
		while (i--)
		{
			buf[3*sizeof(unsigned int)+i] = (char)bufmatvector[i];
		}
		count = 3*sizeof(unsigned int)+bufmatvector.size();
		bufmatvector.clear();
#endif // !USE_OPENCV_HIGHGUI_CPP_API
	}
	else
	{
		val = count; // Number of bytes sent.
		memcpy(buf, (char*)&val, sizeof(unsigned int));
		// Image dimensions.
		memcpy(buf+sizeof(unsigned int), (char*)&img->width, sizeof(unsigned int));
		memcpy(buf+2*sizeof(unsigned int), (char*)&img->height, sizeof(unsigned int));
	}

	*pnbBytes = (int)count;

	return EXIT_SUCCESS;
}

int handlecli(SOCKET sockcli, void* pParam)
{
	// Should send a full image when connecting for method 0 and 1...
	//BOOL bForceSendFullImg = TRUE; 
	BOOL bInitDone = FALSE;
	char httpbuf[2048];
	char* sendbuf = NULL;
	int sendbuflen = 0;

	UNREFERENCED_PARAMETER(pParam);

	sendbuf = (char*)calloc(databuflen, sizeof(char));
	if (!sendbuf)	
	{
		printf("calloc() failed.\n");
		return EXIT_FAILURE;
	}

	for (;;)
	{
		fd_set sock_set;
		int iResult = SOCKET_ERROR;
		struct timeval tv;

		tv.tv_sec = (long)(timeout/1000);
		tv.tv_usec = (long)((timeout%1000)*1000);

		// Initialize a fd_set and add the socket to it.
		FD_ZERO(&sock_set); 
		FD_SET(sockcli, &sock_set);

		iResult = select((int)sockcli+1, NULL, &sock_set, NULL, &tv);

		// Remove the socket from the set.
		// No need to use FD_ISSET() here, as we only have one socket the return value of select() is 
		// sufficient to know what happened.
		FD_CLR(sockcli, &sock_set); 

		switch (iResult)
		{
		case SOCKET_ERROR:
			free(sendbuf);
			return EXIT_FAILURE;
		case 0:
			// The timeout on select() occured.
			break;
		default:
			{
				switch (method)
				{
				case 0:
				case 1:
				case 2:	
					break;
				case 3:	
					if (!bInitDone)
					{
						// Receive the GET request, but do not analyze it...
						tv.tv_sec = (long)(timeout/1000);
						tv.tv_usec = (long)((timeout%1000)*1000);
						if (waitforsocket(sockcli, tv) == EXIT_SUCCESS)
						{
							memset(httpbuf, 0, sizeof(httpbuf));
							if (recv(sockcli, httpbuf, sizeof(httpbuf), 0) <= 0)
							{
								printf("recv() failed.\n");
								free(sendbuf);
								return EXIT_FAILURE;
							}

							// See https://www.w3.org/Protocols/rfc1341/7_2_Multipart.html, 
							// https://stackoverflow.com/questions/47729941/mjpeg-over-http-specification...

							memset(httpbuf, 0, sizeof(httpbuf));
							sprintf(httpbuf, 
								"HTTP/1.1 200 OK\r\n"
								"Server: RemoteWebcamMultiSrv\r\n"
								//"Connection: close\r\n"
								//"Max-Age: 0\r\n"
								//"Expires: 0\r\n"
								//"Cache-Control: no-cache, private, no-store, must-revalidate, pre-check = 0, post-check = 0, max-age = 0\r\n"
								//"Pragma: no-cache\r\n"
								"Content-Type: multipart/x-mixed-replace; boundary=boundary\r\n"
								//"Media-type: image/jpeg\r\n"
								//"\r\n" // CRLF will be in the next encapsulation boundary "\r\n--boundary\r\n"...
							);
							if (sendall(sockcli, httpbuf, strlen(httpbuf)) != EXIT_SUCCESS)
							{
								free(sendbuf);
								return EXIT_FAILURE;
							}
						}
						bInitDone = TRUE;
					}
					break;
				default:
					printf("Invalid parameter.\n");
					free(sendbuf);
					return EXIT_FAILURE;
					break;
				}
			}

			EnterCriticalSection(&sharedbufCS);
			sendbuflen = sharedbuflen;
			memcpy(sendbuf, sharedbuf, sharedbuflen);
			LeaveCriticalSection(&sharedbufCS);

			if (sendall(sockcli, sendbuf, sendbuflen) != EXIT_SUCCESS)
			{
				free(sendbuf);
				return EXIT_FAILURE;
			}
			uSleep(1000*captureperiod);
			break;
		}

		if (bStop) break;
	}

	free(sendbuf);

	return EXIT_SUCCESS;
}

void CleanUp(void)
{
	// Ensure the video recording is correctly stopped.
#ifndef DISABLE_TIMER_RECORDING
	DeleteTimer(&timer, FALSE);
#endif // !DISABLE_TIMER_RECORDING
#ifndef USE_OPENCV_HIGHGUI_CPP_API
	if (videorecordfile) cvReleaseVideoWriter(&videorecordfile);
	videorecordfile = NULL;
#else
	if (videorecordfile.isOpened()) videorecordfile.release();
#endif // !USE_OPENCV_HIGHGUI_CPP_API

	bStop = TRUE;
	StopChronoQuick(&chrono);
#ifndef DISABLE_GUI_REMOTEWEBCAMMULTISRV
#ifndef USE_OPENCV_HIGHGUI_CPP_API
	cvDestroyWindow("Detection");
#else
	cv::destroyWindow("Detection");
#endif // !USE_OPENCV_HIGHGUI_CPP_API
	cvReleaseImage(&detectimage);
#endif // !DISABLE_GUI_REMOTEWEBCAMMULTISRV
	cvReleaseImage(&previmage);
	free(databuf);
	mSleep(15000);
	exit(EXIT_FAILURE);
}

THREAD_PROC_RETURN_VALUE handlecam(void* pParam)
{
	double m[6]; // For rotation...
	CvMat M = cvMat(2, 3, CV_64F, m); // For rotation...
	double hcenter0 = 0, vcenter0 = 0, hshift0 = 0, vshift0 = 0;
	int nbBytes = 0;
	unsigned int val = 0;
#ifndef USE_OPENCV_HIGHGUI_CPP_API
	CvMat* mat = NULL;
#else
	std::vector<uchar> bufmatvector;
	unsigned int i = 0;
#endif // !USE_OPENCV_HIGHGUI_CPP_API
	BOOL bForceSendFullImg = TRUE;
	char httpbuf[2048];
#ifndef DISABLE_GUI_REMOTEWEBCAMMULTISRV
	char szText[256];
	int c = 0;
#endif // !DISABLE_GUI_REMOTEWEBCAMMULTISRV

	UNREFERENCED_PARAMETER(pParam);

	databuf = (char*)calloc(databuflen, sizeof(char));
	if (!databuf)	
	{
		printf("calloc() failed.\n");
		return 0;
	}
	previmage = cvCloneImage(image);
	if (!previmage)	
	{
		printf("Error copying an image.\n");
		free(databuf);
		return 0;
	}
#ifndef DISABLE_GUI_REMOTEWEBCAMMULTISRV
	detectimage = cvCloneImage(image);
	if (!detectimage)	
	{
		printf("Error copying an image.\n");
		cvReleaseImage(&previmage);
		free(databuf);
		return 0;
	}
#endif // !DISABLE_GUI_REMOTEWEBCAMMULTISRV

#ifndef DISABLE_GUI_REMOTEWEBCAMMULTISRV
#ifndef USE_OPENCV_HIGHGUI_CPP_API
	cvNamedWindow("Detection", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("Detection", 0, 0);
	cvShowImage("Detection", image);
	cvWaitKey(10);
#else
	cv::namedWindow("Detection", cv::WINDOW_AUTOSIZE);
	cv::moveWindow("Detection", 0, 0);
	cv::imshow("Detection", cv::cvarrToMat(image));
	cv::waitKey(10);
#endif // !USE_OPENCV_HIGHGUI_CPP_API
#else
	mSleep(10);
#endif // !DISABLE_GUI_REMOTEWEBCAMMULTISRV

	StartChrono(&chrono);

	for (;;)
	{
#ifndef USE_OPENCV_HIGHGUI_CPP_API
		frame = cvQueryFrame(webcam);
		if (!frame)
		{
			printf("Error getting an image from the webcam.\n");
			CleanUp();
			return 0;
		}
#else
		if (!webcam->read(*pframemat))
		{
			printf("Error getting an image from the webcam.\n");
			CleanUp();
			return 0;
		}
		frameipl = (IplImage)*pframemat;
		frame = &frameipl;
#endif // !USE_OPENCV_HIGHGUI_CPP_API

		if (bForceSoftwareResize) cvResize(frame, resizedframe, CV_INTER_LINEAR);
		else resizedframe = frame;

		EnterCriticalSection(&imageCS);
		if ((hcenter == 0)&&(vcenter == 0)&&
			(hscale == 1)&&(vscale == 1)&&(angle == 0)&&
			(hshift == 0)&&(vshift == 0))
		{
			if (bFlip) cvFlip(resizedframe, image, 1); else cvCopy(resizedframe, image, 0);
		}
		else
		{
			// Create a map_matrix, where the left 2x2 matrix is the transform and the right 2x1 is the dimensions.

			hcenter0 = resizedframe->width*0.5+hcenter;
			vcenter0 = resizedframe->height*0.5+vcenter;
			hshift0 = resizedframe->width*0.5+hshift;
			vshift0 = resizedframe->height*0.5+vshift;

			m[0] = cos(angle)/hscale;
			m[1] = sin(angle)/hscale;
			m[3] = -sin(angle)/vscale;
			m[4] = cos(angle)/vscale;
			m[2] = (1-cos(angle)/hscale)*hshift0-(sin(angle)/hscale)*vshift0+hcenter0-hshift0;  
			m[5] = (sin(angle)/vscale)*hshift0+(1-cos(angle)/vscale)*vshift0+vcenter0-vshift0;
			cvWarpAffine(resizedframe, image, &M, CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS+CV_WARP_INVERSE_MAP, cvScalarAll(0));

			if (bFlip) cvFlip(image, NULL, 1);
		}
		LeaveCriticalSection(&imageCS);

#ifdef DISABLE_TIMER_RECORDING
#ifndef USE_OPENCV_HIGHGUI_CPP_API
		if ((videorecordfile != NULL)&&(image != NULL))
		{
			if (!cvWriteFrame(videorecordfile, image))
#else
		if ((videorecordfile.isOpened())&&(image != NULL))
		{
			try
			{
				videorecordfile.write(cv::cvarrToMat(image));
			}
			catch (...)
#endif // !USE_OPENCV_HIGHGUI_CPP_API
			{
				printf("Error writing to a video file.\n");
			}
		}
#endif // DISABLE_TIMER_RECORDING

#ifndef DISABLE_GUI_REMOTEWEBCAMMULTISRV
		cvCopy(image, detectimage, NULL);
#endif // !DISABLE_GUI_REMOTEWEBCAMMULTISRV

		if (
			((method == 0)||(method == 1))&&
			((GetTimeElapsedChronoQuick(&chrono) > 0.001*fullimgperiod)||bForceSendFullImg)
			) 
		{
			StopChronoQuick(&chrono);
			bForceSendFullImg = FALSE;
			val = UINT_MAX; // Special number to indicate a full image with static compression.
			memcpy(databuf, (char*)&val, sizeof(unsigned int));
#ifndef USE_OPENCV_HIGHGUI_CPP_API
			mat = cvEncodeImage(encodetype, image, encodeparams);
			if (mat == NULL)
			{
				printf("cvEncodeImage() failed.\n");
				CleanUp();
				return 0;
			}
			// Static compressed image dimensions.
			memcpy(databuf+sizeof(unsigned int), (char*)&mat->rows, sizeof(unsigned int));
			memcpy(databuf+2*sizeof(unsigned int), (char*)&mat->cols, sizeof(unsigned int));
			// Full image data (with static compression).
			memcpy(databuf+3*sizeof(unsigned int), mat->data.ptr, (size_t)(mat->rows*mat->cols));
			nbBytes = 3*sizeof(unsigned int)+(unsigned int)(mat->rows*mat->cols);
			cvReleaseMat(&mat);
#else
			if (!cv::imencode(encodetype, cv::cvarrToMat(image), bufmatvector, encodeparams))
			{
				printf("cv::imencode() failed.\n");
				CleanUp();
				return 0;
			}
			// Static compressed image dimensions.
			i = bufmatvector.size();
			memcpy(databuf+sizeof(unsigned int), (char*)&i, sizeof(unsigned int));
			i = 1;
			memcpy(databuf+2*sizeof(unsigned int), (char*)&i, sizeof(unsigned int));
			// Full image data (with static compression).
			i = bufmatvector.size();
			while (i--)
			{
				databuf[3*sizeof(unsigned int)+i] = (char)bufmatvector[i];
			}
			nbBytes = 3*sizeof(unsigned int)+bufmatvector.size();
			bufmatvector.clear();
#endif // !USE_OPENCV_HIGHGUI_CPP_API
#ifndef DISABLE_GUI_REMOTEWEBCAMMULTISRV
			sprintf(szText, "Periodic complete image : %d bytes", nbBytes);
			cvPutText(detectimage, szText, cvPoint(0,48), &font, CV_RGB(255,0,0));
#endif // !DISABLE_GUI_REMOTEWEBCAMMULTISRV
			StartChrono(&chrono);
		}
		else
		{
			switch (method)
			{
			case 0:
#ifndef DISABLE_GUI_REMOTEWEBCAMMULTISRV
				if (MovementDetection(previmage, image, detectimage, databuf, &nbBytes,
					encodetype, encodeparams, pixcolorchgthreshold) != EXIT_SUCCESS)
#else
				if (MovementDetection(previmage, image, NULL, databuf, &nbBytes,
					encodetype, encodeparams, pixcolorchgthreshold) != EXIT_SUCCESS)
#endif // !DISABLE_GUI_REMOTEWEBCAMMULTISRV
				{
					CleanUp();
					return 0;
				}
				break;
			case 1:
#ifndef DISABLE_GUI_REMOTEWEBCAMMULTISRV
				if (MovementDetection2(previmage, image, detectimage, databuf, &nbBytes,
					encodetype, encodeparams, pixcolorchgthreshold, timecompressiondividerthreshold) != EXIT_SUCCESS)
#else
				if (MovementDetection2(previmage, image, NULL, databuf, &nbBytes,
					encodetype, encodeparams, pixcolorchgthreshold, timecompressiondividerthreshold) != EXIT_SUCCESS)
#endif // !DISABLE_GUI_REMOTEWEBCAMMULTISRV
				{
					CleanUp();
					return 0;
				}
				break;
			case 2:	
				val = (unsigned int)image->imageSize+3*sizeof(unsigned int); // Special number to indicate a full image without compression.
				memcpy(databuf, (char*)&val, sizeof(unsigned int));
				// Image dimensions.
				memcpy(databuf+sizeof(unsigned int), (char*)&image->width, sizeof(unsigned int));
				memcpy(databuf+2*sizeof(unsigned int), (char*)&image->height, sizeof(unsigned int));
				// Full image data without compression.
				memcpy(databuf+3*sizeof(unsigned int), image->imageData, (size_t)image->imageSize);
				nbBytes = val;
				break;
			case 3:	
				nbBytes = 0;
#ifndef USE_OPENCV_HIGHGUI_CPP_API
				mat = cvEncodeImage(encodetype, image, encodeparams);
				if (mat == NULL)
				{
					printf("cvEncodeImage() failed.\n");
					CleanUp();
					return 0;
				}
				memset(httpbuf, 0, sizeof(httpbuf));
				sprintf(httpbuf,
					"\r\n--boundary\r\n"
					"Content-Type: image/jpeg\r\n"
					"Content-Length: %d\r\n"
					"\r\n", mat->rows*mat->cols);
				memcpy(databuf+nbBytes, httpbuf, strlen(httpbuf));
				nbBytes += strlen(httpbuf);
				// Full image data (with static compression).
				memcpy(databuf+nbBytes, mat->data.ptr, (size_t)(mat->rows*mat->cols));
				nbBytes += (mat->rows*mat->cols);
				cvReleaseMat(&mat);
#else
				if (!cv::imencode(encodetype, cv::cvarrToMat(image), bufmatvector, encodeparams))
				{
					printf("cv::imencode() failed.\n");
					CleanUp();
					return 0;
				}
				memset(httpbuf, 0, sizeof(httpbuf));
				sprintf(httpbuf,
					"\r\n--boundary\r\n"
					"Content-Type: image/jpeg\r\n"
					"Content-Length: %d\r\n"
					"\r\n", (int)bufmatvector.size());
				memcpy(databuf+nbBytes, httpbuf, strlen(httpbuf));
				nbBytes += strlen(httpbuf);
				// Full image data (with static compression).
				i = bufmatvector.size();
				while (i--)
				{
					databuf[nbBytes+i] = (char)bufmatvector[i];
				}
				nbBytes += (int)bufmatvector.size();
				bufmatvector.clear();
#endif // !USE_OPENCV_HIGHGUI_CPP_API
				break;
			default:
				printf("Invalid parameter.\n");
				CleanUp();
				return 0;
				break;
			}
#ifndef DISABLE_GUI_REMOTEWEBCAMMULTISRV
			sprintf(szText, "Data transmitted : %d bytes", nbBytes);
			cvPutText(detectimage, szText, cvPoint(0,32), &font, CV_RGB(255,0,0));
#endif // !DISABLE_GUI_REMOTEWEBCAMMULTISRV
		}

		EnterCriticalSection(&sharedbufCS);
		sharedbuflen = nbBytes;
		memcpy(sharedbuf, databuf, nbBytes);
		LeaveCriticalSection(&sharedbufCS);

#ifndef DISABLE_GUI_REMOTEWEBCAMMULTISRV
		sprintf(szText, "Uncompressed size : %d bytes", image->imageSize);
		cvPutText(detectimage, szText, cvPoint(0,16), &font, CV_RGB(255,0,0));
#endif // !DISABLE_GUI_REMOTEWEBCAMMULTISRV
		cvCopy(image, previmage, NULL);

#ifndef DISABLE_GUI_REMOTEWEBCAMMULTISRV
#ifndef USE_OPENCV_HIGHGUI_CPP_API
		cvShowImage("Detection", detectimage);
		c = cvWaitKey(captureperiod);
#else
		cv::imshow("Detection", cv::cvarrToMat(detectimage));
		c = cv::waitKey(captureperiod);
#endif // !USE_OPENCV_HIGHGUI_CPP_API
		if ((char)c == 27) break;
#else
		uSleep(1000*captureperiod);
#endif // !DISABLE_GUI_REMOTEWEBCAMMULTISRV
		if (bStop) break;
	}

	// Ensure the video recording is correctly stopped.
#ifndef DISABLE_TIMER_RECORDING
	DeleteTimer(&timer, FALSE);
#endif // !DISABLE_TIMER_RECORDING
#ifndef USE_OPENCV_HIGHGUI_CPP_API
	if (videorecordfile) cvReleaseVideoWriter(&videorecordfile);
	videorecordfile = NULL;
#else
	if (videorecordfile.isOpened()) videorecordfile.release();
#endif // !USE_OPENCV_HIGHGUI_CPP_API

	bStop = TRUE;
	StopChronoQuick(&chrono);
#ifndef DISABLE_GUI_REMOTEWEBCAMMULTISRV
#ifndef USE_OPENCV_HIGHGUI_CPP_API
	cvDestroyWindow("Detection");
#else
	cv::destroyWindow("Detection");
#endif // !USE_OPENCV_HIGHGUI_CPP_API
	cvReleaseImage(&detectimage);
#endif // !DISABLE_GUI_REMOTEWEBCAMMULTISRV
	cvReleaseImage(&previmage);
	mSleep(15000);
	exit(EXIT_SUCCESS);

	return 0;
}

#ifndef DISABLE_TIMER_RECORDING
TIMERCALLBACK_RETURN_VALUE VideoRecordCallbackFunction(void* pParam, BOOLEAN b)
{
	UNREFERENCED_PARAMETER(pParam);
	UNREFERENCED_PARAMETER(b);

	EnterCriticalSection(&imageCS);
#ifndef USE_OPENCV_HIGHGUI_CPP_API
	if ((videorecordfile != NULL)&&(image != NULL))
	{
		if (!cvWriteFrame(videorecordfile, image))
#else
	if ((videorecordfile.isOpened())&&(image != NULL))
	{
		try
		{
			videorecordfile.write(cv::cvarrToMat(image));
		}
		catch (...)
#endif // !USE_OPENCV_HIGHGUI_CPP_API
		{
			printf("Error writing to a video file.\n");
		}
	}
	LeaveCriticalSection(&imageCS);
}
#endif // !DISABLE_TIMER_RECORDING

int quitall()
{
	bStop = TRUE;
	mSleep(15000);
	exit(EXIT_SUCCESS);
}

#ifdef __ANDROID__
int AppNativeQuit()
{
	return quitall();
}

int AppNativeMain()
{
#else
#if defined(_WIN32) && !defined(_DEBUG)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	int argc = __argc;
	char** argv = __argv;
#else
int main(int argc, char* argv[])
{
#endif // defined(_WIN32) && !defined(_DEBUG)
#endif // __ANDROID__	
	double m[6]; // For rotation...
	CvMat M = cvMat(2, 3, CV_64F, m); // For rotation...
	double hcenter0 = 0, vcenter0 = 0, hshift0 = 0, vshift0 = 0;
	THREAD_IDENTIFIER handlecamThreadId;
	char videorecordextension[16];
	char videorecordfilename[256];
	double fps = 0;
	int i = 0;

#ifndef __ANDROID__
	INIT_DEBUG;

	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);
#endif // !__ANDROID__

	LoadConfig();

	if (strncmp(encodetype, ".JPEG", min(strlen(encodetype), strlen(".JPEG"))+1) == 0)
	{
#ifndef USE_OPENCV_HIGHGUI_CPP_API
		encodeparams[0] = CV_IMWRITE_JPEG_QUALITY;
		encodeparams[1] = encodequality;
#else
		encodeparams.push_back(cv::IMWRITE_JPEG_QUALITY);
		encodeparams.push_back(encodequality);
#endif // !USE_OPENCV_HIGHGUI_CPP_API
	}
	else if (strncmp(encodetype, ".PNG", min(strlen(encodetype), strlen(".PNG"))+1) == 0)
	{
#ifndef USE_OPENCV_HIGHGUI_CPP_API
		encodeparams[0] = CV_IMWRITE_PNG_COMPRESSION;
		encodeparams[1] = encodequality;
#else
		encodeparams.push_back(cv::IMWRITE_PNG_COMPRESSION);
		encodeparams.push_back(encodequality);
#endif // !USE_OPENCV_HIGHGUI_CPP_API
	}
	else
	{
#ifndef USE_OPENCV_HIGHGUI_CPP_API
		encodeparams[0] = CV_IMWRITE_PXM_BINARY;
		encodeparams[1] = encodequality;
#else
		encodeparams.push_back(cv::IMWRITE_PXM_BINARY);
		encodeparams.push_back(encodequality);
#endif // !USE_OPENCV_HIGHGUI_CPP_API
	}

#ifndef DISABLE_GUI_REMOTEWEBCAMMULTISRV
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5, 0, 1, 8);
#endif // !DISABLE_GUI_REMOTEWEBCAMMULTISRV

#ifndef USE_OPENCV_HIGHGUI_CPP_API
	if ((strlen(szDevPath) == 1)&&(isdigit(szDevPath[0])))
	{
		webcam = cvCreateCameraCapture(atoi(szDevPath));
	}
	else
	{
		webcam = cvCreateFileCapture(szDevPath);
	}
	if (!webcam)
	{
		printf("Error opening the webcam.\n");
		return EXIT_FAILURE;
	}

	cvSetCaptureProperty(webcam, CV_CAP_PROP_FRAME_WIDTH, videoimgwidth);
	cvSetCaptureProperty(webcam, CV_CAP_PROP_FRAME_HEIGHT, videoimgheight);

	// Sometimes the first images are bad, so wait a little bit and take
	// several images in the beginning.
	i = 0;
	while (i < 2)
	{
		mSleep(500);
		frame = cvQueryFrame(webcam);
		i++;
	}

	frame = cvQueryFrame(webcam);
	if (!frame)
	{
		printf("Error getting an image from the webcam.\n");
		cvReleaseCapture(&webcam);
		return EXIT_FAILURE;
	}
#else
	pframemat = new cv::Mat();
	if (!pframemat)
	{
		printf("Out of memory.\n");
		encodeparams.clear();
		return EXIT_FAILURE;
	}
	webcam = new cv::VideoCapture();
	if (!webcam)
	{
		printf("Out of memory.\n");
		delete pframemat;
		encodeparams.clear();
		return EXIT_FAILURE;
	}
	if ((strlen(szDevPath) == 1)&&(isdigit(szDevPath[0])))
	{
		if (!webcam->open(atoi(szDevPath)))
		{
			printf("Error opening the webcam.\n");
			delete webcam;
			delete pframemat;
			encodeparams.clear();
			return EXIT_FAILURE;
		}
	}
	else
	{
		if (!webcam->open(szDevPath))
		{
			printf("Error opening the webcam.\n");
			delete webcam;
			delete pframemat;
			encodeparams.clear();
			return EXIT_FAILURE;
		}
	}

#if (CV_MAJOR_VERSION < 3)
	webcam->set(CV_CAP_PROP_FRAME_WIDTH, videoimgwidth);
	webcam->set(CV_CAP_PROP_FRAME_HEIGHT, videoimgheight);
#else
	webcam->set(cv::CAP_PROP_FRAME_WIDTH, videoimgwidth);
	webcam->set(cv::CAP_PROP_FRAME_HEIGHT, videoimgheight);
#endif // (CV_MAJOR_VERSION < 3)

	// Sometimes the first images are bad, so wait a little bit and take
	// several images in the beginning.
	i = 0;
	while (i < 2)
	{
		mSleep(500);
		webcam->read(*pframemat);
		i++;
	}

	if (!webcam->read(*pframemat))
	{
		printf("Error getting an image from the webcam.\n");
		webcam->release();
		delete webcam;
		delete pframemat;
		encodeparams.clear();
		return EXIT_FAILURE;
	}
	frameipl = (IplImage)*pframemat;
	frame = &frameipl;
#endif // !USE_OPENCV_HIGHGUI_CPP_API

	if (bForceSoftwareResize) 
	{
		resizedframe = cvCreateImage(cvSize(videoimgwidth,videoimgheight), frame->depth, frame->nChannels);
		if (!resizedframe)	
		{
			printf("Error creating an image.\n");
#ifndef USE_OPENCV_HIGHGUI_CPP_API
			cvReleaseCapture(&webcam);
#else
			webcam->release();
			delete webcam;
			delete pframemat;
			encodeparams.clear();
#endif // !USE_OPENCV_HIGHGUI_CPP_API
			return EXIT_FAILURE;
		}
		cvResize(frame, resizedframe, CV_INTER_LINEAR);
	}
	else resizedframe = frame;

	image = cvCloneImage(resizedframe);
	if (!image)	
	{
		printf("Error copying an image.\n");
		if (bForceSoftwareResize) cvReleaseImage(&resizedframe);
#ifndef USE_OPENCV_HIGHGUI_CPP_API
		cvReleaseCapture(&webcam);
#else
		webcam->release();
		delete webcam;
		delete pframemat;
		encodeparams.clear();
#endif // !USE_OPENCV_HIGHGUI_CPP_API
		return EXIT_FAILURE;
	}

	if ((hcenter == 0)&&(vcenter == 0)&&
		(hscale == 1)&&(vscale == 1)&&(angle == 0)&&
		(hshift == 0)&&(vshift == 0))
	{
		if (bFlip) cvFlip(resizedframe, image, 1); else cvCopy(resizedframe, image, 0);
	}
	else
	{
		// Create a map_matrix, where the left 2x2 matrix is the transform and the right 2x1 is the dimensions.

		hcenter0 = resizedframe->width*0.5+hcenter;
		vcenter0 = resizedframe->height*0.5+vcenter;
		hshift0 = resizedframe->width*0.5+hshift;
		vshift0 = resizedframe->height*0.5+vshift;

		m[0] = cos(angle)/hscale;
		m[1] = sin(angle)/hscale;
		m[3] = -sin(angle)/vscale;
		m[4] = cos(angle)/vscale;
		m[2] = (1-cos(angle)/hscale)*hshift0-(sin(angle)/hscale)*vshift0+hcenter0-hshift0;  
		m[5] = (sin(angle)/vscale)*hshift0+(1-cos(angle)/vscale)*vshift0+vcenter0-vshift0;
		cvWarpAffine(resizedframe, image, &M, CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS+CV_WARP_INVERSE_MAP, cvScalarAll(0));

		if (bFlip) cvFlip(image, NULL, 1);
	}

	if (!bDisableVideoRecording)
	{
#ifdef DISABLE_TIMER_RECORDING
		fps = 1000.0/(double)(captureperiod+20); // 20 ms is an approximation of the extra time spent for computations...
#else
		fps = 1000.0/(double)captureperiod;
#endif // DISABLE_TIMER_RECORDING
		memset(videorecordextension, 0, sizeof(videorecordextension));
		if (strncmp(szVideoRecordCodec, "WMV2", strlen("WMV2")) == 0) strcpy(videorecordextension, "wmv");
		else if (strncmp(szVideoRecordCodec, "DIVX", strlen("DIVX")) == 0) strcpy(videorecordextension, "avi");
		else if (strncmp(szVideoRecordCodec, "XVID", strlen("XVID")) == 0) strcpy(videorecordextension, "avi");
		else if (strncmp(szVideoRecordCodec, "MJPG", strlen("MJPG")) == 0) strcpy(videorecordextension, "avi");
		else strcpy(videorecordextension, "avi");
		sprintf(videorecordfilename, VID_FOLDER"vid_%.64s.%.15s", strtimeex_fns(), videorecordextension);
#ifndef USE_OPENCV_HIGHGUI_CPP_API
		videorecordfile = cvCreateVideoWriter(videorecordfilename,
			//CV_FOURCC_PROMPT,
			CV_FOURCC(szVideoRecordCodec[0], szVideoRecordCodec[1], szVideoRecordCodec[2], szVideoRecordCodec[3]),
			fps,
			cvSize(image->width, image->height),
			1);
		if (!videorecordfile)
#else
		if (!videorecordfile.open(videorecordfilename,
#if (CV_MAJOR_VERSION < 3)
			CV_FOURCC(szVideoRecordCodec[0], szVideoRecordCodec[1], szVideoRecordCodec[2], szVideoRecordCodec[3]),
#else
			cv::VideoWriter::fourcc(szVideoRecordCodec[0], szVideoRecordCodec[1], szVideoRecordCodec[2], szVideoRecordCodec[3]),
#endif // (CV_MAJOR_VERSION < 3)
			fps,
			cvSize(image->width, image->height),
			1))
#endif // !USE_OPENCV_HIGHGUI_CPP_API
		{
			printf("Error creating a video file.\n");
		}
	}

	InitCriticalSection(&imageCS);

#ifndef DISABLE_TIMER_RECORDING
	if (CreateTimer(&timer, VideoRecordCallbackFunction, NULL, captureperiod, captureperiod) != EXIT_SUCCESS)
	{
		printf("Error creating a timer.\n");
	}
#endif // !DISABLE_TIMER_RECORDING

	databuflen = image->imageSize+3*sizeof(unsigned int);
	sharedbuflen = 0;
	sharedbuf = (char*)calloc(databuflen, sizeof(char));
	if (!sharedbuf)	
	{
		printf("calloc() failed.\n");
#ifndef DISABLE_TIMER_RECORDING
		DeleteTimer(&timer, FALSE);
#endif // !DISABLE_TIMER_RECORDING
		DeleteCriticalSection(&imageCS);
#ifndef USE_OPENCV_HIGHGUI_CPP_API
		if (videorecordfile) cvReleaseVideoWriter(&videorecordfile);
#else
		if (videorecordfile.isOpened()) videorecordfile.release();
#endif // !USE_OPENCV_HIGHGUI_CPP_API
		if (bForceSoftwareResize) cvReleaseImage(&resizedframe);
		cvReleaseImage(&image);
#ifndef USE_OPENCV_HIGHGUI_CPP_API
		cvReleaseCapture(&webcam);
#else
		webcam->release();
		delete webcam;
		delete pframemat;
		encodeparams.clear();
#endif // !USE_OPENCV_HIGHGUI_CPP_API
		return EXIT_FAILURE;
	}

	if (InitCriticalSection(&sharedbufCS) != EXIT_SUCCESS)
	{
		printf("Error creating critical section.\n");
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		free(sharedbuf);
#ifndef DISABLE_TIMER_RECORDING
		DeleteTimer(&timer, FALSE);
#endif // !DISABLE_TIMER_RECORDING
		DeleteCriticalSection(&imageCS);
#ifndef USE_OPENCV_HIGHGUI_CPP_API
		if (videorecordfile) cvReleaseVideoWriter(&videorecordfile);
#else
		if (videorecordfile.isOpened()) videorecordfile.release();
#endif // !USE_OPENCV_HIGHGUI_CPP_API
		if (bForceSoftwareResize) cvReleaseImage(&resizedframe);
		cvReleaseImage(&image);
#ifndef USE_OPENCV_HIGHGUI_CPP_API
		cvReleaseCapture(&webcam);
#else
		webcam->release();
		delete webcam;
		delete pframemat;
		encodeparams.clear();
#endif // !USE_OPENCV_HIGHGUI_CPP_API
		return EXIT_FAILURE;
	}

	if (CreateDefaultThread(handlecam, NULL, &handlecamThreadId) != EXIT_SUCCESS)
	{
		printf("Error creating thread.\n");
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		DeleteCriticalSection(&sharedbufCS);
		free(sharedbuf);
#ifndef DISABLE_TIMER_RECORDING
		DeleteTimer(&timer, FALSE);
#endif // !DISABLE_TIMER_RECORDING
		DeleteCriticalSection(&imageCS);
#ifndef USE_OPENCV_HIGHGUI_CPP_API
		if (videorecordfile) cvReleaseVideoWriter(&videorecordfile);
#else
		if (videorecordfile.isOpened()) videorecordfile.release();
#endif // !USE_OPENCV_HIGHGUI_CPP_API
		if (bForceSoftwareResize) cvReleaseImage(&resizedframe);
		cvReleaseImage(&image);
#ifndef USE_OPENCV_HIGHGUI_CPP_API
		cvReleaseCapture(&webcam);
#else
		webcam->release();
		delete webcam;
		delete pframemat;
		encodeparams.clear();
#endif // !USE_OPENCV_HIGHGUI_CPP_API
		return EXIT_FAILURE;
	}

	mSleep(1000);

	if (
		(bUDP&&(LaunchUDPSrv(srvport, handlecli, NULL) != EXIT_SUCCESS))||
		((!bUDP)&&(LaunchMultiCliTCPSrv(srvport, handlecli, NULL) != EXIT_SUCCESS))
		)
	{
		printf("Error launching the server.\n");
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		bStop = TRUE;
		mSleep(15000);
		WaitForThread(handlecamThreadId);
		DeleteCriticalSection(&sharedbufCS);
		free(sharedbuf);
#ifndef DISABLE_TIMER_RECORDING
		DeleteTimer(&timer, FALSE);
#endif // !DISABLE_TIMER_RECORDING
		DeleteCriticalSection(&imageCS);
#ifndef USE_OPENCV_HIGHGUI_CPP_API
		if (videorecordfile) cvReleaseVideoWriter(&videorecordfile);
#else
		if (videorecordfile.isOpened()) videorecordfile.release();
#endif // !USE_OPENCV_HIGHGUI_CPP_API
		if (bForceSoftwareResize) cvReleaseImage(&resizedframe);
		cvReleaseImage(&image);
#ifndef USE_OPENCV_HIGHGUI_CPP_API
		cvReleaseCapture(&webcam);
#else
		webcam->release();
		delete webcam;
		delete pframemat;
		encodeparams.clear();
#endif // !USE_OPENCV_HIGHGUI_CPP_API
		return EXIT_FAILURE;
	}

	bStop = TRUE;
	mSleep(15000);
	WaitForThread(handlecamThreadId);
	DeleteCriticalSection(&sharedbufCS);
	free(sharedbuf);
#ifndef DISABLE_TIMER_RECORDING
	DeleteTimer(&timer, FALSE);
#endif // !DISABLE_TIMER_RECORDING
	DeleteCriticalSection(&imageCS);
#ifndef USE_OPENCV_HIGHGUI_CPP_API
	if (videorecordfile) cvReleaseVideoWriter(&videorecordfile);
#else
	if (videorecordfile.isOpened()) videorecordfile.release();
#endif // !USE_OPENCV_HIGHGUI_CPP_API
	if (bForceSoftwareResize) cvReleaseImage(&resizedframe);
	cvReleaseImage(&image);
#ifndef USE_OPENCV_HIGHGUI_CPP_API
		cvReleaseCapture(&webcam);
#else
		webcam->release();
		delete webcam;
		delete pframemat;
		encodeparams.clear();
#endif // !USE_OPENCV_HIGHGUI_CPP_API

	return EXIT_SUCCESS;
}
