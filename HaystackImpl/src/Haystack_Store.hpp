// HayStack_Store.cpp : Defines the entry point for the console application.
//

#include<vector>
#include<map>
#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
using namespace std;



struct ImageMetadata
{
	string filename;
	int offset;
	int sizeinBytes;
	bool IsExist;
};
struct ImageDataStructure
{
	bool IsExist;
	int size;
	char key[8];

	ImageDataStructure()
	{}
	ImageDataStructure(int Size, const string Key,bool exist )
		:size(Size),
		 IsExist(exist)

	{
		strcpy(key, Key.c_str());
	}
};
class HaystackStore
{
	std::map<string,ImageMetadata> MetadataCache;
	string dataPath ;

		static HaystackStore *Instance;
		HaystackStore()
		{
		}


public:

	static HaystackStore* getInstance();
	void setDataPath(std::string datapath);
	void write(string vid, string Imageid,const char* ImageData,int size);
	char* Read(string Imageid);
	void DeleteImage(string Imageid);
};
