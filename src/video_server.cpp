// video_server.cpp: 定义控制台应用程序的入口点。
#include "stdafx.h"
#include "controller.h"

/*
int main(int argc, const char** argv)
{
	controller ThreadManager;
	ThreadManager.ComPrompt();
	thread control(&controller::controltask, &ThreadManager);

	control.join();
	return 0;
}
*/

#include <fstream>
//点击鼠标返回相应坐标位置并存到txtpoints2文件中
#define WINDOW "source image"
Mat src;
void On_mouse(int event, int x, int y, int flags, void*);
Point recent_Point;
ofstream file("point2.txt", ios::out | ios::binary);
int main()
{
	src = imread("jiance.jpg");
	namedWindow(WINDOW, WINDOW_NORMAL);

	setMouseCallback(WINDOW, On_mouse);


	while (1)
	{
		imshow(WINDOW, src);//点击的开始
		waitKey(0);
	}
	imshow(WINDOW, src);//点击的开始
	waitKey(0);
	file.close();
	return 0;

}
void On_mouse(int event, int x, int y, int flags, void*)//每次点击左键，将将当前点坐标存储到txt文件中，并在相应位置画红点
{

	if (event == EVENT_LBUTTONDOWN)
	{
		recent_Point = Point(x, y);
		cout << x << y;
		file << recent_Point.x << " " << recent_Point.y << " ";
		circle(src, recent_Point, 3, Scalar(0, 0, 255), -1);
		imshow(WINDOW, src);

	}
}