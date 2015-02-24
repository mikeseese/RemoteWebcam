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
#pragma comment(lib, "WS2_32.lib");

int main(int argc, char **argv)
{
    CvCapture *capture = 0;
    IplImage  *frame = new IplImage;
    int       key = 0;
	SOCKET kServer = NULL;
	char buf[256];

	kServer = startSocket(kServer);
	if(kServer == 0)
	{
		system("pause");
		return 0;
	}
	socketConnect(kServer, "127.0.0.1", 7777); //connect to client

    /* initialize camera */
    capture = cvCaptureFromCAM(0);

    /* always check */
    if (!capture)
	{
        fprintf(stderr, "Cannot open initialize webcam!\n");
        return 1;
    }

    while(1)
	{
        frame = cvQueryFrame(capture); //get a frame
        if(!frame) break; //always check

		//send the image data
		emptyCharArray(buf);
		itoa(frame->imageSize, buf, 10);
		send(kServer, buf, 256, 0); //imageSize - int
		send(kServer, frame->imageData, frame->imageSize, 0); //imageData - char*
    }

    /* free memory */
    cvReleaseCapture(&capture);

	system("pause");

    return 0;
}