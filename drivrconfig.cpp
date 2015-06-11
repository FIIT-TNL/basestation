
#include <iostream>
#include <iomanip>
#include "drivrconfig.hpp"

using namespace std;


const std::string DrivrConfig::DEFAULT_CONFIG = "drivr = {\n"
	"  basestation = {\n"
	"	stream_left_int = 0;\n"
	"	stream_left = \"tcp ://192.168.1.109:4247\"\n"
	"	stream_right_int = 0"
	"	stream_right = \"tcp ://192.168.1.110:4247\"\n"
	"   font_size = 0.7\n"
	"	text_position = {x = 200; y = 200;}\n"
	"	text_color = {r = 250; g = 200; b = 200;}\n"
	"	joystick_car_control_x_axis = 2\n"
	"	joystick_car_control_y_axis = 3\n"
	"	joystick_exit_axis = 10\n"
	"	joystick_stick_treshold = 13\n"
	"	controls_port = 5566;\n"
	"}}";

DrivrConfig::DrivrConfig(string filename)
{
	loadConfig(filename);
}

void DrivrConfig::loadConfig(string filename)
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
		cerr << "I/O error while reading file." << endl;
		system("PAUSE");
		return;
	}
	catch (const ParseException &pex)
	{
		cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
			<< " - " << pex.getError() << endl;
		system("PAUSE");
		return;
	}
}

string DrivrConfig::getString(Setting *setting, string name)
{
	string str;
	if (!setting->lookupValue(name, str)) {
		str = "";
	}
	return str;
}

int DrivrConfig::getInt(Setting *setting, string name)
{
	int i;
	if (!setting->lookupValue(name, i)) {
		i = -1;
	}
	return i;
}

double DrivrConfig::getDouble(Setting *setting, string name)
{
	double f;
	if (!setting->lookupValue(name, f)) {
		f = 0;
	}
	return f;
}

string DrivrConfig::getCfgName()
{
	string name;
	if (!cfg.lookupValue("configname", name)) {
		name = "";
	}
	return name;
}

bool DrivrConfig::isStreamLeftInt()
{
	int i;
	return bs->lookupValue("stream_left_int", i);
}

int DrivrConfig::getStreamLeftInt()
{
	return getInt(bs, "stream_left_int");
}

string DrivrConfig::getStreamLeft()
{
	string s;
	if (bs->lookupValue("stream_left", s)) {
		return s;
	}
	return "";
}

bool DrivrConfig::isStreamRightInt()
{
	int i;
	return bs->lookupValue("stream_right_int", i);
}

int DrivrConfig::getStreamRightInt()
{
	int i;
	if (bs->lookupValue("stream_right_int", i)) {
		return i;
	}
	return -1;
}

string DrivrConfig::getStreamRight()
{
	string s;
	if (bs->lookupValue("stream_right", s)) {
		return s;
	}
	return "";
}

double DrivrConfig::getFontSize()
{
	return getDouble(bs, "fontsize");
}

