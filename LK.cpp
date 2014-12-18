#include "include/LK.h"
#include <ftw.h>
#include <dirent.h>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <math.h>
#include <pthread.h>
using namespace std;
using namespace cv;

LK::LK() : OP()
{
	SetPara();
	picNumber = 0;
	nameLength = 0;
}

LK::LK(string pic) : OP(pic)
{
	SetPara();
	picNumber = 0;
	nameLength = 0;
}

LK::LK(string pic, string par) : OP(pic)
{
	SetPara(par);
	picNumber = 0;
	nameLength = 0;
}

void LK::SetPara()
{
	winSize = Size(21, 21);
	maxLevel = 3;
	termcrit = TermCriteria(TermCriteria::COUNT+TermCriteria::EPS, 30, 0.01);
	flags = 0;
	minEigThreshold = 0.0001;
	threadsNumber = 4;
	split = 1;
	initial = 0;
}

bool LK::SetPara(string par)
{
	ifstream infile(par.c_str(), ios::in);
	double temp = 0;
	string s;
	try
	{
		temp = ReadNextPar(infile);
		SetSize(Size(temp, temp));
		SetMaxLevel(ReadNextPar(infile)); 
		double criFlag = ReadNextPar(infile);
		double maxCount = ReadNextPar(infile);
		temp = ReadNextPar(infile);
		SetTermcrit(TermCriteria(criFlag, maxCount, temp));
		SetFlags(ReadNextPar(infile));
		SetMinEigThreshold(ReadNextPar(infile));
		SetThreadsNumber(ReadNextPar(infile));
		SetSplit(ReadNextPar(infile));
		SetInitial(ReadNextPar(infile));
		infile.close();
		return true;
	}
	catch(...)
	{
		cout << "Set parameters error" << endl;
		infile.close();
		return false;
	}
}

//计算文件夹下图片序列数量和图片命名长度
void LK::CountPics()
{
	struct dirent *dirp;
	DIR *dp;
	picNumber = 0;
	if((dp = opendir(picFolder.c_str())) == NULL)
	{
		cout << "Open failed" << endl;
		return;
	}
	while((dirp = readdir(dp)) != NULL)
	{
		string s(dirp->d_name);
		//cout << s << endl;
		if(s.size() > 4 && s.substr(s.length() - 4, 4) == ".png")
		{
			picNumber++;
			if(picNumber == 1)
			{
				nameLength = s.size() - 4; 
			}
		}
	}
}

void *CalPartOP(void *args)//5个参数分别为文件名长度，光流起始计算图片，结束计算图片，图片宽度和长度
{
	Para *par = (Para *)args;
	string file1;
	string file2;
	vector<Point2f> points[2];
	Mat gray, prevGray, image;
	vector<double> *result = new vector<double>();
	vector<uchar> status;
	vector<float> err;
	stringstream temp;
	//cout << "start = " << par->picStart << endl;
	//将所要计算光流的特征点设为图片中所有的点
	int k = 0;//用于计数
	points[0].assign(ceil(par->width / par->split) * ceil(par->height / par->split), Point2f(0, 0));
	for(int i = 0; i < par->height; i += par->split)
	{
		for(int j = 0; j < par->width; j += par->split)
		{
			points[0][k++] = Point(i, j);
		}
	}
	temp << setfill('0') << setw(par->nameLength) << par->picStart;
	temp >> file1;
	file1 = par->picFolder + "/" + file1 + ".png";
	image = imread(file1, 1);
	cvtColor(image, prevGray, COLOR_RGB2GRAY);
	int end = par->picStart + par->picNumber;
	for(int i = par->picStart + 1; i <= end; ++i)
	{
		try
		{
			temp.clear();
			temp.str("");
			temp << setfill('0') << setw(par->nameLength) << i;
			temp >> file2;
			file2 = par->picFolder + "/" + file2 + ".png";
			//cout << file1 << endl;
			//cout << file2 << endl;
			image = imread(file2, 1);
			cvtColor(image, gray, COLOR_RGB2GRAY);
			calcOpticalFlowPyrLK(prevGray, gray, points[0], points[1], status, err, par->winSize,
					par->maxLevel, par->termcrit, par->flags, par->minEigThreshold);
			double dx = 0, dy = 0;
			double op = 0;
			for(int j = 0; j < status.size(); ++j)
			{
				if(!status[j])
				{
					dx = abs(points[1][j].x - points[0][j].x);
					dy = abs(points[1][j].y - points[0][j].y);
					op += sqrt(dx * dx + dy * dy);
				}
			}
			//cout << op << endl;
			result->push_back(op);
		}
		catch(Exception e)
		{
			cout << "Cal error!" << endl;
			pthread_exit((void *)2);
		}
		file1 = file2;
		swap(prevGray, gray);
	}
	pthread_exit((void *)result);
}

