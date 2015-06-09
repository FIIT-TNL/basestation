#include "oculus.hpp"

// opengl.cpp : Defines the entry point for the console application.
//
////#define NOMINMAX
//#include "stdafx.h"
//#pragma warning(disable : 4996)
//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <iostream>
//#include <opencv2/imgproc/imgproc.hpp>  
//#include "opencv2/core/core.hpp"
//#include "opencv2/flann/miniflann.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/photo/photo.hpp"
//#include "opencv2/video/video.hpp"
//#include "opencv2/features2d/features2d.hpp"
//#include "opencv2/objdetect/objdetect.hpp"
//#include "opencv2/calib3d/calib3d.hpp"
//#include "opencv2/ml/ml.hpp"
//#include "opencv2/core/core_c.h"
//#include "opencv2/highgui/highgui_c.h"
//
//#include "SDL.h"
//#define GLEW_STATIC
//#include "GL/glew.h"
//#define OVR_OS_WIN32
//#include "OVR_CAPI_GL.h"
//#include "Kernel/OVR_Math.h"
//#include "SDL_syswm.h"
//#include "ovladanie.hpp"

//
#include "framereader.hpp"

#include <OVR_Version.h>

#include <iostream>
#include <fstream>
#include <string>
using namespace OVR;
using namespace cv;


// Turbo hack for opening video in thread (really)
class StreamOpener : public Task {
private:
	VideoCapture *cap;
	bool is_int = 0;
	int int_stream = 0;
	String str_stream = "";

protected:
	void doTask() {
		if (is_int) {
			std::cout << "tukabel str" << cap << int_stream << std::endl;
			*cap = VideoCapture(int_stream);
		}
		else {
			*cap = VideoCapture(str_stream);
			std::cout << "tukabel str" << cap << str_stream << std::endl;
			std::cout << "tukabel str" << cap << std::endl;
		}
		if (!cap->isOpened()) {
			std::cout << "Cannot open the video stream" << cap << std::endl;

		}
		std::cout << "strea otvoreny" << cap << std::endl;
	}

public:
	StreamOpener() {}
	StreamOpener(VideoCapture *cap, int stream) : cap(cap), is_int(1), int_stream(stream) {}
	StreamOpener(VideoCapture *cap, String stream) : cap(cap), is_int(0), str_stream(stream) {}
};


//GLuint textureCV;
//int prvyKrat = 0;
//
//int ifBlack(Mat framBlack);
//Mat rotate(Mat src, double angle);
//int load_textures();
//GLuint textures[2];
//int syncDone = 0;
//int teplota = 0, rychlost = 0, vlhkost = 0;
//char** dataVypis;
//double synFrame1, synFrame2;
//
//Mat recentFrame;
//Mat recentFrame2;
//
//struct recFrame{
//  Mat recentFrame;
//  double poradieFramu;
//} recent1, recent2,testF;
//
//int testFrameNext = 0;
//std::thread t2;
//int sync1Done = 0, sync2Done = 0;
//GLuint cvImage(recFrame texture_cv, int camera);

/*
void task1(VideoCapture cap1 )						//Nacitava framy z kamery1 a uklada do recentFrame
{
	bool bSuccess;
	while(true){
		try
		{
			bSuccess = cap1.read(recent1.recentFrame);
			recent1.poradieFramu = cap1.get(CV_CAP_PROP_POS_FRAMES );
		}catch(int e){}
		
	}
}

void task2(VideoCapture cap2 )						//Nacitava framy z kamery2 a uklada do recentFrame2
{
	bool bSuccess;
	while(true){
		
		try
		{
			bSuccess = cap2.read(recent2.recentFrame);
			recent2.poradieFramu = cap2.get(CV_CAP_PROP_POS_FRAMES );
		}catch(int e){}
	
	}
}

void task3Ovaldanie()								//Spusti funkciu ovladanief(), ktora riesi komunikaciu s vozidlom
{
		ovladanief();
}
*/

