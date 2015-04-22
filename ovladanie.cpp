#include "ovladanie.hpp"


#ifdef _WIN32
#define X 2             //osi packa zlava doprava
#define Y 3             //a zhora dole
#define A 10            //tlacidlo A
#endif
#ifdef __linux__
#define X 3                 //Osi. Toto plati pre MINT
#define Y 4
#define A 0
#endif

#define PORT 5566
#define PI 3.14159265359
#define ATTEMPTS 5
//#define OTHER_SIDE "192.168.1.108"
//#define OTHER_SIDE "192.168.43.15"
#define OTHER_SIDE "192.168.56.101"
#define MIN 13
#define ADDRESS_BROADCAST "255.255.255.255"

using namespace OVR;

//char** dataFromVehicle;

//int processData(char** dataFromVehicle, int recv_len);


Ovladanie::Ovladanie(DrivrConfig *cfg)
	: cfg(cfg)
{}

void Ovladanie::doTask()
{
	std::cout << "-- Controls starting" << std::endl;

SDL_Event event;
    short done = 0;
    int jskey = 0;
    int x_axis=0;
    int y_axis=0;
    double xd = 0;
    double yd = 0;
    int id =0;
    int message_length = 0;
    int count = 0;
	Posef pose;
	float yaw;
	float eyePitch;
	float eyeRoll;

	int mouseX=0;
	int mouseY=0;
	double dmouseX=0;
	double dmouseY=0;
	char cmouseX=0;
	char cmouseY=0;
	
	char y2;
	char p2;
	char r2;
	 
	dataFromVehicle=(char**)malloc(sizeof(char*));
	*dataFromVehicle=(char*)malloc(1500);

	  SDL_Init(SDL_INIT_JOYSTICK);
  atexit(SDL_Quit);

  int num_joysticks = SDL_NumJoysticks();
  int i;
  SDL_Joystick* js = NULL;



  bool debug = false;
 
        ovr_Initialize();
 
        ovrHmd hmd = ovrHmd_Create(0);
 
        if (hmd == NULL)
        {
                hmd = ovrHmd_CreateDebug(ovrHmd_DK1);
 
                debug = true;
        }

















  for(i = 0; i < num_joysticks; ++i)
  {
      SDL_Event event;
    js = SDL_JoystickOpen(i);
    if (js)
        {
          SDL_JoystickGUID guid = SDL_JoystickGetGUID(js);
          char guid_str[1024];
          SDL_JoystickGetGUIDString(guid, guid_str, sizeof(guid_str));
          const char* name = SDL_JoystickName(js);

          int num_axes = SDL_JoystickNumAxes(js);
          int num_buttons = SDL_JoystickNumButtons(js);
          int num_hats = SDL_JoystickNumHats(js);
          int num_balls = SDL_JoystickNumBalls(js);

          printf("%d %s \"%s\" axes:%d buttons:%d hats:%d balls:%d\n",
                 i, guid_str, name, num_axes, num_buttons, num_hats, num_balls);
        }
    else
        {
            SDL_JoystickClose(js);
            //return 2;
			return;
        }
    }
    if(i==0)
        {
            printf("Nepodarilo sa najst ziadny joystick\n");
            //return;
        }
//=====================================================
#ifdef _WIN32
    WSADATA wsa;
#endif
	SOCKET s;
	struct sockaddr_in si_other, si_local;
	int slen_other = sizeof(si_other);
	int slen_local = sizeof(si_local);

	 signed char message[250];    //Snad ziadna sprava nebude dlhsia ako 250B
	 for(i=0;i<250;i++)
		 message[i]=0;

	



#ifdef _WIN32
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
	{
		printf("Failed. Error Code : %d",WSAGetLastError());
		//return 1;
		return;
	}
	printf("Initialised.\n");
#endif
	if((s = socket(AF_INET , SOCK_DGRAM , 0 )) == INVALID_SOCKET)
	{
		perror("socket");
		//return 2;
		return;
	}
	

	printf("Socket created.\n");

    memset((char *) &si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(PORT);
	//si_other.sin_addr.s_addr = inet_addr(OTHER_SIDE);
	si_other.sin_addr.s_addr = inet_addr(ADDRESS_BROADCAST);

	//odosielanie broadcastov - najdenie druhej strany
	 int bp = 1;
	 setsockopt(s, SOL_SOCKET, SO_BROADCAST, (const char *)&bp, sizeof(bp));
		/*message[0]=42;
		message[1]=42;
		message[2]=6;*/
	 message[0]=3;
	 sprintf((char*)message+1,"FIIT_TechNoLogic_Motorcontrol_Discover");
		message_length=strlen("FIIT_TechNoLogic_Motorcontrol_Discover") +1;
		struct sockaddr_in from;
		struct sockaddr_in* from2;
		
	/*	if( bind(s ,(struct sockaddr *)&si_other , sizeof(si_other)) == SOCKET_ERROR)
	{
#ifdef _WIN32
		printf("Bind failed with error code : %d" , WSAGetLastError());
#endif
		perror("bind");
	}	
	*/
		
	 for(i=0;i<ATTEMPTS;i++)
	 {

		if (sendto(s, (char*)message, message_length , 0 , (struct sockaddr *) &si_other, slen_other) == SOCKET_ERROR)
			{
				perror("sendto");
				exit(EXIT_FAILURE);
			}
		bp=getDataFromVehicle(s, dataFromVehicle, 5000000, (struct sockaddr*)&from);
		if(bp<=0)
		{
			printf("nic neprislo\n");
			continue;
		}
		else
		{
			//printf("Port: %d\n", from.sin_port);
			from2=(struct sockaddr_in*)&from;
			//processData((char**)&from2, sizeof(sockaddr_in));
			printf("Adresa: %s\n ", inet_ntoa(from2->sin_addr) );
			//"FIIT_TechNoLogic_Motorcontrol_ACK"
			//processData(dataFromVehicle, strlen("FIIT_TechNoLogic_Motorcontrol_ACK"));
			if((bp=strcmp(*dataFromVehicle, "FIIT_TechNoLogic_Motorcontrol_ACK"))==0)
				printf("Data: %s\n", *dataFromVehicle);
			else
				printf("zle data: %d\n", bp);
			i=0;
			break;
		}
		
	}
	
//	 getchar();
//	 getchar();
	 //exit(1);
	if(i==0)
		si_other.sin_addr = from2->sin_addr;
	else
		si_other.sin_addr.s_addr=inet_addr(OTHER_SIDE);
	i=0;
	message_length=0;
	for(i=0;i<=strlen("FIIT_TechNoLogic_Motorcontrol_Discover");i++)
		message[i]=0;
	message[0]=message[1]=message[2]=0;

	//inicializacia socketu pre prijimanie dat z vozidla. Port 7777
	
	SOCKET s2;
	struct sockaddr_in server;
	//inicializacia premennych pre data
	/*char** dataFromVehicle;
	dataFromVehicle=(char**)malloc(sizeof(char*));
	*dataFromVehicle=(char*)malloc(1500);*/
	int recv_len =0;

#ifdef _WIN32	
	WSADATA wsa2;
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2,2),&wsa2) != 0)
	{
		printf("Failed. Error Code : %d",WSAGetLastError());
		//return 3;
		return;
	}
