
#include <iostream>
#include <iomanip>
#include "drivrconfig.hpp"

//using namespace std;

DrivrConfig::DrivrConfig(std::string filename)
{
	loadConfig(filename);
}

void DrivrConfig::loadConfig(std::string filename)
{
	this->filename = filename;
	try
	{
		cfg.readFile(filename.c_str());
		root = &cfg.getRoot();
		bs = &(*root)["drivr"]["basestation"];


	}
	catch (const FileIOException &fioex)
	{
		std::cerr << "I/O error while reading file." << std::endl;
		system("PAUSE");
		return;
	}
	catch (const ParseException &pex)
	{
		std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
			<< " - " << pex.getError() << std::endl;
		system("PAUSE");
		return;
	}
}

std::string DrivrConfig::getCfgName()
{
	std::string name;
	if (!cfg.lookupValue("configname", name)) {
		name = "";
	}
	return name;
}



bool DrivrConfig::isStream1Int()
{
	int i;
	return bs->lookupValue("stream1_int", i);
}

int DrivrConfig::getStream1Int()
{
	int i;
	if (bs->lookupValue("stream1_int", i)) {
		return i;
	}
	return -1;
}

std::string DrivrConfig::getStream1()
{
	std::string s;
	if (bs->lookupValue("stream1", s)) {
		return s;
	}
	return "";
}

bool DrivrConfig::isStream2Int()
{
	int i;
	return bs->lookupValue("stream2_int", i);
}

int DrivrConfig::getStream2Int()
{
	int i;
	if (bs->lookupValue("stream2_int", i)) {
		return i;
	}
	return -1;
}

std::string DrivrConfig::getStream2()
{
	std::string s;
	if (bs->lookupValue("stream2", s)) {
		return s;
	}
	return "";
}