Oculus::Oculus(DrivrConfig *cfg, Ovladanie *control)
	: cfg(cfg), control(control)
{}

Oculus::~Oculus()
{
	if (fr1) {
		fr1->terminate();
		fr1->join();
		delete fr1;
	}
	if (fr2) {
		fr2->terminate();
		fr2->join();
		delete fr2;
	}
}

void Oculus::doTask()
{
		std::cout << "-- Oculus starting" << std::endl;

        SDL_Init(SDL_INIT_VIDEO);											//Oculus magic
 
        int x = SDL_WINDOWPOS_CENTERED;
        int y = SDL_WINDOWPOS_CENTERED;
        Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
 
        bool debug = false;
 
        ovr_Initialize();
 
        ovrHmd hmd = ovrHmd_Create(0);
 
        if (hmd == NULL)
        {
			hmd = ovrHmd_CreateDebug(ovrHmd_DK1);
 
			debug = true;
        }
 
        if (debug == false)
        {
			x = hmd->WindowsPos.x;
            y = hmd->WindowsPos.y;
            flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
        }
 
        int w = hmd->Resolution.w;
        int h = hmd->Resolution.h;
 
        SDL_Window *window = SDL_CreateWindow("DriVR 0.7", x, y, w, h, flags);
 
        SDL_GLContext context = SDL_GL_CreateContext(window);
 
        glewExperimental = GL_TRUE;
 
        glewInit();
 
        Sizei recommendedTex0Size = ovrHmd_GetFovTextureSize(hmd, ovrEye_Left, hmd->DefaultEyeFov[0], 1.0f);
        Sizei recommendedTex1Size = ovrHmd_GetFovTextureSize(hmd, ovrEye_Right, hmd->DefaultEyeFov[1], 1.0f);
        Sizei renderTargetSize;
        renderTargetSize.w = recommendedTex0Size.w + recommendedTex1Size.w;
        renderTargetSize.h = max(recommendedTex0Size.h, recommendedTex1Size.h);
 
        GLuint frameBuffer;
        glGenFramebuffers(1, &frameBuffer);
 
        GLuint texture;
        glGenTextures(1, &texture);
 
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, renderTargetSize.w, renderTargetSize.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);
 
        GLuint renderBuffer;
        glGenRenderbuffers(1, &renderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, renderTargetSize.w, renderTargetSize.h);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);
 
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
			glDeleteFramebuffers(1, &frameBuffer);
            glDeleteTextures(1, &texture);
            glDeleteRenderbuffers(1, &renderBuffer);
 
            SDL_GL_DeleteContext(context);
 
            SDL_DestroyWindow(window);
 
            ovrHmd_Destroy(hmd);
 
            ovr_Shutdown();
 
            SDL_Quit();
 
            //return 0;
			return;
        }
 
        ovrFovPort eyeFov[2] = { hmd->DefaultEyeFov[0], hmd->DefaultEyeFov[1] };
 
        ovrRecti eyeRenderViewport[2];
        eyeRenderViewport[0].Pos = Vector2i(0, 0);
        eyeRenderViewport[0].Size = Sizei(renderTargetSize.w / 2, renderTargetSize.h);
        eyeRenderViewport[1].Pos = Vector2i((renderTargetSize.w + 1) / 2, 0);
        eyeRenderViewport[1].Size = eyeRenderViewport[0].Size;
 
        ovrGLTexture eyeTexture[2];
        eyeTexture[0].OGL.Header.API = ovrRenderAPI_OpenGL;
        eyeTexture[0].OGL.Header.TextureSize = renderTargetSize;
        eyeTexture[0].OGL.Header.RenderViewport = eyeRenderViewport[0];
        eyeTexture[0].OGL.TexId = texture;
 
        eyeTexture[1] = eyeTexture[0];
        eyeTexture[1].OGL.Header.RenderViewport = eyeRenderViewport[1];

		ovrTexture eyeTex[2];

		eyeTex[0] = eyeTexture[0].Texture;
		eyeTex[1] = eyeTexture[1].Texture;
 
        SDL_SysWMinfo info;
 
        SDL_VERSION(&info.version);
 
        SDL_GetWindowWMInfo(window, &info);
 
        ovrGLConfig cfg;
        cfg.OGL.Header.API = ovrRenderAPI_OpenGL;
        cfg.OGL.Header.RTSize = Sizei(hmd->Resolution.w, hmd->Resolution.h);
        cfg.OGL.Header.Multisample = 1;
