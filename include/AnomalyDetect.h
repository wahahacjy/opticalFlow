#ifndef ANODETECT_H
#define ANODETECT_h

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class AnomalyDetect
{
public:
	AnomalyDetect(){}
	~AnomalyDetect(){}
	int ReadData(string file);//从文件读入数据,并返回数组的大小，为0表示数据读入失败
	int ReadData(const vector<double> &);
	void CalGeneData();//计算归一化变化率,不保存结果至文件
	void CalGeneData(string);//计算归一化变化率，并将结果写入文件
	int Detect(int **location);

	//Set
	void SetTInsert(double t){T_insert = t;}
	void SetTDelete(double t){T_delete = t;}
	//Get
	vector<double> GetData()
	{
		return data;
	}
	vector<double> GetGenCRate()
	{
		return genChangeRate;
	}
	vector<double> GetChangeRate() { return changeRate; }
	double GetMean(){return mean;}
	double GetDelta(){return delta;}
	double GetTInsert(){return T_insert;}
	double GetTDelete(){return T_delete;}
private:
	vector<double> data;
	vector<double> genChangeRate; 
	vector<double> changeRate;
	double mean;
	double delta;
	double T_insert;
	double T_delete;
	void WriteData(const vector<double> &, ofstream &);//将结果写入文件
};
#endif
