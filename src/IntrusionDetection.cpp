#include "IntrusionDetection.h"

IntrusionDetection::IntrusionDetection(const string area_id, const int edge_numbers)
{
	this->area_id = area_id;
	this->edge_numbers = edge_numbers;

	recLeft = 0;//personRect.x
	recRight = 0;//personRect.x + personRect.width
	recTop = 0;// personRect.y
	recBottom = 0;//personRect.y + personRect.height
}  
IntrusionDetection::IntrusionDetection()
{
}
IntrusionDetection::~IntrusionDetection()
{
}

string IntrusionDetection::getCurrenttime()
{
	string Currenttime;
	time_t rawtime;
	struct tm timeinfo;
	time(&rawtime);

	localtime_s(&timeinfo, &rawtime);


	string Year = to_string(timeinfo.tm_year + 1900);
	string Mon = to_string(timeinfo.tm_mon + 1);
	string Day = to_string(timeinfo.tm_mday);
	string Hour = to_string(timeinfo.tm_hour);
	string Min = to_string(timeinfo.tm_min);
	string Second = to_string(timeinfo.tm_sec);

	Currenttime = Year + "_" + Mon + "_" + Day + "-" + Hour + "_" + Min + "_" + Second + ".avi";

	return Currenttime;
}
void IntrusionDetection::DangerDetection(Mat& frame, Rect PersonRect, int threadID)
{
	dealFrame = frame;
	personRect = PersonRect;
	int threadid = threadID;
	bool dangerflag = false;
	int intersected = 0;
	int Iscontinuous = 0;
	string path;
	
	for (int i = 0; i < area_id.length(); i++)
	{
		if (area_id[i] == '1')//if this area is danger
		{
			polyPoints = PloyShape[edge_numbers][i];
			drawTotal(polyPoints, Scalar(255, 0, 0));
			if (detect_region())
			{
				intersected = 1;
			}
				
		}
	}
	if (intersected == 0 && startsave == 1)
	{
		startsave = 0;
	}
	if (intersected == 1 && realdanger.empty() && startsave == 0)
	{
		saveflag = 1;		
	}	

	if (saveflag)
	{
		int num = 0;
		realdanger.push_back(intersected);
		if (realdanger.size() > 3)
		{
			for (int i = 0; i < realdanger.size(); i++)
			{
				if (realdanger[i] == 1)
					num++;
			}
			if (num == realdanger.size())
			{
				string dirName = "dangerVideo\\" + to_string(threadID);
				_mkdir(dirName.c_str());
				path = dirName + "\\" + to_string(threadid) + "_" + getCurrenttime();
				writer.open(path, VideoWriter::fourcc('M', 'P', '4', '2'), 30.0, Size(640, 480), true);
				startsave = 1;
			}
				

			saveflag = 0;
			realdanger.clear();
		}
	}
	if (intersected == 1 && startsave == 1)
	{
		writer << dealFrame;
	}

}



//personRect1
void IntrusionDetection::dealPersonrect()
{
	recLeft = personRect.x;
	recRight = personRect.x + personRect.width;
	recTop = personRect.y;
	recBottom = personRect.y + personRect.height;
	recPoints.clear();
	recPoints.push_back(Point(recLeft, recTop));
	recPoints.push_back(Point(recRight, recTop));
	recPoints.push_back(Point(recRight, recBottom));
	recPoints.push_back(Point(recLeft, recBottom));
}
void IntrusionDetection::addNode(node* &approach, int numX, int numY, int judge)
{
	node *Previous = approach;
	node *Add = NULL;
	Add = new node;
	Add->data[0] = numX;
	Add->data[1] = numY;
	Add->judge = judge;
	Add->pre = Previous;
	Previous->next = Add;
	approach = Add;
}

void IntrusionDetection::initNode()
{
	//先创建两个头指针
	polyPoint = new node;
	recPoint = new node;

	polyPoint->data[0] = recPoint->data[0] = 0;
	polyPoint->data[1] = recPoint->data[1] = 0;
	polyPoint->next = recPoint->next = NULL;
	polyPoint->pre = recPoint->pre = NULL;

	//approach指向头指针
	node *approach = polyPoint;//我理解是将polyPoint存放的地址赋给这个指针

	for (int i = 0; i < polyPoints.size(); i++)
	{
		addNode(approach, polyPoints[i].x, polyPoints[i].y, commonPoint);
	}

	approach->next = polyPoint;
	polyPoint->pre = approach;

	approach = recPoint;
	for (int i = 0; i < recPoints.size(); i++)
	{
		addNode(approach, recPoints[i].x, recPoints[i].y, commonPoint);
	}

	approach->next = recPoint;
	recPoint->pre = approach;
}