#endif	

	printf("Initialised.\n");
	
	//Create a socket
	if((s2 = socket(AF_INET , SOCK_DGRAM , 0 )) == INVALID_SOCKET)
	{
#ifdef _WIN32
		printf("Could not create socket : %d.\n" , WSAGetLastError());
#endif
		perror("socket");
	}
	printf("Socket created.\n");	
	//Prepare the sockaddr_in structure
	memset((char *) &server, 0, sizeof(server));
	server.sin_family = AF_INET;	
	server.sin_port = htons( 7777 );
	server.sin_addr.s_addr = INADDR_ANY;

	//Bind
	if( bind(s2 ,(struct sockaddr *)&server , sizeof(server)) == SOCKET_ERROR)
	{
#ifdef _WIN32
		printf("Bind failed with error code : %d" , WSAGetLastError());
#endif
		perror("bind");
	}	
	puts("Bind done");



	while (!this->isTerminateRequested()){
		count++;
				//ping(count, s, si_other, slen_other);            			

		
			recv_len=getDataFromVehicle(s2, dataFromVehicle);
			processData(dataFromVehicle,recv_len);

			
            message_length = 3;
            message[0] = 1;
            jskey = SDL_JoystickGetButton(js, A);
            if(jskey==1)
                done=1;
            if(jskey==2)
            {


            }

            xd=(double)SDL_JoystickGetAxis(js, X);
            yd=(double)SDL_JoystickGetAxis(js, Y);

			printf("Packa X=%f ... Packa Y=%f\n", xd, yd);

			x_axis= floor(100 * xd/SHRT_MAX + 0.5);
            y_axis=- floor(100 * yd/SHRT_MAX + 0.5);

            if(abs(x_axis)<13)
                x_axis=0;
            if(abs(y_axis)<13)
                y_axis=0;

             message[1]=y_axis+x_axis;
             message[2]=y_axis-x_axis;
            if(message[1]>100)
                    message[1]=100;
            if(message[1]<-100)
                    message[1]=-100;
            if(message[2]>100)
                    message[2]=100;
            if(message[2]<-100)
                    message[2]=-100;

     /*   if (sendto(s, (char*)message, message_length , 0 , (struct sockaddr *) &si_other, slen_other) == SOCKET_ERROR)
             {
                   perror("sendto");
                   exit(EXIT_FAILURE);
             } */
        x_axis=y_axis=xd=yd=0;
        message[0]=message[1]=message[2]=message[3]=0;
		 message[0]=2;            

              ovrTrackingState trackingState = ovrHmd_GetTrackingState(hmd, ovr_GetTimeInSeconds());
				//if (trackingState.StatusFlags & (ovrStatus_OrientationTracked | ovrStatus_PositionTracked))
				//{
					pose = trackingState.HeadPose.ThePose;
					pose.Rotation.GetEulerAngles<Axis_Y, Axis_X, Axis_Z>(&yaw, &eyePitch, &eyeRoll);
					//cout << yaw << endl;
					y2=yaw;
					p2=eyePitch;
					r2=eyeRoll;
					


					y2=(signed char)floor(((yaw+PI)/(2*PI))*100);
					p2=(signed char)floor(((eyePitch+(PI/2))/(PI))*100);
					r2=(signed char)floor(((eyeRoll+PI)/(2*PI))*100);

			/*	y2 = (signed char)abs(floor(((y2+PI)/(PI*2)) * 100 + 0.5));
			p2 = (signed char)abs(floor(((p2+PI/2)/PI) * 100 + 0.5));
					r2 = (signed char)abs(floor(((r2+PI)/(PI*2)) * 100 + 0.5));*/
	
					/*y2 = (signed char)abs(floor((y2/PI) * 100 + 0.5));
					p2 = (signed char)abs(floor((p2/PI) * 100 + 0.5));
					r2 = (signed char)abs(floor((r2/PI) * 100 + 0.5));
	*/

	//				printf("yaw: %f, eyePitch: %f, eyeRoll: %f, y2=%d, p2=%d, r2=%d\n", yaw, eyePitch, eyeRoll, y2, p2, r2);
					message[1]=y2;
					message[2]=p2;
					message[3]=r2;
					message_length=4;
		if (sendto(s, (char*)message, message_length , 0 , (struct sockaddr *) &si_other, slen_other) == SOCKET_ERROR)
             {
                   perror("sendto");
                   exit(EXIT_FAILURE);
             }
		message_length=3;
			 
        p2=r2=y2=0;
        message[0]=message[1]=message[2]=message[3]=0;


		message[0]=2;
		SDL_PollEvent(&event);
			/*if (event.type == SDL_MOUSEMOTION )
            {
				printf("Mys X=%d Y=%d\n", event.motion.xrel, event.motion.yrel);					
			}
			if(event.type == SDL_MOUSEBUTTONDOWN)
			{
				printf("MOUSE BUTTON DOWN\n");
			}*/
		SDL_GetMouseState(&mouseX, &mouseY);
			dmouseX=mouseX;
			dmouseY=mouseY;
		dmouseX=(int)floor((dmouseX/1279) * 100.0);
		dmouseY=(int)floor((dmouseY/799) * 100.0);
		
		message[0]=2;
		message[1]=(char)floor(dmouseX);
		message[2]=(char)floor(dmouseY);
		printf("Mys X=%d Y=%d\n", message[1], message[2]);
		if (sendto(s, (char*)message, message_length , 0 , (struct sockaddr *) &si_other, slen_other) == SOCKET_ERROR)
             {
                   perror("sendto");
                   exit(EXIT_FAILURE);
             }



        #ifdef _WIN32
                    Sleep(10);
        #endif // _WIN32
        #ifdef __linux__
                    usleep(10000);
        #endif
	}



}



