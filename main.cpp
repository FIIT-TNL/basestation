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
		while (!isTerminateRequested()) {
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

	std::cout << " stream1 Is int: " << cfg->isStream1Int() << std::endl;
	std::cout << " stream1: " << cfg->getStream1() << std::endl;
	std::cout << " stream1 int: " << cfg->getStream1Int() << std::endl;
	std::cout << " stream2 Is int: " << cfg->isStream2Int() << std::endl;
	std::cout << " stream2: " << cfg->getStream2() << std::endl;
	std::cout << " stream2 int: " << cfg->getStream2Int() << std::endl;
	
	// test task
	Asdf a;
	a.start();
	a.terminate();


	std::cout << "testing finished" << std::endl;
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