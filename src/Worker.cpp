#include <stdio.h>
#include "Worker.h"
//#include <windows.h>//头文件



int test = 0;

void worker::detect_function(int threadID)
{

	int threadname = threadID;
	clock_t start, finish;
	//First, read back public area data
	int cameralabel = atoi(ParameterTable[threadname][2].var.c_str());
	string filepath = ParameterTable[threadname][3].var;
	string areaId = ParameterTable[threadname][4].var.c_str();
	int edgeNum = atoi(ParameterTable[threadname][5].var.c_str());
	string method = ParameterTable[threadname][6].var;

	VideoCapture cap;
	PersonDetection persondetect;
	Ptr<BackgroundSubtractor> model;
	IntrusionDetection mydetection;

	if (cameralabel >= 0)
	{
		cap.open(cameralabel);
	}
	else
	{
		cap.open(filepath.c_str());
	}
	if (!cap.isOpened())
	{
		//mutex.lock();
		cout << "Can not open video stream: '" << (cameralabel >= 0 ? " camera " + cameralabel : filepath) << "'" << endl;
		//mutex.unlock();(cameralabel >= 0 ? " camera " + cameralabel : filepath)
		assert(cap.isOpened());
	}

	model = persondetect.BGS_algorithm(method);

	if (!model)
	{
		//mutex.lock();
		cout << "Can not create background model using provided method: '" << method << "'" << endl;
		//mutex.unlock();
		assert(model);
	}

	bool doUpdateModel = true;
	bool doSmoothMask = false;
	Mat inputFrame, frame;
	Rect personrect;

	mydetection = IntrusionDetection(areaId, edgeNum);

	try {
		int i = 1;
		for (;;)
		{
			boost::this_thread::interruption_point();
			if (threadoperator[threadname] == 2)
			{
				cameralabel = atoi(ParameterTable[threadname][2].var.c_str());
				filepath = ParameterTable[threadname][3].var;
				areaId = ParameterTable[threadname][4].var.c_str();
				edgeNum = atoi(ParameterTable[threadname][5].var.c_str());
				method = ParameterTable[threadname][6].var;

				if (cameralabel >= 0)
				{
					cap.open(cameralabel);
				}
				if (!cap.isOpened())
				{
					//mutex.lock();
					cout << "Can not open video stream: '" <<  " camera " + cameralabel  << "'" << endl;
					//mutex.unlock();(cameralabel >= 0 ? " camera " + cameralabel : filepath)
					assert(cap.isOpened());
				}

				model = persondetect.BGS_algorithm(method);
				mydetection = IntrusionDetection(areaId, edgeNum);
				threadoperator[threadname] = 1;
			}
			//在里面实时读修改信息
			// prepare input frame
			if (test == 0)
			{
				start = clock();
			}

			cap >> inputFrame;

			if (inputFrame.empty())
			{
				//mutex.lock();
				cout << "Finished reading: empty frame" << endl;
				threadoperator[threadname] = 0;
				destroyAllWindows();
				//mutex.lock();
				break;
			}
			// resize the frame
			const Size scaledSize(640, 640 * inputFrame.rows / inputFrame.cols);
			resize(inputFrame, frame, scaledSize, 0, 0, INTER_LINEAR);

			// detect moving person and return the rectangular box
			personrect = persondetect.Gain_Rect(frame, model, threadname);

			//draw the rectangular box of the person on the frame
			rectangle(frame, personrect, Scalar(255, 255, 0), 1);

			//judge whether the rectangular frame and the monitoring area insersect and mark the intersection on the frame
			mydetection.DangerDetection(frame, personrect,threadname);
			test++;
			if (test == 100)
			{
				finish = clock();
				cout << (double)(finish - start) / CLOCKS_PER_SEC << endl;
				test = 0;
			}



			namedWindow(ParameterTable[threadname][0].var);
			imshow(ParameterTable[threadname][0].var, frame);

			const char key = (char)waitKey(30);

		}
	}
	catch (thread_interrupted&)
	{
		cout << "NO." << threadname << " thread close" << endl;
	}

}