double Ovladanie::ping(int ping_id, SOCKET s, struct sockaddr_in si_other, int slen_other)
{
#ifdef _WIN32
    LARGE_INTEGER frequency;
    LARGE_INTEGER start;
    LARGE_INTEGER end;
    double interval;
#endif
	fd_set fds ;
	int n=0 ;
	struct timeval tv ;

    int i=0;
    u_long ul_ping_id;
    int message_length = sizeof(u_long) + 1;
    int BUFLEN = 1500;
    char message[25];
	int recv_len;
	char buf[1500];
#ifdef _WIN32

    WSADATA wsa;
if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
	{
		printf("Failed. Error Code : %d",WSAGetLastError());
		return 1;
	}
#endif // _WIN32

#ifdef _WIN32
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);
#endif
    message[0] = 0;
    ul_ping_id = htonl(ping_id);
    memcpy(message+ 1, (char*)&ul_ping_id, sizeof(u_long));

	FD_ZERO(&fds) ;
	FD_SET(s, &fds) ;

	tv.tv_sec = 3 ;
	tv.tv_usec = 0 ;

    if (sendto(s, message, message_length , 0 , (struct sockaddr *) &si_other, slen_other) == SOCKET_ERROR)
             {
                   perror("sendto");
                   exit(EXIT_FAILURE);
             }
        memset(buf,'\0', BUFLEN);
				
		i=0;
		while(1)
		{
			n = select ( s, &fds, NULL, NULL, &tv );
					if (n==0)
					{ 
					  printf("Timeout: ");
					  QueryPerformanceCounter(&end);
					  interval = (double) (end.QuadPart - start.QuadPart) / frequency.QuadPart;
					  printf(" Milisekundy: %f -- Ping ID: %d, n=%d\n", interval * 1000, ping_id, n);
					  return 1;
					}
					else if( n == -1 )
					{
					  perror("select");
					  printf("Failed. Error Code : %d\n",WSAGetLastError());
					  QueryPerformanceCounter(&end);
					  return -1;
					}		
				printf("po selecte: n=%d, pingID=%d\n",n, ping_id);		

		if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen_other)) != SOCKET_ERROR)
			{
				i=0;
				while(i<recv_len)
					{
						printf("%02X ", (*(buf+i)) & 0xff);
						i++;
					}
#ifdef _WIN32
				QueryPerformanceCounter(&end);
				interval = (double) (end.QuadPart - start.QuadPart) / frequency.QuadPart;
				printf("OK - Milisekundy: %f -- Ping ID: %d, n=%d, prijate: %d\n", interval * 1000, ping_id, n, recv_len);
			
#endif
				return 0;
			}
			else
			{
#ifdef _WIN32
				QueryPerformanceCounter(&end);
#endif
				//perror("recvfrom");
				//printf("Chyba: %d\t\t", recv_len);
			}
		}
	//printf("koniec cakania: %d\n", ping_id);
	return 0;
}



