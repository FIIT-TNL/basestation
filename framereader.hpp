#ifndef FRAMEREADER_HPP
#define FRAMEREADER_HPP

#include "task.hpp"
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

class FrameReader : public Task {
private:
	VideoCapture *cap;
	Mat *recentFrame;

protected:
	void doTask();

public:
	FrameReader(VideoCapture *cap, Mat *recentFrame);
};

#endif // FRAMEREADER_HPP