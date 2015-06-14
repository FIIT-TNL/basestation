
#include <iostream>
#include <iomanip>
#include "drivrconfig.hpp"

using namespace std;


const std::string DrivrConfig::DEFAULT_CONFIG = "drivr = {\n"
	"  basestation = {\n"
	"    stream_left_int = 0\n"
	"    stream_left = \"udp://@0.0.0.0:5568\"\n"
	"    stream_left_rotation = -90\n"
	"    stream_right_int = 0\n"
	"    stream_right = \"udp ://@0.0.0.0:5567\"\n"
	"    stream_right_rotation = 90\n"
	"    fontsize = 2.0\n"
	"    text_position_x = 350\n"
	"    text_position_y = 350\n"
	"    text_color_red = 200\n"
	"    text_color_green = 200\n"
	"    text_color_blue = 250\n"
	"    controls_port = 5566\n"
	"    controls_send_interval = 10\n"
	"    joystick_car_control_x_axis = 2\n"
	"    joystick_car_control_y_axis = 3\n"
	"    joystick_car_control_x_treshold = 20\n"
	"    joystick_car_control_y_treshold = 20\n"
	"    joystick_car_control_x_zero_fix_intervals = 10\n"
	"    joystick_exit_axis = 10\n"
	"    oculus_position_ratio = 1.0\n"
	"}}";

DrivrConfig::DrivrConfig(string filename)
{
	loadConfig(filename);
	loadDefaults();
}

void DrivrConfig::loadDefaults()
{
	this->filename = filename;
	try
	{
		def_cfg.readString(DrivrConfig::DEFAULT_CONFIG);
		def_root = &def_cfg.getRoot();
		def_bs = &(*def_root)["drivr"]["basestation"];
		
	}
	catch (const FileIOException &fioex)
	{
		cerr << "I/O error while reading default config" << endl;
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

string DrivrConfig::getString(Setting *setting, Setting *def_setting, string name)
{
	string str;
	if (!setting->lookupValue(name, str)) {
		if (!def_setting->lookupValue(name, str)) {
			str = "";
		}
	}
	return str;
}

int DrivrConfig::getInt(Setting *setting, Setting *def_setting, string name)
{
	int i;
	if (!setting->lookupValue(name, i)) {
		if (!def_setting->lookupValue(name, i)) {
			i = 0;
		}
	}
	return i;
}

unsigned int DrivrConfig::getUInt(Setting *setting, Setting *def_setting, string name)
{
	unsigned int i;
	if (!setting->lookupValue(name, i)) {
		if (!def_setting->lookupValue(name, i)) {
			i = 0;
		}
	}
	return i;
}

double DrivrConfig::getDouble(Setting *setting, Setting *def_setting, string name)
{
	double f;
	if (!setting->lookupValue(name, f)) {
		if (!def_setting->lookupValue(name, f)) {
			f = 0.0;
		}
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

bool DrivrConfig::isStreamLeftStr()
{
	string s;
	return bs->lookupValue("stream_left", s);
}

int DrivrConfig::getStreamLeftInt()
{
	return getInt(bs, def_bs, "stream_left_int");
}

string DrivrConfig::getStreamLeft()
{
	return getString(bs, def_bs, "stream_left");
}

bool DrivrConfig::isStreamRightStr()
{
	string s;
	return bs->lookupValue("stream_right", s);
}

int DrivrConfig::getStreamRightInt()
{
	return getInt(bs, def_bs, "stream_right_int");
}

string DrivrConfig::getStreamRight()
{
	return getString(bs, def_bs, "stream_right");
}

int DrivrConfig::getStreamLeftRotation()
{
	return getInt(bs, def_bs, "stream_left_rotation");
}

int DrivrConfig::getStreamRightRotation()
{
	return getInt(bs, def_bs, "stream_right_rotation");
}

/* -- text -- */

double DrivrConfig::getFontSize()
{
	return getDouble(bs, def_bs, "fontsize");
}

int DrivrConfig::getTextPositionX()
{
	return getInt(bs, def_bs, "text_position_x");
}

int DrivrConfig::getTextPositionY()
{
	return getInt(bs, def_bs, "text_position_y");
}

int DrivrConfig::getTextColorRed()
{
	return getInt(bs, def_bs, "text_color_red");
}

int DrivrConfig::getTextColorGreen()
{
	return getInt(bs, def_bs, "text_color_green");
}

int DrivrConfig::getTextColorBlue()
{
	return getInt(bs, def_bs, "text_color_blue");
}

/* -- controls -- */

unsigned int DrivrConfig::getControlsPort()
{
	return getUInt(bs, def_bs, "controls_port");
}

unsigned int DrivrConfig::getControlsSendInterval()
{
	return getUInt(bs, def_bs, "controls_send_interval");
}

/* -- joystick -- */

int DrivrConfig::getJoystickCarControlXAxis()
{
	return getInt(bs, def_bs, "joystick_car_control_x_axis");
}

int DrivrConfig::getJoystickCarControlXTreshold()
{
	return getInt(bs, def_bs, "joystick_car_control_x_treshold");
}

int DrivrConfig::getJoystickCarControlXZeroFixIntervalCnt()
{
	return getInt(bs, def_bs, "joystick_car_control_x_zero_fix_intervals");
}

int DrivrConfig::getJoystickCarControlYAxis()
{
	return getInt(bs, def_bs, "joystick_car_control_y_axis");
}

int DrivrConfig::getJoystickCarControlYTreshold()
{
	return getInt(bs, def_bs, "joystick_car_control_y_treshold");
}

int DrivrConfig::getJoystickExitAxit()
{
	return getInt(bs, def_bs, "joystick_exit_axis");
}

/* -- Oculus -- */

double DrivrConfig::getOculusRotationRatio()
{
	return getDouble(bs, def_bs, "oculus_position_ratio");
}

