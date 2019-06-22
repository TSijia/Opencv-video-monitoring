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
	//��ֵ��
	threshold(maskFrame, binaryzation, 244, 255, THRESH_BINARY);
	//��ֵ�˲�blur(binaryzation, dilateimage, Size(3, 3), Point(-1, -1));
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

	//�½�һ����ǩͼ���ʼ��Ϊ0���ص㣬Ϊ�˼�¼ÿ�����ص����״̬�ı�ǩ��0����δ��飬1�������ڼ��,2�����鲻�ϸ���Ҫ��ת��ɫ����3������ϸ������   
	//��ʼ����ͼ��ȫ��Ϊ0��δ���  
	Mat PointLabel = Mat::zeros(Src.size(), CV_8UC1);
	Mat Dst = Mat::zeros(Src.size(), CV_8UC1);
	if (CheckMode == 1)//ȥ��С��ͨ����İ�ɫ��  
	{
		//cout << "ȥ��С��ͨ��.";
		for (int i = 0; i < Src.rows; i++)
		{
			for (int j = 0; j < Src.cols; j++)
			{
				if (Src.at<uchar>(i, j) < 10)
				{
					PointLabel.at<uchar>(i, j) = 3;//��������ɫ����Ϊ�ϸ�����Ϊ3  
				}
			}
		}
	}
	else//ȥ���׶�����ɫ������  
	{
		cout << "ȥ���׶�";
		for (int i = 0; i < Src.rows; i++)
		{
			for (int j = 0; j < Src.cols; j++)
			{
				if (Src.at<uchar>(i, j) > 10)
				{
					PointLabel.at<uchar>(i, j) = 3;//���ԭͼ�ǰ�ɫ���򣬱��Ϊ�ϸ�����Ϊ3  
				}
			}
		}
	}


	vector<Point2i>NeihborPos;//������ѹ������  
	NeihborPos.push_back(Point2i(-1, 0));
	NeihborPos.push_back(Point2i(1, 0));
	NeihborPos.push_back(Point2i(0, -1));
	NeihborPos.push_back(Point2i(0, 1));
	if (NeihborMode == 1)
	{
		//cout << "Neighbor mode: 8����." << endl;
		NeihborPos.push_back(Point2i(-1, -1));
		NeihborPos.push_back(Point2i(-1, 1));
		NeihborPos.push_back(Point2i(1, -1));
		NeihborPos.push_back(Point2i(1, 1));
	}
	else cout << "Neighbor mode: 4����." << endl;
	int NeihborCount = 4 + 4 * NeihborMode;
	int CurrX = 0, CurrY = 0;
	//��ʼ���  
	for (int i = 0; i < Src.rows; i++)
	{
		for (int j = 0; j < Src.cols; j++)
		{
			if (PointLabel.at<uchar>(i, j) == 0)//��ǩͼ�����ص�Ϊ0����ʾ��δ���Ĳ��ϸ��  
			{   //��ʼ���  
				vector<Point2i>GrowBuffer;//��¼������ص�ĸ���  
				GrowBuffer.push_back(Point2i(j, i));
				PointLabel.at<uchar>(i, j) = 1;//���Ϊ���ڼ��  
				int CheckResult = 0;


				for (long z = 0; z < GrowBuffer.size(); z++)
				{
					for (int q = 0; q < NeihborCount; q++)
					{
						CurrX = GrowBuffer.at(z).x + NeihborPos.at(q).x;
						CurrY = GrowBuffer.at(z).y + NeihborPos.at(q).y;
						if (CurrX >= 0 && CurrX<Src.cols&&CurrY >= 0 && CurrY<Src.rows)  //��ֹԽ��    
						{
							if (PointLabel.at<uchar>(CurrY, CurrX) == 0)
							{
								GrowBuffer.push_back(Point2i(CurrX, CurrY));  //��������buffer    
								PointLabel.at<uchar>(CurrY, CurrX) = 1;           //���������ļ���ǩ�������ظ����    
							}
						}
					}
				}
				if (GrowBuffer.size()>AreaLimit) //�жϽ�����Ƿ񳬳��޶��Ĵ�С����1Ϊδ������2Ϊ����    
					CheckResult = 2;
				else
				{
					CheckResult = 1;
					RemoveCount++;//��¼�ж�������ȥ��  
				}


				for (long z = 0; z < GrowBuffer.size(); z++)
				{
					CurrX = GrowBuffer.at(z).x;
					CurrY = GrowBuffer.at(z).y;
					PointLabel.at<uchar>(CurrY, CurrX) += CheckResult;//��ǲ��ϸ�����ص㣬����ֵΪ2  
				}
				//********�����õ㴦�ļ��**********    


			}
		}


	}


	CheckMode = 255 * (1 - CheckMode);
	//��ʼ��ת�����С������    
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

