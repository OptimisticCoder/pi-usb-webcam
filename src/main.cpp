#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include <stdio.h>
#include <iostream>

using namespace cv;

int main( int argc, const char* argv[] )
{
	VideoCapture cap(CV_CAP_ANY);
	if(cap.isOpened())
	{
		printf( "Connected to webcam.\n" );
		while(cap.isOpened())
		{
			Mat frame;
			if(!cap.read(frame))
				break;

		}		

		printf("Press enter to exit.\n");
		std::cin.get();
	}
	else
		printf( "Couldn't connect to webcam.\n" );
}