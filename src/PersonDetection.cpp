#include "PersonDetection.h"



BGS PersonDetection::BGS_algorithm(string method)
{

	BGS model;
	switch (Select_algorithm[method])
	{
	case 0:
		model = createBackgroundSubtractorKNN();
		break;
	case 1:
		model = createBackgroundSubtractorMOG2();
		break;
	}
	return model;

}

Rect PersonDetection::Gain_Rect(Mat &frame , BGS model, int threadID)
{
	int threadname = threadID;
	Mat binaryzation;
	Mat dilateimage;
	Mat quxiaokong;
	Mat element1 = getStructuringElement(MORPH_RECT, Size(3, 3));
	Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
	cvtColor(frame, frame_gray, CV_BGR2GRAY);
	model->apply(frame_gray, foregroundMask, doUpdateModel ? -1 : 0);

	maskFrame = foregroundMask.clone();
	//二值化
	threshold(maskFrame, binaryzation, 244, 255, THRESH_BINARY);
	//均值滤波blur(binaryzation, dilateimage, Size(3, 3), Point(-1, -1));
	//bilateralFilter(binaryzation, dilateimage, 5, 100, 3);
	//quxiaokong = RemoveSmallRegion(binaryzation,20, 1, 1);
	erode(binaryzation, dilateimage, element1);
	dilate(dilateimage, dilateimage, element);
	dilate(dilateimage, dilateimage, element);
	dilate(dilateimage, dilateimage, element);
	dilate(dilateimage, dilateimage, element);
//	dilate(dilateimage, dilateimage, element);

	namedWindow("foreground mask" + to_string(threadname));
	imshow("foreground mask" + to_string(threadname), foregroundMask);
	namedWindow("binaryzation" + to_string(threadname));
	imshow("binaryzation" + to_string(threadname), binaryzation);
	namedWindow("dilateimage" + to_string(threadname));
	imshow("dilateimage" + to_string(threadname), dilateimage);

	findContours(dilateimage, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0, 0));
	vector<vector<Point>> contours_poly(contours.size());
	boundRect.clear();
	double Maxarea = 0;
	int myindex = 0;
	personRect = { 0,0,0,0 };
	//cout << contours.size() << endl;
	if (contours.size() > 0)
	{
		for (int index = 0; index < contours.size(); index++)
		{
			if (Maxarea < fabs(contourArea(contours[index], true)))
			{
				Maxarea = fabs(contourArea(contours[index], true));
				myindex = index;
			}
		}
		if (Maxarea > 700)
		{
			approxPolyDP(Mat(contours[myindex]), contours_poly[myindex], 3, true);
			personRect = boundingRect(Mat(contours_poly[myindex]));
		}

	}

	return personRect;
}
Mat PersonDetection::RemoveSmallRegion(Mat &Src, int AreaLimit, int CheckMode, int NeihborMode)
{
	int RemoveCount = 0;

	//新建一幅标签图像初始化为0像素点，为了记录每个像素点检验状态的标签，0代表未检查，1代表正在检查,2代表检查不合格（需要反转颜色），3代表检查合格或不需检查   
	//初始化的图像全部为0，未检查  
	Mat PointLabel = Mat::zeros(Src.size(), CV_8UC1);
	Mat Dst = Mat::zeros(Src.size(), CV_8UC1);
	if (CheckMode == 1)//去除小连通区域的白色点  
	{
		//cout << "去除小连通域.";
		for (int i = 0; i < Src.rows; i++)
		{
			for (int j = 0; j < Src.cols; j++)
			{
				if (Src.at<uchar>(i, j) < 10)
				{
					PointLabel.at<uchar>(i, j) = 3;//将背景黑色点标记为合格，像素为3  
				}
			}
		}
	}
	else//去除孔洞，黑色点像素  
	{
		cout << "去除孔洞";
		for (int i = 0; i < Src.rows; i++)
		{
			for (int j = 0; j < Src.cols; j++)
			{
				if (Src.at<uchar>(i, j) > 10)
				{
					PointLabel.at<uchar>(i, j) = 3;//如果原图是白色区域，标记为合格，像素为3  
				}
			}
		}
	}


	vector<Point2i>NeihborPos;//将邻域压进容器  
	NeihborPos.push_back(Point2i(-1, 0));
	NeihborPos.push_back(Point2i(1, 0));
	NeihborPos.push_back(Point2i(0, -1));
	NeihborPos.push_back(Point2i(0, 1));
	if (NeihborMode == 1)
	{
		//cout << "Neighbor mode: 8邻域." << endl;
		NeihborPos.push_back(Point2i(-1, -1));
		NeihborPos.push_back(Point2i(-1, 1));
		NeihborPos.push_back(Point2i(1, -1));
		NeihborPos.push_back(Point2i(1, 1));
	}
	else cout << "Neighbor mode: 4邻域." << endl;
	int NeihborCount = 4 + 4 * NeihborMode;
	int CurrX = 0, CurrY = 0;
	//开始检测  
	for (int i = 0; i < Src.rows; i++)
	{
		for (int j = 0; j < Src.cols; j++)
		{
			if (PointLabel.at<uchar>(i, j) == 0)//标签图像像素点为0，表示还未检查的不合格点  
			{   //开始检查  
				vector<Point2i>GrowBuffer;//记录检查像素点的个数  
				GrowBuffer.push_back(Point2i(j, i));
				PointLabel.at<uchar>(i, j) = 1;//标记为正在检查  
				int CheckResult = 0;


				for (long z = 0; z < GrowBuffer.size(); z++)
				{
					for (int q = 0; q < NeihborCount; q++)
					{
						CurrX = GrowBuffer.at(z).x + NeihborPos.at(q).x;
						CurrY = GrowBuffer.at(z).y + NeihborPos.at(q).y;
						if (CurrX >= 0 && CurrX<Src.cols&&CurrY >= 0 && CurrY<Src.rows)  //防止越界    
						{
							if (PointLabel.at<uchar>(CurrY, CurrX) == 0)
							{
								GrowBuffer.push_back(Point2i(CurrX, CurrY));  //邻域点加入buffer    
								PointLabel.at<uchar>(CurrY, CurrX) = 1;           //更新邻域点的检查标签，避免重复检查    
							}
						}
					}
				}
				if (GrowBuffer.size()>AreaLimit) //判断结果（是否超出限定的大小），1为未超出，2为超出    
					CheckResult = 2;
				else
				{
					CheckResult = 1;
					RemoveCount++;//记录有多少区域被去除  
				}


				for (long z = 0; z < GrowBuffer.size(); z++)
				{
					CurrX = GrowBuffer.at(z).x;
					CurrY = GrowBuffer.at(z).y;
					PointLabel.at<uchar>(CurrY, CurrX) += CheckResult;//标记不合格的像素点，像素值为2  
				}
				//********结束该点处的检查**********    


			}
		}


	}


	CheckMode = 255 * (1 - CheckMode);
	//开始反转面积过小的区域    
	for (int i = 0; i < Src.rows; ++i)
	{
		for (int j = 0; j < Src.cols; ++j)
		{
			if (PointLabel.at<uchar>(i, j) == 2)
			{
				Dst.at<uchar>(i, j) = CheckMode;
			}
			else if (PointLabel.at<uchar>(i, j) == 3)
			{
				Dst.at<uchar>(i, j) = Src.at<uchar>(i, j);

			}
		}
	}
	//cout << RemoveCount << " objects removed." << endl;
	return Dst;
}

