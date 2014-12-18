#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include "include/OP.h"

//const string gPicPath = "/home/cjy/Desktop/OP/Pics/";

OP::OP()
{
	isVideo = false;
	picFolder = "";
}

OP::OP(string folder)
{
	isVideo = false;
	SetPicFolder(folder);
}

OP::~OP()
{
}

bool OP::ReadVideo(const string file)
{
	inputFile = file;
	string name = GetFilename(file);
	//cout <<  inputFile << endl;
	if(picFolder == "")
	{
		size_t index = file.find_last_of('/');
		picFolder = file.substr(0, index + 1) + name;
	}
	else
	{
		picFolder = picFolder + "/" + name;
	}
	//cout << "解码图片保存在" << picFolder << endl;
	OP::isVideo = true;
	//先删除原本存在的文件夹
	if(mkdir(picFolder.c_str(), S_IRWXU | S_IRWXG) < 0)
	{
		string del = "rm -rf '";
		del += picFolder;
		del += "/*.png'";
		system(del.c_str());
		//system("rm -rf Pics/delete");
		//cout << picFolder << "下图片文件已清空" << endl;
	}
	//cmd = ffmpeg -i video picFolder/%6d;
	string cmd = "/usr/local/bin/ffmpeg -i '";
	cmd += file;
	cmd += "' '";
	cmd += picFolder;
	cmd += "/%6d.png' > /dev/null 2>&1";	
	//cout << cmd << endl;
	FILE *ffmpeg = popen(cmd.c_str(), "r");
	if(!ffmpeg)
	{
		cout << "failed" << endl;
		cout << "解码失败！" << endl;
	}
	else
	{
		//cout << "decoding" << endl;
		//cout << "开始解码！" << endl;
	}
	pclose(ffmpeg);
}

inline string OP::GetFilename(const string file)
{
	size_t index1 = file.find_last_of('/');
	size_t index2 = file.find_last_of('.');
	return file.substr(index1 + 1, index2 - index1 - 1);
}

/*int OP::CalPryLK(int split)
{
	lk->SetPicFolder(picFolder);
	return lk->CalPryLK(split);
}

int OP::CalPryLK(string par, int split)
{
	lk->SetPicFolder(picFolder);
	if(lk->SetPara(para))
	{
		return lk->CalPryLK();
	}
	else
	{
		cout << "参数设置错误！" << endl;
		return 3;
	}
}*/



