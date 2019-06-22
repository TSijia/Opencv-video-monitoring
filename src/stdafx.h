// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once
#define _SCL_SECURE_NO_WARNINGS

//#include "targetver.h"
#include <stdio.h>
#include <tchar.h>
#include <boost/thread/thread.hpp>
#include <time.h>
#include <stack>
#include <direct.h>

/******---IntrusionDetection.h---******/
//#include <iostream>
#include <highgui.h>
#include <vector>
#include <cmath>
#include <opencv2/imgproc/imgproc.hpp>

/******---PersionDetection.h---******/
//#include "opencv2/core.hpp"
//#include "opencv2/video.hpp"
//#include <vector>
//#include <map>

#include <iostream>
#include <map>
#include "opencv2/core.hpp"
#include "opencv2/video.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/opencv.hpp>
#include <cassert>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp> 
#include <String>


using namespace cv;
using namespace std;
using namespace boost;

//using std::tr1::shared_ptr;
//#include <iostream>

struct CommandAndVar {
	string   command;
	string   var;
};

// TODO: 在此处引用程序需要的其他头文件
