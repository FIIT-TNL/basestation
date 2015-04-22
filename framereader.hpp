#ifndef FRAMEREADER_HPP
#define FRAMEREADER_HPP

#include "task.hpp"
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

struct RecentFrame {
	Mat recentFrame;
	double poradieFramu;
};

class FrameReader : public Task {
private:
	VideoCapture *cap;
	RecentFrame *recentFrame;

protected:
	void doTask();

public:
	FrameReader(VideoCapture *cap, RecentFrame *recentFrame);
};

#endif // FRAMEREADER_HPP