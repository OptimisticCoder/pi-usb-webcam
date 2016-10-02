#include "webcam_service.h"

webcam_service::webcam_service()
{
	recordingThread = NULL;
	isRecording = false;
	isModifiedAvailable = false;
	params = { CV_IMWRITE_JPEG_QUALITY, 30 };
	fps = 15;
	delay = 1000 / fps;
}

webcam_service::~webcam_service()
{
	if (isRecording) {
		StopRecording();
	}

	if (capture.isOpened()) {
		capture.release();
	}

	if (recordingThread)
	{
		recordingThread->interrupt();
		delete recordingThread;
	}
}

bool webcam_service::StartRecording()
{
	capture.open(CV_CAP_ANY);

	if (!capture.isOpened()){
		printf("No camera available!");
		return false;
	}

	printf("starting recording...");

	//camera settings
	capture.set(CV_CAP_PROP_FPS, fps);
	//Possible resolutions : 1280x720, 640x480; 440x330
	capture.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

	printf("Camera settings: ");
	printf(std::string("FPS: ").append(std::to_string(capture.get(CV_CAP_PROP_FPS))).c_str());
	printf(std::string("Resolution: ").append(std::to_string(capture.get(CV_CAP_PROP_FRAME_WIDTH))).append("x").append(std::to_string(capture.get(CV_CAP_PROP_FRAME_HEIGHT))).c_str());
	printf(std::string("Codec: ").append(std::to_string(capture.get(CV_CAP_PROP_FOURCC))).c_str());
	printf(std::string("Format: ").append(std::to_string(capture.get(CV_CAP_PROP_FORMAT))).c_str());

	isRecording = true;
	recordingThread = new boost::thread(&webcam_service::RecordingCore, this);

	printf("started recording");

	return true;
}

bool webcam_service::StopRecording()
{
	printf("stopping recording...");

	if (recordingThread) {
		isRecording = false;
		printf("recording activity stop requested");
		recordingThread->join();
		printf("recording activity stopped successfully");
	}
	else {
		printf("recording activity is already stopped!");
	}

	printf("stopped recording");

	return true;
}

Mat& webcam_service::GetLastImage()
{
	std::lock_guard<std::mutex> lock(lastImgMutex);
	return lastImage;
}

vector<uchar>* webcam_service::GetModifiedImage()
{
	std::lock_guard<std::mutex> lock(modifiedImgMutex); //will be released after leaving scop
	vector<uchar> *tempImg = new vector<uchar>(modifiedImage.begin(), modifiedImage.end());
	return tempImg;
}

void webcam_service::SetModifiedImage(Mat& image)
{
	std::lock_guard<std::mutex> lock(modifiedImgMutex);
	// encode mat to jpg and copy it to content
	cv::imencode(".jpg", image, modifiedImage, params);
}

bool webcam_service::IsRecording()
{
	return capture.isOpened() && isRecording;
}

int webcam_service::GetFPS()
{
	return fps;
}

int webcam_service::GetDelay()
{
	return delay;
}

void webcam_service::RecordingCore() {
	Mat frame;

	//Stopwatch sw;
	boost::posix_time::microsec_clock clock;
	boost::posix_time::ptime time = clock.local_time();

	int newDelay = 0;

	while (isRecording) {
		if (!capture.isOpened()) {
			printf("Lost connection to webcam!");
			break;
		}

		//sw.restart();

		//Create image frames from capture
		capture >> frame;

		//clock.update();
		if (!frame.empty()) {
			{
				std::lock_guard<std::mutex> lock(lastImgMutex);
				lastImage = frame; //Clone image
				//logger.information("new image");
			}

			//sw.stop();
			//printf("Capture frame: %f ms\n", sw.elapsed() * 0.001);
			//sw.restart();

			//Notify();

			//sw.stop();
			//printf("Notifiy: %f ms\n", sw.elapsed() * 0.001);
		}
		else {
			printf("Captured empty webcam frame!");
		}

		boost::posix_time::ptime timeAfter = clock.local_time();
		boost::posix_time::time_duration duration = timeAfter - time;

		newDelay = delay - duration.ticks() * 0.001;

		if (newDelay > 0) {
			//webcam can only be queried after some time again
			//according to the FPS rate
			boost::this_thread::sleep(boost::posix_time::milliseconds(newDelay));
		}
	}

	isRecording = false;
}