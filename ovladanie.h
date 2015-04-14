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
    #define X 2             //osi packa zlava doprava
    #define Y 3             //a zhora dole
    #define A 10            //tlacidlo A
#endif
#ifdef __linux__
    #include <SDL2/SDL.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <errno.h>
    #define SOCKET int
    #define INVALID_SOCKET  (SOCKET)(~0)
    #define SOCKET_ERROR            (-1)
    #define X 3                 //Osi. Toto plati pre MINT
    #define Y 4
    #define A 0
#endif

#define PORT 5566
#define PI 3.14159265359
#define ATTEMPTS 5
//#define OTHER_SIDE "192.168.1.108"
#define OTHER_SIDE "192.168.43.15"
#define MIN 13
#define ADDRESS_BROADCAST "255.255.255.255"


using namespace OVR;

//using namespace std;



	

 double ping(int ping_id, SOCKET s, struct sockaddr_in si_other, int slen_other);
void ping2();
//int getDataFromVehicle(SOCKET s, struct sockaddr_in si_other, int slen_other, char** dataFromVehicle);
int getDataFromVehicle(SOCKET s, char** dataFromVehicle);
int getDataFromVehicle2(SOCKET s, char** dataFromVehicle, int timeout_us,struct sockaddr *src_addr);

int processData(char** dataFromVehicle, int recv_len);
int ovladanief();
char* getDataOvladanie();




