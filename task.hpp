#ifndef TASK_HPP
#define TASK_HPP

#include <iostream>
#include <thread>

//using namespace std;

class Task {
private:
	std::thread *thr = NULL;
	volatile bool _running = false;
	volatile bool _terminate_req = false;

	void _doTaskInternal();

public:
	~Task();
	virtual void doTask() = 0;
	bool isTerminationRequested();
	bool isRunning();
	bool start();
	void terminate();
	bool join();

};

#endif // TASK_HPP