#if defined(OVR_OS_WIN32)
        if (!(hmd->HmdCaps & ovrHmdCap_ExtendDesktop))
                ovrHmd_AttachToWindow(hmd, info.info.win.window, NULL, NULL);
 
        cfg.OGL.Window = info.info.win.window;
        cfg.OGL.DC = NULL;
#elif defined(OVR_OS_LINUX)
        cfg.OGL.Disp = info.info.x11.display;
        cfg.OGL.Win = info.info.x11.window;
#endif
 
        ovrEyeRenderDesc eyeRenderDesc[2];
 
        ovrHmd_ConfigureRendering(hmd, &cfg.Config, ovrDistortionCap_Chromatic | ovrDistortionCap_Vignette | ovrDistortionCap_TimeWarp | ovrDistortionCap_Overdrive, eyeFov, eyeRenderDesc);
 
        ovrHmd_SetEnabledCaps(hmd, ovrHmdCap_LowPersistence | ovrHmdCap_DynamicPrediction);
 
        ovrHmd_ConfigureTracking(hmd, ovrTrackingCap_Orientation | ovrTrackingCap_MagYawCorrection | ovrTrackingCap_Position, 0);
		
       
		const GLchar *vertexShaderSource[] = {
                "#version 150\n"
                "uniform mat4 MVPMatrix;\n"
				"in vec2 texture_coord;"
				"out vec2 texture_coord_from_vshader;"
                "in vec3 position;\n"
                "void main()\n"
                "{\n"
                "    gl_Position = MVPMatrix * vec4(position, 1.0);\n"
					"texture_coord_from_vshader = texture_coord;"
                "}"
        };
 
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, vertexShaderSource, NULL);
        glCompileShader(vertexShader);
 
        const GLchar *fragmentShaderSource[] = {
                "#version 150\n"
                "out vec4 outputColor;\n"
				"in vec2 texture_coord_from_vshader;"
				"uniform sampler2D texture_sampler;"
                "void main()\n"
                "{\n"
                "    outputColor = texture(texture_sampler, texture_coord_from_vshader);"
                "}"
        };
 
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
 
        GLuint program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);
        glUseProgram(program);
 
        GLuint MVPMatrixLocation = glGetUniformLocation(program, "MVPMatrix");
        GLuint positionLocation = glGetAttribLocation(program, "position");
 
        GLuint vertexArray;
        glGenVertexArrays(1, &vertexArray);
        glBindVertexArray(vertexArray);
 
        GLfloat vertices[] = {
                -1.0, -1.0, -0.5,
				1.0, -1.0, -0.5,
				1.0, 1.0, -0.5,
				-1.0, 1.0, -0.5,
				
        };

		GLfloat texture_coord[] = {
			0.0, 0.0,
			1.0, 0.0,
			1.0, 1.0,
			0.0, 1.0,
		};

		GLuint indices[6] = {
			0, 1, 2,
			2, 3, 0
		};

		
        GLuint positionBuffer;
        glGenBuffers(1, &positionBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(texture_coord) , vertices, GL_STATIC_DRAW);    // + texture_coord
        // Transfer the vertex positions:
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

		// Transfer the texture coordinates:
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(texture_coord), texture_coord);
		glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(positionLocation);
 



		GLuint eab;
		glGenBuffers(1, &eab);
	
		// Transfer the data from indices to eab
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eab);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);



		//Citanie config suboru so streamami
	/*	std::string line;
		std::string streams[2];
		int streamPor = 0;
		std::ifstream myfile ("streams.cfg");
		if (myfile.is_open())
		{
			while ( std::getline (myfile,line) )
			{
					streams[streamPor] = line;
					streamPor++;
			}
			myfile.close();
		} else std::cout << "Unable to open file"; 
*/
			
		//Otvorenie 1. streamu
		VideoCapture cap;
		StreamOpener opener1;

		if (this->cfg->isStream1Int()) {
			opener1 = StreamOpener(&cap, this->cfg->getStream1Int());
		}
		else {
			opener1 = StreamOpener(&cap, this->cfg->getStream1());
		}
		opener1.start();
		/*
		if (this->cfg->isStream1Int()) {
			cap = VideoCapture(this->cfg->getStream1Int());
			std::cout << "tukabel 1 int" << std::endl;
		} else {
			cap = VideoCapture(this->cfg->getStream1());
			std::cout << "tukabel 1 str" << std::endl;
		}
		if (!cap.isOpened()) {
			std::cout << "Cannot open the video stream1" << std::endl;

		}
		std::cout << "tukabel 1 otvoreny" << std::endl;
		*/

		//Otvorenie 2. streamu
		VideoCapture cap2;
		StreamOpener opener2;

		if (this->cfg->isStream2Int()) {
			opener2 = StreamOpener(&cap2, this->cfg->getStream2Int());
		}
		else {
			opener2 = StreamOpener(&cap2, this->cfg->getStream2());
		}
		opener2.start();

		// yeah madafaka wait
		opener1.join();
		opener2.join();
		std::cout << "TUKABEL: otvorene" << std::endl;

		/*
		if (this->cfg->isStream2Int()) {
			cap2 = VideoCapture(this->cfg->getStream2Int());
		}
		else {
			cap2 = VideoCapture(this->cfg->getStream2());
			std::cout << "tukabel 2 str" << std::endl;
		}
		if (!cap2.isOpened()) {
			std::cout << "Cannot open the video stream2" << std::endl;

		}
		std::cout << "tukabel 2 otvoreny" << std::endl;
		*/
		
			//http://192.168.1.10:8080/video?dummy=video.mjpg
			
			//VideoCapture cap2("http://192.168.1.10:8080/video?dummy=video.mjpg");					//Otvorenie 2. streamu
			//VideoCapture cap2(0);
			

			// Old threads
			//std::thread t1(task1, cap);				//Start threadov a citanie stale novych framov z kamery
			//std::thread t2(task2, cap2);
			//std::thread t3(task3Ovaldanie);			//Start threadu, ktory riesi ovladanie vozidla a komunikaciu

			// Start frame reader threads 
			fr1 = new FrameReader(&cap, &recent1);
			fr1->start();
			fr2 = new FrameReader(&cap2, &recent2);
			fr2->start();
	

			std::cout << "TUKABEL: framer" << std::endl;
