// HayStack_Store.cpp : Defines the entry point for the console application.
//

#include<map>
#include<string>
#include"Haystack_Store.hpp"

using namespace std;
#include<stdio.h>

//static member
HaystackStore *HaystackStore::Instance = NULL;



HaystackStore* HaystackStore::getInstance()
{
	if(Instance == NULL)
		Instance = new HaystackStore();
	return Instance;
}

void HaystackStore::setDataPath(string dataPath)
{

	this->dataPath = dataPath;
}

void HaystackStore::write(string vid, string Imageid,const char* ImageData,int size)
	{
		std::map<string,ImageMetadata>::iterator it;
		it = MetadataCache.find(Imageid);
		if(it == MetadataCache.end()) // Images should have Unique ID
		{
			ImageDataStructure idata(size,Imageid,true);

			string Name = "_ImageData.bin";

			Name = vid + Name;
			dataPath = dataPath +"/" +Name;
			ImageMetadata metadata ;
			metadata.filename = dataPath;
			metadata.sizeinBytes = size;
			metadata.IsExist = true;
			cout<<"Writing to " <<dataPath<<std::endl;

			FILE* fp = fopen(dataPath.c_str(), "ab+");
			if (fp==NULL)
  		{
				cerr<<"Internal error occurred\n";
				return;
			}

			fseek(fp, 0L, SEEK_END);
			int sz2 = ftell(fp);
			metadata.offset = sz2;
			MetadataCache[Imageid] = metadata;
			std::cout << "Write called...."<< std::endl;
			size_t ret = fwrite(&idata,sizeof(ImageDataStructure),1,fp);
			fwrite(ImageData, size, 1, fp);
			fclose(fp);

		}
	}

	char* HaystackStore::Read(string Imageid)
		{
			std::map<string,ImageMetadata>::iterator it;
			it = MetadataCache.find(Imageid);
			if(it != MetadataCache.end())
			{
				ImageMetadata metadata = MetadataCache[Imageid];
				if(metadata.IsExist)
				{
					FILE* fp = fopen(metadata.filename.c_str(), "rb");

					char *data = new char[metadata.sizeinBytes];
					int offset = metadata.offset +sizeof(ImageDataStructure);
					fseek(fp,offset, SEEK_SET);
					fread(data, metadata.sizeinBytes, 1, fp);
					fclose(fp);
					return data;
				}
				else
					return NULL;
			}
			else
				return NULL;
		}

		void HaystackStore::DeleteImage(string Imageid)
		{
			std::map<string,ImageMetadata>::iterator it;
			it = MetadataCache.find(Imageid);
			if(it != MetadataCache.end())
			{
				ImageMetadata metadata = MetadataCache[Imageid];
				if(metadata.IsExist)
				{

					MetadataCache[Imageid].IsExist = false;
					FILE* fp = fopen(metadata.filename.c_str(), "r+");

					if(fp == NULL)
					{
						cout<<"null ptr";
						return;
					}
					ImageDataStructure imagData(metadata.sizeinBytes,Imageid,metadata.IsExist);
					int ret = fseek(fp, metadata.offset, SEEK_SET);

					if (ret)
					{
						fclose(fp);
						return;
					}

					ret = fread(&imagData, sizeof(ImageDataStructure), 1, fp);
					if (ret == 0)
					{
						fclose(fp);
						return;
					}
					imagData.IsExist = false;
					cout<<imagData.key;
					ret = fseek(fp, metadata.offset, SEEK_SET);

					if (ret)
					{
						fclose(fp);
						return;
					}

					int sz2 = ftell(fp);
					ret = fwrite(&imagData,sizeof(ImageDataStructure),1,fp);
					if (ret == 0)
					{
						fclose(fp);
						return;
					}
					fclose(fp);
				}
					cout<<"The file does not exist";
			}
		}
