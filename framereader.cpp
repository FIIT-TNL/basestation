#include "framereader.hpp"


FrameReader::FrameReader(VideoCapture *cap, Mat *recentFrame)
	: cap(cap), recentFrame(recentFrame)
{}

void FrameReader::doTask()
{
	bool bSuccess;
	while (true){
		try
		{
			bSuccess = cap->read(*recentFrame);
		}
		catch (int e){}

	}
}