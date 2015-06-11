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
	std::string filename;
	Setting *root = NULL;
	Setting *bs = NULL;		//basestation config
	
	std::string getString(Setting *setting, std::string name);
	int getInt(Setting *setting, std::string name);
	double getDouble(Setting *setting, std::string name);

public:
	DrivrConfig() : DrivrConfig("drivr.cfg") {}
	DrivrConfig(std::string filename);

	void loadConfig(std::string filename);
		
	std::string getCfgName();
	bool isStreamLeftInt();
	int getStreamLeftInt();
	std::string getStreamLeft();
	bool isStreamRightInt();
	int getStreamRightInt();
	std::string getStreamRight();
	double getFontSize();
	//int 
};

#endif // DRIVRCONFIG_HPP