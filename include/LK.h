#ifndef LK_H
#define LK_H

#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <ctype.h>
#include <iostream>
#include <fstream>
#include "OP.h"
#include "Para.h"
using namespace cv;

class LK : public OP
{
public:
	LK();//默认构造函数，参数设置为默认值
	LK(string);//只读入图片序列的构造函数，将参数设置为默认值
	LK(string pic, string);//根据文件设置参数
	bool SetPara(string);//通过文件设置光流的参数,返回true表示设置成功
	int CalOP();//返回0表示计算成功，1表示文件夹中没有图片，2表示计算光流时出错
	void CountPics();//计算图片数量
	//Set
	void SetSize(Size size) { winSize = size; }
	void SetMaxLevel(int level) { maxLevel = level; }
	void SetTermcrit(TermCriteria crit) { termcrit = crit; }
	void SetFlags(int flags) { LK::flags = flags; }
	void SetMinEigThreshold(double thre) { minEigThreshold = thre; }
	void SetThreadsNumber(int thread) { threadsNumber = thread; }
	void SetSplit(int st) { split = st; }
	void SetInitial(char i) { initial = i; }
	//Get
	vector<uchar> GetStatus() { return status; }
	vector<float> GetError() { return err; }
	int GetPicNumber() { return picNumber; }
	Size GetSize() { return winSize; }
	int GetMaxLevel() { return maxLevel; }
	TermCriteria GetTermcrit() { return termcrit; }
	int GetFlags() { return flags; }
	double GetMinEigThreshold() { return minEigThreshold; }
	int GetThreadsNmuber() { return threadsNumber; }
	~LK();
private:
	inline double ReadNextPar(std::ifstream &);//读取参数文件的下一个参数
	void SetPara();//设置为默认参数
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
