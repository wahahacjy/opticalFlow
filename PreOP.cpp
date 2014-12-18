#include <iostream>
#include <sys/stat.h>
#include <cstring>
#include <fstream>
#include <vector>
#include "include/OP.h"
#include "include/LK.h"
#include <time.h>
using namespace std;

int main(int argc, char **argv)
{
	char *inputFilename = NULL, *outputFilename = NULL, *para = NULL, *pic = NULL;
	double tInsert = 8;
	double tDelete = 5;
	ofstream result;
	//Command line parsing
	//-i 输入视频文件或者图片序列文件夹 -dec 视频解码图片保存文件夹，不设置的话默认保存在视频文件夹下
	//以视频名字新建的文件夹下
	for(int i = 1; i < argc; ++i)
	{
		if(strcmp(argv[i], "-i") == 0)
		{
			i++;
			//cout << "Input is " << argv[i] << endl;
			inputFilename = argv[i];
		}else if(strcmp(argv[i], "-dec") == 0)
		{
			i++;
			//cout << "Pictures decoded to " << argv[i] << endl;
			pic = argv[i];
		}
	}
	time_t start = time(NULL);
	//clock_t start,stop;
	//start = clock();
	LK *lk = new LK();
	if(inputFilename != NULL)
	{
		struct stat buf;//判断输入是视频文件还是文件夹
		if(stat(inputFilename, &buf) < 0)
		{
			//cout << "No input file" << endl;
			return 0;
		}
		if(S_ISREG(buf.st_mode))//视频文件
		{
			if(pic != NULL)
				lk->SetPicFolder(pic);
			lk->ReadVideo(inputFilename);//不输入图片保存路径则默认存在当前视频文件夹下
		}else if(S_ISDIR(buf.st_mode))//文件夹
		{
			lk->SetPicFolder(inputFilename);
		}
		lk->CountPics();
		cout << lk-> GetPicNumber() << endl;
	}
	else
	{
		//cout << "No input file" << endl;
		return 0;
	}
	delete lk;
	string cmd = "/usr/local/bin/ffprobe -i '";
	cmd += inputFilename;
	cmd+= "' -v quiet -show_streams -select_streams v | grep -Ew 'codec_name|duration|width|height'";
	FILE *info = popen(cmd.c_str(), "r");
	char buf[1024];
	while(fgets(buf, 1024, info) != NULL)
	{
		/*string tmp(buf);
		int pos = -1;
		if((pos = tmp.find(codec)) != -1)
		{
			cout << tmp.substr(pos + codec.size());
		}else if((pos = tmp.find(duration)) != -1)
		{
			cout << tmp.substr(pos + duration.size());
	
		}*/
		cout << buf;
	}
	pclose(info);
	return 0;
}
