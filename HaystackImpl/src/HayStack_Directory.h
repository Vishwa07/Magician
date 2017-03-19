#include<map>
#include<vector>
#include<ctime>
#include<iostream>
#include<sstream>
#include<string.h>
using namespace std;
enum FileAction
{
	Idle,
	Read,
	Write,
	Delete
};
class Directory
{
static Directory *Instance;
static const int TotalVolumes =4;
int AvailableVolumesToWrite[TotalVolumes];
std::map<int,unsigned long > SpaceMap;
static const unsigned long MaxSpaceForVolume = 1000000000;
std::map<string,int> StorageMap;
Directory();
public:
static Directory* getInstance();
string AssignTokenToWrite(string ImageID, unsigned long ImageSize);
string GetTokenToRead(string imageid);
//int timeslice = 100;
};