#ifdef _WIN32
			Sleep(300);
#endif // _WIN32
#ifdef __linux__
			usleep(300000);
#endif
			std::cout << "TUKABEL: delay" << std::endl;


		glGenTextures(2, textures);


		// Texture coord attribute
		GLint texture_coord_attribute = glGetAttribLocation(program, "texture_coord");
		glVertexAttribPointer(texture_coord_attribute, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)sizeof(vertices));
		glEnableVertexAttribArray(texture_coord_attribute);

	

        glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
        glClearDepth(1.0f);
 
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_DEPTH_TEST);
 
        bool running = true;

		std::cout << "TUKABEL: pred cyklom" << std::endl;
		while (running == true && !this->isTerminateRequested())					//Spustenie cyklu
        {
                SDL_Event event;
 
                while (SDL_PollEvent(&event))										//Health sprava
                {
                        switch (event.type)
                        {
                        case SDL_QUIT:
                                running = false;
                                break;
                        case SDL_KEYDOWN:
                                ovrHmd_DismissHSWDisplay(hmd);
 
                                switch (event.key.keysym.sym)
                                {
                                case SDLK_ESCAPE:
                                        running = false;
                                        break;
                                default:
                                        break;
                                }
                                break;
                        default:
                                break;
                        }
                }

				//Prva verzia nacitavania framov
				//Mat frame[2];
				//Mat frame2;
				////cap.retrieve(frame);
				//bool bSuccess = cap.read(frame[0]);
				//bool bSuccess2 = cap2.read(frame[1]);
				//if (bSuccess) //if not success, break loop
				//{
				//	cvImage(frame);
				//}
				//frame[0].release();
				//if (bSuccess2) //if not success, break loop
				//{
				//	textures[1] = cvImage(frame2);
				//}
				//frame[1].release();
				//glBindTexture(GL_TEXTURE_2D, textures[0]);
				//glActiveTexture(GL_TEXTURE1);
				// 
				

				//------------------
                ovrFrameTiming frameTiming = ovrHmd_BeginFrame(hmd, 0);
 
                glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
 
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
                ovrPosef eyeRenderPose[2];
 



                for (int eyeIndex = 0; eyeIndex < ovrEye_Count; eyeIndex++)							//Pre kazde oko
                {

					if (eyeIndex == 1)																//Pre oko 1 spusti funkciu cvImage pre frame z kamery1 a opacne
					{
						cvImage(recent2,2);		// na lavu obraz z kamery1
						//std::cout << recent2.poradieFramu << "\n";
						
						
						
					}else{
						cvImage(recent1,1);		// na lavu obraz z kamery1
						//std::cout << recent1.poradieFramu << "::";
						
					}

					ovrEyeType eye = hmd->EyeRenderOrder[eyeIndex];	

#if (OVR_MAJOR_VERSION == 0 && OVR_MINOR_VERSION == 4 && OVR_BUILD_VERSION == 2) //Oculus SDK 0.4.2
					eyeRenderPose[eye] = ovrHmd_GetEyePose(hmd, eye);		//Oculus SDK 0.4.2
#else
					eyeRenderPose[eye] = ovrHmd_GetHmdPosePerEye(hmd, eye);	//Oculus SDK >= 0.4.3
#endif 					
						
					ovrPosef eyeRenderPoseNula;											//Vytvorenie objektu ovrPosef a naplnenie orientacie, potrebne na to, aby obraz nestal na jednom mieste v priestore, ale bol pre ocami
					eyeRenderPoseNula.Orientation.w = 1;
					eyeRenderPoseNula.Orientation.x = 0;
					eyeRenderPoseNula.Orientation.z = 0;
					eyeRenderPoseNula.Orientation.y = 0;

#if (OVR_MAJOR_VERSION == 0 && OVR_MINOR_VERSION == 4 && OVR_BUILD_VERSION == 2) //Oculus SDK 0.4.2
					Matrix4f MVPMatrix = Matrix4f(ovrMatrix4f_Projection(eyeRenderDesc[eye].Fov, 0.01f, 10000.0f, true)) * Matrix4f::Translation(eyeRenderDesc[eye].ViewAdjust) * Matrix4f(Quatf(eyeRenderPoseNula.Orientation).Inverted());
#else //Oculus SDK >= 0.4.3
					Matrix4f MVPMatrix = Matrix4f(ovrMatrix4f_Projection(eyeRenderDesc[eye].Fov, 0.01f, 10000.0f, true)) * Matrix4f::Translation(eyeRenderDesc[eye].HmdToEyeViewOffset) * Matrix4f(Quatf(eyeRenderPoseNula.Orientation).Inverted());
#endif 

					glUniformMatrix4fv(MVPMatrixLocation, 1, GL_FALSE, &MVPMatrix.Transposed().M[0][0]);
 					
                    glViewport(eyeRenderViewport[eye].Pos.x, eyeRenderViewport[eye].Pos.y, eyeRenderViewport[eye].Size.w, eyeRenderViewport[eye].Size.h);
							
					glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);		//Vykresli texturu
                    
                }
 
                glBindVertexArray(0);
 
                ovrHmd_EndFrame(hmd, eyeRenderPose, eyeTex);
 
                glBindVertexArray(vertexArray);
        }
 
        glDeleteVertexArrays(1, &vertexArray);
        glDeleteBuffers(1, &positionBuffer);
 
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteProgram(program);
 
        glDeleteFramebuffers(1, &frameBuffer);
        glDeleteTextures(1, &texture);
        glDeleteRenderbuffers(1, &renderBuffer);
 
        SDL_GL_DeleteContext(context);
 
        SDL_DestroyWindow(window);
 
        ovrHmd_Destroy(hmd);
 
        ovr_Shutdown();
 
        SDL_Quit();
 
		//t1.join();
		//t2.join();
		//t3.join();

        //return 0;
		return;
}




