#include "Globals.h"

int LoadConfig()
{
	FILE* file = NULL;
	char line[MAX_BUF_LEN];
	double d0 = 0;

	// Default values.
	camid = 0;
	memset(srvport, 0, sizeof(srvport));
	sprintf(srvport, "27254");
	videoimgwidth = 640; 
	videoimgheight = 480; 
	captureperiod = 100;
	timeout = 0;
	angle = 0*M_PI/180.0;
	scale = 1;
	bFlip = 0;
	bUDP = FALSE;
	pixcolorchgthreshold = 3; 
	timecompressiondividerthreshold = 4;
	fullimgperiod = 1000; 
	encodequality = 95;
	memset(encodetype, 0, sizeof(encodetype));
	sprintf(encodetype, ".JPEG");
	method = 0;
	bDisableVideoRecording = FALSE;

#ifdef __ANDROID__
	file = fopen("/storage/sdcard0/download/RemoteWebcamMultiSrv.txt", "r");
#else
	file = fopen("RemoteWebcamMultiSrv.txt", "r");
#endif // __ANDROID__
	if (file != NULL)
	{
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%d", &camid) != 1) printf("Invalid configuration file.\n");
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
		if (sscanf(line, "%lf", &d0) != 1) printf("Invalid configuration file.\n");
		angle = d0*M_PI/180.0;
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &scale) != 1) printf("Invalid configuration file.\n");
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

		//if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		//if (sscanf(line, "%lf", &val) != 1) printf("Invalid configuration file.\n");

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
		captureperiod = 100;
	}
	if (timecompressiondividerthreshold < 1)
	{
		printf("Invalid parameter : timecompressiondividerthreshold.\n");
		timecompressiondividerthreshold = 4;
	}

	return EXIT_SUCCESS;
}

int MovementDetection(IplImage* previmg, IplImage* img, IplImage* detectimg, char* buf, int* pnbBytes)
{
	unsigned int i = 0, val = 0, count = 0, indexb = 0, indexg = 0, indexr = 0;
	int r = 0, g = 0, b = 0;
	unsigned char* prevdata = (unsigned char*)previmg->imageData;
	unsigned char* data = (unsigned char*)img->imageData;
	unsigned char* detectdata = (unsigned char*)detectimg->imageData;
	CvMat* mat = NULL;

	// First compute the static compression to be able to compare with the time compression.
	mat = cvEncodeImage(encodetype, img, encodeparams);
	if (mat == NULL)
	{
		printf("cvMat() failed.\n");
		return EXIT_FAILURE;
	}

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
		if ((abs(b)+abs(g)+abs(r)) > 3*pixcolorchgthreshold)
		{
			// Blue index value of the pixel.
			memcpy(buf+count, (char*)&i, sizeof(unsigned int));
			count += sizeof(unsigned int);
			// BGR values.
			memcpy(buf+count, img->imageData+i, 3*sizeof(char));
			count += 3*sizeof(char);
			// Show in green pixels that significantly changed.
			detectdata[indexb] = 0;
			detectdata[indexg] = 255;
			detectdata[indexr] = 0;
		}

		// A full image will be sent if the time compression is worse than the static one.
		if (count > (unsigned int)(mat->rows*mat->cols)) break;
	}	

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
	else
	{
		val = count; // Number of bytes sent.
		memcpy(buf, (char*)&val, sizeof(unsigned int));
		// Image dimensions.
		memcpy(buf+sizeof(unsigned int), (char*)&img->width, sizeof(unsigned int));
		memcpy(buf+2*sizeof(unsigned int), (char*)&img->height, sizeof(unsigned int));
	}

	*pnbBytes = (int)count;

	cvReleaseMat(&mat);

	return EXIT_SUCCESS;
}