int LK::CalOP()
{
	CountPics();
	if(picNumber == 0)
	{
		cout << "No picture!" << endl;
		return 1;
	}
	cout << picNumber << endl;
	//cout << nameLength << endl;
	string file1;
	string file2;
	vector<Point2f> points[2];
	Mat gray, prevGray, image;
	stringstream temp;
	int start = 0;
	double width, height;
	for( ; ; ++start)
	{
		temp << setfill('0') << setw(nameLength) << start;
		temp >> file1;
		file1 = picFolder + "/" + file1 + ".png";
		image = imread(file1 ,1);
		if(image.cols > 0)
		{
			cvtColor(image, prevGray, COLOR_RGB2GRAY);
			width = prevGray.cols;
			height = prevGray.rows;//确定图片长度和宽度
			break;
		}
		temp.clear();
		temp.str("");
	}
	if(!initial)
	{
		pthread_t tid[threadsNumber];
		int picPerTh = picNumber / threadsNumber;
		for(int i = 0; i < threadsNumber; i++)
		{
			Para *args = new Para();
			args->picFolder = picFolder;
			args->picStart = start + picPerTh * i;
			args->width = width;
			args->height = height;
			args->nameLength = nameLength;
			args->winSize = winSize;
			args->maxLevel = maxLevel;
			args->termcrit = termcrit;
			args->flags = flags;
			args->minEigThreshold = minEigThreshold;
			args->split = split;
			if(i == threadsNumber - 1)
			{
				//最后一帧不需要计算，因此需要-1
				args->picNumber = picPerTh + picNumber % threadsNumber - 1;
			}
			else
			{
				args->picNumber = picPerTh;
			}
			pthread_create(&tid[i], NULL, CalPartOP, (void *)args);
		}
		vector<double> *res[threadsNumber];
		for(int i = 0; i < threadsNumber; ++i)
		{
			pthread_join(tid[i], (void **)&res[i]);
		}
		for(int i = 0; i < threadsNumber; ++i)
		{
			result.insert(result.end(), res[i]->begin(), res[i]->end());
		}
	}
	else
	{
		//cout << result.size() << endl;
		//cout << picPerTh << endl;
		//将所要计算光流的特征点设为图片中所有的点
		int k = 0;//用于计数
		points[0].assign(ceil(width / split) * ceil(height / split), Point2f(0, 0));
		for(int i = 0; i < height; i += split)
		{
			for(int j = 0; j < width; j += split)
			{
				points[0][k++] = Point(i, j);
			}
		}
		vector<double>().swap(result);
		int end = start + picNumber;
		for(int i = start + 1; i < end; ++i)
		{
			try
			{
				temp.clear();
				temp.str("");
				temp << setfill('0') << setw(nameLength) << i;
				temp >> file2;
				file2 = picFolder + "/" + file2 + ".png";
				//cout << file1 << endl;
				//cout << file2 << endl;
				image = imread(file2, 1);
				cvtColor(image, gray, COLOR_RGB2GRAY);
				calcOpticalFlowPyrLK(prevGray, gray, points[0], points[1], status, err, winSize,
						maxLevel, termcrit, flags, minEigThreshold);
				double dx = 0, dy = 0;
				double op = 0;
				for(int j = 0; j < status.size(); ++j)
				{
					if(!status[j])
					{
						dx = abs(points[1][j].x - points[0][j].x);
						dy = abs(points[1][j].y - points[0][j].y);
						op += sqrt(dx * dx + dy * dy);
					}
				}
				cout << op << endl;
				result.push_back(op);
			}
			catch(Exception e)
			{
				//cout << "Cal error!" << endl;
				return 2;
			}
			int l,m;
			//cout << "initial" << endl;
			for( l = m = 0; l < points[1].size(); ++l )
			{
				if( !status[l] )
					continue;
				points[1][m++] = points[1][l];//去除未移动的点
			}
			points[1].resize(m);
			swap(points[1], points[0]);

			file1 = file2;
			swap(prevGray, gray);
		}
	}
	return 0;
}

inline double LK::ReadNextPar(ifstream &file)
{
	string s;
	file >> s;
	double par = atof(s.c_str());
	getline(file, s, '\n');
	return par;
}

LK::~LK()
{
	//cout << "Destructor" << endl;
}