void Ovladanie::ping2()
{
	char command[21];
	strcpy(command, "ping ");
	strcpy(command + 5, OTHER_SIDE);
	printf("command: %s\n", command);
	system(command);
}










int Ovladanie::getDataFromVehicle(SOCKET s, char** dataFromVehicle)
{
	#ifdef _WIN32
    LARGE_INTEGER frequency;
    LARGE_INTEGER start;
    LARGE_INTEGER end;
    double interval;
#endif
	fd_set fds ;
	int n=0 ;
	struct timeval tv ;

    int i=0;
    u_long ul_ping_id;
    int message_length = sizeof(u_long) + 1;
    int BUFLEN = 1500;
	int recv_len=0;	
#ifdef _WIN32

    WSADATA wsa;
if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
	{
		printf("Failed. Error Code : %d",WSAGetLastError());
		return NULL;
	}
#endif // _WIN32

#ifdef _WIN32
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);				//meranie casu - len pre windows
#endif

	FD_ZERO(&fds) ;
	FD_SET(s, &fds) ;

	tv.tv_sec = 0 ;
	tv.tv_usec = 1 ;							//Timeout je 1usec.

	memset(*dataFromVehicle,'\0', BUFLEN);
		i=0;
		
	n = select ( s, &fds, NULL, NULL, &tv );
			if (n==0)							//Ak nastal timeout, teda nic neprislo
			{ 
				//printf("\ntimeout\n");
				QueryPerformanceCounter(&end);
				interval = (double) (end.QuadPart - start.QuadPart) / frequency.QuadPart;
				return -1;
			}
			else if( n == -1 )						//Ak nastala nejaka velmi vazna chyba. Toto by nemalo nastat nikdy
			{
				perror("select");
				printf("Failed. Error Code : %d\n",WSAGetLastError());
				QueryPerformanceCounter(&end);
				return -2;
			}			

		if ((recv_len = recvfrom(s, *dataFromVehicle, BUFLEN, 0,NULL, NULL)) != SOCKET_ERROR)
		{
#ifdef _WIN32
			QueryPerformanceCounter(&end);
			interval = (double) (end.QuadPart - start.QuadPart) / frequency.QuadPart;
			//printf("Prisli data - Milisekundy: %f -- Ping ID: %d, n=%d\n", interval * 1000, 0, n);
#endif
			return recv_len;
		}
		else
		{
#ifdef _WIN32
			QueryPerformanceCounter(&end);
#endif
			perror("recvfrom");
			printf("Chyba: %d\t\t", WSAGetLastError());
		}		
	return -3;
}


