#pragma once
#ifndef _WORKER_H_
#define _WORKER_H_

//#include "stdafx.h"
#include "PersonDetection.h"
#include "IntrusionDetection.h"

extern vector<CommandAndVar> commandvar;
extern map<int, vector<CommandAndVar>> ParameterTable;
extern map<int, int> threadoperator;


class worker {


public:

	/**detect_function
	*
	*@param threadID  : the unique numbers of thread
	*
	*Complete dangerous area detection
	*
	*/
	void detect_function(int threadID);

public:
	mutex cout_mutex;
};

#endif
