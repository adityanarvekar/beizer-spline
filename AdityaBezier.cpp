#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv/cv.h>
#include <string>
#include <iostream>

using namespace cv;
using namespace std;

Mat canvas;
std::vector<Point> ptdata;
std::vector<Point> ptdatareference;
int startRange = 0;
int answer = 0;
int counter = 0;
int editIndex = 0;
int drag = 0;
static const char alphanum[] =
"0123456789"
"ABCDEFGHIJKLMNOPQRSTUVWXYZ";
int stringLength = sizeof(alphanum) - 1;
double bezierdata[4][4] = { {-1,3,-3,1}, {3, -6, 3, 0}, {-3,3,0,0}, {1,0,0,0} };
void drawSpline(Point p1, Point p2, Point p3, Point p4, bool makeBlack)
{
	
	Mat M = Mat(4, 4, CV_64FC1, bezierdata);
	double pointsdata[4][2] = { {(double)p1.x,(double)p1.y}, {(double)p2.x, (double)p2.y}, {(double)p3.x,(double)p3.y}, {(double)p4.x, (double)p4.y} };
	Mat G = Mat(4, 2, CV_64FC1, pointsdata);
	Mat A = M * G;
	for (double t = 0; t < 1; t += 0.001) {
		double param1 = (A.ptr<double>(0)[0] * t*t*t) + (A.ptr<double>(1)[0] * t*t) + (A.ptr<double>(2)[0] * t) + A.ptr<double>(3)[0];
		double param2 = (A.ptr<double>(0)[1] * t*t*t) + (A.ptr<double>(1)[1] * t*t) + (A.ptr<double>(2)[1] * t) + A.ptr<double>(3)[1];
		if (makeBlack) {
			circle(canvas, Point(param1, param2), 1, Scalar(0, 0, 0), 1);
		}
		else {
			circle(canvas, Point(param1, param2), 1, Scalar(255, 255, 255), 1);
		}
	}
}

void drawRedLine(Point p1, Point p2, bool drawRed) {
	if (drawRed) {
		line(canvas, p1, p2, Scalar(0, 0, 255), 1, 4, 0);
	}
	else {
		line(canvas, p1, p2, Scalar(255, 255, 255), 1, 4, 0);
	}
}


void drawRedPoints (){
	if (counter > 0) {
		for (int el = 0; el < ptdata.size(); el++) {
			circle(canvas, ptdata[el], 1, Scalar(0, 0, 255), 2);
		}
	}
}

void drawRedLines() {
	if (counter > 1) {

	}
}
void drawBlackSplines() {
	/*if (counter % 4 == 0 && ptdata.size()>0) {

		for(int i=0; i<ptdata.size())



	}*/
}