int Ovladanie::processData(char** dataFromVehicle, int recv_len)
{
	int i=0;
	if(recv_len==-1)
		{return recv_len;}
	if(recv_len<=0)
	{
		printf("Error: recv_len==%d\n",recv_len);
		return recv_len;
	}
	printf("ProcessData: ");
	while(i<recv_len)
		{
			
			printf("%02X ", (*(*dataFromVehicle+i)) & 0xff);
			i++;
		}
	putchar('\n');
	return 0;
}

char* Ovladanie::getDataOvladanie(){
	
	//sprintf(*dataFromVehicle, "fgdgdfgdfgdfg");

  //TODO: Osetrit mutexami
	return *dataFromVehicle;
}



int Ovladanie::getDataFromVehicle(SOCKET s, char** dataFromVehicle, int timeout_us, struct sockaddr *src_addr)
{
	#ifdef _WIN32
    LARGE_INTEGER frequency;
    LARGE_INTEGER start;
    LARGE_INTEGER end;
    double interval;
#endif
	fd_set fds ;
	int n=0 ;
	struct timeval tv ;
	int fromlen = sizeof(sockaddr_in);

    int i=0;
    u_long ul_ping_id;
    int message_length = sizeof(u_long) + 1;
    int BUFLEN = 1500;
	int recv_len=0;	
#ifdef _WIN32

    WSADATA wsa;
if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
	{
		printf("Failed. Error Code : %d",WSAGetLastError());
		return NULL;
	}
#endif // _WIN32

#ifdef _WIN32
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);				//meranie casu - len pre windows
#endif

	FD_ZERO(&fds) ;
	FD_SET(s, &fds) ;

	tv.tv_sec = 0 ;
	tv.tv_usec = timeout_us ;							//Timeout je 1usec.

	memset(*dataFromVehicle,'\0', BUFLEN);
		i=0;
		
	n = select ( s, &fds, NULL, NULL, &tv );
			if (n==0)							//Ak nastal timeout, teda nic neprislo
			{ 
				//printf("\ntimeout\n");
				QueryPerformanceCounter(&end);
				interval = (double) (end.QuadPart - start.QuadPart) / frequency.QuadPart;
				return -1;
			}
			else if( n == -1 )						//Ak nastala nejaka velmi vazna chyba. Toto by nemalo nastat nikdy
			{
				perror("select");
				printf("Failed. Error Code : %d\n",WSAGetLastError());
				QueryPerformanceCounter(&end);
				return -2;
			}			

			if ((recv_len = recvfrom(s, *dataFromVehicle, BUFLEN, 0,(sockaddr*)src_addr, &fromlen  )) != SOCKET_ERROR)
		{
#ifdef _WIN32
			QueryPerformanceCounter(&end);
			interval = (double) (end.QuadPart - start.QuadPart) / frequency.QuadPart;
			//printf("Prisli data - Milisekundy: %f -- Ping ID: %d, n=%d\n", interval * 1000, 0, n);
#endif
			//printf("recv_len: %d\n", recv_len);
			return recv_len;
		}
		else
		{
#ifdef _WIN32
			QueryPerformanceCounter(&end);
#endif
			perror("recvfrom");
			printf("Chyba: %d\t\t", WSAGetLastError());
		}		
	return -3;
}



