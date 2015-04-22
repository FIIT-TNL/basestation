#ifndef OCULUS_HPP
#define OCULUS_HPP

#include "drivrconfig.hpp"
#include "task.hpp"
#include "ovladanie.hpp"


//#define NOMINMAX
#include "stdafx.h"
#pragma warning(disable : 4996)
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>  
#include "opencv2/core/core.hpp"
#include "opencv2/flann/miniflann.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/photo/photo.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/ml/ml.hpp"
#include "opencv2/core/core_c.h"
#include "opencv2/highgui/highgui_c.h"

#include "SDL.h"
#define GLEW_STATIC
#include "GL/glew.h"
#define OVR_OS_WIN32
#include "OVR_CAPI_GL.h"
#include "Kernel/OVR_Math.h"
#include "SDL_syswm.h"

#include "framereader.hpp"
using namespace cv;

class Oculus : public Task {
private:
	DrivrConfig *cfg;
	Ovladanie *control;
	
	GLuint textureCV;
	int prvyKrat = 0;
	GLuint textures[2];
	int syncDone = 0;
	int teplota = 0, rychlost = 0, vlhkost = 0;
	char** dataVypis;
	double synFrame1, synFrame2;
	Mat recentFrame;
	Mat recentFrame2;

	RecentFrame recent1, recent2, testF;

	int testFrameNext = 0;
	std::thread t2;
	int sync1Done = 0, sync2Done = 0;

	FrameReader *fr1 = NULL;
	FrameReader *fr2 = NULL;

	// fns
	GLuint cvImage(RecentFrame texture_cv, int camera);
	Mat rotate(Mat src, double angle);
	int load_textures();
	int ifBlack(Mat framBlack);

protected:
	void doTask();

public:
	Oculus(DrivrConfig *cfg, Ovladanie *control);
	~Oculus();
};
#endif // OCULUS_HPP