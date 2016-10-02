#ifndef __WEBCAM_SERVICE_H__
#define __WEBCAM_SERVICE_H__

#include <vector>
#include <mutex>
#include <boost/thread.hpp> 
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"

using namespace cv;
using namespace std;
using namespace boost;

class webcam_service
{
public:

	webcam_service();
	~webcam_service();

	bool StartRecording();
	bool StopRecording();

	Mat& GetLastImage();
	vector<uchar>* GetModifiedImage();
	void SetModifiedImage(Mat& image);
	bool IsRecording();
	int GetFPS();
	int GetDelay();

private:

	bool isRecording;
	bool isModifiedAvailable;
	int fps;
	int delay;
	VideoCapture capture;
	Mat lastImage;
	vector<uchar> modifiedImage;
	boost::thread* recordingThread;
	std::mutex lastImgMutex;
	std::mutex modifiedImgMutex;
	vector<int> params;

	void RecordingCore();
};

#endif