int IntrusionDetection::cross(const node* one, const node* two, const int edg)
{
	int result = 0;
	switch (edg) {
	case T:
		if ((one->data[1] < recTop) && (two->data[1] > recTop))
		{
			result = 1;//entry point
		}
		if ((one->data[1] > recTop) && (two->data[1] < recTop))
		{
			result = 2;//out point
		}
		break;
	case R:
		if ((one->data[0] > recRight) && (two->data[0] < recRight))
		{
			result = 1;
		}
		if ((one->data[0] < recRight) && (two->data[0] > recRight))
		{
			result = 2;
		}
		break;
	case B:
		if ((one->data[1] > recBottom) && (two->data[1] < recBottom))
		{
			result = 1;
		}
		if ((one->data[1] < recBottom) && (two->data[1] > recBottom))
		{
			result = 2;
		}
		break;
	case L:
		if ((one->data[0] < recLeft) && (two->data[0] > recLeft))
		{
			result = 1;
		}
		if ((one->data[0] > recLeft) && (two->data[0] < recLeft))
		{
			result = 2;
		}
		break;
	default:
		break;
	}
	return result;
	}

void IntrusionDetection::testPoint(int edg)
{
	node *p = polyPoint->next;
	node *q = p->next;
	Intersection tem;

	while (p != polyPoint)
	{
		int x = 0, y = 0;
		if (q == polyPoint)
		{
			q = q->next;
		}
		float k = 0;
		if (q->data[0] - p->data[0] != 0)
		{
			k = (q->data[1] - p->data[1])*1.0 / (q->data[0] - p->data[0]);
		}
		switch (edg) {
		case L:
			if (cross(p, q, L) != 0)
			{
				x = recLeft;
				y = p->data[1] + (x - p->data[0]) * k;
				if ((y <= recBottom) && (y >= recTop))
				{
					tem.x = x;
					tem.y = y;
					tem.judge = cross(p, q, L);
					intersection.push_back(tem);
				}
			}
			break;
		case R:
			if (cross(p, q, R) != 0)
			{
				x = recRight;
				y = p->data[1] + (x - p->data[0]) * k;
				if ((y <= recBottom) && (y >= recTop))
				{
					tem.x = x;
					tem.y = y;
					tem.judge = cross(p, q, R);
					intersection.push_back(tem);
				}

			}
			break;
		case B:
			if (cross(p, q, B) != 0)
			{
				if (k == 0)
				{
					if (p->data[1] != q->data[1])
					{
						y = recBottom;
						x = p->data[0];
					}
					else
					{
						break;
					}
				}
				if (k != 0)
				{
					y = recBottom;
					x = p->data[0] + (y - p->data[1]) / k;
				}
				if ((x >= recLeft) && (x < recRight))
				{
					tem.x = x;
					tem.y = y;
					tem.judge = cross(p, q, B);
					intersection.push_back(tem);
				}

			}
			break;
		case T:
			if (cross(p, q, T) != 0)
			{
				if (k == 0)
				{
					if (p->data[1] != q->data[1])
					{
						y = recTop;
						x = p->data[0];
					}
					else
					{
						break;
					}
				}
				if (k != 0)
				{
					y = recTop;
					x = p->data[0] + (y - p->data[1]) / k;
				}
				if ((x >= recLeft) && (x <= recRight))
				{
					tem.x = x;
					tem.y = y;
					tem.judge = cross(p, q, T);
					intersection.push_back(tem);
				}
			}
			break;
		default:
			break;
		}
		p = p->next;
		q = p->next;
	}
}

void IntrusionDetection::getPoint()
{
	//node *p = polyPoint->next;
	for (int i = 0; i < recPoints.size(); i++)
	{
		testPoint(i);
	}
}

void IntrusionDetection::insertNode(node *&one, node *&two, const Intersection Three, bool change)
{
	node* tem = new node;
	tem = new node;
	tem->data[0] = Three.x;
	tem->data[1] = Three.y;
	tem->judge = Three.judge;
	tem->next = two;
	two->pre = tem;
	tem->pre = one;
	one->next = tem;
	if (change)
	{
		two = tem;
	}
	else
	{
		one = tem;
	}
}