void mouseHandler(int event, int x, int y, int flags, void* param)
{
	if (event == CV_EVENT_LBUTTONDOWN && !drag)
	{
		std::vector<Point>::iterator it = std::find(ptdatareference.begin(), ptdatareference.end(), Point(x, y));
		if (it != ptdatareference.end()) {
			editIndex = std::distance(ptdatareference.begin(), it);
			answer = editIndex / 9;
			answer = answer + 1;
			drag = 1;
			printf("Clicked on Point");
		}
		else {
			circle(canvas, Point(x, y), 1, Scalar(0, 0, 255), 2);
			counter++;
			ptdata.push_back(Point(x, y));
			ptdatareference.push_back(Point(x - 1, y - 1));
			ptdatareference.push_back(Point(x - 1, y));
			ptdatareference.push_back(Point(x - 1, y + 1));
			ptdatareference.push_back(Point(x, y - 1));
			ptdatareference.push_back(Point(x, y));
			ptdatareference.push_back(Point(x, y + 1));
			ptdatareference.push_back(Point(x + 1, y - 1));
			ptdatareference.push_back(Point(x + 1, y));
			ptdatareference.push_back(Point(x + 1, y + 1));
			if (counter % 4 == 0) {
				drawRedLine(ptdata[counter - 4], ptdata[counter - 3], true);
				drawRedLine(ptdata[counter - 2], ptdata[counter - 1], true);
				drawSpline(ptdata[counter - 4], ptdata[counter - 3], ptdata[counter - 2], ptdata[counter - 1], true);
			}
			printf("Left Click \n");
			cout << counter << endl;
		}
	}
	if (event == CV_EVENT_MOUSEMOVE && drag) {
		startRange = 0;
		if (answer % 2 == 0) {
			drawRedLine(ptdata[answer - 2], ptdata[answer - 1], false);
			drawSpline(ptdata[answer - 2], ptdata[answer - 1], ptdata[answer], ptdata[answer + 1], false);
			circle(canvas, ptdata[answer - 1], 1, Scalar(255, 255, 255), 2);
			ptdata[answer - 1] = Point(x, y);
			startRange = editIndex / 9;
			startRange = startRange * 9;
			ptdatareference[startRange - 1] = Point(x - 1, y - 1);
			ptdatareference[startRange] = Point(x - 1, y);
			ptdatareference[startRange + 1] = Point(x - 1, y + 1);
			ptdatareference[startRange + 2] = Point(x, y - 1);
			ptdatareference[startRange + 3] = Point(x, y);
			ptdatareference[startRange + 4] = Point(x, y + 1);
			ptdatareference[startRange + 5] = Point(x + 1, y - 1);
			ptdatareference[startRange + 6] = Point(x + 1, y);
			ptdatareference[startRange + 7] = Point(x + 1, y + 1);
			circle(canvas, Point(x, y), 1, Scalar(0, 0, 255), 2);
			drawRedLine(Point(x, y), ptdata[answer - 2], true);
			drawSpline(ptdata[answer - 2], ptdata[answer - 1], ptdata[answer], ptdata[answer + 1], true);
		}
		else if (answer % 2 == 1) {
			drawRedLine(ptdata[answer - 1], ptdata[answer], false);
			drawSpline(ptdata[answer - 3], ptdata[answer - 2], ptdata[answer - 1], ptdata[answer], false);
			circle(canvas, ptdata[answer - 1], 1, Scalar(255, 255, 255), 2);
			ptdata[answer - 1] = Point(x, y);
			startRange = editIndex / 9;
			startRange = startRange * 9;
			ptdatareference[startRange - 1] = Point(x - 1, y - 1);
			ptdatareference[startRange] = Point(x - 1, y);
			ptdatareference[startRange + 1] = Point(x - 1, y + 1);
			ptdatareference[startRange + 2] = Point(x, y - 1);
			ptdatareference[startRange + 3] = Point(x, y);
			ptdatareference[startRange + 4] = Point(x, y + 1);
			ptdatareference[startRange + 5] = Point(x + 1, y - 1);
			ptdatareference[startRange + 6] = Point(x + 1, y);
			ptdatareference[startRange + 7] = Point(x + 1, y + 1);
			circle(canvas, Point(x, y), 1, Scalar(0, 0, 255), 2);
			drawRedLine(ptdata[answer - 1], ptdata[answer], true);
			drawSpline(ptdata[answer - 3], ptdata[answer - 2], ptdata[answer - 1], ptdata[answer], true);
		}
	}
	if (event == CV_EVENT_LBUTTONUP) {
		drag = 0;
	}
	if (event == CV_EVENT_RBUTTONDOWN) {
		if (counter == 0) {

		}
		else if (counter % 4 == 1) {
			circle(canvas, ptdata[counter - 1], 1, Scalar(255, 255, 255), 2);
			ptdata.pop_back();
			for (int xyz = -4; xyz <= 4; xyz++) {
				ptdatareference.pop_back();
			}
			counter--;
		}
		else if (counter % 4 == 2) {
			circle(canvas, ptdata[counter - 1], 1, Scalar(255, 255, 255), 2);
			drawRedLine(ptdata[counter - 2], ptdata[counter - 1], false);
			ptdata.pop_back();
			for (int xyz = -4; xyz <= 4; xyz++) {
				ptdatareference.pop_back();
			}
			counter--;
		}
		else if (counter % 4 == 3)
		{
			circle(canvas, ptdata[counter - 1], 1, Scalar(255, 255, 255), 2);
			ptdata.pop_back();
			for (int xyz = -4; xyz <= 4; xyz++) {
				ptdatareference.pop_back();
			}
			counter--;
		}
		else if (counter % 4 == 0) {
			circle(canvas, ptdata[counter - 1], 1, Scalar(255, 255, 255), 2);
			drawRedLine(ptdata[counter - 2], ptdata[counter - 1], false);
			drawSpline(ptdata[counter - 4], ptdata[counter - 3], ptdata[counter - 2], ptdata[counter - 1], false);
			ptdata.pop_back();
			for (int xyz = -4; xyz <= 4; xyz++) {
				ptdatareference.pop_back();
			}
			counter--;
		}
		printf("Right Click \n");
		cout << counter << endl;
	}
}

char genRandom() {
	return alphanum[rand() % stringLength];
}

int main()
{
	int c;
	canvas = Mat(800, 1000, CV_8UC3);
	canvas = Scalar(255, 255, 255);
	while (1) {
		imshow("Canvas", canvas);
		cvSetMouseCallback("Canvas", mouseHandler, NULL);

		drawRedPoints();
		drawRedLines();
		drawBlackSplines();

		c = waitKey(10);
		if (c == 27) {
			break;
		}
		else if ((c == 's' || c == 'S')) {
			String randomString;
			for (unsigned int i = 0; i < 10; ++i) {
				randomString += genRandom();
			}
			randomString = randomString + ".jpg";
			imwrite(randomString, canvas);
			printf("\nImage Saved");
		}
		else if ((c == 'r' || c == 'R')) {
			for (int j = 0; j < canvas.rows; j++) {
				for (int i = 0; i < canvas.cols; i++) {
					if (canvas.ptr<uchar>(j)[3 * i + 2] == 255 && canvas.ptr<uchar>(j)[3 * i + 1] == 0 && canvas.ptr<uchar>(j)[3 * i + 0] == 0) {
						canvas.ptr<uchar>(j)[3 * i + 0] = 255;
						canvas.ptr<uchar>(j)[3 * i + 1] = 255;
					}
				}
			}
			counter = 0;
			ptdata.clear();
			ptdatareference.clear();
		}
	}
	return 0;
}
