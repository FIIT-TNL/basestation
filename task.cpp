#include "task.hpp"

using namespace std;

Task::~Task()
{
	if (thr) {
		thr->join();
		delete thr;
	}
}

void Task::_doTaskInternal()
{
	_running = true;
	doTask();
	_running = false;
}

bool Task::isTerminateRequested()
{
	return _terminate_req;
}

bool Task::isRunning()
{
	return _running;
}

bool Task::start() {
	if (_running) {
		return false;
	}
	if (thr) {
		thread *tmp_thr = thr;
		thr = NULL;
		delete tmp_thr;
	}

	thr = new thread(&Task::_doTaskInternal, this);
	return true;
}

void Task::terminate() {
	_terminate_req = true;
}

bool Task::join() {
	if (thr) {
		thr->join();
		return true;
	}
	return false;
}