void IntrusionDetection::sortPointRec(node *& Point, int numFlag)
{
	node *p = Point->next;
	node *q = p->next;
	bool change = false;
	double distance;
	double insertDistance_P;
	double insertDistance_Q;
	double Difference;
	double accuracy = 0.5;

	if (intersection[0].y == recTop)
	{
		change = true;
	}
	if (numFlag == 2)
	{
		change = false;
	}
	for (int i = 0; i < intersection.size(); ++i)
	{


		if (q == Point)
		{
			q = q->next;
		}

		distance = sqrt(pow(q->data[0] - p->data[0], 2) + pow(q->data[1] - p->data[1], 2));
		insertDistance_P = sqrt(pow(p->data[0] - intersection[i].x, 2) + pow(p->data[1] - intersection[i].y, 2));
		insertDistance_Q = sqrt(pow(q->data[0] - intersection[i].x, 2) + pow(q->data[1] - intersection[i].y, 2));
		Difference = abs(distance - insertDistance_P - insertDistance_Q);

		if (intersection[i].y == recBottom)
		{
			if (Difference <= accuracy)
			{
				insertNode(p, q, intersection[i], change);
				if (change == true)
				{
					p = p->next;
					q = p->next;
				}

			}
			else
			{
				p = p->next;
				q = p->next;
				i--;
			}
		}
		else if (intersection[i].x == recRight)
		{
			if (Difference <= accuracy)
			{
				insertNode(p, q, intersection[i], change);
				if (change == true)
				{
					p = p->next;
					q = p->next;
				}

			}
			else
			{
				p = p->next;
				q = p->next;
				i--;
			}
		}
		else if (intersection[i].y == recTop)
		{
			if (Difference <= accuracy)
			{
				insertNode(p, q, intersection[i], change);
				if (change == true)
				{
					p = p->next;
					q = p->next;
				}
			}
			else
			{
				p = p->next;
				q = p->next;
				i--;
			}
		}
		else if (intersection[i].x == recLeft)
		{
			if (Difference <= accuracy)
			{
				insertNode(p, q, intersection[i], change);
				if (change == true)
				{
					p = p->next;
					q = p->next;
				}
			}
			else
			{
				p = p->next;
				q = p->next;
				i--;
			}
		}
	}

}

void IntrusionDetection::getArray()
{
	sortPointRec(recPoint, sortRec);
	sortPointRec(polyPoint, sortPol);
}

float IntrusionDetection::drawUnion()
{
	vector<Intersection> Q, S, W;
	vector<Point> areaPoint;
	float area=0.0, maxarea=0.0;
	node *one = polyPoint->next;
	node *two = recPoint->next;
	Intersection tem;
	int c = 1, judge = 0;
	bool f = false, begin = true, flag = false;

	while (c != 0)
	{
		if (one->judge == 1)
		{   //入点  
			tem.x = one->data[0];
			tem.y = one->data[1];
			tem.judge = 0;
			one->judge = 0;   //去掉入点标记  
			S.push_back(tem);   //暂存入点  
			f = true;
		}
		if (f)
		{
			tem.x = one->data[0];
			tem.y = one->data[1];
			tem.judge = 0;
			Q.push_back(tem);
			areaPoint.push_back(Point(tem.x, tem.y));

		}
		if (one->judge == 2 && S.size() != 0)
		{   //出点  
			f = false;
			while ((two->data[0] != one->data[0]) || (two->data[1] != one->data[1]))
			{   //数组跳转  
				two = two->next;
			}
			//two = two->next;
			while ((two->data[0] != S[0].x) || (two->data[1] != S[0].y))
			{
				if (two->judge == 1)
				{
					while ((one->data[0] != two->data[0]) || (one->data[1] != two->data[1]))
					{
						one = one->next;
					}
					tem.x = one->data[0];
					tem.y = one->data[1];
					tem.judge = 0;
					one->judge = 0;
					W.push_back(tem);
					// one = one->next;
					flag = true;
				}
				if (flag)
				{
					tem.x = one->data[0];
					tem.y = one->data[1];
					tem.judge = 0;
					Q.push_back(tem);
					areaPoint.push_back(Point(tem.x, tem.y));
					one = one->next;
					judge = 1;
				}
				if (one->judge == 2 && W.size() != 0 && judge == 1)
				{
					flag = false;
					while ((two->data[0] != one->data[0]) || (two->data[1] != one->data[1]))
					{   //数组跳转  
						two = two->next;
					}
					judge = 0;
					tem.x = two->data[0];
					tem.y = two->data[1];
					tem.judge = 0;
					Q.push_back(tem);
					areaPoint.push_back(Point(tem.x, tem.y));
				}

				two = two->next;
				if (two != recPoint && flag == false && two->judge != 1)
				{    //防止把头节点压入输出数组  
					tem.x = two->data[0];
					tem.y = two->data[1];
					tem.judge = 0;
					Q.push_back(tem);
					areaPoint.push_back(Point(tem.x, tem.y));
				}
				//two = two->next;
			}
			two = recPoint->next;
			drawTotal(Q, Scalar(0,255,255), 2);
			area = fabs(contourArea(areaPoint, true));
			if (maxarea < area)
				maxarea = area;

			Q.clear();
			S.clear();

			//检测是否有入点标记的点存在  
			node *ma = polyPoint;
			node *na;
			ma = polyPoint->next;
			na = ma;

			c = 0;
			while (ma != na)
			{
				if (ma->judge == 1)
				{
					c++;
				}
				ma = ma->next;
			}
		}
		one = one->next;
		if (one == polyPoint)
		{
			one = one->next;
		}
	}
	return maxarea;
}

