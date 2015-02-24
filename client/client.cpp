/**
 * Display video from webcam
 *
 * Author  Nash
 * License GPL
 * Website http://nashruddin.com
 */
 
#include "stdafx.h"
#include <winsock2.h>
#include <cv.h>
#include <highgui.h>
#include <includes.h>
#include <winsockfunctions.h>

#pragma comment(lib, "cv120.lib")
#pragma comment(lib, "cxcore120.lib")
#pragma comment(lib, "highgui120.lib")
#pragma comment(lib, "WS2_32.lib")

int main( int argc, char **argv )
{
	//IplImage realFrame;
    IplImage  *frame = cvCreateImage(cvSize(320, 480), IPL_DEPTH_8U, 1);
    int       key = 0;
	long int i;
	long int j;
	long int k;
	long int		bytesRecv = 0;
	long int		totalBytes = 0;
	int imageSize;
	char buf[230400];
	char data[230400];
	SOCKET kClient = NULL;

	kClient = startSocket(kClient);
	if(kClient == 0)
	{
		system("pause");
		return 0;
	}

	socketListen(kClient, 7777);
	kClient = accept(kClient, 0, 0);

    /* create a window for the video */
    cvNamedWindow("result", CV_WINDOW_AUTOSIZE);

    while(key != 'q')
	{
		//recieve the data for the image structure
		emptyCharArray(buf);
		recv(kClient, buf, 256, 0); //imageSize - int
		terminateCharArray(buf);
		imageSize = atoi(buf);
		totalBytes = 0;
		while(totalBytes < frame->imageSize)
		{
			emptyCharArray(buf);
			bytesRecv = recv(kClient, buf, frame->imageSize, 0); //imageData - char*
			CharArrayCopySelection(buf, data, 0, bytesRecv, totalBytes);
			totalBytes += bytesRecv;
		}
		//terminateCharArray(data);
		for (i = 0, k = 0; i < frame->height; i++)
		{
			for (j = 0; j < frame->width; j++)
			{
				((uchar*)(frame->imageData + i * frame->widthStep))[j] = data[k++];
			}
		}
		/*emptyCharArray(data);
		totalBytes = 0;
		bytesRecv = 0;
		while(totalBytes != frame->imageSize)
		{
			emptyCharArray(buf);
			bytesRecv = recv(kClient, buf, frame->imageSize, 0); //imageDataOrigin - char*
			CharArrayCopySelection(buf, data, 0, bytesRecv, totalBytes);
			totalBytes = totalBytes + bytesRecv;
		}*/
		//terminateCharArray(data);
		frame->imageDataOrigin = data;
		emptyCharArray(data);
		emptyCharArray(buf);
		recv(kClient, buf, 256, 0); //nChannels - int
		terminateCharArray(buf);
		frame->nChannels = atoi(buf);
		emptyCharArray(buf);
		recv(kClient, buf, 256, 0); //nSize - int
		terminateCharArray(buf);
		frame->nSize = atoi(buf);
		emptyCharArray(buf);
		recv(kClient, buf, 256, 0); //origin - int
		terminateCharArray(buf);
		frame->origin = atoi(buf);
		emptyCharArray(buf);
		recv(kClient, buf, 256, 0); //width - int
		terminateCharArray(buf);
		frame->width = atoi(buf);
		emptyCharArray(buf);
		recv(kClient, buf, 256, 0); //widthStep - int
		terminateCharArray(buf);
		frame->widthStep = atoi(buf);

        /* always check */
        if(!frame) break;
        
        /* display current frame */
        cvShowImage("result", frame);

        /* exit if user press 'q' */
        key = cvWaitKey(1);
    }

    /* free memory */
    cvDestroyWindow( "result" );

	system("pause");

    return 0;
}