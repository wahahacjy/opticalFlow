#include <iostream>
#include <sys/stat.h>
#include <cstring>
#include <fstream>
#include <vector>
#include "include/OP.h"
#include "include/AnomalyDetect.h"
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
	//-i 输入视频文件或者图片序列文件夹 -o 输出结果文件夹，会在其中自动创建result文件夹,不设置的话默认保存在图片序列文件夹下的result文件夹下 -p 参数文件 -dec 视频解码图片保存文件夹，不设置的话默认保存在视频文件夹下
	//以视频名字新建的文件夹下
	for(int i = 1; i < argc; ++i)
	{
		if(strcmp(argv[i], "-i") == 0)
		{
			i++;
			//cout << "Input is " << argv[i] << endl;
			inputFilename = argv[i];
		}
		else if(strcmp(argv[i], "-o") == 0)
		{
			i++;
			//cout << "Output is " << argv[i] << endl;
			outputFilename = argv[i];
		}else if(strcmp(argv[i], "-p") == 0)
		{
			i++;
			//cout << "Parameter file is " << argv[i] << endl;
			para = argv[i];
		}else if(strcmp(argv[i], "-dec") == 0)
		{
			i++;
			//cout << "Pictures decoded to " << argv[i] << endl;
			pic = argv[i];
		}else if(strcmp(argv[i], "-ti") == 0)
		{
			i++;
			tInsert = atof(argv[i]);
			//cout << "Insert theshold is " << tInsert << endl;
		}else if(strcmp(argv[i], "-td") == 0)
		{
			i++;
			tDelete = atof(argv[i]);
			//cout << "Delete theshold is " << tDelete << endl;

		}
	}
	time_t start = time(NULL);
	//clock_t start,stop;
	//start = clock();
	LK *lk = new LK();
	if(para != NULL)
	{
		if(!lk->SetPara(para))
		{
			cout << "Set Parameters failed" << endl;
			return 0;
		}
	}
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
		if(lk->CalOP() == 0)
		{
			//异常检测测试
			vector<double> re = lk->GetResult();
			for(int i = 0; i < re.size(); ++i)
			{
				cout << re[i] << endl;
			}
			AnomalyDetect *ad = new AnomalyDetect();
			ad->ReadData(re);
			ad->SetTInsert(tInsert);
			ad->SetTDelete(tDelete);
			string output;
			if(outputFilename == NULL)
			{
				//ad->CalGeneData(lk->GetPicFolder());
				output = lk->GetPicFolder();
			}
			else
			{
				output = outputFilename;
			}
			ad->CalGeneData(output);
			result.open((output + "/result/result.txt").c_str(), ios::out);
			int *location;
			int type = ad->Detect(&location);
			//1表示帧删除，2表示帧插入，0表示无篡改
			if(type == 1)
			{
				cout << 1 << endl;
				cout << location[0] << endl;
				result << 1 << endl;
				result << location[0] << endl;
			}else if(type == 2)
			{
				cout << 2 << endl;
				cout << location[0] << endl;
				cout << location[1] << endl;
				result << 2 << endl;
				result << location[0] << endl;
				result << location[1] << endl;
			}else
			{
				cout << 0 << endl;
				result << 0 << endl;
			}
			//cout << ad->GetMean() << "," << ad->GetDelta() << endl;
		}
		else
		{
			//cout << "计算出错" << endl;
		}
	}
	else
	{
		//cout << "No input file" << endl;
		return 0;
	}
	//LK *lk = new LK("/home/cjy/Desktop/OP/Pics/Megamind/");
	//LK *lk1 = new LK("./Pics/akiyo");
	//cout << lk->GetMaxLevel() << endl;
	//cout << lk->GetSize() << endl;
	//cout << lk->GetPicNumber() << endl;
	
	time_t stop = time(NULL);
	//stop = clock();
	cout << (stop - start)/* / CLOCKS_PER_SEC*/<< endl;
	result << (stop - start)/* / CLOCKS_PER_SEC*/<< endl;
	//lk1->CalPryLK();
	delete lk;
	return 0;
}
