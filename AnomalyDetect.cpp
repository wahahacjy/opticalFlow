#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <math.h>
#include <sys/stat.h>
#include "include/AnomalyDetect.h"
using namespace std;


int AnomalyDetect::ReadData(string filename)
{
	double s;
	if(data.size() > 0)//如果data中有数据，则先清空其中的数据
	{
		vector<double>().swap(data);
		vector<double>().swap(genChangeRate);
		vector<double>().swap(changeRate);
	}
	ifstream file(filename.c_str(), ios::in);
	while(file >> s)
	{
		data.push_back(s);
		cout << s << endl;
	}
	file.close();
	return data.size();
}

int AnomalyDetect::ReadData(const vector<double> &d)
{
	if(data.size() > 0)//如果data中有数据，则先清空其中的数据
	{
		vector<double>().swap(data);
		vector<double>().swap(genChangeRate);
		vector<double>().swap(changeRate);
	}
	data.assign(d.begin(), d.end());
	return data.size();
}

void AnomalyDetect::CalGeneData()
{
	int count = data.size();
	double total = 0;
	//计算均值和标准差
	genChangeRate.assign(count - 2, 0);
	changeRate.assign(count -2, 0);
	for(int i = 1; i < count - 1; ++i)
	{
		if((data[i + 1] + data[i - 1]) != 0)
		{
			double d = 2 * data[i] / (data[i + 1] + data[i - 1]);
			changeRate[i - 1] = d;
			//cout << d << endl;
			total += d;
		}
		else
		{
			changeRate[i - 1] = 0;
		}
	}
	mean = total / (count - 2);
	delta = 0;
	for(int i = 0; i < count -2; ++i)
	{
		delta += pow((changeRate[i] - mean), 2);
	}
	delta /= (count -2);
	delta = sqrt(delta);
	//cout << "mean = " << mean << ", delta = " << delta << endl;
	//设置成归一化的变化率
	if(delta != 0)
	{
		for(int i = 0; i < count -2; ++i)
		{
			genChangeRate[i] = fabs(changeRate[i] - mean) / delta;
			cout << genChangeRate[i] << endl;
		}
	}
	else
	{
		for(int i = 0; i < count -2; ++i)
		{
			cout << 0 << endl;
		}
	}
}

void AnomalyDetect::CalGeneData(string fd)
{
	CalGeneData();
	string folder = fd + "/result";
	mkdir(folder.c_str(), S_IRWXU | S_IRWXG);
	ofstream dataFile((folder + "/data.txt").c_str(), ios::out);
	ofstream crFile((folder + "/change_rate.txt").c_str(), ios::out);
	ofstream genCRFile((folder + "/gen_change_rate.txt").c_str(), ios::out);
	if(!dataFile.is_open() || !crFile.is_open() ||!genCRFile.is_open())
	{
		cout << "结果保存失败！" << endl;
		return;
	}
	WriteData(data, dataFile);
    WriteData(changeRate, crFile);
	WriteData(genChangeRate, genCRFile);
}

void AnomalyDetect::WriteData(const vector<double> &data, ofstream &file)
{
	for(int i = 0; i < data.size(); ++i)
	{
		file << data[i] << endl;
	}
	file.close();
}

//0表示无篡改，1表示帧删除，2表示帧插入
int AnomalyDetect::Detect(int **location)
{
	int end = genChangeRate.size();
	*location = new int[2];
	(*location)[0] = (*location)[1] = 0;
	map<double, int> m;
	map<double, int>::iterator mIter;
	for(int i = 0; i != end; ++i)
	{
		m.insert(pair<double, int>(genChangeRate[i], i + 1));
		/*
		//始终保持detRes[0]中的是较小的
		if(temp > T_insert && temp > detRes[0].data)
		{
			//t = genChangeRate[i];
			//(*location)[0] = i + 1;
			detRes[0].data = temp;
			detRes[0].pos = i + 1;
			if(detRes[0].data > detRes[1].data)
			{
				swap(detRes[0].data, detRes[1].data);
				swap(detRes[0].pos, detRes[1].pos);
			}
		}*/
	}
	mIter = m.end();
	mIter--;
	if((--mIter)->first > T_insert)
	{
		//cout << "insert" << endl;
		(*location)[0] = mIter->second;
		(*location)[1] = (++mIter)->second;
		if((*location)[0] > (*location)[1])
			swap((*location)[0],(*location)[1]);
		//cout << (*location)[0] << endl;
		//cout << (*location)[1] << endl;
		return 2;
	}else if((++mIter)->first > T_delete)
	{
		//cout << "delete" << endl;
		(*location)[0] = mIter->second;
		return 1;
	}
	return 0;
	
} 

void swap(double &a, double &b)
{
	a = a - b;
	b = a + b;
	a = b - a;
}
