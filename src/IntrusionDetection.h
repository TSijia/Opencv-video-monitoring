#pragma once
#pragma once
#ifndef _INTRUSIONDETECTION_H_
#define _INTRUSIONDETECTION_H_

#include "stdafx.h"

#define commonPoint 0
#define sortRec 1//Insert the intersection into a rectangular box
#define sortPol 2//Insert the intersection into the polygon

//Data structure of linked list nodes
struct node {
	int   data[2];//(x,y)
	int   judge;
	node* next;
	node* pre;
};
//Data structure of intersection 
struct Intersection {
	int x;
	int y;
	int judge;
};
//The four sides of the rectangle:
// T(TOP)->0, R(RIGHT)->1, B(BOTTOM)->2, L(LEFT)->3
enum mark { T, R, B, L };

class IntrusionDetection
{
public:

	/**IntrusionDetection(const string , const int)
	*
	*@param area_id       :  Set the area to be detected
	*@param edge_numbers  :  Set the shape of the inspection area
	*
	*/
	IntrusionDetection(const string area_id, const int edge_numbers);
	IntrusionDetection();
	~IntrusionDetection();

	/**DangerDetection(Mat&, Rect );
	*
	*@param & frame     :  References to video frames
	*@param PersonRect  :  Mark the moving object's rectangle
	*                      (x, y, width, height)
	*@param threadID    :  
	*
    * This function : call detect_region() function
	*                 1. detect the invasion of dangerous areas 
	*                 2. show the intrusion on the video frame. 
	*                 3. trigger recording and saving intruder video
	*/
	void DangerDetection(Mat& frame, Rect PersonRect, int threadID);



private:

	/**dealPersonrect()
	*
	* This function :  calculates the coordinates of the four points of 
	*                  the rectangle according to "personRect" 
	* personRect->(x,y,width,height): (x, y)  The coordinates of the upper left corner of the rectangle
	*                                  width  The width of the rectangle
	*                                  height The height of the rectangle
	*/                                 
	void dealPersonrect();

	/**addNode(...);
	*
	*@param &approach  :  References to list
	*@param num1   :  X coordinate
	*@param num2   :  Y coordinate
	*@param judge  :  Node type : 0 ordinary node
	*                             1 entry point
	*                             2 out point
	*/
	void addNode(node* &approach, int num1, int num2, int judge);

	/**initNode()
	*
	* This function : call addNode(...) function to store the points 
	*                 of rectangles and polygons in two bidirectional 
	                  circular linked lists
	*                                
	*/
	void initNode();

	/**cross(...);
	*
	*@param one   :  a node of the list of polygons
	*@param two   :  the next node of the list of polygons(clockwise)
	*@param edg   :  one side of the rectangular 
	*
	* return result : 0  no point of intersection                            
	*                 1  intersection is the entry point   
	*                 2  Intersection is the out point
	*
	*/
	int cross(const node* one, const node* two, const int edg);

	/**testPoint(...);
	*
	*@param edg   :  the value of enum mark { T, R, B, L };
	*
	* this function : 1. find the intersection of a polygon and a side of a rectangle
	*                 2. save the intersection to "intersection" 
	*
	*/
	void testPoint(int edg);

	/**getPoint();
	*
	*
	* the function : 1. find the intersection of a polygon and a side of a rectangle
	*                2. save the intersection to "intersection"
	*
	*/

	void getPoint();

	/**insertNode(...);
	*
	* the function : insert "Three" between "one" and "two"
	*               
	*/
	void insertNode(node *&one, node *&two, const Intersection Three, bool change);

	/**sortPointRec(...);
	*
	*@param recPoint : the head pointer of list 
	*@param num      : distinguish between rectangles and polygons
	*                  "sortRec"->rectangles
	*                  "sortPlo"->polygons
	* 
	*the function : insert all intersections into the linked list
	*/
	void sortPointRec(node *& recPoint, int num);

	/**getArray();
	*
	*the function : call sortPointRec(...) function to get the list of 
	*               rectangles and polygons added to the intersection
	*
	*/
	void getArray();

	/**drawUnion();
	*
	*the function : Get the intersection area between rectangle 
	*                and polygon ,and draw it
	*               
	*/
	float drawUnion();

	/**delete_vertex()
	*
	*the function : if there is a vertex of the rectangle or polygon 
	*               in the intersection, delete it.                
	*/	
	void delete_vertex();

	/**pointInRegion(...);
	*
	*@param Points : an array of vertex coordinates of a polygon
	*@param vertex : the coordinates of a point
	*
	*the function : determine whether a point is within the polygon
	*/
	bool pointInRegion(vector<Point> Points, Point vertex);

	/**drawTotal(...);
	*
	*@param Points    : an array of vertex coordinates of a polygon
	*@param color     : the color of the line(RGB  eg:(255,0,0))
	*@param thickness : the thickness of the line(default is 1)
	*
	*the function : draw a polygon
	*/
	template<typename T>
	void drawTotal(vector<T> Points, const Scalar& color, int thickness = 1);

	/**detect_region();
	*
	*the function : execution detection process
	*/
	bool detect_region();


	string getCurrenttime();

private:

	Mat dealFrame;
	Rect personRect;

	int recLeft;//per.x
	int recRight;//per.x + per.width
	int recTop;// per.y
	int recBottom;//per.y + per.height

	string area_id;
	int edge_numbers;

	int intersected = 0;
	int startsave = 0;
	int saveflag = 0;
	vector<int> realdanger;
	VideoWriter writer;

	vector<Intersection> intersection;
	vector<Point> polyPoints;
	vector<Point> recPoints;
	node *polyPoint = new node;
	node *recPoint = new node;


	map<int, vector<Point>> fourEdges = {
		//{ 0,{ Point(209,289),Point(353,269),Point(451,347),Point(197,386) } },
	 //   { 1,{ Point(190,160),Point(340,159),Point(353,259),Point(200,269) } }
		{ 0,{ Point(80,45),Point(240,45),Point(240,145),Point(80,145) }},
	    { 1,{ Point(400,45),Point(560,45),Point(560,145),Point(400,145) }},
	    { 2,{ Point(80,190),Point(240,190),Point(240,290),Point(80,290) } },
	    { 3,{ Point(400,190),Point(560,190),Point(560,290),Point(400,290) } },
	    { 4,{ Point(80,335),Point(240,335),Point(240,435),Point(80,435) } },
	    { 5,{ Point(400,335),Point(560,335),Point(560,435),Point(400,435) } }
	};
	map<int, map<int, vector<Point>>> PloyShape = {
		{ 4 , fourEdges }
	};


};

#endif
