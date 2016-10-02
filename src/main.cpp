#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include <stdio.h>
#include <iostream>
#include "webcam_service.h"

using namespace cv;

int main( int argc, const char* argv[] )
{
	webcam_service webcam;

	webcam.StartRecording();

	printf("Press enter to exit.\n");
	std::cin.get();

	webcam.StopRecording();

	return 0;
}