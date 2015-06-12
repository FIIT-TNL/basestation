#ifndef DRIVRCONFIG_HPP
#define DRIVRCONFIG_HPP

#include <iostream>
#include <string>
#include "libconfig.h++"

//using namespace std;
using namespace libconfig;

class DrivrConfig {
private:
	static const std::string DEFAULT_CONFIG;

	Config cfg;
	Config def_cfg;
	std::string filename;
	Setting *root = NULL;
	Setting *bs = NULL;			//basestation config
	Setting *def_root = NULL;	//defaults root
	Setting *def_bs = NULL;		//default basestation config

	void loadDefaults();
	
	std::string getString(Setting *setting, Setting *def_setting, std::string name);
	int getInt(Setting *setting, Setting *def_setting, std::string name);
	unsigned int getUInt(Setting *setting, Setting *def_setting, std::string name);
	double getDouble(Setting *setting, Setting *def_setting, std::string name);

public:
	DrivrConfig() : DrivrConfig("drivr.cfg") {}
	DrivrConfig(std::string filename);

	void loadConfig(std::string filename);
		
	std::string getCfgName();

	bool isStreamLeftStr();
	int getStreamLeftInt();
	std::string getStreamLeft();
	int getStreamLeftRotation();
	bool isStreamRightStr();
	int getStreamRightInt();
	std::string getStreamRight();
	int getStreamRightRotation();

	double getFontSize();
	int getTextPositionX();
	int getTextPositionY();
	int getTextColorRed();
	int getTextColorGreen();
	int getTextColorBlue();
	
	unsigned int getControlsPort();
	unsigned int getControlsSendInterval();

	int getJoystickCarControlXAxis();
	int getJoystickCarControlXTreshold();
	int getJoystickCarControlYAxis();
	int getJoystickCarControlYTreshold();
	int getJoystickExitAxit();

	double getOculusRotationRatio();
};

#endif // DRIVRCONFIG_HPP