int MovementDetection2(IplImage* previmg, IplImage* img, IplImage* detectimg, char* buf, int* pnbBytes)
{
	unsigned int i = 0, val = 0, count = 0, indexb = 0, indexg = 0, indexr = 0;
	int r = 0, g = 0, b = 0;
	unsigned char* prevdata = (unsigned char*)previmg->imageData;
	unsigned char* data = (unsigned char*)img->imageData;
	unsigned char* detectdata = (unsigned char*)detectimg->imageData;
	CvMat* mat = NULL;

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
		if ((abs(b)+abs(g)+abs(r)) > 3*pixcolorchgthreshold)
		{
			// Blue index value of the pixel.
			memcpy(buf+count, (char*)&i, sizeof(unsigned int));
			count += sizeof(unsigned int);
			// BGR values.
			memcpy(buf+count, img->imageData+i, 3*sizeof(char));
			count += 3*sizeof(char);
			// Show in green pixels that significantly changed.
			detectdata[indexb] = 0;
			detectdata[indexg] = 255;
			detectdata[indexr] = 0;
		}

		// A full image will be sent if the time compression is bad.
		if (count > (unsigned int)(img->imageSize/timecompressiondividerthreshold)) break;
	}	

	if (count > (unsigned int)(img->imageSize/timecompressiondividerthreshold)) 
	{
		val = UINT_MAX; // Special number to indicate a full image.
		memcpy(buf, (char*)&val, sizeof(unsigned int));
		mat = cvEncodeImage(encodetype, img, encodeparams);
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
							memset(httpbuf, 0, sizeof(httpbuf));
							sprintf(httpbuf, 
								"HTTP/1.1 200 OK\r\n"
								"Server: RemoteWebcamMultiSrv\r\n"
								//"Connection: close\r\n"
								//"Max-Age: 0\r\n"
								//"Expires: 0\r\n"
								//"Cache-Control: no-cache, private, no-store, must-revalidate, pre-check = 0, post-check = 0, max-age = 0\r\n"
								//"Pragma: no-cache\r\n"
								"Content-Type: multipart/x-mixed-replace; boundary=--boundary\r\n"
								//"Media-type: image/jpeg\r\n"
								"\r\n");
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
			mSleep(captureperiod);
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
#endif // DISABLE_TIMER_RECORDING
	if (videorecordfile) cvReleaseVideoWriter(&videorecordfile);
	videorecordfile = NULL;

	bStop = TRUE;
	StopChronoQuick(&chrono);
#ifndef DISABLE_GUI_REMOTEWEBCAMSRV
	cvDestroyWindow("Detection");
#endif // DISABLE_GUI_REMOTEWEBCAMSRV
	cvReleaseImage(&detectimage);
	cvReleaseImage(&previmage);
	free(databuf);
	mSleep(15000);
	exit(EXIT_FAILURE);
}

