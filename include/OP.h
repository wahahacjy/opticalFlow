#ifndef OP_H
#define OP_H

#include <string>
#include <iostream>
#include <vector>
using namespace std;

class OP
{
public:
	OP();
	OP(string);//设置图片序列文件夹
	~OP();

	bool ReadVideo(const string);//读取视频文件并解码
	virtual bool SetPara(string) = 0;//设置光流参数
	virtual int CalOP() = 0;//计算光流,返回0表示计算成功，1表示文件夹中没有图片，2表示计算光流时出错,返回3表示设置参数出错

	void SetPicFolder(string folder) { picFolder = folder; }
	//Get
	string GetPicFolder() { return picFolder; }
	vector<double> GetResult() { return result; }
protected:
	bool isVideo;//true时为视频文件
	string inputFile;//输入文件的文件名，当为视频文件时使用，包含整个路径。当为图片序列文件夹时，该变量不使用，只有picFolder有用。
	string picFolder;//图片序列所在文件夹,当为视频时解码图片放在该文件夹下
	vector<double> result;
	inline string GetFilename(const string);
};

#endif
