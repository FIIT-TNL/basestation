#ifndef DRIVRCONFIG_HPP
#define DRIVRCONFIG_HPP

#include <iostream>
#include <string>
#include "libconfig.h++"

//using namespace std;
using namespace libconfig;

class DrivrConfig {
private:
	Config cfg;
	std::string filename;
	Setting *root = NULL;
	Setting *bs = NULL;		//basestation config

	void loadConfig(std::string filename);

public:
	DrivrConfig() : DrivrConfig("drivr.cfg") {}
	DrivrConfig(std::string filename);

	std::string getCfgName();

	bool isStream1Int();
	int getStream1Int();
	std::string getStream1();

	bool isStream2Int();
	int getStream2Int();
	std::string getStream2();




};

#endif // DRIVRCONFIG_HPP