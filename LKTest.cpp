#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <ctype.h>
#include <fstream>
#include <ftw.h>
using namespace cv;
using namespace std;

int list(const char *name,const struct stat *sb,int type)
{
    if(type==FTW_F)  
    {
        printf("%s\n",name);
    }
    return 0;
}

int main(int argc, char **argv)
{
	int searchSize = atoi(argv[1]);
	float width = 720, height = 576;
	Mat gray, preGray, image;
	vector<float> err;
	Size winSize;
	winSize = Size(searchSize, searchSize);//搜索窗口的大小
    TermCriteria termcrit(4, 20, 0.03);//结束条件
	vector<Point2f> points[2];
	//namedWindow("color",1);
	//namedWindow("gray", 1);
	image = imread("/var/www/html/op/Pics/delete/000003.png", 1);
	cvtColor(image, preGray, COLOR_RGB2GRAY);
	//imshow("color",preGray);
	image = imread("/var/www/html/op/Pics/delete/000004.png", 1);
	cvtColor(image, gray, COLOR_RGB2GRAY);
	//imshow("gray", gray);
	cout << gray.cols << endl;
	points[0].assign(width * height, Point2f(0, 0));
	points[1].assign(width * height, Point2f(0, 0));
	int k = 0;
	for(int i = 0; i < height; ++i)
	{
		for(int j = 0; j < width; ++j)
		{
			points[0][k++] = Point(i, j);
		}
	}
	cout << points[0].size() << endl;
	cout << k << endl;
	vector<uchar> status;
	/*string x = "par";
	ifstream file(x.c_str(), ios::in);
	string s;
	double n = 0;
	while(file >> s)
	{
		cout << s << endl;
		n = atof(s.c_str());
		cout << n << endl;
		getline(file, s, '\n');
	}    
	Mat C = (Mat_<double>(3,3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
	cout << C.at<double>(1,1) << endl;*/
    //cout << ftw("./Pics/",&list,10) << endl; 
	try
	{
		calcOpticalFlowPyrLK(preGray, gray, points[0], points[1], status, err, winSize,
				3, termcrit, 4, 0.001);
		cout << points[1].size() << endl;
		cout << status.size() << endl;
		double dx = 0, dy = 0;
		double op = 0;
		int n = 0;
		for(int i = 0; i < status.size(); ++i)
		{
				if(!status[i])
				{
					n++;
					dx = abs(points[1][i].x - points[0][i].x);
					dy = abs(points[1][i].y - points[0][i].y);
					op += sqrt(dx * dx + dy * dy);
				}
		}
		cout << n << endl;
		cout << op << endl;
	}
	catch(Exception e)
	{
		cout << "error" << endl;
	}
	int x = CV_TERMCRIT_ITER|CV_TERMCRIT_EPS;
	cout <<x << endl;
	//char c = waitKey();
	return 0;
}
