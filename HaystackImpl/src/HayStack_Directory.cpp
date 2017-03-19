// HayStack_Directory.cpp : Defines the entry point for the console application.
//

#include"HayStack_Directory.h"
using namespace std;

Directory::Directory()
{
	for(int i=1;i<=4;i++)
	{
		SpaceMap[i]= 0;
		AvailableVolumesToWrite[i-1]=i;
	}
}


//static member
Directory *Directory::Instance = NULL;


Directory* Directory::getInstance()
{
	if(Instance == NULL)
		Instance = new Directory();
	return Instance;
}

string Directory::AssignTokenToWrite(string ImageID, unsigned long ImageSize)
{
	static int NextVolumeLocation = 0;
	int start =0;
	int end =NextVolumeLocation;
	string result;
	for(int i=NextVolumeLocation; i < TotalVolumes && AvailableVolumesToWrite[i] != 0; i++)
	{
		int WritableVolume = AvailableVolumesToWrite[i];
		if((SpaceMap[WritableVolume]+ImageSize) <= MaxSpaceForVolume)
		{
			//ActionMap[WritableVolume] = ;
			SpaceMap[WritableVolume] += ImageSize;
			NextVolumeLocation = (i+1)%(TotalVolumes);
			StorageMap[ImageID] = WritableVolume;// what if image names repeat??
			stringstream convert;
			convert << WritableVolume;
			result = (convert.str()+","+ImageID);
			return result;
		}
		else
		{
			AvailableVolumesToWrite[i] = 0;
		}

	}
	if(start!=end)
	{
		for(int i=start;i<end;i++)
		{
			int WritableVolume = AvailableVolumesToWrite[i];
			if((SpaceMap[WritableVolume]+ImageSize) <= MaxSpaceForVolume)
			{
				SpaceMap[WritableVolume] += ImageSize;
				NextVolumeLocation = (i+1)%(TotalVolumes);
				StorageMap[ImageID] = WritableVolume;// what if image names repeat??
				stringstream convert;
				convert << WritableVolume;
				result = (convert.str()+","+ImageID);
				return result;
			}
			else
			{
				AvailableVolumesToWrite[i] = 0;
			}
		}
	}

	return "0";//No writable volumes found. All are full
}
string Directory::GetTokenToRead(string Imageid)
{
	std::map<string,int>::iterator it;
	it = StorageMap.find(Imageid);
	string result;
	if(it != StorageMap.end())
	{
		stringstream convert;
		convert << StorageMap[Imageid];
		result = (convert.str()+","+Imageid);
		return result ;
	}
	else
		return 0;//Image ID not found in the map.
}