void IntrusionDetection::delete_vertex()
{
	for (int i = 0; i < intersection.size(); i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (intersection[i].x == polyPoints[j].x && intersection[i].y == polyPoints[j].y)
			{
				intersection.erase(intersection.begin() + i);
				break;
			}
			else if (intersection[i].x == recPoints[j].x && intersection[i].y == recPoints[j].y)
			{
				intersection.erase(intersection.begin() + i);
				break;
			}
		}
	}
}

bool IntrusionDetection::pointInRegion(vector<Point> Points, Point vertex)
{
	int nCross = 0;
	for (int i = 0; i < 4; i++)
	{
		Point p1;
		Point p2;

		p1 = Points[i];
		p2 = Points[(i+1) % Points.size()];
		if (p1.y == p2.y)
			continue;
		if (vertex.y < min(p1.y, p2.y))
			continue;
		if (vertex.y >= max(p1.y, p2.y))
			continue;
		double x = (double)(vertex.y - p1.y) * (double)(p2.x - p1.x) / (double)(p2.y - p1.y) + p1.x;
		if (x > vertex.x)
			nCross++;
	}
	if (nCross % 2 == 1)
		return true;
	else
		return false;
}

template<typename T>
void IntrusionDetection::drawTotal(vector<T> Points, const Scalar& color, int thickness)
{
	Point beginPoint, endPoint;
	for (int i = 0; i < (Points.size() - 1); ++i)
	{
		beginPoint = Point(Points[i].x, Points[i].y);
		endPoint = Point(Points[i + 1].x, Points[i + 1].y);
		line(dealFrame, beginPoint, endPoint, color, thickness);
	}
	beginPoint = Point(Points[Points.size() - 1].x, Points[Points.size() - 1].y);
	endPoint = Point(Points[0].x, Points[0].y);
	line(dealFrame, beginPoint, endPoint, color, thickness);
}

bool IntrusionDetection::detect_region()
{
	float intersectarea = 0.0;
	bool Isdanger = false;
	intersection.clear();
	//Calculate the coordinates of the vertex of the rectangular box
	dealPersonrect();
	//Save the coordinates of rectangular boxe and polygons to two bidirectional circular lists.
	initNode();
	//Judge whether each edge of the rectangle has intersection point with the polygon and save the intersection point.
	getPoint();
	delete_vertex();

	if (intersection.size() == 0 && pointInRegion(polyPoints, recPoints[0]))
	{//The rectangle is inside the polygon
		drawTotal(recPoints, Scalar(0,255,255), 2);
		intersectarea = fabs(contourArea(recPoints, true));
	}
	if (intersection.size() == 0 && pointInRegion(recPoints, polyPoints[0]))
	{//The polygon is inside the rectangle
		drawTotal(polyPoints, Scalar(0, 255, 255), 2);
		intersectarea = fabs(contourArea(polyPoints, true));
	}
	if (intersection.size()%2 == 0 && intersection.size() != 0)
	{//The intersection of normal intersects is always paired
		getArray();
		intersectarea = drawUnion();
	}
	if (intersectarea > (fabs(contourArea(polyPoints, true))* 0.1))
		Isdanger = true;
	return Isdanger;
}