THREAD_PROC_RETURN_VALUE handlecam(void* pParam)
{
	double m[6]; // For rotation...
	CvMat M = cvMat(2, 3, CV_64F, m); // For rotation...
	int nbBytes = 0;
	char szText[MAX_BUF_LEN];
	unsigned int val = 0;
	CvMat* mat = NULL;
	BOOL bForceSendFullImg = TRUE;
	char httpbuf[2048];
#ifndef DISABLE_GUI_REMOTEWEBCAMMULTISRV
	int c = 0;
#endif // DISABLE_GUI_REMOTEWEBCAMMULTISRV

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
	detectimage = cvCloneImage(image);
	if (!detectimage)	
	{
		printf("Error copying an image.\n");
		cvReleaseImage(&previmage);
		free(databuf);
		return 0;
	}

#ifndef DISABLE_GUI_REMOTEWEBCAMMULTISRV
	cvNamedWindow("Detection", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("Detection", 0, 0);
	cvShowImage("Detection", image);
	cvWaitKey(10);
#else
	mSleep(10);
#endif // DISABLE_GUI_REMOTEWEBCAMMULTISRV

	StartChrono(&chrono);

	for (;;)
	{
		frame = cvQueryFrame(webcam);
		if (!frame)
		{
			printf("Error getting an image from the webcam.\n");
			CleanUp();
			return 0;
		}
		EnterCriticalSection(&imageCS);
		if ((angle == 0)&&(scale == 1))
		{
			if (bFlip) cvFlip(frame, image, 1); else cvCopy(frame, image, 0);
		}
		else
		{
			//// Create a map_matrix, where the left 2x2 matrix is the transform and the right 2x1 is the dimensions.
			//m[0] = cos(pVideo->angle);
			//m[1] = sin(pVideo->angle);
			//m[3] = -m[1];
			//m[4] = m[0];
			//m[2] = pVideo->frame->width*0.5;  
			//m[5] = pVideo->frame->height*0.5;  
			//cvGetQuadrangleSubPix(pVideo->frame, img, &M);

			cvWarpAffine(frame, image, 
				cv2DRotationMatrix(cvPoint2D32f(frame->width*0.5,frame->height*0.5), -angle*180.0/M_PI, scale, &M), 
				CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS, cvScalarAll(0));

			if (bFlip) cvFlip(image, NULL, 1);
		}
		LeaveCriticalSection(&imageCS);

#ifdef DISABLE_TIMER_RECORDING
		if ((videorecordfile != NULL)&&(image != NULL))
		{
			if (!cvWriteFrame(videorecordfile, image)) 
			{
				printf("Error writing to a video file.\n");
			}
		}
#endif // DISABLE_TIMER_RECORDING

		cvCopy(image, detectimage, NULL);

		if (
			((method == 0)||(method == 1))&&
			((GetTimeElapsedChronoQuick(&chrono) > 0.001*fullimgperiod)||bForceSendFullImg)
			) 
		{
			StopChronoQuick(&chrono);
			bForceSendFullImg = FALSE;
			val = UINT_MAX; // Special number to indicate a full image with static compression.
			memcpy(databuf, (char*)&val, sizeof(unsigned int));
			mat = cvEncodeImage(encodetype, image, encodeparams);
			if (mat == NULL)
			{
				printf("cvMat() failed.\n");
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
			sprintf(szText, "Periodic complete image : %d bytes", nbBytes);
			cvPutText(detectimage, szText, cvPoint(0,48), &font, CV_RGB(255,0,0));
			StartChrono(&chrono);
		}
		else
		{
			switch (method)
			{
			case 0:
				if (MovementDetection(previmage, image, detectimage, databuf, &nbBytes) != EXIT_SUCCESS)
				{
					CleanUp();
					return 0;
				}
				break;
			case 1:
				if (MovementDetection2(previmage, image, detectimage, databuf, &nbBytes) != EXIT_SUCCESS)
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
				mat = cvEncodeImage(encodetype, image, encodeparams);
				if (mat == NULL)
				{
					printf("cvMat() failed.\n");
					CleanUp();
					return 0;
				}
				memset(httpbuf, 0, sizeof(httpbuf));
				sprintf(httpbuf, 
					"--boundary\r\n"
					"Content-Type: image/jpeg\r\n"
					"Content-Length: %d\r\n"
					"\r\n", mat->rows*mat->cols);
				memcpy(databuf+nbBytes, httpbuf, strlen(httpbuf));
				nbBytes += strlen(httpbuf);
				// Full image data (with static compression).
				memcpy(databuf+nbBytes, mat->data.ptr, (size_t)(mat->rows*mat->cols));
				nbBytes += (mat->rows*mat->cols);
				cvReleaseMat(&mat);
				break;
			default:
				printf("Invalid parameter.\n");
				CleanUp();
				return 0;
				break;
			}
			sprintf(szText, "Data transmitted : %d bytes", nbBytes);
			cvPutText(detectimage, szText, cvPoint(0,32), &font, CV_RGB(255,0,0));
		}

		EnterCriticalSection(&sharedbufCS);
		sharedbuflen = nbBytes;
		memcpy(sharedbuf, databuf, nbBytes);
		LeaveCriticalSection(&sharedbufCS);

		sprintf(szText, "Uncompressed size : %d bytes", image->imageSize);
		cvPutText(detectimage, szText, cvPoint(0,16), &font, CV_RGB(255,0,0));
		cvCopy(image, previmage, NULL);

#ifndef DISABLE_GUI_REMOTEWEBCAMMULTISRV
		cvShowImage("Detection", detectimage);

		c = cvWaitKey(captureperiod);
		if ((char)c == 27) break;
#else
		mSleep(captureperiod);
#endif // DISABLE_GUI_REMOTEWEBCAMMULTISRV
		if (bStop) break;
	}

	// Ensure the video recording is correctly stopped.
#ifndef DISABLE_TIMER_RECORDING
	DeleteTimer(&timer, FALSE);
#endif // DISABLE_TIMER_RECORDING
	if (videorecordfile) cvReleaseVideoWriter(&videorecordfile);
	videorecordfile = NULL;

	bStop = TRUE;
	StopChronoQuick(&chrono);
#ifndef DISABLE_GUI_REMOTEWEBCAMSRV
	cvDestroyWindow("Detection");
#endif // DISABLE_GUI_REMOTEWEBCAMSRV
	cvReleaseImage(&detectimage);
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
	if ((videorecordfile != NULL)&&(image != NULL))
	{
		if (!cvWriteFrame(videorecordfile, image)) 
		{
			printf("Error writing to a video file.\n");
		}
	}
	LeaveCriticalSection(&imageCS);
}
#endif // DISABLE_TIMER_RECORDING

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
	THREAD_IDENTIFIER handlecamThreadId;
	char videorecordfilename[MAX_BUF_LEN];
	int i = 0;

#ifndef __ANDROID__
	INIT_DEBUG;

	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);
#endif // __ANDROID__

	LoadConfig();

	if (strncmp(encodetype, ".JPEG", min(strlen(encodetype), strlen(".JPEG"))+1) == 0)
	{
		encodeparams[0] = CV_IMWRITE_JPEG_QUALITY;
		encodeparams[1] = encodequality;
	}
	else if (strncmp(encodetype, ".PNG", min(strlen(encodetype), strlen(".PNG"))+1) == 0)
	{
		encodeparams[0] = CV_IMWRITE_PNG_COMPRESSION;
		encodeparams[1] = encodequality;
	}
	else
	{
		encodeparams[0] = CV_IMWRITE_PXM_BINARY;
		encodeparams[1] = encodequality;
	}

	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5, 0, 1, 8);

	//webcam = cvCreateFileCapture("SAUCISSE wall ball - VIDEO2605.mp4");
	//webcam = cvCreateFileCapture("test3.wmv");
	webcam = cvCreateCameraCapture(camid);
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

	image = cvCloneImage(frame);
	if (!image)	
	{
		printf("Error copying an image.\n");
		cvReleaseCapture(&webcam);
		return EXIT_FAILURE;
	}

	if ((angle == 0)&&(scale == 1))
	{
		if (bFlip) cvFlip(frame, image, 1); else cvCopy(frame, image, 0);
	}
	else
	{
		//// Create a map_matrix, where the left 2x2 matrix is the transform and the right 2x1 is the dimensions.
		//m[0] = cos(pVideo->angle);
		//m[1] = sin(pVideo->angle);
		//m[3] = -m[1];
		//m[4] = m[0];
		//m[2] = pVideo->frame->width*0.5;  
		//m[5] = pVideo->frame->height*0.5;  
		//cvGetQuadrangleSubPix(pVideo->frame, img, &M);

		cvWarpAffine(frame, image, 
			cv2DRotationMatrix(cvPoint2D32f(frame->width*0.5,frame->height*0.5), -angle*180.0/M_PI, scale, &M), 
			CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS, cvScalarAll(0));

		if (bFlip) cvFlip(image, NULL, 1);
	}

	if (!bDisableVideoRecording)
	{
#ifdef __ANDROID__
		sprintf(videorecordfilename, "/storage/sdcard0/download/video_%.64s.avi", strtime_fns());
		videorecordfile = cvCreateVideoWriter(videorecordfilename, 
			CV_FOURCC('M','J','P','G'), // Might work starting with OpenCV 3 for Android?
			//CV_FOURCC('D','I','V','X'), 
			//CV_FOURCC('I', 'Y', 'U', 'V'), 
#else
		sprintf(videorecordfilename, "video_%.64s.wmv", strtime_fns());
		videorecordfile = cvCreateVideoWriter(videorecordfilename, 
			CV_FOURCC('W','M','V','2'), 
#endif // __ANDROID__
#ifdef DISABLE_TIMER_RECORDING
			1000.0/(double)(captureperiod+20), // 20 ms is an approximation of the extra time spent for computations...
#else
			1000.0/(double)captureperiod, 
#endif // DISABLE_TIMER_RECORDING
			cvSize(image->width,image->height), 
			1);
		if (!videorecordfile)
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
#endif // DISABLE_TIMER_RECORDING

	databuflen = image->imageSize+3*sizeof(unsigned int);
	sharedbuflen = 0;
	sharedbuf = (char*)calloc(databuflen, sizeof(char));
	if (!sharedbuf)	
	{
		printf("calloc() failed.\n");
#ifndef DISABLE_TIMER_RECORDING
		DeleteTimer(&timer, FALSE);
#endif // DISABLE_TIMER_RECORDING
		DeleteCriticalSection(&imageCS);
		if (videorecordfile) cvReleaseVideoWriter(&videorecordfile);
		cvReleaseImage(&image);
		cvReleaseCapture(&webcam);
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
#endif // DISABLE_TIMER_RECORDING
		DeleteCriticalSection(&imageCS);
		if (videorecordfile) cvReleaseVideoWriter(&videorecordfile);
		cvReleaseImage(&image);
		cvReleaseCapture(&webcam);
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
#endif // DISABLE_TIMER_RECORDING
		DeleteCriticalSection(&imageCS);
		if (videorecordfile) cvReleaseVideoWriter(&videorecordfile);
		cvReleaseImage(&image);
		cvReleaseCapture(&webcam);
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
#endif // DISABLE_TIMER_RECORDING
		DeleteCriticalSection(&imageCS);
		if (videorecordfile) cvReleaseVideoWriter(&videorecordfile);
		cvReleaseImage(&image);
		cvReleaseCapture(&webcam);
		return EXIT_FAILURE;
	}

	bStop = TRUE;
	mSleep(15000);
	WaitForThread(handlecamThreadId);
	DeleteCriticalSection(&sharedbufCS);
	free(sharedbuf);
#ifndef DISABLE_TIMER_RECORDING
	DeleteTimer(&timer, FALSE);
#endif // DISABLE_TIMER_RECORDING
	DeleteCriticalSection(&imageCS);
	if (videorecordfile) cvReleaseVideoWriter(&videorecordfile);
	cvReleaseImage(&image);
	cvReleaseCapture(&webcam);

	return EXIT_SUCCESS;
}
