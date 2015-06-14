#ifndef OVLADANIE_HPP
#define OVLADANIE_HPP

#include "drivrconfig.hpp"
#include "task.hpp"

#include "stdafx.h"

#include "SDL.h"
#define GLEW_STATIC
#include "GL/glew.h"
#define OVR_OS_WIN32
#include "OVR_CAPI_GL.h"
#include "Kernel/OVR_Math.h"
#include "SDL_syswm.h"

#ifdef _WIN32
#include <SDL.h>
#include <winsock.h>
#include <windows.h>
//#define X 2             //osi packa zlava doprava
//#define Y 3             //a zhora dole
//#define A 10            //tlacidlo A
#endif
#ifdef __linux__
#include <SDL2/SDL.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#define SOCKET int
#define INVALID_SOCKET  (SOCKET)(~0)
#define SOCKET_ERROR            (-1)
//#define X 3                 //Osi. Toto plati pre MINT
//#define Y 4
//#define A 0
#endif

//using namespace OVR;
//using namespace std;

class Ovladanie : public Task {
private:
	DrivrConfig *cfg;
	char* dataFromVehicle = NULL;
	char *sensorTemperatureStr = NULL;
	
	// config
	unsigned int controls_port = 0;
	unsigned int controls_send_interval = 0;
	int joystick_car_control_x_axis = 0;
	int joystick_car_control_x_treshold = 0;
	int joystick_car_control_x_zero_fix_intervals = 0;
	int joystick_car_control_y_axis = 0;
	int joystick_car_control_y_treshold = 0;
	int joystick_exit_axis = 0;
	double oculus_position_ratio = 1.0;

	double ping(int ping_id, SOCKET s, struct sockaddr_in si_other, int slen_other);
	void ping2();
	//int getDataFromVehicle(SOCKET s, struct sockaddr_in si_other, int slen_other, char** dataFromVehicle);
	int getDataFromVehicle(SOCKET s, char* dataFromVehicle);
	int getDataFromVehicle(SOCKET s, char* dataFromVehicle, int timeout_us, struct sockaddr *src_addr);

	int printData(char* dataFromVehicle, int recv_len);
	void processSensoricData(char* dataFromVehicle, int recv_len);

protected:
	void doTask();

public:
	Ovladanie(DrivrConfig *cfg);
	char* getSensorDataTemperature();


};

#endif // OVLADANIE_HPP


