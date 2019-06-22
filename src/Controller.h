#pragma once
#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include "stdafx.h"
#include "Worker.h"

extern vector<CommandAndVar> commandvar;
extern map<int, vector<CommandAndVar>> ParameterTable;
extern map<int, int> threadoperator;


class controller {

public:

	/**ComPrompt
	*
	*the function : print command line interaction information
	*
	*/
    void ComPrompt();

	/**controltask
	*
	*the function : 1. receive user commands and parse
	*               2. call stop() to creat thread
	*               3. change thread parameters
	*               4  call stop() to close thread
	*
	*/
	void controltask();

	/**start
	*
	*the function : creat thread
	*
	*/
	void start(int threadID);

	/**stop
	*
	*the function : close thread
	*
	*/
	void stop(int threadID);


private:
	//vector<thread> workthread(10);
	thread workthread[100];

};



#endif