GLuint Oculus::cvImage(RecentFrame texture_cv, int camera){     //Funkcia na konverziu OpenCV Mat na OpenGL texturu a pridanie textu
	cv::Mat flipped;
	teplota++;
	rychlost++;
	vlhkost++;
	char teplotaa[250] = "";							//Docasne vztvaranie random textu
	sprintf(teplotaa, "Teplota %d", teplota/50);
	char vlkosta[250] = "";
	sprintf(vlkosta, "Vlhkost %d", rychlost/80);
	char rychlosta[5000] = "";
	int dlzka = recent1.recentFrame.rows;
	int sirka = recent1.recentFrame.cols;
	
	Mat opacne;
	Mat text = Mat::zeros(texture_cv.recentFrame.rows, texture_cv.recentFrame.cols, texture_cv.recentFrame.type());;
	double alpha = 0.5; double beta; 

	//std::cout << "\n" << camera << "-" << ifBlack(texture_cv.recentFrame) << "\n";
	//std::cout << "\n" << camera << "-" << ifBlack(texture_cv.recentFrame) << "\n";
	/*if(ifBlack(texture_cv.recentFrame) == 1 && syncDone < 2 && sync1Done == 0){
		if(camera == 1){
			synFrame1 = texture_cv.poradieFramu;
			std::cout << "\nFRAME SYNC1 " << synFrame1 << "\n";
			syncDone++;
			sync1Done++;
		}
	}
	

	if(ifBlack(texture_cv.recentFrame) == 1 && camera == 2 && sync2Done == 0){
			synFrame2 = texture_cv.poradieFramu;
			std::cout << "\nFRAME SYNC2 " << synFrame2 << "\n";
			syncDone++;
			sync2Done++;
	}


	if(syncDone > 1){
		testFrameNext = texture_cv.poradieFramu + synFrame2 - synFrame1;	
	}
	if (synFrame1 > synFrame2){ 
		std::cout << "\n"<< synFrame1 << "****" << synFrame2 << "\n";
	}else{
		std::cout << "\n "<< synFrame2 << "****" <<  synFrame1 << "\n";
	}*/

	//Mat imageOrient = imread("E:\\Download\\orient.png");
	//std::cout << "\n MYYYYS" << getDataMouse().mysX << std::endl;
	/*Point spodBod(sirka/2, 350);
	Point vrchBod(sirka/2-50, 350);
	Point rot = vrchBod - spodBod;
	Point rotated;*/
	/*int angle = 45;
	rotated.x = rot.x*cos(45*3.14/180) - rot.y*sin(45*3.14/180);
	rotated.y = rot.x*sin(45*3.14/180) + rot.y*cos(45*3.14/180);

	line( text, Point( sirka/100*getDataMouse().mysX, 300), Point( sirka/2, 350), Scalar( 0, 255, 255 ),  3, 8 );*/
	//imageOrient.copyTo(text(cv::Rect(400,200,imageOrient.cols, imageOrient.rows)));

	if (control) {
		sprintf(rychlosta, "Data %s", control->getDataOvladanie());
	}
	else {
		sprintf(rychlosta, "NO Control");
	}
	
	cv::putText(text, teplotaa, cvPoint(sirka/3,200), FONT_HERSHEY_COMPLEX_SMALL, 0.7, cvScalar(200,200,250), 0.6, CV_AA);	//Vlozenie textu do OpenCV Mat
	cv::putText(text, "sdsdsds", cvPoint(sirka/3,220), FONT_HERSHEY_COMPLEX_SMALL, 0.7, cvScalar(200,200,250), 0.6, CV_AA);		//Vlozenie textu do OpenCV Mat
	cv::putText(text, rychlosta, cvPoint(sirka/3,240), FONT_HERSHEY_COMPLEX_SMALL, 0.7, cvScalar(100,100,100), 0.6, CV_AA);	//Vlozenie textu do OpenCV Mat

	
	//if (camera ==2) texture_cv.recentFrame = texture_cv.recentFrame + text;
	

	cv::flip(texture_cv.recentFrame, flipped, 0);				//Otocenie textury
	texture_cv.recentFrame = flipped;       
		

	if (camera == 2){
		opacne = rotate(texture_cv.recentFrame, 90);
		texture_cv.recentFrame = opacne;

		Mat textFlip2;
		cv::flip(text, textFlip2, 0);
		texture_cv.recentFrame = texture_cv.recentFrame + textFlip2;
		
	}
	
	//imageOrient.copyTo(texture_cv.recentFrame.rowRange(1, 51).colRange(3, 53));
	

	if(camera == 1){
		opacne = rotate(texture_cv.recentFrame, -90);
		texture_cv.recentFrame=opacne;

		cv::flip(text, flipped, 0);				//Otocenie textury
		text = flipped;       
		texture_cv.recentFrame = texture_cv.recentFrame + text;
	}
		
	
	


	

	if(prvyKrat == 0){
		glGenTextures(1, &textureCV);                  //Ak sa funkcia spustila 1. krat, tak vztvor OpenGL texturu, treba spustit len raz, inac sa zahlti RAM
		prvyKrat++;
	}

		
    glBindTexture(GL_TEXTURE_2D, textureCV);            //Namapovanie Mat do OpenGL textury
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S , GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, texture_cv.recentFrame.cols, texture_cv.recentFrame.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, texture_cv.recentFrame.data);
		
	
	texture_cv.recentFrame.release();   
	text.release();
	flipped.release();
	return textureCV;
  
}

Mat Oculus::rotate(Mat src, double angle)
{
    Mat dst;
    cv::Point2f pt(src.cols/2., src.rows/2.);    
    Mat r = getRotationMatrix2D(pt, angle, 1.0);
	    warpAffine(src, dst, r, cv::Size(src.cols, src.rows));
    return dst;
}

int Oculus::ifBlack(Mat framBlack){
	Mat obr;
	cvtColor(framBlack,obr,CV_BGR2GRAY );
	int TotalNumberOfPixels = obr.rows * obr.cols;
	int count_black = 0;
	for( int y = 0; y < obr.rows; y++ ) {
		for( int x = 0; x < obr.cols; x++ ) {
			if ( obr.at<uchar>(y,x) < 3 ) {
				count_black++;
			} 
		}
	}


	if (TotalNumberOfPixels-count_black < TotalNumberOfPixels/3 && count_black != 0){
		count_black = 1;	//cierne
	}else{
		count_black = 0;	//nie
	}
	return count_black;
}