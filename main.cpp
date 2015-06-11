#include <iostream>
#include <string>
#include "drivrconfig.hpp"
#include "task.hpp"
#include "oculus.hpp"
//#include "ovladanie.hpp"

using namespace libconfig;

class Asdf : public Task {
public:
	std::string s = "a";
	void doTask() {
		while (!isTerminationRequested()) {
			std::cout << s << std::endl;
		}
	}
};

int main(int argc, char *argv[])
{
	DrivrConfig *cfg;
	if (argc > 1) {
		cfg = new DrivrConfig(argv[1]);
	}
	else {
		cfg = new DrivrConfig();
	}

	//test cfg
	std::string cfgname = cfg->getCfgName();
	std::cout << "Configuration name: " << cfgname << std::endl;

	std::cout << " stream_left = " << cfg->isStreamLeftStr() << std::endl;
	std::cout << " stream_left_int = " << cfg->getStreamLeftInt() << std::endl;
	std::cout << " getStreamLeft = " << cfg->getStreamLeft() << std::endl;
	std::cout << " stream_left_rotation = " << cfg->getStreamLeftRotation() << std::endl;
	std::cout << " isStreamRightStr = " << cfg->isStreamRightStr() << std::endl;
	std::cout << " stream_right_int = " << cfg->getStreamRightInt() << std::endl;
	std::cout << " stream_right = " << cfg->getStreamRight() << std::endl;
	std::cout << " stream_right_rotation = " << cfg->getStreamRightRotation() << std::endl << std::endl;

	std::cout << " fontsize = " << cfg->getFontSize() << std::endl;
	std::cout << " text_position_x = " << cfg->getTextPositionX() << std::endl;
	std::cout << " text_position_y = " << cfg->getTextPositionY() << std::endl;
	std::cout << " text_color_red = " << cfg->getTextColorRed() << std::endl;
	std::cout << " text_color_green = " << cfg->getTextColorGreen() << std::endl;
	std::cout << " text_color_blue = " << cfg->getTextColorBlue() << std::endl << std::endl;

	std::cout << " controls_port = " << cfg->getControlsPort() << std::endl;
	std::cout << " controls_send_interval = " << cfg->getSendInterval() << std::endl << std::endl;

	std::cout << " joystick_car_control_x_axis = " << cfg->getJoystickCarControlXAxis() << std::endl;
	std::cout << " joystick_car_control_x_treshold = " << cfg->getJoystickCarControlXTreshold() << std::endl;
	std::cout << " joystick_car_control_y_axis = " << cfg->getJoystickCarControlYAxis() << std::endl;
	std::cout << " joystick_car_control_y_treshold = " << cfg->getJoystickCarControlYTreshold() << std::endl;
	std::cout << " joystick_exit_axis = " << cfg->getJoystickExitAxit() << std::endl << std::endl;

	std::cout << " oculus_position_ratio = " << cfg->getOculusRotationRatio() << std::endl << std::endl;
	
	// test task
	Asdf a;
	a.start();
	a.terminate();


	std::cout << std::endl << "testing finished" << std::endl;
	system("PAUSE");


	//Oculus oc(cfg, NULL);//&ovl);
	//ovl.start();
	//oc.start();


	//oc.join();
	//ovl.join();
	//doTask();

	Ovladanie ovl(cfg);
	Oculus oculus(cfg, &ovl);
	ovl.start();
	oculus.start();
	oculus.join();
	ovl.terminate();
	ovl.join();


	delete cfg;
	return 0;
}