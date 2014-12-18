#ifndef PARA_H
#define PARA_H

#include "opencv2/highgui/highgui.hpp"
using namespace cv;
class Para
{
public:
	string picFolder;
	double width;
	double height;
	int picStart;//图片序列开始编号
	int picNumber;//图片数量
	int nameLength;//图片序列的文件名长度
	vector<uchar> status;
	vector<float> err;
	Size winSize;
	int maxLevel;
	TermCriteria termcrit;
	int flags;
	double minEigThreshold;
	int threadsNumber;//计算光流使用的线程数
	int split; //计算像素间隔
	char initial;//0表示光流计算不使用上次计算的结果，1表示以上次计算结果为下次计算的初始特征点

};

#endif
