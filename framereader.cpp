#include "framereader.hpp"

#include "opencv2/videoio/videoio_c.h"


FrameReader::FrameReader(VideoCapture *cap, RecentFrame *recentFrame)
	: cap(cap), recentFrame(recentFrame)
{}

void FrameReader::doTask()
{
	bool bSuccess;
	while (!this->isTerminateRequested()){
		try
		{
			bSuccess = cap->read(recentFrame->recentFrame);
			recentFrame->poradieFramu = cap->get(CV_CAP_PROP_POS_FRAMES);
		}
		catch (int e){}

	}
}