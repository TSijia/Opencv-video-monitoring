#pragma once
#ifndef  _PERSONDETECTION_H_
#define  _PERSONDETECTION_H_

#include "stdafx.h"

using namespace cv;
using namespace std;


typedef Ptr<BackgroundSubtractor> BGS;

class PersonDetection {

public:

	/**BGS_algorithm
	*
	*@param method  :  Create different model objects according to the user's choice
	*
	*return created model object
	*/
	BGS BGS_algorithm(string method);


	/**Gain_Rect
	*
	*@param &frame   :  frame reference
	*@param model    :  model objects
	*@param threadID :  each thread has a unique ID
	*
	*return : rectangular box of moving person
	*/
	Rect Gain_Rect(Mat &frame, BGS model, int threadID);

	Mat RemoveSmallRegion(Mat &Src, int AreaLimit, int CheckMode, int NeihborMode);
	
private:
	//map string to int and apply to switch
	map<string, int> Select_algorithm = {
		                                 {"knn", 0},
	                                     {"mog2", 1}
	                                     };
	Mat frame, frame_gray, foregroundMask;
	bool doUpdateModel = true;

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	Rect personRect = {0,0,0,0};
	vector<Rect> boundRect;
	Mat maskFrame;


};

#endif // 

